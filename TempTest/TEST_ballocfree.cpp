

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
    super_block.s_nfree = free_dblock - (free_dblock % 50 + 1);
    super_block.s_pfree = NICFREE - 1; // 堆栈指针

    for (int i = free_dblock / NICFREE; i > 0; i--) {
        if (i == free_dblock / NICFREE) {
            // 最后一块
            super_block.s_free[0] = 0;
        } else {
            super_block.s_free[0] = datas + (i * NICFREE - 1) * blocksi;
        }

        for (int j = 1; j < NICFREE + 1; j++) {
            super_block.s_free[j] = datas + (i * NICFREE - (j + 1)) * blocksi;
        }

        if (i == 1) {
            for (int m = 0; m < 3; m++)
                balloc();
        } else {
            //写到 datas + ((i-1) * NICFREE-1) * blocksi
            sub[datas + ((i - 1) * NICFREE - blocksi) * blocksi] = super_block;
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
            struct super_block temp =
                sub[super_block.s_free[super_block.s_pfree]];
            for (int m = 0; m < NICFREE; m++) {
                super_block.s_free[m] = temp.s_free[m];
            }
            super_block.s_pfree = NICFREE - 1;
        }
    } else {
        super_block.s_pfree--;
    }

    super_block.s_nfree--;
    super_block.s_fmod = 1;

    return free_block;
}

void bfree(unsigned int block_num) {
    /*
    释放一个空闲块,加入到当前空闲块堆栈中
    注意在每次调用该函数后,在使用该函数的函数结束时应检查超级块修改标志并写回虚拟盘
     */

    super_block.s_nfree++;
    if (super_block.s_pfree == NICFREE - 1) {
        //写回block_num
        // TODO 写回block_num
        sub[block_num] = super_block;
        //更新当前sb
        super_block.s_pfree = 0;
        super_block.s_free[super_block.s_pfree] = block_num;
    } else {
        super_block.s_pfree++;
        super_block.s_free[super_block.s_pfree] = block_num;
    }
}

void outDisk() {
    cout << endl;
    cout << "------------------------------------------------------------------"
            "------"
         << endl;

    cout << "sb:"
         << "(" << super_block.s_pfree << ")";

    for (int i = 0; i < NICFREE; i++) {
        cout << super_block.s_free[i] << " ";
    }
    cout << endl;

    for (int k = 0; k < 300; k++) {
        if (sub[k].s_free[1] != 0) {
            cout << k << ":"
                 << "(" << sub[k].s_pfree << ")";

            for (int i = 0; i < NICFREE; i++) {
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
    cout << "balloc:";
    for (int i = 0; i < 97; i++) {
        unsigned int k =  balloc();
        if(k == -1){
            cout << "-1 ";
        }
        else{
            cout << k << " ";
        }
    }
    cout << endl;
    outDisk();

    bfree(5);
    bfree(20);
    bfree(12);

    outDisk();

    cout << endl;
    cout << "balloc:";
    for (int i = 0; i < 1; i++) {
        cout << balloc() << " ";
    }
    cout << endl;
    outDisk();
}
