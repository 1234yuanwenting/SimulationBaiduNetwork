#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>//网络编程
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>//多线程
#include<sys/wait.h>
#include<fcntl.h>
void start_thread(int c);
