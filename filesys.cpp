#include "fileSys.h"
#include "virtualDisk.h"
using namespace std;
/*全局变量 */
 class virtualDisk vD;
 struct dir cur_dir;
 struct file sys_ofile[SYSOPENFILE];
 struct super_block super_block;
 struct password password[PWDNUM];
 struct user user[USERNUM];
 FILE *fd;
 struct inode *cur_path_node;
 int user_id, file_block;
 unsigned int inode_data_start_block; //当前使用的Inode块的数据的起始地址

 unsigned int inode_ino; //当前inode ino
 unsigned int dir_ino;   //目录对应文件ino
 unsigned int home_ino;  //根目录的inode
 string cur_dir_name;

//文件打开表
 map<unsigned int, struct inode> inode_o;       // inode打开表
 map<string, unsigned int> dir_list;            //当前目录表
 map<unsigned int, struct inode *> inode_sys_o; // inode 系统打开表
 map<unsigned int, struct inode *>
    inode_user_o; // inode 打开表(用户文件打开表)


int main(){
    fstream file;
    file.open(virtualDName,ios::in);
    // if(!file){
    //     vD.newDisk(ALLBLOCKNUM*BLOCKSIZ);
    //     format(vD);
    // }
    return 0;
}
