#include"setport.h"
#include"modbus.h"
#include<fcntl.h>

int main(int argc,char **argv)
{
    int fd;
    int baud=0;
    int pro_num=0;
    pro_Data pro[BUFF_SIZE];

    for(int i=1;i<argc;i++){
        if (strpbrk(argv[i],"./") == NULL){
            break;
        }
        if (fopen(argv[i],"r")==NULL){
            printf("open file error[%s]:[%s] [portfile] [<Serial file>] [baud]\n",argv[i],argv[0]);
            return -1;
        }
    }

    for(int i=2;i<argc;i++){
        if(strpbrk(argv[i],"./")==NULL){
            break;
        }
        pro[pro_num]=GetconfigData(argv[i]);
        pro_num=pro_num+1;
    }
    printf("pro_num===%d\n",pro_num);
    for (int i=0;i<pro_num;i++){
        printf("addr=%d,code=%d\n",pro[i].addr,pro[i].code);
        for(int j=0;j<pro[i].len;j++){
            printf("id=%d,n=%d,coeff=%f,desc===%s\n",pro[i].attr_data[j].id,pro[i].attr_data[j].n,pro[i].attr_data[j].coeff,pro[i].attr_data[j].desc);
        }
        printf("len=%d\n",pro[i].len);
    }
    printf("\n");
    baud=(strpbrk(argv[argc-1],"./")==NULL) ? (atoi(argv[argc-1])):DEFAULT_BAUD;

    if ((fd = open(argv[1],O_RDWR|O_NOCTTY))== -1)
    {
        printf("cannot port fail!\n");
        return -1;
    }

    if (set_info(fd,baud,DEFAULT_DATABITS,DEFAULT_STOPBITS,DEFAULT_PARITY) == -1)
    {
        printf("Set info Errorn!\n");
        return -1;
    }

    while(1){
        for(int i=0;i<pro_num;i++){
            modbus_data(fd,pro[i]);
        }
        printf("\n");
        sleep(1);
    }
    close(fd);
    return 0;
}
