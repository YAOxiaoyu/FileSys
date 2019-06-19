#include "fileSys.h"
#include <iostream>

using namespace std;
struct super_block super_block;

void format_sb_freedi() {

    //总块数可以先忽略根目录,etc,pwd分配的物理磁盘块,最后再分配
    // int free_dblock = ALLBLOCKNUM - (2 + dinodeBLK);
    int free_dblock = 101;
    //一共这么多个空闲数据块
    super_block.s_nfree = free_dblock;

    //初始化空闲块
    //从最后一块开始分组
    for (int i = free_dblock / NICFREE; i > 0; i--) {
        if (i == free_dblock / NICFREE) {
            // 最后一块
            super_block.s_free[0] = NICFREE - 1;
            super_block.s_free[1] = -1;
        } else {
            super_block.s_free[0] = NICFREE;
            super_block.s_free[1] =
                0 + (i * NICFREE - 1) * 1; //?改了 //指向下一个记录空闲块的物理块
        }
        for (int j = 2; j < NICFREE + 1; j++) {
            super_block.s_free[j] = 0 + (i * NICFREE - (j)) * 1; //?改了
        }
        if (i == 1) {
            //第一个磁盘块
            //写到第4个block
            super_block.s_free[0] = NICFREE - 3;
            super_block.s_nfree -= 3; // 分配了三个,减3
            // TODO 把第一个空闲组 -> 写到第4个盘
            // TODO 写回虚拟盘

        } else {
            //写到 0 + (i-1) * NICFREE * 1
            int k = 0 + ((i-1) * NICFREE-1) * 1;//?改了
            cout << k;
            // TODO 写回虚拟盘
        }
    }
    // TODO 多出来的多余的记录到哪里?

    //因为前三个已经被占用,所以空闲块堆栈指针指向当前空的第一个数据块->即第4个
    super_block.s_pfree = 0 + 3 * 1;
}

unsigned int balloc() {
    /*
    从当前空闲块堆栈中返回一个当前空闲块id
     */
    unsigned free_block, free_block_num;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
     */
}

int main() {
    format_sb_freedi();
    return 0;
}
