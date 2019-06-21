#include "fileSys.h"
#include <iostream>

using namespace std;
struct super_block super_block;

void format_sb_freedi() {

    //总块数可以先忽略根目录,etc,pwd分配的物理磁盘块,最后再分配
    int free_dblock = ALLBLOCKNUM - (2 + dinodeBLK);
    super_block.s_nfree = free_dblock;
    super_block.s_pfree = NICFREE - 1; // 堆栈指针

    for (int i = free_dblock / NICFREE; i > 0; i--) {
        if (i == free_dblock / NICFREE) {
            // 最后一块
            super_block.s_free[0] = 0;
        } else {
            super_block.s_free[0] = DATASTART + (i * NICFREE - 1) * BLOCKSIZ;
        }
        for (int j = 1; j < NICFREE + 1; j++) {
            super_block.s_free[j] =
                DATASTART + (i * NICFREE - (j + 1)) * BLOCKSIZ;
        }
        if (i == 1) {
            //只需记录在超级块中即可
            for (int m = 0; m < 3; m++)
                //分配初始化用掉的数据块
                balloc()
        } else {
            //写到 DATASTART + ((i-1) * NICFREE-1) * BLOCKSIZ
            // TODO 写回虚拟盘
        }
    }
    // TODO 处理多出来的磁盘
}

unsigned int balloc() {
    /*
    从当前空闲块堆栈中返回一个当前空闲块id
    返回值为空闲块地址
    若返回值为-1,则说明虚拟盘中无空闲空间,分配失败
    注意在每次调用该函数后,在使用该函数的函数结束时应检查超级块修改标志并写回虚拟盘
     */

    if (super_block.s_nfree == 0) {
        return -1;
    }

    unsigned int free_block = super_block.s_free[super_block.s_pfree];

    if (super_block.s_pfree == 0) {
        if (super_block.s_free[super_block.s_pfree] != 0) {
            //当前空闲块堆栈不为最后一组
            //是最后一组的话就说明没空间了,也没啥要干的了,nfree-1后为0即可
            // TODO 读super_block[s_pfree]
            super_block.s_pfree = NICFREE - 1;
        }
    } else {
        super_block.s_pfree--;
    }

    super_block.s_nfree--;
    return free_block;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
    注意在每次调用该函数后,在使用该函数的函数结束时应检查超级块修改标志并写回虚拟盘
     */

    super_block.s_nfree++;
    if()
}