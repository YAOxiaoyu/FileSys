#include "fileSys.h"
#include <iostream>

using namespace std;
struct super_block super_block;

void format_sb_freedi() {

    //总块数可以先忽略根目录,etc,pwd分配的物理磁盘块,最后再分配
    int free_dblock = ALLBLOCKNUM - (2 + dinodeBLK);
    super_block.s_nfree = free_dblock;

    for (int i = free_dblock / NICFREE; i > 0; i--) {
        if (i == free_dblock / NICFREE) {
            // 最后一块
            super_block.s_free[0] = NICFREE - 1;
            super_block.s_free[1] = 0;
        } else {
            super_block.s_free[0] = NICFREE;
            super_block.s_free[1] = DATASTART + (i * NICFREE - 1) * BLOCKSIZ;
        }
        for (int j = 2; j < NICFREE + 1; j++) {
            super_block.s_free[j] = DATASTART + (i * NICFREE - j) * BLOCKSIZ;
        }
        if (i == 1) {
            //只需记录在超级块中即可
            super_block.s_free[0] = NICFREE - 3;
            super_block.s_nfree -= 3;
        } else {
            //写到 DATASTART + ((i-1) * NICFREE-1) * BLOCKSIZ
            // TODO 写回虚拟盘
        }
    }
    // TODO 处理多出来的磁盘

    super_block.s_pfree = DATASTART + 3 * BLOCKSIZ;
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

    unsigned int free_block;

    free_block = super_block.s_pfree;

    if (super_block.s_free[0] != 1) {
        super_block.s_free[0]--;
        if (super_block.s_free[1] == 0)
            super_block.s_pfree = super_block.s_free[super_block.s_free[0] + 1];
        else
            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
        super_block.s_nfree--;
    } else {
        if (super_block.s_free[1] != 0) {
            //如果当前不是最后一块
            //读super_block.s_free[1]处的block并付给super_block.sfree[];
            /// TODO 读super_block[1]
            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
            super_block.s_nfree--;
        } else {
            //当前块为最后一块,即最后一块也用完了
            super_block.s_pfree = 0;
            super_block.s_nfree--; //自减后应为0
        }
    }

    super_block.s_fmod = 1;

    return free_block;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
    注意在每次调用该函数后,在使用该函数的函数结束时应检查超级块修改标志并写回虚拟盘
     */
    super_block.s_nfree++;

    if ((super_block.s_pfree == NICFREE && super_block.s_free[1] != 0) ||
        super_block.s_pfree == NICFREE - 1 && super_block.s_free[1] == 0) {
        //此种状态下要将super.s_free[]写到虚拟盘中了
        //要写到的地址为block_num
        //TODO 写回磁盘
        
        //更新当前空闲块堆栈
        super_block.s_free[0] = 1;
        super_block.s_free[1] = block_num;
        super_block.s_pfree = super_block.s_free[0];
    }else{
        super_block.s_free[0]++;
        super_block.s_free[super_block.s_free[0]] = block_num;
        super_block.s_pfree = super_block.s_free[0];
    }

    super_block.s_fmod = 1;

    //TODO 将blcok_num地址的内容清空
}

int main() { format_sb_freedi(); }
