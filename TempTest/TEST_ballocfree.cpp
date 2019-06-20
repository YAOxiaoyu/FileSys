#include "../fileSys.h"
#include <iostream>

using namespace std;
struct super_block super_block;
int datas = 0;
int blocksi = 1;

void format_sb_freedi() {

    //总块数可以先忽略根目录,etc,pwd分配的物理磁盘块,最后再分配
    // int free_dblock = ALLBLOCKNUM - (2 + dinodeBLK);
    int free_dblock = 201;
    super_block.s_nfree = free_dblock;

    for (int i = free_dblock / NICFREE; i > 0; i--) {
        if (i == free_dblock / NICFREE) {
            // 最后一块
            super_block.s_free[0] = NICFREE - 1;
            super_block.s_free[1] = 0;
        } else {
            super_block.s_free[0] = NICFREE;
            super_block.s_free[1] = datas + (i * NICFREE - 1) * blocksi;
        }
        for (int j = 2; j < NICFREE + 1; j++) {
            super_block.s_free[j] = datas + (i * NICFREE - j) * blocksi;
        }
        if (i == 1) {
            super_block.s_free[0] = NICFREE - 3;
            super_block.s_nfree -= 3;
            //写到 datas + 3 *　NICFREE * blocksi
            // TODO 把第一个空闲组 -> 写到第4个盘

        } else {
            //写到 datas + ((i-1) * NICFREE-1) * blocksi
            // TODO 写回虚拟盘
        }
        cout << i << " : ";
        cout << super_block.s_free[0] << " ";
        cout << super_block.s_free[1] << " ";
        cout << super_block.s_free[2] << ".." << super_block.s_free[NICFREE];
        cout << endl;
    }
    // TODO 处理多出来的磁盘

    super_block.s_pfree = datas + 3 * blocksi;
}

unsigned int balloc() {
    /*
    从当前空闲块堆栈中返回一个当前空闲块id
    返回值为空闲块地址
    若返回值为-1,则说明虚拟盘中无空闲空间,分配失败
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
            /// TODO 读super_block
            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
            super_block.s_nfree--;
        } else {
            //当前块为最后一块,即最后一块也用完了
            super_block.s_pfree = 0;
            super_block.s_nfree--; //自减后应为0
        }
    }
    return free_block;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
     */
    super_block.s_nfree++;

    if ((super_block.s_pfree == NICFREE && super_block.s_free[1] != 0) ||
        super_block.s_pfree == NICFREE - 1 && super_block.s_free[1] == 0) {
        //此种状态下要将super.s_free[]写到虚拟盘中了
        //要写到的地址为block_num
        // TODO 写回磁盘

        //更新当前空闲块堆栈
        super_block.s_free[0] = 1;
        super_block.s_free[1] = block_num;
        super_block.s_pfree = super_block.s_free[0];
    } else {
        super_block.s_free[0]++;
        super_block.s_free[super_block.s_free[0]] = block_num;
        super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
    }

    // TODO 将blcok_num地址的内容清空
}

int main() {
    format_sb_freedi();
    cout << endl;
    for (int i = 0; i < 20; i++) {
        cout << balloc() << " ";
    }
    cout << endl;
    for (int i = 0; i < 1; i++) {
        bfree(5);
    }
    cout << balloc() << " ";
    cout << endl;
}
