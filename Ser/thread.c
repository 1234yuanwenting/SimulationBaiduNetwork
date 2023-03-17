#include"thread.h"
#define ARG_MAX 10
char* get_cmd(char buff[],char *myargv[])//解析命令
{    //ps -l
    if(buff==NULL||myargv==NULL)
        return NULL;
    int i=0;
    char*ptr=NULL;
    char *s=strtok_r(buff," ",&ptr);//ps
    while(s!=NULL)
    {
        myargv[i++]=s;//myargv[0]=ps my[1]=-l
        s=strtok_r(NULL," ",&ptr);
    }
    return myargv[0];
}
//下载
void send_file(int c,char*name)
{
    if(name==NULL)
    {
        send(c,"err",3,0);
        return;
    }
    int fd=open(name,O_RDONLY);
    if(fd==-1)
    {
        send(c,"err",3,0);
        return;
    }
    //获取文件大小
    int filesize=lseek(fd,0,SEEK_END);//文件偏移量放末尾
    lseek(fd,0,SEEK_SET);//文件偏移量放回开头
    char status[32]={0};
    sprintf(status,"ok#%d",filesize);//sprintf为打印到status中
    send(c,status,strlen(status),0);
    memset(status,0,32);//清空
    int num=recv(c,status,31,0);
    if(num<=0)
    {
        printf("client close\n");
        close(fd);
        return;
    }
    //if客户端不下载了
    if(strcmp(status,"err")==0)
    {
        close(fd);
        return;
    }
    char data[1024];
    num=0;
    while((num=read(fd,data,1024))>0)
    {//读了多少读取文件内容到data,传送给客户端
        send(c,data,num,0);
    }
    close(fd);
}

void* work_thread(void*arg)
{
    int c=(int)arg;
    while(1)
    {
        char buff[128]={0};//字符串

        int n=recv(c,buff,127,0);//get a.mp4 ,rm a.c ls
        if (n<=0)
        {
            break;
        }
        char *myargv[ARG_MAX]={0};//指针数组,字符串数组
        char *cmd=get_cmd(buff,myargv);//用myargv[0]去匹配下面的就好
        if(strcmp(cmd,"get")==0)
        {
            send_file(c,myargv[1]);//myargv:get +文件名

        }
        else if(strcmp(cmd,"up")==0)
        {
            //上传
        }
        else
        {
            int fd[2]={0};//无名管道用于父子进程f[0]为读端,fd[1]为写端
            if(pipe(fd)==-1)
            {
                printf("pipe err\n");
                send(c,"er1",3,0);//标志位=0
                continue;
            }
            pid_t pid=fork();
            if(pid==-1)
            {
                printf("fork err");
                send(c,"er1",3,0);
                continue;
            }
            if(pid==0)
            {
                //dup
                dup2(fd[1],1);//用它覆盖标准输出和标准错误输出
                dup2(fd[1],2);
                execvp(cmd,myargv);//这里传的是整个数组，整个数组作为第二个参数，
               printf("cmd not find");//此处若已成功替换则不会被执行
                exit(0);//子进程结束退出
            }
            close(fd[1]);
            wait(NULL);
            char send_buff[1024]={"ok#"};
            int num=read(fd[0],send_buff+3,1020);//从管道中读取数据传给客户端
            send(c,send_buff,num+3,0);
           // close(fd[0]);

        }
        printf("buff=%s",buff);
        send(c,"ok",2,0);

    }
    sleep(2);
    close(c);
    printf("client close\n");
}
void start_thread(int c)
{
    pthread_t id;
    pthread_create(&id,NULL,work_thread,(void*)c);
}
