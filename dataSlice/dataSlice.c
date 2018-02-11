/*****************************************************************
Name : Ū���S�w�榡���
Date : 2017/06/06
By   : CharlotteHonG
Final: 2018/02/10
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#define POINT_IS_NULL(msg) \
	printf("%s \t\t # %s::%d --> \"%s()\" \n" , \
		(msg) , __FILENAME__, __LINE__, __FUNCTION__)

#ifndef __cplusplus
#define nullptr NULL
#define or ||
#define and &&
#define OR ||
#define AND &&
#endif // !__cplusplus

#include "dataSlice.h"

//==================================================================
void List_basic_ctor(ListNode* _this, const char* s){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	char* buff = nullptr;
	if(s){
		buff = (char*)malloc(sizeof(char)*strlen(s)+1);
		strcpy(buff, s);
	}
	_this->data = buff;
	_this->next = nullptr;
}
void List_basic_dtor(ListNode* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	if(_this->data) free(_this->data);
	_this->data = nullptr;
	_this->next = nullptr;
}
ListNode* List_basic_new(const char* s){
	ListNode* _this = (ListNode*)malloc(sizeof(ListNode));
	List_basic_ctor(_this, s);
	return _this;
}
void List_basic_delete(ListNode* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	List_basic_dtor(_this);
	free(_this);
}
//------------------------------------------------------------------
void List_basic_append(ListNode* _this, const char* s) {
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	int buff_len = strlen(s) + strlen(_this->data);
	char* buff = (char*)malloc(sizeof(char)*buff_len + 1);
	strcpy(buff, _this->data);
	strcat(buff, s);

	if(_this->data) free(_this->data);
	_this->data = buff;
}


//==================================================================
void List_ctor(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	_this->listHead = List_basic_new(nullptr);
	_this->listEnd = _this->listHead;
	_this->ListNum = 0;
}
void List_dtor(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); }

	if(_this->listHead){
		// �����쵲�귽
		ListNode* node = nullptr;
		for(ListNode* l=_this->listHead->next; l; l=l->next){
			if(node)
				List_basic_delete(node);
			node=l;
		} List_basic_delete(node);
		List_basic_delete(_this->listHead);
	}

	// ���񥻦a�귽
	_this->listHead = nullptr;
	_this->listEnd = nullptr;
	_this->ListNum = 0;
}
List* List_new(){
	List* _this = (List*)malloc(sizeof(List));
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

	for(ListNode* l=_this->listHead->next; l; l=l->next){
		printf("%s, ", l->data);
	}
}
void List_append(List* _this, const char* s){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	ListNode* new_node = List_basic_new(s);
	_this->listEnd->next = new_node; // ��s�I���W
	_this->listEnd = new_node;       // ��s�����I
	++_this->ListNum;                // �֭p�p��
}
void List_strSlice(List* _this, const char* src, const char* delim){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	char* buff = (char*)malloc(sizeof(char)*strlen(src)+1);
	strcpy(buff, src);

	for(char* pch = strtok(buff, delim); pch; pch = strtok(NULL, delim)){
		List_append(_this, pch);
	}

	/*char *next = nullptr;
	char *pch  = strtok_s(buff, delim, &next);
	for(;pch; pch = strtok_s(nullptr, delim, &next)){
		List_append(_this, pch);
	} next = pch = nullptr;*/
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
	List_strSlice(_this, contacts, " \n\r\t");
	free(contacts);
}
void List_loadConsole(List* _this){
	if (!_this) { POINT_IS_NULL("point is NULL"); return; }

	for(char s[16]; ~scanf("%s",s);){
		List_append(_this, s);
	}
}


//==================================================================
void Data_Slice(List*** dst, int* lenth, const List* src) {
	if (!dst) { POINT_IS_NULL("point is NULL"); return; }
	if (*dst) { POINT_IS_NULL("point is invalid"); return; }

	List** temp_data = (List**)malloc(sizeof(List*) * src->ListNum);
	int item_len = 0, line_len=0;

	// �ɨ��V��Ū�����w��
	List_append((List*)src, "");
	List_append((List*)src, "");

	// �}�l�B�z
	for(ListNode* l = src->listHead->next; l->next->next; l=l->next) {
		const char* currStr = l->data;
		const char* nextStr = l->next->data;
		const char* next2_Str = l->next->next->data;
		// �O�}�Y���׮ɷs�W�쵲
		if(item_len == 0){
			char buf[32];
			item_len = atoi(currStr);
			sprintf(buf,"%d",item_len);
			temp_data[line_len] = List_new();
			List_append(temp_data[line_len], buf);
			++line_len;
		}
		// �D�}�Y���׮ɻ���ءA�B�s�W�^��P�Ʀr
		else if(isalpha(currStr[0])) {
			List_append(temp_data[line_len-1], currStr);
			// �̫�@�Ӷ����ˬd����
			if(item_len == 1) {
				// ���p�̫�@�լO�^��
				if(isalpha(next2_Str[0])) {
					--item_len;
					continue;
				}
			}
			// �U�@�ӬO�Ʀr�N�ɤW
			if(!isalpha(nextStr[0])) {
				l=l->next;
				List_basic_append(temp_data[line_len-1]->listEnd, nextStr);
			}
			--item_len;
		}
	}
	// ��X�ƾ�
	*lenth = line_len;
	*dst = (List**)malloc(sizeof(List*) * line_len);
	for(int i = 0; i < line_len; i++){
		(*dst)[i] = temp_data[i]; // move
	} if(temp_data) free(temp_data);
}
void use_DataSlice(const char* filename){
	// ��l�Ƽƾ�
	List* list = List_new();

	// ���J��r(���ΪŮ�P����)
	if(filename){
		List_loadFile(list, filename);
	} else{
		List_loadConsole(list);
	}
	//List_print(list);

	// �ѪR�榡
	int lenth = 0;
	List** dst = nullptr;
	Data_Slice(&dst, &lenth, list);

	// �d�ݤG���}�C
	for(int j = 0; j < lenth; j++){
		ListNode* node = dst[j]->listHead->next;
		for(ListNode* l = node; l; l = l->next){
			printf("%s", l->data);
			if(l->next) printf(", ");
		} printf("-\n");
	} printf("\n");



	// ���񤺦s
	if(dst){
		for(int i = 0; i < lenth; i++){
			List_delete(dst[i]);
		} free(dst);
	}
	if(list) List_dtor(list);
}