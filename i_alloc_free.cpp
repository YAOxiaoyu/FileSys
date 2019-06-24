#include <iostream>
#include "fileSys.h"

struct inode *ialloc()
{
	//在DINODE位图中顺序查找空闲的DINODE，找到后将其信息读入一个内存新建的inode，并返回。
	struct inode *temp=new inode;
	if (super_block.s_ninode == 0)
	{
		std::cout << "没有空闲inode可以分配\n";
		return NULL;
	}
	else
	{
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

void ifree(unsigned int inode_id)
{
	//释放inode
	if (super_block.binode_bitmap[inode_id] == 0)
	{
		std::cout << "该inode还未使用，无法释放\n";
		return;
	}
	super_block.s_ninode++;
	super_block.binode_bitmap[inode_id] = 0;

	//TODO 超级块内容写回磁盘以进行信息更新
	return;
}