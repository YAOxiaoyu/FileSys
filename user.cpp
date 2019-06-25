#include "fileSys.h"
#include<iostream>
#include<conio.h>
struct userListItem{
	char uName[20];
	unsigned short uId;
	unsigned short gId;
};

enum mode{READ,WRITE,X};
void login() {

}

void logout() {

}
void getPassword(char *password) {
	for (int i = 0; i < 20; i++) {
		password[i] = '\0';
	}
	char c;
	int i = 0;
	c = getch();
	while (c!=(char)13){
		if (i < PWDSIZ) {
			password[i++] = c;
			cout << '*';
		}
		else {
			break;
			//TODO:错误信息
		}
	}
}

void createUser(char uName[20],unsigned short uId,unsigned short gId) {
	userListItem userAdd;
	for (int i = 0; i < 20; i++) {
		userAdd.uName[i] = uName[i];
	}
	userAdd.uId = uId;
	userAdd.gId = gId;
	struct password pAdd;
	pAdd.p_uid = uId;
	pAdd.P_gid = gId;
	cout << "输入密码:";
	getPassword(pAdd.password);//TODO:密码规则
	//todo 写入用户列表，密码列表
}
bool access(inode* inode,struct user* user,mode m) {
	unsigned short acc= inode->di_mode;
	unsigned short key=0b111111111;
	if (user->u_uid == inode->di_uid) {
		//拥有者
		key = 0b111000000;
		acc = acc & key;
		acc /= 64;
	}
	else if (user->u_gid == inode->di_gid) {
		//同组
		key = 0b000111000;
		acc = acc & key;
		acc /= 8;
	}
	else {
		key = 0b000000111;
		acc = acc & key;
	}
	switch (m) {
	case READ:
		if(acc & 0b100>0)
			return true;
		break;
	case WRITE:
		if (acc & 0b010>0)
			return true;
		break;
	case X:
		if (acc & 0b001>0)
			return true;
	}
	return false;
}