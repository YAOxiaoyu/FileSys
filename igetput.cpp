#include "fileSys.h"
#include<iostream>


using namespace std;

//inode打开表(系统文件打开表)
//vector<struct inode>inode_open;
//map<unsigned int,struct inode>inode_o;

//返回inode指向block地址
//返回指针
struct  inode *iget(unsigned int inode_id)
{
	long addr;
	//struct inode *temp;
    //判断是否已经调入
    if(inode_o.count(inode_id))
    {

        //引用计数+1
        inode_o[inode_id].i_count+=1;
        return &inode_o[inode_id];
    }

    struct inode *newinode=new inode;
    addr=DINODESTART +inode_id * DINODESIZ;
    //读入数据
	fseek(fd, addr, SEEK_SET);
	fread (&(newinode ->di_number), DINODESIZ, 1, fd);

    newinode->i_ino=inode_id;
    newinode->i_count=1;
	newinode->i_flag=0;
    inode_o[inode_id]=*newinode;
    return newinode;
};

void iput(unsigned int inode_id)
{
    unsigned int block_num;
    if(inode_o[inode_id].i_count>1){
        inode_o[inode_id].i_count--;
    } else {
        if(inode_o[inode_id].di_number!=0){ //还有其他用户的文件关联此inode，写回。
            fseek(fd,DINODESTART+inode_o[inode_id].i_ino*DINODESIZ,SEEK_SET);
            fwrite(&inode_o[inode_id].di_number,DINODESIZ,1,fd);
        } else {  //没有文件关联此inode,删除
            block_num=inode_o[inode_id].di_size/BLOCKSIZ;
            for(unsigned int j=0;j<block_num;j++){
                bfree(inode_o[inode_id].di_addr[j]);
            }
            ifree(inode_id);
        }
        inode_o.erase(inode_id);
        delete iget(inode_id);
    }
}
