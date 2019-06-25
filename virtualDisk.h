#ifndef _VIRTUALDISK_H
#define _VIRTUALDISK_H
#include <fstream>
#define blockSize 512
//B
#define virtualDName "thisFileSystem.lfs"

struct block{
    char data[512];
};

class virtualDisk{
private:
    std::fstream curDisk;
    //ofstream curDiskW;
    int diskSize;
public:
    virtualDisk();
    ~virtualDisk();
    void newDisk(int);//创建虚拟磁盘
	void writeBack();//写回物理磁盘
    bool readBlock(int,void*);//读取块
    bool writeBlock(int,void*);//写入块
    bool writeAAddr(int,int,void*);//在特定地址处写
};

#endif