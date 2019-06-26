#include "fileSys.h"
#include <iostream>
#include <fstream>
using namespace std;

void onload() {
	fstream viDisk;
	viDisk.open(virtualDName, ios::in);
	if (!viDisk) {
		//磁盘未创建
		vD.newDisk(ALLBLOCKNUM*BLOCKSIZ);
		format(vD);
	}
	else {
		viDisk.close();
	}
	activeUser.u_uid = 0000;
	//读取用户表到user
	//读取密码表到password
	pwdNum = read_f("password",password);
	//superBlock
	login();
}