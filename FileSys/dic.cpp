#include "fileSys.h"
#include<iostream>
#include<string>

struct dir cur_dir;
unsigned int home_ino;
unsigned int dir_ino;
unsigned int inode_ino;



map<string,unsigned int>dir_list;

string split(string &s,char flag)
{
    string path_left;
    if(s[0]=='/')
    {
        s=s.substr(1);
    }
    int i=s.find(flag);
    if(i!=s.npos)
    {
        path_left=s.substr(i);
        s=s.substr(0,i);
        return path_left;
    }
    else
    {
        return "";
    }
    
}

//获取当前目录
//ls
//output默认为1，输出目录表
void get_cur_dir(unsigned int inode_ino,int output=1)
{
    //初次调用ls
    if(dir_list.empty() | |dir_ino!=inode_ino)
    {
    inode *cur;
    //cur=iget(inode_ino);
    //TODO:根据inode_ino的地址索引找到目录地址，保存目录到dir
    }
    
    for(int i=0;i<cur_dir.size;i++)
    {
        dir_list[cur_dir.dir[i].d_name]=cur_dir.dir[i].d_ino;
        if(output=1)
        {
        cout<<"当前目录下的文件|Inode地址"<<endl;
        cout<<cur_dir.dir[i].d_name;
        cout.width(12);
        cout<<cur_dir.dir[i].d_ino<<endl;
        }
    }
}

//获取多级目录
void get_dir(string path)
{
    //当前inode ino
    unsigned int inode_ino_now;
    string path_letf="";
    string path_now="";
    path_now=path;
    path_letf=split(path_now,'/');
    if(path_now=="home")
    {
        //从根目录开始查找
        inode_ino_now=home_ino;
        dir_ino=home_ino;
    }
    else
    {
        //从目录中匹配
        if(dir_list.count(path_now))
        {
            inode_ino_now=dir_list[path_now];
            dir_ino=inode_ino_now;
        }
        else
        {
            cout<<"请输入正确路径"<<endl;
        }
        
    }
    //更新dir_list 目录列表
    //更新inode
    inode_ino=inode_ino_now;
    dir_list.clear();
    get_cur_dir(inode_ino_now,0);
    if(path_letf!="")
    {
       get_dir(path_letf);
    }
}



// 当前目录下生成新文件夹
// void mkdir(string new_name)
// {
//     if(dir_list.count(new_name))
//     {
//         cout<<"同名文件夹已存在"<<endl;
//         return;
//     }
//     struct dir_item new_item;
//     new_item.d_name=new_name;

//     //TODO:分配空间
//     //TODO:写回操作
//     dir_list[new_name]=??;
//     new_item.d_ino=??;
//     cur_dir.size+=1;
//     cur_dir.dir[i]=new_item;

    
// }

int main()
{
    string a="lo";
    string b;
    b=a;
    string d;
    d.assign(a);

    string s=split(a,'/');
    cout<<s<<endl;
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<d;

    return 0;
}