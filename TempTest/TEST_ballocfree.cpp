

#include "../fileSys.h"
#include <iostream>

using namespace std;
struct super_block super_block;

struct super_block sub[300];

int datas = 0;
int blocksi = 1;

void format_sb_freedi() {

    //总块数可以先忽略根目录,etc,pwd分配的物理磁盘块,最后再分配
    // int free_dblock = ALLBLOCKNUM - (2 + dinodeBLK);

    int free_dblock = 100;

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
        } else {
            //写到 datas + ((i-1) * NICFREE-1) * blocksi
            // TODO 写回虚拟盘
            //模拟写回虚拟盘

            sub[datas + ((i - 1) * NICFREE - 1) * blocksi] = super_block;
        }
        // cout << i << " : ";
        // cout << super_block.s_free[0] << " ";
        // cout << super_block.s_free[1] << " ";
        // cout << super_block.s_free[2] << ".." << super_block.s_free[NICFREE];
        // cout << endl;
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
        if (super_block.s_free[1] == 0) {
            super_block.s_pfree = super_block.s_free[super_block.s_free[0] + 1];
            // super_block.s_free[super_block.s_free[0] + 1] = 0;
        } else {

            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
            // super_block.s_free[super_block.s_free[0]] = 0;
        }
    } else {
        if (super_block.s_free[1] != 0) {
            //如果当前不是最后一块
            //读super_block.s_free[1]处的block并付给super_block.sfree[];
            // TODO 读super_block[1]

            //模拟读
            // super_block = sub[super_block.s_free[1]];
            struct super_block temp = sub[super_block.s_free[1]];
            for (int i = 0; i < NICFREE + 1; i++) {
                super_block.s_free[i] = temp.s_free[i];
            }

            super_block.s_pfree = super_block.s_free[super_block.s_free[0] + 1];
        } else {
            //当前块为最后一块,即最后一块也用完了
            super_block.s_pfree = 0;
        }
    }

    super_block.s_nfree--;
    return free_block;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
     */
    super_block.s_nfree++;

    if ((super_block.s_free[0] == NICFREE && super_block.s_free[1] != 0) ||
        super_block.s_free[0] == NICFREE - 1 && super_block.s_free[1] == 0) {
        //此种状态下要将super.s_free[]写到虚拟盘中了
        //要写到的地址为block_num
        // TODO 写回磁盘

        //模拟写
        sub[block_num] = super_block;

        //更新当前空闲块堆栈
        super_block.s_free[0] = 1;
        super_block.s_free[1] = block_num;
        if (super_block.s_free[1] != 0)
            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
        else
            super_block.s_pfree = super_block.s_free[super_block.s_free[0] + 1];
    } else {
        super_block.s_free[0]++;
        if (super_block.s_free[1] != 0) {
            super_block.s_free[super_block.s_free[0]] = block_num;
            super_block.s_pfree = super_block.s_free[super_block.s_free[0]];
        } else {
            super_block.s_free[super_block.s_free[0] + 1] = block_num;
            super_block.s_pfree = super_block.s_free[super_block.s_free[0] + 1];
        }
    }

    // TODO 将blcok_num地址的内容清空
}

void outDisk() {
    cout << endl;
    cout << "------------------------------------------------------------------"
            "------"
         << endl;

    cout << "sb:";

    for (int i = 0; i < NICFREE + 1; i++) {
        cout << super_block.s_free[i] << " ";
    }
    cout << endl;

    for (int k = 0; k < 300; k++) {
        if (sub[k].s_free[0] != 0) {
            cout << k << ":";
            for (int i = 0; i < NICFREE + 1; i++) {
                cout << sub[k].s_free[i] << " ";
            }
            cout << endl;
        }
    }
    cout << "------------------------------------------------------------------"
            "------"
         << endl;
}

int main() {
    format_sb_freedi();

    outDisk();

    cout << endl;
    cout << super_block.s_nfree << endl;
    for (int i = 0; i < 96; i++) {
        cout << balloc() << " ";
    }

    outDisk();
    cout << super_block.s_nfree << endl;

    // bfree(5);

    // outDisk();

    // bfree(20);

    // outDisk();

    // cout << endl;
    // cout << balloc() << " ";

    cout << endl;
}
