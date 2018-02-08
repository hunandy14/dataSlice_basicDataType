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

vector<string> ReadFile(string file_name);
vector<vector<string>> Data_Slice(vector<string>& v);
void dataSlice1(){
#define READ_FILE
#ifdef READ_FILE
	auto list = ReadFile(file_name);
#else
	vector<string> list;
	for(string str; cin >> str;)
		list.emplace_back(str);
#endif // READ_FILE
	// 解析格式
	auto data = Data_Slice(list);
	// 查看二維陣列
	for(auto&& j : data) {
		for(auto&& i : j) {
			cout << i << ", ";
		}cout << endl;
	}
}


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
void List_print(List* list){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	for(List_basic* l=listHead->next; l; l=l->next){
		cout << l->data << endl;
	}
}
void List_append(List* list, const char* s){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	Str temp = new char[strlen(s)];
	strcpy(temp, s);

	listEnd->next = (List_basic*)malloc(sizeof(List_basic));
	listEnd = listEnd->next;
	++ListNum;

	listEnd->data = temp;
	listEnd->next = nullptr;
}
void List_strSlice(List* list, const char* src, const char* delim = " \n\r"){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	Str buff = (Str)malloc(sizeof(char)*strlen(src));
	strcpy(buff, src);
	for(char* pch = strtok(buff, delim); pch; pch = strtok(NULL, delim)){
		List_append(list, pch);
	}
}

void List_ctor(List* list, const char* filename){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	char* contacts = nullptr;
	read_ContactsRaw(filename, &contacts);
	List_strSlice(list, contacts);
	free(contacts);
}
void List_dtor(List* list){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	if(listHead->next){
		List_basic* temp = nullptr;
		for(List_basic* l=listHead->next; l; l=l->next){
			if(temp){
				free(temp);
			}
			temp=l;
		} 
		free(temp);

		listHead->next = nullptr;
		ListNum = 0;
	}
}

void List_getStr(List* list, Str** dst, int* num){
	List_basic*& listHead = list->listHead;
	List_basic*& listEnd = list->listEnd;
	int& ListNum = list->ListNum;

	Str* temp = (Str*)malloc(sizeof(Str)*(ListNum-1));
	size_t idx = 0;
	List_basic* l=listHead->next;
	for(;l; l=l->next){
		temp[idx] = (Str)malloc(sizeof(char)*strlen(l->data));
		strcpy(temp[idx++], l->data);
	}

	*dst = temp;
	*num = ListNum;
	List_dtor(list);
}


//==================================================================
Str** Data_Slice(const Str* v) {
	/*
	vector<vector<string>> data;
	size_t item_len = 0, idx = 0, line_len=0;
	size_t end_mode = 0; // 0.補0;  1.補英文
	
	// 補數字
	auto Append_Num = [&]() {
		if(!isalpha(v[idx + 1][0])) // 下一個是數字就接著補上
			data[line_len-1][data[line_len-1].size()-1] += v[++idx];
		--item_len;
	};

	// 開始處理
	for(; idx < v.size()-2; ++idx) {
		// 是頭長度時
		if(item_len == 0){
			++line_len;
			item_len = stoi(v[idx]);
			data.emplace_back(vector<string> {v[idx]});
		}
		// 非頭長度是英文遞減
		else if(isalpha(v[idx][0])) {
			data[line_len-1].emplace_back(v[idx]);
			if(item_len == 1) { // 最後一個檢查
				if(isalpha(v[idx + 2][0])) { // 最後一組缺數字
					--item_len;
					end_mode = 1;
					continue;
				}
			}
			Append_Num(); // 補數字
			end_mode = 0;
		}
	}
	// 結尾處理
	if(item_len==1 and end_mode ==0) {
		// 補一組
		data[line_len-1].emplace_back(v[idx]);
		Append_Num();
	} else {
		if(end_mode==0) {
			data.emplace_back(vector<string> {v[idx]}); // 補 0
		} else {
			data[line_len-1].emplace_back(v[++idx]); // 補英文
		}
	}
	return data;
	*/
	return nullptr;
}


//==================================================================
int main(int argc, char const *argv[]) {
	List list{};
	list.listHead = new List_basic{};
	list.listEnd = list.listHead;
	list.ListNum = 0;
	
	// 載入文字(自動消除空格與跳行)
	List_ctor(&list, "str.txt");
	List_print(&list);

	
	// 輸出整後後的陣列
	Str* Contact = nullptr;
	int num = 0;
	List_getStr(&list, &Contact, &num);
	vector<string> v(num);
	// 印出
	for(size_t i = 0; i < num; i++){
		//cout << Contact[i] << endl;
		v[i] = string(Contact[i]);
	}
	
	// 解析格式
	auto data = Data_Slice(v);
	// 查看二維陣列
	for(auto&& j : data) {
		for(auto&& i : j) {
			cout << i << ", ";
		}cout << endl;
	}
	
}