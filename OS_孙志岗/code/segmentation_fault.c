
#include <stdio.h>

int main () {
    char a[] = "hello";
    char* c = "test"; // 指针c指向字符串常量地址，字符串常量不能被修改 
    puts(a);
    puts(c);
    *a = '\0';
    // a[1] = '\0';
    puts(a);
    *c = '\0'; // 修改字符串常量，报段错误
    // c = "abc"; // 修改指针指向的地址，没有错误
    puts(c);
    
    return 0;
}

