#include <iostream>
#include <string>
#include <string.h>

using namespace std;

int main() {
    char s[10] = "";
    string ss = "12345";
    strcpy(s,ss.c_str());
    cout << s;


    char a[10] = "abcde";
    string sss;
    sss.assign(a);
    cout <<sss;
}