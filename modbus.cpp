#include"modbus.h"

/*CRC16*/
short CRC16_Modbus (char *pdata, int len)
{
    unsigned short crc=0xFFFF;
    int i, j;
    for ( j=0; j<len;j++)
    {
        crc=crc^pdata[j];
        for ( i=0; i<8; i++)
        {
            if( ( crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^ 0xa001;
            }
            else
                crc=crc>>1;
        }
    }
    return htons(crc);
}

//read
int read_data(int fd, char *buff, size_t bytes, unsigned int timout)
{
    int nfds;
    int res;
    fd_set  readfds;
    struct timeval tv;

    tv.tv_sec = timout;
    tv.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    nfds = select(fd+1, &readfds, NULL, NULL, &tv);

    int len=0;
    memset(buff,0,SIZE_BUF);
    while(1)
    {
        if(nfds > 0 && FD_ISSET(fd,&readfds))
        {
            res=read(fd,buff+len,bytes-len);

            len=len+res;
            if (res<0)
            {
                perror("read error");
                return -1;
            }

            if (len>=bytes)
            {
                break;
            }
        }else if (nfds==0){
            perror("Waiting for timeout");
            return -1;
        }else if(nfds==-1){
            perror("select error:");
            return -1;
        }
    }
    return len;
}

float analysis(char high,char low)
{
    float data=(high<<8)+low;
    return data;
}

int modbus_data(int fd,pro_Data pro){
    char mod_data[8]={0};
    float w;
    memset(mod_data,0,8);
    mod_data[0]=pro.addr;
    mod_data[1]=pro.code;
    if (pro.len<=0)
    {
        printf("No ID or N parameters are set\n");
        return -1;
    }
    for (int i=0;i<pro.len;i++){
        mod_data[2]=pro.attr_data[i].id>>8;
        mod_data[3]=pro.attr_data[i].id&0xff;
        mod_data[4]=pro.attr_data[i].n>>8;
        mod_data[5]=pro.attr_data[i].n&0xff;

        short crc1=CRC16_Modbus(mod_data,6);
        mod_data[6]=(crc1&0xff00)>>8;
        mod_data[7]=crc1&0xff;

        int rlen=0;
        int wlen=write(fd, mod_data, 8);
        if(wlen<0)
        {
            perror("write fail!");
            return -1;
        }

        char buff[SIZE_BUF]={0};
        rlen=read_data(fd,buff,3,3);
        if (rlen<3)
        {
            printf("read len error!\n");
            return -1;
        }

        read_data(fd,buff,buff[rlen-1]+2,3);

        if (pro.attr_data[i].coeff<=0){
            pro.attr_data[i].coeff=1;
        }

         w=(float)(analysis(buff[0],buff[1])*pro.attr_data[i].coeff);
         printf("%s:%0.2f\n",pro.attr_data[i].desc,w);

    }
    return 0;
}
