#include"table.h"
#include<iostream>


using namespace std;

//inode打开表(系统文件打开表)
//vector<struct inode>inode_open;
map<unsigned int,struct inode>inode_o;

//返回inode指向block地址
//返回指针
struct  inode *iget(unsigned int inode_id)
{
	long addr;	
	struct inode *temp;
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
    inode_o[inode_id].i_count--;

    //TODO:写回操作在哪？
    if(inode_o[inode_id].i_count==0)
    {
        //没有用户在使用，释放
        inode_o.erase(inode_id);

        delete *iget(inode_id);
    }
}
