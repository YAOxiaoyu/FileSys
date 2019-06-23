#include"table.h"
#include<iostream>


struct dir cur_dir;
unsigned int inode_ino;
unsigned int dir_ino;

map<string,unsigned int>dir_list;

// dir 哪里来？？
//获取当前目录
//ls
void get_cur_dir()
{
    //初次调用ls
    if(dir_list.size<1 || inode_ino!=dir_ino)
    {
    struct inode *cur=*iget(inode_ino);
    //TODO:根据inode的地址索引找到目录地址，保存目录到dir
    }
    cout<<"当前目录下的文件|Inode地址"<<endl;
    for(int i=0;i<cur_dir.size;i++)
    {
        dir_list[cur_dir.dir[i].d_name]=cur_dir.dir[i].d_ino;
        cout<<cur_dir.dir[i].d_name;
        cout.width(12);
        cout<<cur_dir.dir[i].d_ino<<endl;
    }
}

//当前目录下生成新文件夹
void mkdir(string new_name)
{
    if(dir_list.count(new_name))
    {
        cout<<"同名文件夹已存在"<<endl;
        return;
    }
    struct dir_item new_item;
    new_item.d_name=new_name;

    //TODO:分配空间
    dir_list[new_name]=??;
    new_item.d_ino=??;
    cur_dir.size+=1;
    cur_dir.dir[i]=new_item;

    //TODO:写回操作
}

int main()
{
    char a[10]="helo";
    cout<<a;
    return 0;
}