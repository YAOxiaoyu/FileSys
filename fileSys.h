/*目前各种数据结构 */

#ifndef FILESYS_H
#define FILESYS_H


#include "virtualDisk.h"
#include <iostream>



#define BLOCKSIZ 512   //每块大小
#define SYSOPENFILE 40 //系统打开文件表最大项数
#define DIRNUM 128 //每个目录所包含的最大目录项数（文件数）
#define DIRSIZ 14 //每个目录项名字部分所占字节数，另加i节点号2个字节
#define PWDSIZ 12 //口令字
#define PWDNUM 32 //最多可设32个口令登录
#define NOFILE 20 //每个用户最多可打开20个文件，即用户打开文件最大次数
#define NADDR 10 //每个i节点最多指向10块，addr[0]~addr[9]
//这里应该是只用了十个直接索引////TODO:13个?
#define NHINO 128 //共128个Hash链表，提供索引i节点（必须为2的幂）
#define USERNUM 10               //最多允许10个用户登录
#define dinodeSIZ 32             //每个磁盘i节点所占字节
#define dinodeBLK 32             //所有磁盘i节点共占32个物理块
#define FILEBLK 512              // 共有512个目录文件物理块
#define NICFREE 50              //超级块中空闲块数组的最大块数
#define NICINOD 50               ///超级块中空闲节点的最大块数
#define dinodeSTART 2 * BLOCKSIZ // i节点起始地址
#define DATASTART (2 + dinodeBLK) * BLOCKSIZ //目录、文件区起始地址

#define iNODESIZE 64      // 目前每个iNODE占64B
#define ALLBLOCKNUM 10240 // 共有10240个物理块 5M?


/*文件数据结构*/
struct super_block {

    
    unsigned short s_isize;       //索引节点块块数
    unsigned long s_fsize;        //数据块块数

    unsigned int s_nfree;         //空闲块块数
    unsigned short s_pfree;       //空闲块指针
    unsigned int s_free[NICFREE + 1]; //空闲块堆栈 //多出来的一个是栈底保存当前栈中有多少个空闲块

    unsigned int s_ninode;         //空闲索引节点数
    unsigned short s_pinode;       //空闲索引节点指针
    unsigned int s_inode[NICINOD]; //空闲索引节点数组
    unsigned int s_rinode;         //铭记索引节点

    char s_fmod; //超级块修改标志
};

struct inode {

    struct inode *i_forw;
    struct inode *i_back;

    char i_flag;
    unsigned int i_ino; //磁盘索引节点标志

    unsigned int i_count;     //引用计数
    unsigned short di_number; //关联文件数,为0时,删除该文件

    unsigned short di_mode; //存取权限
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned int di_size;        //文件大小
    unsigned int di_addr[NADDR]; //物理块号

    // TODO:多级索引
};

struct dinode {
    unsigned short di_number; //关联文件数,为0时,删除该文件

    unsigned short di_mode; //存取权限
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned int di_size;        //文件大小
    unsigned int di_addr[NADDR]; //物理块号
};

struct dir_item {
    char d_name[DIRSIZ];
    unsigned int d_ino;
};

struct dir {
    class dir_item dir[DIRNUM];
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
    unsigned short u_ofile[NOFILE]; //用户打开文件表
};

/*全局变量 */
// extern struct hinode hinode[NHINO]; //?什么东西
extern struct dir cur_dir;
extern struct file sys_ofile[SYSOPENFILE]; //?什么东西
extern struct super_block super_block;
extern struct password password[PWDNUM];
extern struct user user[USERNUM];
extern FILE *fd;
extern struct inode *cur_path_node;
extern int user_id, file_block;
extern class virtualDisk vir_disk;

// 函数声明
void format();
unsigned int balloc(); //磁盘块分配函数
void bfree(unsigned int block_num);          //磁盘块释放函数



#endif