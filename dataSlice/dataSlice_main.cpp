/*****************************************************************
Name : 讀取特定格式文件
Date : 2017/06/06
By   : CharlotteHonG
Final: 2018/02/10
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dataSlice.h"

//==================================================================
int main(int argc, char const *argv[]) {
	// 初始化數據
	List* list = List_new();

	// 載入文字(切割空格與跳行)
	List_loadFile(list, "str2.txt");
	//List_loadConsole(list);
	//List_print(list);

	// 解析格式
	int lenth = 0;
	List** dst = nullptr;
	Data_Slice(&dst, &lenth, list);

	// 查看二維陣列
	for(int j = 0; j < lenth; j++){
		ListNode* node = dst[j]->listHead->next;
		for(ListNode* l = node; l; l = l->next){
			printf("%s", l->data);
			if(l->next) printf(", ");
		} printf("-\n");
	} printf("\n");



	// 釋放內存
	if(dst){
		for(int i = 0; i < lenth; i++){
			List_delete(dst[i]);
		} free(dst);
	}
	if(list) List_dtor(list);
}