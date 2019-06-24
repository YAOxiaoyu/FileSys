#include "../fileSys.h"
#include <iostream>

int main() {
     inode i;
    std::cout << sizeof(i) << std::endl;

     dinode ii;
    std::cout << sizeof(ii) << std::endl;

     dir di;
     std::cout << sizeof(di) << std::endl;

     dir_item dit;
     std::cout << sizeof(dit) << std::endl;
}