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
	//读取用户表到user
	//读取密码表到password
	pwdNum = read_f("password",password,0)/sizeof(struct password);
	//superBlock
	vD.readBlock(BLOCKSIZ, &super_block);
	login();
}