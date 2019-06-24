#include "fileSys.h"
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

void open_file(string file_name) {

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i]);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            break;
        }
        if (i == cur_dir.size - 1) {
            //说明没找到
            cout << "当前目录下不存在该文件,请输入正确的名字" << endl;
            return;
        }
    }

    //查看在当前用户打开表下是否有该文件
    if (inode_user_o.find(file_inode_no) != inode_user_o.end()) {
        cout << "该文件已打开,请勿重复操作" << endl;
        return;
    }

    struct inode *file_inode = iget(file_inode_no);

    //如果输入的是一个目录名
    if (file_inode->i_flag == 0) {
        cout << file_name << "为目录名而是文件名" << endl;
        return;
    }

    // TODO 可访问权限
    if (access() == 0) {

        //在用户文件打开表中添加该inode
        inode_user_o[file_inode_no] = *file_inode;

        //如果在系统文件打开表中也已经存在此项
        if (inode_o.find(file_inode_no) != inode_o.end()) {
            inode_o[file_inode_no].i_count++;
        }
    } else {
        cout << "对不起,您无权限执行此操作" << endl;
        return;
    }
}

void create_file(string file_name) {

    if (size(file_name) >= NAMESIZ) {
        cout << "对不起,文件名过长" << endl;
        return;
    }

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i]);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            cout << "该文件已经存在,请勿重复创建" << endl;
            break;
        }
        // if (i == cur_dir.size - 1) {
        //     //说明没找到
        //     cout << "当前目录下不存在该文件,请输入正确的名字" << endl;
        //     return;
        // }
    }

    struct inode *file_inode;

    // TODO 在当前目录下可创建文件权限
    if (access() == 0) {

        // //在当前目录表找一个空项
        // int dir_item_no;
        // for (int i = 2; i < DIRNUM; i++) {
        //     //前两项为 .. 和 .
        //     if (cur_dir.dir[i].d_ino == 0) {
        //         dir_item_no = i;
        //         break;
        //     }
        //     if (i == DIRNUM - 1) {
        //         // 说明没找到
        //         cout << "当前目录下目录项已满" << endl;
        //         return;
        //     }
        // }

        //分配inode
        file_inode = ialloc();
        if (file_inode == NULL) {
            cout << "inode分配失败" << endl;
            return;
        }

        //修改inode中相关值
        file_inode->di_number = 1;
        file_inode->i_flag = 1;
        file_inode->di_size = 0;
        // TODO 修改用户以及权限相关

        //分配balloc
        file_inode->di_addr[0] = balloc();

        //创建目录项 并修改相关值
        struct dir_item temp_dir;
        temp_dir.d_ino = file_inode->i_ino;
        strcpy(temp_dir.d_name, file_name.c_str());

        //将目录项添加到当前目录中
        cur_dir.dir[cur_dir.size - 1] = temp_dir;

        // TODO 更新inode
        // TODO 更新目录项
        // TODO 更新超级块
    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}

void write_file(string file_name) {

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i]);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            break;
        }
        if (i == cur_dir.size - 1) {
            //说明没找到
            //没找到就以当前文件名创建该文件
            // cout << "当前目录下不存在该文件,请输入正确的名字" << endl;
            create_file(file_name);
            return;
        }
    }

    if (inode_user_o.find(file_inode_no) == inode_user_o.end()) {
        //若文件还没打开,自动打开文件
        open_file(file_name);
    }

    struct inode *file_inode = inode_user_o[file_inode_no];

    // TODO 写权限
    if (access() == 0) {
        //计算当前文件剩余大小
        int remain = FILEMAXSIZE - file_inode->di_size;
        system("clear");
        char text_buf[remain];
        int text_length = 0;
        bool is_max = false;
        while ((c = getchar()) != "#") {
            if (length >= remain) {
                is_max = true;
                break;
            }
            text_buf[text_length++] = c;
        }

        //文件最后一块实际占用的空间
        int last_block_used;
        if (file_inode->di_size % BLOCKSIZ != 0)
            last_block_used = file_inode->di_size % BLOCKSIZ;
        else
            last_block_used = blockSize;

        //文件占用block数
        int block_num = file_inode->di_size / BLOCKSIZ + 1 : 0
            ? file_inode->di_size % BLOCKSIZ;

        //文件最后的block里的剩余空间
        int last_block_remain = BLOCKSIZ - last_block_used;

        //文件最后一个block的地址
        int last_block;

        if (block_num <= 10)
            last_block = file_inode->di_addr[block_num - 1];
        else {
            // TODO 多级索引
        }

        if (text_length <= last_block_remain) {
            //无需再重新分配
            if (block_num <= 10) {
                vD.writeAAddr(last_block + last_block_used, text_length,
                              &text_buf);
            }
            // TODO 多级索引
        } else {
            //当前空间不足,需要再重新分配block

            //先填满当前的
            char temp_text[BLOCKSIZ];
            strncpy(temp_text, text_buf, last_block_remain);
            vD.writeAAddr(last_block + last_block_used, tlast_block_remain,
                          &temp_text);

            //还需再分配多少块
            //（没写进去的－已经写进去的）／每块大小 + (取余为0加0 取余为1加1)
            int new_block_num = (text_length - last_block_remain) / BLOCKSIZ + (1:0?(text_length - last_block_remain)%BLOCKSIZ);
            for (int i = 0; i < new_block_num; i++) {

                if (block_num++ = NADDR) {
                    cout << "对不起,文件已达最大长度,系统已为您保存您输入的内容"
                         << endl;
                    return;
                }

                unsigned int addr = balloc();
                if (addr == -1) {
                    cout << "BLOCK 分配失败" << endl;
                    return;
                }

                strcpy(temp_text, text_buf, last_block_remain + i * BLOCKSIZ,
                       BLOCKSIZ);
                vD.writeBlock(addr, &temp_text);

                //分配完后还＜＝10,正确
                if (block_num <= 10) {
                    file_inode->di_addr[block_num - 1] = addr;
                } else {
                    // TODO 多级索引
                }
            }
        }

        file_inode->size += text_length;
        //TODO 更新inode

        if (is_max == true) {
            cout << "对不起,文件已达最大长度,系统已为您保存您输入的内容"
                 << endl;
        }

    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}

void read_file() {

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i]);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            break;
        }
        if (i == cur_dir.size - 1) {
            //说明没找到
            cout << "当前目录下不存在该文件,请输入正确的名字" << endl;
            return;
        }
    }

    if (inode_user_o.find(file_inode_no) == inode_user_o.end()) {
        //若文件还没打开,自动打开文件
        open_file(file_name);
    }

    struct inode *file_inode = inode_user_o[file_inode_no];

    // TODO 读权限
    if (access() == 0) {

        system("clear");
        int block_num = file_inode->di_size / BLOCKSIZ + (1 : 0
            ? file_inode->di_size % BLOCKSIZ);
        if (block_num <= 10) {
            for (int i = 0; i < block_num; i++) {
                unsigned int addr = file_inode->di_addr[block_num - 1];
                char temp_text[BLOCKSIZ];
                vD.readBlock(addr, &temp_text);
                cout << temp_text;
            }
            cout << "#" << endl;
        }
        // TODO 多级索引

    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}

void delete_file(string file_name) {

    //看当前目录下有没有该文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i]);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            break;
        }
        if (i == cur_dir.size - 1) {
            //说明没找到
            cout << "当前目录下不存在该文件,请输入正确的名字" << endl;
            return;
        }
    }

    struct inode *file_inode = iget(file_inode_no);

    // TODO 用户有删除权限
    if (access() == 0) {

        //删除目录项
        for (int i = dir_item_no; i < cur_dir.size - 1; i++) {
            cur_dir.dir[i] = cur_dir.dir[i + 1];
        }
        cur_dir.size--;

        //在系统打开表和用户打开表中删除该项
        if (inode_o.find(file_inode->i_ino) != inode_o.end()) {
            inode_o.erase(inode_o.find(file_inode->i_ino));
        }
        if (inode_user_o.find(file_inode->i_ino) != inode_user_o.end()) {
            inode_user_o.erase(inode_user_o.find(file_inode->i_ino));
        }

        //检查文件关联数
        if (file_inode->di_number > 1) {
            file_inode->di_number--;
        } else {

            //释放block
            //计算当前文件占了几个block
            int block_num = file_inode->di_size / BLOCKSIZ + (1 : 0
                ? file_inode->di_size % BLOCKSIZ);
            if (block_num <= 10) {
                //先不考虑三级索引
                for (int i = 0; i < block_num; i++) {
                    bfree(file_inode->di_addr[i]);
                }
            }
            // TODO 多级索引

            //释放inode
            iput(file_inode->i_ino);
            ifree(file_inode);

            // TODO 更新超级块
            // TODO 更新目录
        }
    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}