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
        temp_name.assign(cur_dir.dir[i].d_name);
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
    if (1) {

        //在用户文件打开表中添加该inode
        inode_user_o[file_inode_no] = file_inode;

        if (inode_sys_o.find(file_inode_no) != inode_sys_o.end()) {
            //如果在系统文件打开表中也已经存在此项
            file_inode->i_count++;
        } else {
            //为打开
            inode_sys_o[file_inode_no] = file_inode;
        }
    } else {
        cout << "对不起,您无权限执行此操作" << endl;
        return;
    }
}

void create_file(string file_name) {

    if (sizeof(file_name) >= NAMESIZ) {
        cout << "对不起,文件名过长" << endl;
        return;
    }

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i].d_name);
        if (temp_name == file_name && iget(cur_dir.dir[i].d_ino)->i_flag != 0) {
            //存在且不是一个目录
            file_inode_no = cur_dir.dir[i].d_ino;
            dir_item_no = i;
            cout << "该文件已经存在,请勿重复创建" << endl;
            break;
        }
    }

    struct inode *file_inode;

    // TODO 在当前目录下可创建文件权限
    if (1) {

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

        //更新目录
        // 1. 创建目录项 并修改相关值
        struct dir_item temp_dir;
        temp_dir.d_ino = file_inode->i_ino;
        strcpy(temp_dir.d_name, file_name.c_str());

        // 2. 将目录项添加到当前目录中
        cur_dir.dir[cur_dir.size - 1] = temp_dir;

        // 3. 更新当前dir_map
        get_cur_dir(0);

        // iput时写回inode即可

        // cd目录时 写回目录即可

        // 0号块闲置,1号块为超级快,地址刚好为BLOCKSIZ
        vD.writeBlock(BLOCKSIZ, &super_block);

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
        temp_name.assign(cur_dir.dir[i].d_name);
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
    if (1) {

        system("clear");
        read_file(file_name);
        cout << "以上为文件目前内容" << endl;
        cout << "------------------------------------------------------"
             << endl;
        cout << "请输入文件新内容" << endl;

        //计算文件最大大小
        int max_size = NADDR * BLOCKSIZ;
        char text_buf[max_size];
        int text_length = 0;
        bool is_max = false;
        char c;
        while ((c = getchar()) != '#') {
            if (text_length >= max_size) {
                is_max = true;
                break;
            }
            text_buf[text_length++] = c;
        }

        int block_num = text_length / BLOCKSIZ; //不考虑最后一块
        int last_block_size = text_length - block_num * BLOCKSIZ;

        for (int i = 0; i < block_num + 1; i++) {

            char temp_text[BLOCKSIZ];
            if (i == block_num) {
                if (last_block_size == 0)
                    break;
                else
                    strncpy(temp_text, text_buf + i * BLOCKSIZ,
                            last_block_size);
            } else {
                strncpy(temp_text, text_buf + i * BLOCKSIZ, BLOCKSIZ);
            }

            if (i < 10) {

                //分配数据块
                unsigned int addr = balloc();
                if (addr == -1) {
                    cout << "block分配失败" << endl;
                    return;
                }

                //写数据
                vD.writeBlock(addr, &temp_text);

                //修改inode
                file_inode->di_addr[i] = addr;
            } else {
                if (i == 10) {

                    //分配索引块
                    unsigned int index_addr = balloc();
                    if (index_addr == -1) {
                        cout << "block分配失败" << endl;
                        return;
                    }

                    //分配数据块
                    unsigned int addr = balloc();
                    if (addr == -1) {
                        cout << "block分配失败" << endl;
                        return;
                    }

                    //写数据
                    vD.writeBlock(addr, &temp_text);

                    //修改inode
                    unsigned int temp_addr[BLOCKSIZ / 4];
                    temp_addr[i - 10] = addr;
                    vD.writeBlock(index_addr, &temp_addr);
                    file_inode->first_index_addr = index_addr;
                } else {
                    //无需分配索引块

                    //分配数据块
                    unsigned int addr = balloc();
                    if (addr == -1) {
                        cout << "block分配失败" << endl;
                        return;
                    }

                    //写数据
                    vD.writeBlock(addr, &temp_text);

                    //读索引块
                    unsigned int temp_addr[BLOCKSIZ / 4];
                    vD.readBlock(file_inode->first_index_addr, &temp_addr);
                    temp_addr[i - 10] = addr;
                    vD.writeBlock(file_inode->first_index_addr, &temp_addr);
                }
            }
        }

        file_inode->di_size = text_length;

        //若最大长度了,保存后给出提示
        if (is_max == true) {
            cout << "对不起,文件已达最大长度,系统已为您保存您输入的内容"
                 << endl;
        }

    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}

void read_file(string file_name) {

    // TODO 多级目录正则
    //保存当前目录名
    // string cur_path;
    // cur_path.assign(cur_dir.dir[1].d_name);
    //解析file_name

    //在当前目录表中查找此文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i].d_name);
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
    if (1) {

        system("clear");
        int block_num = file_inode->di_size / BLOCKSIZ +
                        ((file_inode->di_size % BLOCKSIZ) ? 1 : 0);
        if (block_num <= 10) {
            for (int i = 0; i < block_num; i++) {
                unsigned int addr = file_inode->di_addr[i];
                char temp_text[BLOCKSIZ];
                vD.readBlock(addr, &temp_text);

                if (i == block_num - 1) {
                    int last_size =
                        file_inode->di_size - (block_num - 1) * BLOCKSIZ;
                    temp_text[last_size] = '\0';
                }

                cout << temp_text;
                memset(temp_text, '\0', sizeof(temp_text)); //清空
            }
            cout << "#" << endl;
        } else {
            for (int i = 0; i < 10; i++) {
                unsigned int addr = file_inode->di_addr[i];
                char temp_text[BLOCKSIZ];

                //前十块一定是读完一整块
                vD.readBlock(addr, &temp_text);
                cout << temp_text;
                memset(temp_text, '\0', sizeof(temp_text)); //清空
            }

            unsigned int temp_addr[BLOCKSIZ / 4];
            vD.readBlock(file_inode->first_index_addr, &temp_addr);

            for (int i = 0; i < block_num - 10; i++) {
                unsigned int addr = temp_addr[i];

                char temp_text[BLOCKSIZ];

                vD.readBlock(addr, &temp_text);
                if (i == block_num - 1) {
                    int last_size =
                        file_inode->di_size - (block_num - 1) * BLOCKSIZ;
                    temp_text[last_size] = '\0';
                }
                cout << temp_text;
                memset(temp_text, '\0', sizeof(temp_text)); //清空
            }
        }

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
        temp_name.assign(cur_dir.dir[i].d_name);
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
    if (1) {

        //更新目录:删除目录项和更新map
        for (int i = dir_item_no; i < cur_dir.size - 1; i++) {
            cur_dir.dir[i] = cur_dir.dir[i + 1];
        }
        cur_dir.size--;
		


        //更新 map
        get_cur_dir(0);

        //在系统打开表和用户打开表中删除该项
        if (inode_sys_o.find(file_inode->i_ino) != inode_sys_o.end()) {
            inode_sys_o.erase(inode_sys_o.find(file_inode->i_ino));
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
            int block_num = file_inode->di_size / BLOCKSIZ +
                            ((file_inode->di_size % BLOCKSIZ) ? 1 : 0);
            if (block_num <= 10) {
                //先不考虑三级索引
                for (int i = 0; i < block_num; i++) {
                    bfree(file_inode->di_addr[i]);
                }
            } else {
                for (int i = 0; i < 10; i++) {
                    bfree(file_inode->di_addr[i]);
                }
                unsigned int temp_addr[BLOCKSIZ / 4];
                vD.readBlock(file_inode->first_index_addr, &temp_addr);
                for (int i = 0; i < block_num - 10; i++) {
                    bfree(temp_addr[i]);
                }
            }

            //释放inode
            iput(file_inode->i_ino);
            ifree(file_inode->i_ino);

            // cd切换目录时更新即可

            vD.writeBlock(BLOCKSIZ, &super_block);
        }
    } else {
        cout << "对不起,您无权限执行此操作" << endl;
    }
}

void close_file(string file_name) {
    //看当前目录下有没有该文件
    int file_inode_no;
    int dir_item_no;
    for (int i = 0; i < cur_dir.size; i++) {
        string temp_name;
        temp_name.assign(cur_dir.dir[i].d_name);
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
        //未打开该文件
        cout << "对不起,您当前未打开该文件" << endl;
        return;
    }

    struct inode *file_inode = iget(file_inode_no);

    //在用户打开表中删除
    inode_user_o.erase(inode_user_o.find(file_inode_no));

    //在系统打开表中--或删除
    if (file_inode->i_count > 1) {
        file_inode->i_count--;
        //更新系统打开表
        inode_sys_o[file_inode->i_ino]->i_count--;
    } else {
        inode_sys_o.erase(inode_sys_o.find(file_inode_no));
        iput(file_inode->i_ino);
    }
}

void write_f(string file_name, void *file_context, int size,
             unsigned int inode_number) {
    //鉴于这是文件系统调用的,那么就说明一切应该正常,当前dir_list中应该会有此文件

    struct inode *file_inode;

    if (inode_number == 0)
        file_inode = iget(dir_list[file_name]);
    else {
        //参数中的file_name = ""
        file_inode = iget(inode_number);
    }

    int block_num = size / BLOCKSIZ; //不考虑最后一块
    int last_block_size = size - block_num * BLOCKSIZ;

    for (int i = 0; i < block_num + 1; i++) {

        unsigned int offset = i * BLOCKSIZ;

        if (i == block_num && last_block_size == 0)
            break; //写完了,不用再写了

        if (i < 10) {

            //分配数据块
            unsigned int addr = balloc();

            //写数据
            vD.writeBlock(addr, file_context + offset);

            //修改inode
            file_inode->di_addr[i] = addr;
        } else {
            if (i == 10) {

                //分配索引块
                unsigned int index_addr = balloc();

                //分配数据块
                unsigned int addr = balloc();

                //写数据
                vD.writeBlock(addr, file_context + offset);

                //修改inode
                unsigned int temp_addr[BLOCKSIZ / 4];
                temp_addr[i - 10] = addr;
                vD.writeBlock(index_addr, &temp_addr);
                file_inode->first_index_addr = index_addr;
            } else {
                //无需分配索引块

                //分配数据块
                unsigned int addr = balloc();
                if (addr == -1) {
                    cout << "block分配失败" << endl;
                    return;
                }

                //写数据
                vD.writeBlock(addr, file_context + offset);

                //读索引块
                unsigned int temp_addr[BLOCKSIZ / 4];
                vD.readBlock(file_inode->first_index_addr, &temp_addr);
                temp_addr[i - 10] = addr;
                vD.writeBlock(file_inode->first_index_addr, &temp_addr);
            }
        }
    }

    file_inode->di_size = size;
}

unsigned int read_f(string file_name, void *file_context,
                    unsigned int inode_number = 0) {
    //鉴于这是文件系统调用的,那么就说明一切应该正常,当前dir_list中应该会有此文件

    struct inode *file_inode;

    if (inode_number == 0)
        file_inode = iget(dir_list[file_name]);
    else {
        //参数中的file_name = ""
        file_inode = iget(inode_number);
    }

    int block_num = file_inode->di_size / BLOCKSIZ +
                    ((file_inode->di_size % BLOCKSIZ) ? 1 : 0);
    if (block_num <= 10) {
        for (int i = 0; i < block_num; i++) {
            unsigned int addr = file_inode->di_addr[i];

            vD.readBlock(addr, file_context + i * BLOCKSIZ);
            //假如文件只有500B,但读出的是512B,但实际是知道文件的大小是500KB的,所以只需这样读就可以了?
        }
    } else {
        for (int i = 0; i < 10; i++) {
            unsigned int addr = file_inode->di_addr[i];
            vD.readBlock(addr, file_context + i * BLOCKSIZ);
        }

        unsigned int temp_addr[BLOCKSIZ / 4];
        vD.readBlock(file_inode->first_index_addr, &temp_addr);

        for (int i = 0; i < block_num - 10; i++) {
            unsigned int addr = temp_addr[i];
            vD.readBlock(addr, file_context + i * BLOCKSIZ);
        }
    }

    return file_inode->di_size;
}
