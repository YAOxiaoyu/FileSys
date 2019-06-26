#include "fileSys.h"
#include "virtualDisk.h"
using namespace std;
int main(){
    fstream file;
    file.open(virtualDName,ios::in);
    if(!file){
        virtualDisk disk;
        disk.newDisk(ALLBLOCKNUM*BLOCKSIZ);
        format(disk);
    }
    return 0;
}
