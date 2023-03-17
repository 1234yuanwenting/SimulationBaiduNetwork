#include"thread.h"
#include"sock_init.h"
#define PORT 6000
#define IPS "127.0.0.1"
#define LISMAX 5
int create_socket(struct info_data *p);
int accept_client(int sockfd);
int main()
{   struct info_data dt;
    read_info_cnf(&dt);//port,ips
    int sockfd=create_socket(&dt);
    if(-1==sockfd)
    {printf("creat sockfd err\n");
        exit(0);
    }

while(1)
{
    int c=accept_client(sockfd);
    if(-1==c)
    {
        continue;
    }
    start_thread(c);
}
}

int accept_client(int sockfd)
{
    if(-1==sockfd)
    {
     return -1;   
    }
    struct sockaddr_in caddr;//专用套接字地址结构
    int len=sizeof(caddr);
    int c=accept(sockfd,(struct sockaddr*)&caddr,&len);//sockaddr通用socket地址结构
    return c;}
int create_socket(struct info_data *p)
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==sockfd)
    {return -1;}
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(p->port);
    saddr.sin_addr.s_addr=inet_addr(p->ips);
    int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(-1==res)
    {return -1;}
    if(listen(sockfd,p->lismax)==-1)
    {
    return -1;
    }
    return sockfd;
}
