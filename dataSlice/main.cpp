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


int dataSlice2(const char* argv);


void read_ContactsRaw(const char* filename, char** buf) {
	if(filename==nullptr and buf!=nullptr){
		perror("Error read_ContactsRaw input");
	}

	FILE *pFile = fopen(filename,"rb");
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);

	*buf = (char*) malloc (sizeof(char)*lSize);
	if (*buf == nullptr) {fputs ("Memory error",stderr); exit (2);}

	fread(*buf, 1, lSize, pFile);
	fclose(pFile);
}
//==================================================================
typedef char* Str;
struct List{
	Str data;
	List* next;
};
static List* const listHead = new List{};
static List* listEnd = listHead;
static size_t ListNum = 0;

void List_append(const char* s){
	Str temp = new char[strlen(s)];
	strcpy(temp, s);

	listEnd->next = (List*)malloc(sizeof(List));
	listEnd = listEnd->next;
	++ListNum;

	listEnd->data = temp;
	listEnd->next = nullptr;
}
void List_print(){
	for(List* l=listHead->next; l; l=l->next){
		cout << l->data << endl;
	}
}
void List_strSlice(const char* src, const char* delim = " \n"){
	Str buff = new char[strlen(src)];
	strcpy(buff, src);
	for(char* pch = strtok(buff, delim); pch; pch = strtok(NULL, delim)){
		List_append(pch);
	}
}

void List_ctor(const char* filename){
	char* contacts = nullptr;
	read_ContactsRaw(filename, &contacts);
	List_strSlice(contacts);
	free(contacts);
}
void List_dtor(){
	List* temp = nullptr;
	if(listHead->next){
		for(List* l=listHead->next; l; temp=l=l->next){
			if(temp){
				free(temp);
			}
		} 
		//free(temp);

		listHead->next = nullptr;
		//ListNum = 0;
	}
}

void List_getStr(Str** dst, int* num){
	Str* temp = (Str*)malloc(sizeof(Str)*(ListNum-1));
	size_t idx = 0;
	List* l=listHead->next;
	for(;l; l=l->next){
		temp[idx] = (Str)malloc(sizeof(char)*strlen(l->data));
		strcpy(temp[idx++], l->data);
	}

	*dst = temp;
	*num = ListNum;
}
/*enum listHead;
enum listEnd;
enum LisuNum;*/
//==================================================================
int readFiles();
int main(int argc, char const *argv[]) {
	List_ctor("str.txt");
	//List_print();
	Str* Contact = nullptr;
	int num = 0;
	//List_getStr(&Contact, &num);

	for(size_t i = 0; i < num; i++){
		//cout << Contact[i] << endl;
	}

	List_dtor();
}