#include "fileSys.h"
#include <iostream>
#include <fstream>
using namespace std;

void onload() {
	fstream viDisk;
	viDisk.open(virtualDName, ios::in);
	if (!viDisk) {
		//����δ����
		vD.newDisk(ALLBLOCKNUM*BLOCKSIZ);
		format(vD);
	}
	else {
		viDisk.close();
	}
	//��ȡ�û���user
	//��ȡ�����password
	pwdNum = read_f("password",password,0)/sizeof(struct password);
	//superBlock
	vD.readBlock(BLOCKSIZ, &super_block);
	login();
}