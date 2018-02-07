/*****************************************************************
Name : C �ɮ�<->���c Ū�g
Date : 2017/06/17
By   : CharlotteHonG
Final: 2017/06/17
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int No;
	char name [20];
	char phoneNumber[30];
	char Email_address[45];
} Contact;
// �s�W�@�����
void app_Person(const char* filename, Contact* con) {
	char lf[]="\n";
	char tok[]=",";

	char temp[32];
	_itoa(con->No, temp, 10);

	FILE *pFile = fopen(filename,"ab+");
	fwrite(temp, sizeof(char), strlen(temp), pFile);
	fwrite(tok, sizeof(char), strlen(tok), pFile);
	fwrite(con->name, sizeof(char), strlen(con->name), pFile);
	fwrite(tok, sizeof(char), strlen(tok), pFile);
	fwrite(con->phoneNumber, sizeof(char), strlen(con->phoneNumber), pFile);
	fwrite(tok, sizeof(char), strlen(tok), pFile);
	fwrite(con->Email_address, sizeof(char), strlen(con->Email_address), pFile);
	fwrite(lf, sizeof(char), strlen(lf), pFile);
	fclose(pFile);
}
// �g�J�q�T��
void write_Contacts(const char* filename ,Contact* con, size_t len) {
	FILE *pFile = fopen(filename,"wb+");
	if(!pFile) {
		perror("Error opening file");
		exit(1);
	}
	fclose(pFile);
	for(unsigned i = 0; i < len; ++i) {
		app_Person(filename, con+i);
	}
}
// Ū���q�T��
void read_Contacts(const char* filename ,Contact* con, size_t len) {
	FILE *pFile = fopen(filename,"rb");
	if(!pFile) {
		perror("Error opening file");
		exit(1);
	}
	for(char buf[128], *pch; fgets(buf, 128, pFile) != NULL; ++con) {
		pch = strtok (buf,",");
		con->No = atoi(pch);
		pch = strtok (NULL, ",");
		strcpy(con->name, pch);
		pch = strtok (NULL, ",");
		strcpy(con->phoneNumber, pch);
		pch = strtok (NULL, ",");
		strcpy(con->Email_address, pch);
	}
	fclose(pFile);
}
// Ū���q�T��RAW
void read_ContactsRaw(const char* filename, char* buf) {
	FILE *pFile = fopen(filename,"rb");
	if(!pFile) {
		perror("Error opening file");
		exit(1);
	}
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	fread(buf, 1, lSize, pFile);
	fclose(pFile);
}
// �L�X���c
void pri_person(Contact* con){
	printf("%d, %s, %s, %s", con->No, con->name, con->phoneNumber, con->Email_address);
}
void pri_Contacts(Contact* con, size_t len){
	for(unsigned i = 0; i < len; ++i) {
		pri_person(con+i);
	}
}
// �L�X�q�T��
int readFiles() {
	Contact w_con[2] = {
		{1, "AAA", "0912345678", "AAA@gmail.com"},
	{2, "BBB", "0912345678", "BBB@gmail.com"}
	};
	// �g�J�q�T��
	write_Contacts("Contact.txt", w_con, 2);
	// Ū���q�T��
	Contact r_con[2];
	read_Contacts("Contact.txt", r_con, 2);
	pri_Contacts(r_con, 2);
	// Ū���ɮ�
	char buf[256];
	read_ContactsRaw("Contact.bin", buf);
	printf("buf = \n%s\n", buf);

	return 0;
}