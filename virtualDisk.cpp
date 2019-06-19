#include "virtualDisk.h"
#include <iostream>
using namespace std;

virtualDisk::virtualDisk(){
    curDisk.open(virtualDName,ios::in|ios::out|ios::binary);
}

virtualDisk::~virtualDisk(){
    curDisk.close();
}

void virtualDisk::writeBack() {
	curDisk.close();
	curDisk.open(virtualDName, ios::in | ios::out | ios::binary);
}
void virtualDisk::newDisk(int size){//新建虚拟磁盘
    diskSize=size;
    block initBlock;
    ofstream vDisk;
    vDisk.open(virtualDName,ios::out|ios::binary);
    for(int dSize=diskSize;dSize>0;dSize-=blockSize){
        vDisk.write((char*)&initBlock,blockSize);
    }
    vDisk.close();
}
/*块编号应为块起始字节编号 */
bool virtualDisk::readBlock(int blockPosition,void* target){//读取块
    //越界问题需要解决
	curDisk.clear();
    curDisk.seekg(blockPosition,ios::beg);
    //block* blockRead=new block;//内存泄漏
    curDisk.read((char*)target,blockSize);
    //return blockRead;
	return true;
}
bool virtualDisk::writeBlock(int blockPosition,void* blockSource){//写入块
	curDisk.clear();
    curDisk.seekp(blockPosition,ios::beg);
	//可能越界
    curDisk.write((char*)blockSource,blockSize);
	return true;
}