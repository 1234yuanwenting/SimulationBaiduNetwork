#include"sock_init.h"

int read_info_cnf(struct info_data *p)
{
    if(p==NULL)
        return -1;
    FILE *fp=fopen("infom.cnf","r");
    if(fp==NULL)
        return -1;
    //从文件中一行一行读
    while(1)
    {
        char buff[128]={0};
        fgets(buff,128,fp);
        if(buff[0]=='\0')
        {
            printf("file EOF\n");
            break;
        }
        if(buff[0]=='#'||buff[0]=='\n')
        {
            continue;
        }
        buff[strlen(buff)-1]='\0';
        if(strncmp(buff,"ips=",4)==0)
        {
            strcpy(p->ips,buff+4);
            continue;
        }
        else if(strncmp(buff,"port=",5)==0)
        {
            p->port=atoi(buff+5);
            continue;
        }
        else if(strncmp(buff,"lismax=",7)==0)
        {
            p->lismax=atoi(buff+7);
            continue;
        }
        else 
        {
            printf("infom.cnf :err %s",buff);
        }
            
    }
    fclose(fp);
    return 0;
}
