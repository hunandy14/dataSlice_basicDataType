/*****************************************************************
Name : 讀取特定格式文件
Date : 2017/06/06
By   : CharlotteHonG
Final: 2017/06/06
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

vector<string> ReadFile(string file_name);
vector<vector<string>> Data_Slice(vector<string>& v);

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

//==================================================================
typedef char* Str;
typedef struct List_basic List_basic;
struct List_basic{
	Str data;
	List_basic* next;
}; 
typedef struct List List;
struct List{
	List_basic* listHead;
	List_basic* listEnd;
	int ListNum;
};
void List_print(List* _this){
	if (!_this) POINT_IS_NULL("point is NULL");

	if(_this==NULL) 
	for(List_basic* l=_this->listHead->next; l; l=l->next){
		cout << l->data << endl;
	}
}
void List_append(List* _this, const char* s){
	if (!_this) POINT_IS_NULL("point is NULL");

	char* temp = (char*)malloc(sizeof(char)*strlen(s)+1);
	strcpy(temp, s);

	_this->listEnd->next = (List_basic*)malloc(sizeof(List_basic));
	_this->listEnd = _this->listEnd->next;
	++_this->ListNum;

	_this->listEnd->data = temp;
	_this->listEnd->next = nullptr;
}
void List_strSlice(List* _this, const char* src, const char* delim = " \n\r"){
	if (!_this) POINT_IS_NULL("point is NULL");

	char* buff = (char*)malloc(sizeof(char)*strlen(src)+1);
	strcpy(buff, src);
	for(char* pch = strtok(buff, delim); pch; pch = strtok(NULL, delim)){
		List_append(_this, pch);
	}
}

void List_ctor(List* _this, const char* filename){
	if (!_this) POINT_IS_NULL("point is NULL");

	char* contacts = nullptr;
	read_ContactsRaw(filename, &contacts);
	List_strSlice(_this, contacts);
	free(contacts);
}
void List_dtor(List* _this){
	if (!_this) POINT_IS_NULL("point is NULL");

	if(_this->listHead){
		List_basic* temp = nullptr;
		for(List_basic* l=_this->listHead->next; l; l=l->next){
			if(temp) free(temp);
			temp=l;
		} if(temp) free(temp);

		free(_this->listHead);
		_this->listHead = nullptr;
		_this->ListNum = 0;
	}
}

void List_getStr(List* _this, Str* (*dst), int* num){
	if (!_this) POINT_IS_NULL("point is NULL");

	char** temp = (char**)malloc(sizeof(char*)*(_this->ListNum-1));
	size_t idx = 0;
	List_basic* l=_this->listHead->next;
	for(;l; l=l->next){
		temp[idx] = (char*)malloc(sizeof(char)*strlen(l->data)+1);
		strcpy(temp[idx++], l->data);
	}

	*dst = temp;
	*num = _this->ListNum;
}

//==================================================================
auto Data_Slice(List* v) {
	//vector<List> data;
	vector<vector<string>> data;

	size_t item_len = 0, idx = 0, line_len=0;
	size_t end_mode = 0; // 0.補0;  1.補英文
	
	List_basic* l=v->listHead->next;
	Str currStr = nullptr;
	Str next_Str = nullptr;
	Str next2_Str = nullptr;
	// 補數字
	auto Append_Num = [&]() {
		if(!isalpha(next_Str[0])) { // 下一個是數字就接著補上
			++idx, l=l->next;
			data[line_len-1][data[line_len-1].size()-1] += next_Str;
		}
		--item_len;
	};

	// 開始處理
	for(; idx < (v->ListNum)-2; ++idx and l, l=l->next) {
		currStr   = l->data;
		next_Str  = l->next->data;
		next2_Str = l->next->next->data;
		// 是頭長度時
		if(item_len == 0){
			++line_len;
			item_len = stoi(currStr);
			data.push_back(vector<string> {currStr});
		}
		// 非頭長度是英文遞減
		else if(isalpha(currStr[0])) {
			data[line_len-1].push_back(currStr);
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
	if(item_len==1 and end_mode ==0) {
		// 補一組
		data[line_len-1].push_back(currStr);
		// 補數字
		Append_Num();
	} else {
		if(end_mode==0) {
			data.push_back(vector<string> {currStr}); // 補 0
		} else {
			++idx, l=l->next;
			data[line_len-1].push_back(next_Str); // 補英文
		}
	}

	return data;
}


//==================================================================
int main(int argc, char const *argv[]) {

	

	// 初始化數據
	List list{};
	list.listHead = (List_basic*)malloc(sizeof(List_basic));
	list.listEnd = list.listHead;
	list.ListNum = 0;
	
	// 載入文字(自動消除空格與跳行)
	List_ctor(&list, "str.txt");
	//List_print(&list);

	// 輸出整後後的陣列
	char** Contact = nullptr;
	int num = 0;
	List_getStr(&list, &Contact, &num);

	vector<string> v(num);
	// 印出
	for(size_t i = 0; i < num; i++){
		//cout << Contact[i] << endl;
		v[i] = string(Contact[i]);
	}
	
	// 解析格式
	auto data = Data_Slice(&list);
	//auto data = Data_Slice(v);
	// 查看二維陣列

	/*for(size_t j = 0; j < data.size(); j++){
		List* _this = &data[j];
		for(List_basic* l=_this->listHead->next; l; l=l->next){
			cout << l->data <<  ", ";
		} cout << endl;
	} cout << endl;*/
	for(auto&& j : data) {
		for(auto&& i : j) {
			cout << i << ", ";
		}cout << "---" << endl;
	}


	List_dtor(&list);
}