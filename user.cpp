#include "fileSys.h"
#include<iostream>
#include<string>
#include<sstream>
#include<conio.h>
using namespace std;

bool login() {
	//u_default_mode
	struct user userLogging;
	cin >> userLogging.u_uid;
	struct password pLogging;
	pLogging.p_uid = userLogging.u_uid;
	getPassword(pLogging.password);
	int i;
	for (i = 0; i < PWDNUM;i++) {//ƥ������
		if (password[i].p_uid = pLogging.p_uid) {
			for (int j = 0; j < PWDSIZ; j++) {
				if (password[i].password[i] != pLogging.password[i]) {
					cout << "password error!" << endl;
					return false;
				}
			}
			break;
		}
	}
	if (i==PWDNUM) {
		return false;
	}

	vector<struct user>::iterator findUser = userLogin.begin();
	while (findUser!=userLogin.end()){
		if (findUser->u_uid = userLogging.u_uid) {//�ѵ�¼���л��û�
			activeUser->u_ofile = inode_user_o;
			activeUser = findUser.operator*;
			inode_user_o = activeUser->u_ofile;
			break;
		}
		findUser++;
	}
	if (findUser == userLogin.end()) {//δ��¼
		userLogin.push_back(userLogging);
		activeUser =&userLogin.back();
	}
	return true;
}

void deleteUser() {

}

void logout(struct user userQuit) {
	//�ر�ȫ���ļ�
	vector<struct user>::iterator findUser=userLogin.begin();
	while (findUser != userLogin.end()) {
		if (findUser->u_uid == userQuit.u_uid) {
			userLogin.erase(findUser);
			break;
		}
		findUser++;
	}
	if (findUser == userLogin.end()) {
		cout << "error" << endl;
		return;
	}
	get_dir("home");
	login();
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
			//TODO:������Ϣ
		}
	}
}

void createUser(unsigned short uId,unsigned short gId) {
	userListItem userAdd;
	/*for (int i = 0; i < 20; i++) {
		userAdd.uName[i] = uName[i];
	}*/
	userAdd.uId = uId;
	userAdd.gId = gId;
	struct password pAdd;
	pAdd.p_uid = uId;
	pAdd.P_gid = gId;
	cout << "��������:";
	getPassword(pAdd.password);//TODO:�������
	if (pwdNum >= PWDNUM) {
		cout << "�û�����" << endl;
		return;
	}
	userList.push_back(userAdd);//д���û��б������б�
	password[pwdNum]=pAdd;
	get_dir("home");
	stringstream s;
	string uIdS;
	s << uId;
	s >> uIdS;
	mkdir(uIdS);
	//TODO:�ص�ԭĿ¼
}

bool access(inode* inode,struct user* user,mode m) {
	if (user->u_uid = 0000) {
		return true;
	}
	unsigned short acc= inode->di_mode;
	unsigned short key=0b111111111;
	if (user->u_uid == inode->di_uid) {
		//ӵ����
		key = 0b111000000;
		acc = acc & key;
		acc /= 64;
	}
	else if (user->u_gid == inode->di_gid) {
		//ͬ��
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