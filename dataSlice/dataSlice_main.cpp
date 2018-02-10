﻿/*****************************************************************
Name : 讀取特定格式文件
Date : 2017/06/06
By   : CharlotteHonG
Final: 2018/02/10
*****************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
using namespace std;
constexpr char file_name[] = "str.txt";

#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#define POINT_IS_NULL(msg) \
	printf("%s \t\t # %s::%d --> \"%s()\" \n" , \
		(msg) , __FILENAME__, __LINE__, __FUNCTION__)


//==================================================================
typedef struct List_basic List_basic;
struct List_basic{
	char* data;
	List_basic* next;
};
void List_basic_ctor(List_basic* _this, const char* s = nullptr){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	// 建立類別所指向資料
	char* buff = nullptr;
	if(s){
		buff = (char*)malloc(sizeof(char)*strlen(s)+1);
		strcpy(buff, s);
	}
	// 放入類別的指向
	_this->data = buff;
	_this->next = nullptr;
}
void List_basic_dtor(List_basic* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	// 刪除類別所指向資料
	if(_this->data) free(_this->data);
	// 移出類別的指向
	_this->data = nullptr;
	_this->next = nullptr;
}
List_basic* List_basic_new(const char* s = nullptr){
	// 建立類別內的資料
	List_basic* _this = (List_basic*)malloc(sizeof(List_basic));
	List_basic_ctor(_this, s);
	return _this;
}
void List_basic_delete(List_basic* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	// 釋放類別內的資源
	List_basic_dtor(_this);
	free(_this);
}
//------------------------------------------------------------------
void List_basic_append(List_basic* _this, const char* s) {
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }
	
	int buff_len = strlen(s) + strlen(_this->data);
	char* buff = (char*)malloc(sizeof(char)*buff_len + 1);
	strcpy(buff, _this->data);
	strcat(buff, s);

	if(_this->data) free(_this->data);
	_this->data = buff;
}


//==================================================================
typedef struct List List;
struct List{
	List_basic* listHead;
	List_basic* listEnd;
	int ListNum;
};
void List_ctor(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	_this->listHead = List_basic_new();
	_this->listEnd = _this->listHead;
	_this->ListNum = 0;
}
void List_dtor(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); }

	if(_this->listHead){
		// 釋放鏈結資源
		List_basic* temp = nullptr;
		for(List_basic* l=_this->listHead->next; l; l=l->next){
			if(temp)
				List_basic_delete(temp);
			temp=l;
		} List_basic_delete(temp);
		List_basic_delete(_this->listHead);
	}

	// 釋放本地資源
	_this->listHead = nullptr;
	_this->listEnd = nullptr;
	_this->ListNum = 0;
}
List* List_new(){
	List* _this = (List*)malloc(sizeof(List));;
	List_ctor(_this);
	return _this;
}
void List_delete(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return;}

	List_dtor(_this);
	free(_this);
}
//------------------------------------------------------------------
void List_print(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	if(_this==NULL) 
	for(List_basic* l=_this->listHead->next; l; l=l->next){
		cout << l->data << endl;
	}
}
void List_append(List* _this, const char* s){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	List_basic* new_node = List_basic_new(s);
	_this->listEnd->next = new_node; // 把新點接上
	_this->listEnd = new_node;       // 更新結尾點
	++_this->ListNum;                // 累計計數
}
void List_strSlice(List* _this, const char* src, const char* delim = " \n\r"){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	char* buff = (char*)malloc(sizeof(char)*strlen(src)+1);
	strcpy(buff, src);
	for(char* pch = strtok(buff, delim); pch; pch = strtok(NULL, delim)){
		List_append(_this, pch);
	}
}
//------------------------------------------------------------------
void read_ContactsRaw(const char* filename, char** buf) {

	if(filename==nullptr and buf!=nullptr){
		perror("Error read_ContactsRaw input");
	}

	FILE *pFile = fopen(filename,"rb");
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);

	*buf = (char*) malloc (sizeof(char)*(lSize+1));
	if (*buf == nullptr) {fputs ("Memory error",stderr); exit (2);}
	(*buf)[lSize] = '\0';

	fread(*buf, 1, lSize, pFile);
	fclose(pFile);
}
void List_loadFile(List* _this, const char* filename){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	char* contacts = nullptr;
	read_ContactsRaw(filename, &contacts);
	List_strSlice(_this, contacts);
	free(contacts);
}
void List_loadConsole(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	for(char s[16],a;~scanf("%s",&s);){
		List_append(_this, s);
	}
}
void Data_Slice(List*** dst, int* lenth, const List* v) {
	if (!dst) { POINT_IS_NULL("point is NULL"); return; }
	if (*dst) { POINT_IS_NULL("point is invalid"); return; }

	List** temp_data = (List**)malloc(sizeof(List*) * v->ListNum);

	int item_len = 0, idx = 0, line_len=0;
	int end_mode = 0; // 0.補0;  1.補英文
	
	List_basic* l=v->listHead->next;
	char* currStr = nullptr;
	char* nextStr = nullptr;
	// 補數字
	auto Append_Num = [&]() {
		if(!isalpha(nextStr[0])) { // 下一個是數字就接著補上
			++idx, l=l->next;
			List_basic_append(temp_data[line_len-1]->listEnd, nextStr);
		}
		--item_len;
	};

	// 開始處理
	for(; idx < (v->ListNum)-2; ++idx and l, l=l->next) {
		currStr = l->data;
		nextStr = l->next->data;
		char* next2_Str = l->next->next->data;
		// 是頭長度時
		if(item_len == 0){
			item_len = stoi(currStr);
			temp_data[line_len] = List_new();
			List_append(temp_data[line_len], currStr);
			++line_len;
		}
		// 非頭長度是英文遞減
		else if(isalpha(currStr[0])) {
			List_append(temp_data[line_len-1], currStr);
			if(item_len == 1) { // 最後一個檢查
				if(isalpha(next2_Str[0])) { // 最後一組缺數字
					--item_len;
					end_mode = 1;
					continue;
				}
			}
			// 補數字
			Append_Num();
			end_mode = 0;
		}
	}

	// 結尾處理
	currStr = l->data;
	nextStr = l->next->data;
	if(item_len==1 and end_mode ==0) {
		// 補一組
		List_append(temp_data[line_len-1], currStr);
		// 補數字
		Append_Num();
	} else {
		if(end_mode==0) {
			temp_data[line_len] = List_new();
			List_append(temp_data[line_len], currStr);
		} else {
			++idx, l=l->next;
			List_append(temp_data[line_len-1], nextStr);

		}
	}

	// 輸出數據
	*lenth = line_len;
	*dst = (List**)malloc(sizeof(List*) * line_len);
	for(int i = 0; i < line_len; i++){
		(*dst)[i] = temp_data[i];
	} if(temp_data) free(temp_data);
}


//==================================================================
int main(int argc, char const *argv[]) {

	// 初始化數據
	List* list = List_new();

	// 載入文字(切割空格與跳行)
	List_loadFile(list, "str.txt");
	//List_loadConsole(list);


	// 解析格式
	int lenth = 0;
	List** dst = nullptr;
	Data_Slice(&dst, &lenth, list);

	// 查看二維陣列
	for(int j = 0; j < lenth; j++){
		List* _this = dst[j];
		for(List_basic* l=_this->listHead->next; l; l=l->next){
			cout << l->data;
			if(l->next) 	cout <<  ", ";
		} cout << endl;
	} cout << endl;



	// 釋放內存
	if(dst){
		for(int i = 0; i < lenth; i++){
			List_delete(dst[i]);
		} free(dst);
	}
	if(list) List_dtor(list);
}