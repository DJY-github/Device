#include"GetProfile.h"

char * l_trim(char* szOutput, const char* szInput)
{
    if (szInput == NULL )
    {
        printf("szinput null!");
        return NULL;
    }else if (szOutput == NULL)
    {
        printf("szinput null!");
        return NULL;
    }else if (szOutput == szInput)
    {
        printf("szinput null!");
        return NULL;
    }

    for(;*szInput!='\0' && isspace(*szInput);++szInput);

    return strcpy(szOutput, szInput);
}

char* a_trim(char * szOutput, const char * szInput)
{
    char *p;
    if (szInput == NULL )
    {
        printf("szinput null!");
        return NULL;
    }else if (szOutput == NULL)
    {
        printf("szinput null!");
        return NULL;
    }

    l_trim(szOutput, szInput);
    for(p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p){
        ;
    }
    *(++p) = '\0';
    return szOutput;
}

int atoh(char s[]){
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
    {
        i = 2;
    }
    else
    {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
    {
        if (tolower(s[i]) > '9')
        {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        }
        else
        {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return n;
}

float stof(char s[]){
    int spot=0;
    float n=0;
    if (s[0] =='.' ){
        printf("Ineffective coefficient!");
        return -1;
    }

    for (int i=0;i<strlen(s);i++){
        if (isalpha(s[i])){
            printf("Coefficient contains letters\n");
            return -1;
        }
    }

    for (int i=0;(s[i]>='0'&& s[i]<='9') || s[i] == '.' ;++i){
        if (spot==0 && s[i]=='.'){
            spot=1;
            continue;
        }
        if (s[i]=='.' && spot==1){
            printf("Redundant decimal point\n");
            return -1;
        }
   }

   return atof(s);
}

int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal )
{
    char appname[SIZE],keyname[SIZE];
    char *buf,*c;
    char buf_i[SIZE], buf_o[SIZE];
    FILE *fp;

    int found=0; /* 1 AppName 2 KeyName */
    if( (fp=fopen( profile,"r" ))==NULL ){
        printf( "openfile [%s] error [%s]\n",profile,strerror(errno) );
        return(-1);
    }
    fseek( fp, 0, SEEK_SET );
    memset( appname, 0, sizeof(appname) );
    sprintf( appname,"[%s]", AppName );

    while( !feof(fp) && fgets( buf_i, SIZE, fp )!=NULL ){
        l_trim(buf_o, buf_i);
        if( strlen(buf_o) <= 0 )
            continue;
        buf = NULL;
        buf = buf_o;

        if( found == 0 )
        {
            if( buf[0] != '[' )
            {
                continue;
            }else if (strncmp(buf,appname,strlen(appname))==0 )
            {
                found = 1;
                continue;
            }
        } else if( found == 1 ){
            if( buf[0] == '#' ){
                continue;
            } else if ( buf[0] == '[' ) {
                break;
            } else {
                if((c = strstr(buf,KeyName) )!=NULL){
                int len=strlen(KeyName);
                sscanf(c+len+1, "%[^\n]", KeyVal );
                char *KeyVal_o = (char *)malloc(strlen(KeyVal) + 1);
                if(KeyVal_o != NULL)
                {
                    memset(KeyVal_o, 0, sizeof(KeyVal_o));
                    a_trim(KeyVal_o, KeyVal);
                    if(KeyVal_o && strlen(KeyVal_o) >0)
                        strcpy(KeyVal, KeyVal_o);
                    free(KeyVal_o);
                    KeyVal_o = NULL;
                }
                found = 2;
                break;
                }else{
                    continue;
                }
            }
        }
    }
    fclose( fp );
    if( found == 2 )
        return atoi(KeyVal);
    else
        return -1;
}

int GetProfileData(char *profile, char *AppName, Field* KeyVal )
{
    char appname[SIZE],keyname[SIZE];
    char *buf,*id,*n,*c,*desc;
    char buf_i[SIZE], buf_o[SIZE];
    FILE *fp;

    int found=0; /* 1 AppName 2 KeyName */
    if( (fp=fopen( profile,"r" ))==NULL ){
        printf( "openfile [%s] error [%s]\n",profile,strerror(errno) );
        return -1;
    }
    fseek( fp, 0, SEEK_SET );
    memset( appname, 0, sizeof(appname) );
    sprintf( appname,"[%s]", AppName );

    int len=0;
    while( !feof(fp) && fgets( buf_i, SIZE, fp )!=NULL ){
        l_trim(buf_o, buf_i);
        if( strlen(buf_o) <= 0 )
            continue;
        buf = NULL;
        buf = buf_o;
        if( found == 0 )
        {
            if( buf[0] != '[' )
            {
                continue;
            }else if (strncmp(buf,appname,strlen(appname))==0 )
            {
                found = 1;
                continue;
            }
        } else if( found == 1 ){
            if( buf[0] == '#' ){
                continue;
            } else if ( buf[0] == '[' ) {
                break;
            } else {
                if (strstr(buf,"val=")!=NULL){
                    continue;
                }

                if( (id = strstr(buf,"id=")) != NULL && (n=strstr(buf,"n=")) != NULL && \
                    (c=strstr(buf,"coeff=")) != NULL && (desc=strstr(buf,"(")) != NULL){
                        char c_id[SIZE],c_n[SIZE],c_con[SIZE],c_desc[SIZE];
                        char o_id[SIZE],o_n[SIZE],o_con[SIZE];
                        memset(c_id,0,SIZE);
                        memset(c_n,0,SIZE);
                        memset(c_con,0,SIZE);
                        sscanf(id+3, "%[^\H ]", c_id);

                        a_trim(o_id,c_id);

                        sscanf(n+2, "%[^\ ]", c_n);

                        a_trim(o_n,c_n);

                        sscanf(c+6, "%[^\ ]", c_con);
                        a_trim(o_con,c_con);

                        sscanf(desc+1,"%[^\)]",c_desc);

                        int id=atoh(o_id);
                        int n=atoh(o_n);
                        float coeff=atof(o_con);
                        if (n>0){
                            KeyVal[len].id=id;
                            KeyVal[len].n=n;
                            KeyVal[len].coeff=coeff;
                            memcpy(KeyVal[len].desc,c_desc,SIZE);

                            len+=1;
                        }else{
                            continue;
                        }
                    }
                }
            }
        }
    fclose( fp );
    return len;
}

pro_Data GetconfigData(char *profile)
{
    pro_Data pro;
    memset(&pro,0,sizeof(pro));
    char addr[SIZE],code[SIZE];

    int i,j;
    i=GetProfileString(profile,"Address","addr",addr);
    j=GetProfileString(profile,"Function","code",addr);
    pro.addr=i;
    pro.code=j;

    Field data[BUFF_SIZE]={0};
    int len=GetProfileData(profile,"Data",data);

    for (int i=0;i<len;i++){
        pro.attr_data[i].id=data[i].id;
        pro.attr_data[i].n=data[i].n;
        pro.attr_data[i].coeff=data[i].coeff;
        memcpy(pro.attr_data[i].desc,data[i].desc,SIZE);
    }
    pro.len=len;
    return pro;
}
