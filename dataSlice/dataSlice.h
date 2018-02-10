/*****************************************************************
Name : 讀取特定格式文件
Date : 2017/06/06
By   : CharlotteHonG
Final: 2018/02/10
*****************************************************************/
#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct ListNode ListNode;
struct ListNode{
	char* data;
	ListNode* next;
};
typedef struct List List;
struct List{
	ListNode* listHead;
	ListNode* listEnd;
	int ListNum;
};


//==================================================================
void List_basic_ctor(ListNode * _this, const char * s);
void List_basic_dtor(ListNode * _this);
ListNode * List_basic_new(const char * s);
void List_basic_delete(ListNode * _this);
void List_basic_append(ListNode * _this, const char * s);
//==================================================================
void List_ctor(List * _this);
void List_dtor(List * _this);
List * List_new();
void List_delete(List * _this);
void List_print(List * _this);
void List_append(List * _this, const char * s);
//------------------------------------------------------------------
void List_strSlice(List * _this, const char * src, const char * delim);
void read_ContactsRaw(const char * filename, char ** buf);
void List_loadFile(List * _this, const char * filename);
void List_loadConsole(List * _this);
void Data_Slice(List *** dst, int * lenth, const List * src);
//==================================================================


#ifdef  __cplusplus
}
#endif