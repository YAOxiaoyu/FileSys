#include "virtualDisk.h"
#include "fileSys.h"
using namespace std;
void format(virtualDisk& vD) {
	
	for (int i = 0; i < DINODE_NUM; i++) {
		super_block.binode_bitmap[i] = 0;
	}
	super_block.s_ninode = DINODE_NUM;

	inode initInode[16];
	inode *iInode=ialloc();//TODO: 根目录i结点
	iInode->di_addr[0] = 33 * BLOCKSIZ;
	iInode->di_size = DIRSIZ + 2;
	iInode->di_mode = 0;
	iInode->di_number = 1;
	iInode->i_count = 0;
	iInode->i_ino = 0;//?
	initInode[0] = *iInode;
	vD.writeBlock(2 * blockSize,initInode);
	dir_item iDir[16];//根目录
	iDir[0].d_name[0] = '.';
	iDir[0].d_ino = 0;
	vD.writeBlock(33 * blockSize, iDir);


	/*超级块初始化*/
	int free_dblock = ALLBLOCKNUM - (2 + DINODEBLK);
	super_block.s_nfree = free_dblock - (free_dblock % 50 + 1);
	super_block.s_pfree = NICFREE - 1; // 堆栈指针

	for (int i = free_dblock / NICFREE; i > 0; i--) {
		if (i == free_dblock / NICFREE) {
			super_block.s_free[0] = 0;
		}
		else {
			super_block.s_free[0] = DATASTART + (i * NICFREE - 1) * BLOCKSIZ;
		}
		for (int j = 1; j < NICFREE + 1; j++) {
			super_block.s_free[j] =
				DATASTART + (i * NICFREE - (j + 1)) * BLOCKSIZ;
		}
		if (i == 1) {
			//只需记录在超级块中即可
			for (int m = 0; m < 1; m++)
				balloc();
		}
		else {
			//写到 DATASTART + ((i-1) * NICFREE-1) * BLOCKSIZ
			vD.writeBlock(blockSize, &super_block);//写回虚拟盘
			vD.writeBack();
		}
	}

	vD.writeBack();
}