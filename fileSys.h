/*目前各种数据结构 */

#ifndef FILESYS_H
#define FILESYS_H

#include "virtualDisk.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#define BLOCKSIZ 512   //每块大小
#define SYSOPENFILE 40 //系统打开文件表最大项数
#define DIRNUM 128 //每个目录所包含的最大目录项数（文件数）
#define NAMESIZ 16 //每个文件或者目录名最大字节
#define DIRSIZ 20  //计算可得每个目录项最大占20字节
#define PWDSIZ 12  //口令字
#define PWDNUM 32  //最多可设32个口令登录
#define NOFILE 20 //每个用户最多可打开20个文件，即用户打开文件最大次数

#define NADDR 138 //每个i节点最多指向10块，addr[0]~addr[9]+一级索引

#define USERNUM 10 //最多允许10个用户登录

#define DINODESIZ 64 //每个磁盘i节点所占字节
#define DINODEBLK 32 //所有磁盘i节点共占32个物理块
#define DINODE_NUM DINODEBLK *BLOCKSIZ / DINODESIZ

#define FILEBLK 512              // 共有512个目录文件物理块
#define NICFREE 50               //超级块中空闲块数组的最大块数
#define NICINOD 50               ///超级块中空闲节点的最大块数
#define DINODESTART 2 * BLOCKSIZ // i节点起始地址
#define DATASTART (2 + DINODEBLK) * BLOCKSIZ //目录、文件区起始地址

#define FILEMAXSIZE NADDR *BLOCKSIZ;

#define ALLBLOCKNUM 10240 // 共有10240个物理块 5M?

/*文件数据结构*/
struct super_block {

    // unsigned short s_isize;       //索引节点块块数
    unsigned long s_fsize; //数据块块数

    unsigned int s_nfree;         //空闲块块数
    unsigned short s_pfree;       //空闲块指针
    unsigned int s_free[NICFREE]; //空闲块堆栈

    unsigned int s_ninode; //空闲索引节点数
    unsigned short binode_bitmap[DINODE_NUM];

    char s_fmod; //超级块修改标志
};

struct inode {

    int i_flag;         //如果为0为目录,如果为1为文件
    unsigned int i_ino; //磁盘索引节点标志

    unsigned int i_count;     //引用计数
    unsigned short di_number; //关联文件数,为0时,删除该文件

    unsigned short di_mode; //存取权限
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned int di_size;     //文件大小
    unsigned int di_addr[10]; //物理块号

    //三级索引
    unsigned int first_index_addr;  // 512/4 * 512 = 2^7 * 2^9 = 64KB
                                    // (文件最多10+128=138块)
    unsigned int second_index_addr; //(512/4)^2 * 512 = 2^14 * 2*9 = 8MB
    unsigned int third_index_addr;  //(512/4)^3 * 512 = 2^14 * 2*9 = 1G
};

struct dinode {
    unsigned short di_number; //关联文件数,为0时,删除该文件

    unsigned short di_mode; //存取权限
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned int di_size;     //文件大小
    unsigned int di_addr[10]; //物理块号

    //三级索引
    unsigned int first_index_addr;  // 512/4 * 512 = 2^7 * 2^9 = 64KB
                                    // (文件最多10+128=138块)
    unsigned int second_index_addr; //(512/4)^2 * 512 = 2^14 * 2*9 = 8MB
    unsigned int third_index_addr;  //(512/4)^3 * 512 = 2^14 * 2*9 = 1G
};

struct dir_item {
    char d_name[NAMESIZ];
    unsigned int d_ino;
};

struct dir {
    struct dir_item dir[DIRNUM];
    int size; //当前目录大小
};

struct password {
    unsigned short p_uid;
    unsigned short P_gid;

    char password[PWDSIZ];
};

struct file {
    char f_flag;          //文件操作标志
    unsigned int f_count; //引用计数

    struct inode *f_inode; //指向内存索引节点
    unsigned long f_off;   //读写指针
};

struct user {
    unsigned short u_default_mode;
    unsigned short u_uid;
    unsigned short u_gid;
    map<unsigned int, struct inode *> u_ofile; //用户打开文件表
};

struct userListItem {
    // char uName[20];
    unsigned short uId;
    unsigned short gId;
};

enum mode { READ, WRITE, X };

/*全局变量 */
extern class virtualDisk vD;
extern struct dir cur_dir;
extern struct file sys_ofile[SYSOPENFILE];
extern struct super_block super_block;
extern unsigned int pwdNum;
extern struct password password[PWDNUM];

/*用户*/
extern vector<struct userListItem> userList;
extern vector<struct user> userLogin;
extern struct user* activeUser;
extern int userNum;

extern FILE *fd;
extern struct inode *cur_path_node;
extern int user_id, file_block;
extern unsigned int inode_data_start_block; //当前使用的Inode块的数据的起始地址

extern unsigned int inode_ino; //当前inode ino
extern unsigned int dir_ino;   //目录对应文件ino
extern unsigned int home_ino;  //根目录的inode
extern string cur_dir_name;

//文件打开表
extern map<unsigned int, struct inode> inode_o; // inode打开表
extern map<string, unsigned int> dir_list;      //当前目录表

extern map<unsigned int, struct inode *> inode_sys_o; // inode 系统打开表
extern map<unsigned int, struct inode *>
    inode_user_o; // inode 打开表(用户文件打开表) -> 当前用户打开表
// TODO 切换用户时,文件打开表怎么办?

// 函数声明
void format(virtualDisk &vD);
void format();
unsigned int balloc();              //磁盘块分配函数
void bfree(unsigned int block_num); //磁盘块释放函数

void get_cur_dir(int output); //获取当前文件的目录表          //获取当前目录

void mkdir(string new_name); //当前目录下创建新文件夹
void get_dir(string path);   // 多级目录

inode *iget(unsigned int inode_id); //获取inode ino 对应inode节点
void iput(unsigned int inode_id);
void ifree(unsigned int inode_id);
struct inode *ialloc();

int access();

void open_file(string file_name);
void create_file(string file_name);
void write_file(string file_name);
void read_file(string file_name);
void delete_file(string file_name);
void close_file(string file_name);

void write_f(string, void *, int, unsigned int);
unsigned int read_f(string, void *, unsigned int);

void close_sys();

bool login();
void logout(struct user userQuit);
void getPassword(char *password);
void createUser(unsigned short uId, unsigned short gId);
bool access(inode *inode, struct user *user, mode m);
void onload();

#endif
