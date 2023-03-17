#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "assert.h"
#include "sys/socket.h"
#include "string.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include"fcntl.h"
int connect_ser(char *ips,int port)
{   if(ips==NULL||port<=0)
    {return -1;}
   int sockfd= socket(AF_INET,SOCK_STREAM,0);
   if(sockfd==-1)
   {return -1;}
   struct sockaddr_in saddr;
   memset(&saddr,0,sizeof(saddr));
   saddr.sin_family=AF_INET;
   saddr.sin_port=htons(port);
   saddr.sin_addr.s_addr=inet_addr(ips);
   int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res==-1)
        return -1;
    return sockfd;}
//这个函数用来生成ip,port//它的参数来自主函数
void get_arg(int argc,char*argv[],char **ips,int *p)
{
    if(argc<=1||argv==NULL||ips==NULL||p==NULL)
    {
        return;
    }
    for(int i=1;i<argc;i++)
    {
       if( strncmp(argv[i],"ip=",3)==0)
        {*ips=argv[i]+3;
            continue;
        }
       if(strncmp(argv[i],"port=",5)==0)
       {
           *p=atoi(argv[i]+5);//将字符串转数字
           continue;
       }


}}
char* get_cmd(char buff[],char *myargv[])
{
    if(buff==NULL||myargv==NULL)
        return NULL;
   // char*ptr=NULL;
    char *s=strtok(buff," ");//分割命令
    int i=0;
    while(s!=NULL)
    {myargv[i++]=s;
        s=strtok(NULL," ");
    }
    return myargv[0];}

void recv_file(int c,char*name,char*s)
{
    if(name==NULL||s==NULL)
    {
        return;
    }
    send(c,s,strlen(s),0);
    char status[32]={0};
    int num=recv(c,status,31,0);
    if(num<=0)
    {printf("ser close\n");
        return;
    }
    if(strncmp(status,"ok#",3)!=0)
    {
        
        printf("文件不存在");
        return;
    }
    int filesize=atoi(status+3);
    //打开失败则创建
    int fd=open(name,O_WRONLY|O_CREAT,0600);
    if(filesize==0)
    {
      close(fd);
        return;
    }
    if(fd==-1)
    {   send(c,"err",3,0);
        return;
    }
    send(c,"ok#",3,0);
    int curr_size=0;
    float f=0.0;
    num=0;
    char data[1024];
    while(1)
    {
        int num=recv(c,data,1024,0);
        if(num<=0)
        {printf("服务器关闭\n");
            break;
        }
        sleep(1);
        write(fd,data,num);
        curr_size+=num;
        f=curr_size*100.0/filesize;
        //printf("down %s :%.1f%%\r",name,f);
        fflush(stdout);
        if(curr_size>=filesize)
        {
            break;
        }
    }
    printf("\n");
    close(fd);
}


//客户端利用主函数传递参数，服务器thread利用文件传递ip,port参数
//为了让用户可以传自己想要传递的ip,port
int main(int argc,char *argv[])
{
    
    char *ips="127.0.0.1";
    int port=6000;
    get_arg(argc,argv,&ips,&port);
    int sockfd=connect_ser(ips,port);
    

while(1)
{
printf("connect: %s >>: ",ips);
fflush(stdout);
char buff[128];
fgets(buff,128,stdin);//ls get a.c
buff[strlen(buff)-1]=0;
char cmd_buff[128]={0};
strcpy(cmd_buff,buff);
char* myargv[10]={0};
char* cmd=get_cmd(buff,myargv);
if(strcmp(cmd,"get")==0)
{
recv_file(sockfd,myargv[1],cmd_buff);


}
else if(strcmp(cmd,"up")==0)
{
}
else if(strcmp(cmd,"end")==0)
{break;}
else
{
char recv_buff[1024]={0};
send(sockfd,cmd_buff,strlen(cmd_buff),0);//传输未分割的命令
int num=recv(sockfd,recv_buff,1024,0);
if(num<=0)
{
    printf("ser close\n");
    break;
}
if(strncmp(recv_buff,"ok#",3)==0)
{
    printf("%s",recv_buff+3);}
    else
{
printf("执行命令失败");
}
}
}
sleep(2);
close(sockfd);
exit(0);}
