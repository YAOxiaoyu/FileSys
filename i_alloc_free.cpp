#include <iostream>
#include "fileSys.h"
/*
下面函数建立在头文件进行如下修改情况下编写
#define DINODE_NUM DINODEBLK*BLOCKSIZ/DINODESIZ
struct super_block {
    
    //unsigned short s_isize;       //索引节点块块数
    unsigned long s_fsize;        //数据块块数

    unsigned int s_nfree;         //空闲块块数
    unsigned short s_pfree;       //空闲块指针
    unsigned int s_free[NICFREE]; //空闲块堆栈

    unsigned int s_ninode;         //空闲索引节点数
    //unsigned short s_pinode;       //空闲索引节点指针
    //unsigned int s_inode[NICINOD]; //空闲索引节点数组
    //unsigned int s_rinode;         //铭记索引节点
    unsigned short binode_bitmap[DINODE_NUM];
    char s_fmod; //超级块修改标志
};
 */
struct inode *ialloc()
{
	//在DINODE位图中顺序查找空闲的DINODE，找到后将其信息读入一个内存新建的inode，并返回。
	struct inode *temp;
	if (super_block.s_ninode == 0)
	{
		std::cout << "没有空闲inode可以分配\n";
		return NULL;
	}
	else
	{
		temp = (struct inode *)malloc(sizeof(struct inode));
		//顺序查找空闲的DINODE
		int i;
		for (i = 0; i < DINODE_NUM; i++)
		{
			if (super_block.binode_bitmap[i] == 0) //找到空闲DINODE
				break;
		}

		super_block.s_ninode--;
		super_block.binode_bitmap[i] = 1;

		//TODO 超级块内容写回磁盘以进行信息更新

		temp->i_ino = i;
		temp->di_number = 0;
		temp->di_size = 0;
		return temp;
	}
}

void ifree(struct inode *temp)
{
	//释放inode
	if (super_block.binode_bitmap[temp->i_ino] == 0)
	{
		std::cout << "该inode还未使用，无法释放\n";
		return;
	}
	super_block.s_ninode++;
	super_block.binode_bitmap[temp->i_ino] = 0;

	//TODO 超级块内容写回磁盘以进行信息更新
	return;
}