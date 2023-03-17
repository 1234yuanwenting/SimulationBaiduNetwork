#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ips_max 32
struct info_data
{
    char ips[ips_max];
    int port;
    int lismax;
};
//获取配置文件中的ip port lismax
int read_info_cnf(struct info_data *p);
