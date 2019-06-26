#include "fileSys.h"
#include <iostream>
#include <string.h>
#include <string>

// struct dir cur_dir;
// unsigned int home_ino;
// unsigned int dir_ino;
// unsigned int inode_ino;
// string cur_dir_name;

// map<unsigned int,struct inode>file_open;
// map<string,unsigned int>dir_list;
// map<unsigned int,struct inode>inode_o;

string split(string &s, char flag) {
    string path_left;
    if (s[0] == '/') {
        s = s.substr(1);
    }
    int i = s.find(flag);
    if (i != s.npos) {
        path_left = s.substr(i);
        s = s.substr(0, i);
        return path_left;
    } else {
        return "";
    }
}

//获取当前目录
// ls
// output默认为1，输出目录表
void get_cur_dir(int output = 1) {
    unsigned int inode_ino_cur = inode_ino;
    if (output == 1) {
        cout << "当前目录：" + cur_dir_name << endl;
        cout << "-----------------------------------------" << endl;
    }
    if (dir_ino != inode_ino_cur || dir_list.empty()) {
        //清空目录表
        dir_ino = inode_ino_cur;
        dir emppt_dir;
        cur_dir = emppt_dir;
        dir_list.clear();

        inode *cur;
        //已经在内存中调用过
        if (inode_o.count(inode_ino_cur)) {
            *cur = inode_o[inode_ino_cur];
        } else {
            cur = iget(inode_ino_cur);
        }
        unsigned int dir_size;
        //根据inode_ino_cur的地址索引找到目录地址，保存目录到cur_dir
        dir_item *tem_dir = new dir_item[DIRNUM];
        dir_size = read_f("", &tem_dir, inode_ino_cur);
        cur_dir.size = dir_size / sizeof(dir_item);
        for (int i = 0; i < cur_dir.size; i++) {
            string temp_name = tem_dir[i].d_name;
            dir_list[temp_name] = tem_dir[i].d_ino;
            cur_dir.dir[i].d_ino = tem_dir[i].d_ino;
            // cur_dir.dir[i].d_name=temp_name;
            strcpy(cur_dir.dir[i].d_name, temp_name.c_str());
        }

        delete[] tem_dir;
    }

    //更新cur_dir 到dir_list
    for (int i = 0; i < cur_dir.size; i++) {
        string temp = cur_dir.dir[i].d_name;
        dir_list[temp] = cur_dir.dir[i].d_ino;
        if (output == 1) {
            cout << "当前目录下的文件|Inode地址" << endl;
            cout << cur_dir.dir[i].d_name;
            cout.width(12);
            cout << cur_dir.dir[i].d_ino << endl;
        }
    }
}

//获取多级目录
// cd path
void get_dir(string path) {
    //当前inode ino
    inode *temp;
    unsigned int inode_ino_now;
    string path_letf = "";
    string path_now = "";
    path_now = path;
    path_letf = split(path_now, '/');
    if (path_now == "root") {
        //从根目录开始查找
        inode_ino_now = home_ino;
        dir_ino = home_ino;
    } else {
        //从目录中匹配
        get_cur_dir(0);
        if (dir_list.count(path_now)) {
            inode_ino_now = dir_list[path_now];
            dir_ino = inode_ino_now;
        } else {
            cout << "请输入正确路径" << endl;
        }
    }
    //调用
    if (inode_o.count(inode_ino_now)) {
        *temp = inode_o[inode_ino_now];
    } else {
        temp = iget(inode_ino_now);
    }

    //判断是否是文件
    if (temp->i_flag == 0) {
        cur_dir_name = path_now;
        //添加到文件打开表
        inode_sys_o[inode_ino_now] = temp;

        //目录信息写回磁盘
        dir_item *tem_dir = new dir_item[cur_dir.size];
        for (int i = 0; i < cur_dir.size; i++) {
            tem_dir[i].d_ino = cur_dir.dir[i].d_ino;
            strcpy(tem_dir[i].d_name, cur_dir_name.c_str());
        }
        write_f("", tem_dir, cur_dir.size * sizeof(dir_item), inode_ino);

        delete[] tem_dir;
        //更新dir_list cur_dir
        //更新inode
        inode_ino = inode_ino_now;
        dir_list.clear();
        get_cur_dir(0);
        if (path_letf != "") {
            get_dir(path_letf);
        }
    } else {
        cout << path_now + " 不是一个文件夹" << endl;
    }
}
//当前目录下生成新文件夹
void mkdir(string new_name) {
    if (dir_list.count(new_name)) {
        cout << "同名文件夹已存在" << endl;
        return;
    }

    struct dir_item new_item;
    for (int i = 0; i < new_name.length(); i++) // string => char[]
    {
        new_item.d_name[i] = new_name[i];
    }
    inode *temp = ialloc();
    unsigned int new_inode_ino = temp->i_ino;
    dir_list[new_name] = new_inode_ino;
    new_item.d_ino = new_inode_ino;
    //生成新文件夹的目录 cd .. 和cd.
    dir_item new_dir[BLOCKSIZ / sizeof(dir_item)];
    new_dir[0].d_ino = inode_ino;
    strcpy(new_dir[0].d_name, "..");
    new_dir[1].d_ino = new_inode_ino;
    strcpy(new_dir[1].d_name, ".");

    // TODO:写回磁盘
    int block = balloc();
    fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
    fwrite(new_dir, 1, BLOCKSIZ, fd);
    temp->di_addr[0] = block;
    temp->di_number = 1;
    temp->di_size = 2 * sizeof(dir_item);
    iput(temp->i_ino);

    cur_dir.size += 1;
    cur_dir.dir[cur_dir.size] = new_item;
}
