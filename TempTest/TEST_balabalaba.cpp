#include <iostream>
#include <string.h>
#include <string>

using namespace std;

int main() {
    char s[10] = "";
    string ss = "12345";
    strcpy(s, ss.c_str());
    cout << s;

    char a[10] = "abcde";
    string sss;
    sss.assign(a);
    cout << sss;
}


//备份一下
// FUCK 西湖的水我的泪
// fuuuuuuuuuuuuuuuuuuuuuuuuuck
//计算当前文件剩余大小
// int remain = FILEMAXSIZE - file_inode->di_size;
// char text_buf[remain];
// int text_length = 0;
// bool is_max = false;
// while ((c = getchar()) != "#") {
//     if (length >= remain) {
//         is_max = true;
//         break;
//     }
//     text_buf[text_length++] = c;
// }

// //文件最后一块实际占用的空间
// int last_block_used;
// if (file_inode->di_size % BLOCKSIZ != 0)
//     last_block_used = file_inode->di_size % BLOCKSIZ;
// else
//     last_block_used = blockSize;

// //文件占用block数
// int block_num = file_inode->di_size / BLOCKSIZ + 1 : 0
//     ? file_inode->di_size % BLOCKSIZ;

// //文件最后的block里的剩余空间
// int last_block_remain = BLOCKSIZ - last_block_used;

// //文件最后一个block的地址
// int last_block;

// if (block_num <= 10)
//     last_block = file_inode->di_addr[block_num - 1];
// else {

//     unsigned int temp_addr[BLOCKSIZ / 4];

//     //读索引块
//     vD.readBlock(file_inode->first_index_addr, &temp_addr);

//     last_block = temp_addr[block_num - 10 - 1];
// }

// if (text_length <= last_block_remain) {
//     //无需再重新分配

//     vD.writeAAddr(last_block + last_block_used, text_length,
//     &text_buf);
// } else {
//     //当前空间不足,需要再重新分配block

//     //先填满当前的
//     char temp_text[BLOCKSIZ];
//     strncpy(temp_text, text_buf, last_block_remain);
//     vD.writeAAddr(last_block + last_block_used, tlast_block_remain,
//                   &temp_text);

//     //还需再分配多少块
//     //（没写进去的－已经写进去的）／每块大小 + (取余为0加0
//     取余为1加1) int new_block_num = (text_length - last_block_remain)
//     / BLOCKSIZ + (1:0?(text_length - last_block_remain)%BLOCKSIZ);

//     //填写再分配的这些块
//     for (int i = 0; i < new_block_num; i++) {

//         if (block_num++ = NADDR) {
//             cout <<
//             "对不起,文件已达最大长度,系统已为您保存您输入的内容"
//                  << endl;
//             return;
//         }

//         unsigned int addr = balloc();
//         if (addr == -1) {
//             cout << "BLOCK 分配失败" << endl;
//             return;
//         }

//         if (block_num <= 10) {
//             file_inode->di_addr[block_num - 1] = addr;
//             strcpy(temp_text, text_buf,
//                    last_block_remain + i * BLOCKSIZ, BLOCKSIZ);
//             vD.writeBlock(addr, &temp_text);
//         } else {

//             unsigned int temp_addr[BLOCKSIZ / 4];

//             if (block_num == 11) {

//                 //上面分配的addr就是索引块
//                 unsigned int index_addr = addr;
//                 file_inode->first_index_addr = addr;

//                 //为数据申请一个新地址
//                 addr = balloc();
//                 if (addr == -1) {
//                     cout << "BLOCK 分配失败" << endl;
//                     //既然数据分配失败了,改回索引块
//                     file_inode->first_index_addr = 0;
//                     return;
//                 }

//                 //写数据
//                 strcpy(temp_text, text_buf,
//                        last_block_remain + i * BLOCKSIZ, BLOCKSIZ);
//                 vD.writeBlock(addr, &temp_text);

//                 //更新索引块
//                 temp_addr[block_num - 10 - 1] = addr;
//                 vD.writeBlock(index_addr, &temp_addr);
//             } else {

//                 //写数据
//                 strcpy(temp_text, text_buf,
//                        last_block_remain + i * BLOCKSIZ, BLOCKSIZ);
//                 vD.writeBlock(addr, &temp_text);

//                 //读索引块
//                 vD.readBlock(file_inode->first_index_addr,
//                 &temp_addr);

//                 //更新索引块
//                 temp_addr[block_num - 10 - 1] = addr;
//                 vD.writeBlock(ile_inode->first_index_addr,
//                 &temp_addr)
//             }
//         }
//     }
// }

// //修改inode中的相关值
// file_inode->size += text_length;

// // iput时更新inode即可,此处不用更新

// //更新超级块
// vD.writeBlock(BLOCKSIZ, &super_block);

// //若最大长度了,保存后给出提示
// if (is_max == true) {
//     cout << "对不起,文件已达最大长度,系统已为您保存您输入的内容"
//          << endl;
// }
