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
	activeUser.u_uid = 0000;
	//��ȡ�û���user
	//��ȡ�����password
	pwdNum = read_f("password",password);
	//superBlock
	login();
}