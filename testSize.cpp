#include "fileSys.h"
#include <iostream>

int main() {
    struct inode i;
    std::cout << sizeof(i) << std::endl;

    struct dinode ii;
    std::cout << sizeof(ii) << std::endl;
}