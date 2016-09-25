//
//  main.c
//  MyIO
//
//  Created by 陈志浩 on 16/8/2.
//  Copyright © 2016年 陈志浩. All rights reserved.
//

//#include "MYIO.h"
//#include "libmyio.a"

int main(int argc, const char * argv[]) {
    char str[50];
    char ch;
    int a;
    float b;
    double c;
    long double d = 12.00;
//    myscanf("%c%d", &ch, &a);
//    myprintf("%c  %d", ch, a);
//    scanf("%s%d%f%lf", str, &a, &b, &c);
    myscanf("%s%d%f%lf", str, &a, &b, &c);
    myprintf("str:%s int:%d float:%f double:%f long double:%Lf\n", str, a, b, c, d);
    printf("str:%s int:%d float:%f double:%f long double:%Lf\n", str, a, b, c, d);
    return 0;
}
