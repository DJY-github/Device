#ifndef GETPROFILE_H_INCLUDED
#define GETPROFILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include<unistd.h>

#include <arpa/inet.h>
#define SIZE 64
#define BUFF_SIZE 256

typedef struct field_data{
    int id;
    int n;
    float coeff;
    char desc[SIZE];
}Field;

typedef struct profile_data{
    uint8_t addr;
    uint8_t code;

    Field attr_data[BUFF_SIZE];
    uint8_t len;
}pro_Data;

char* l_trim(char *, const char *);

char* a_trim(char *, const char * szInput);

int GetProfileString(char*, char*, char*, char*);

int GetField(char*,char* ,char* );

int GetProfileData(char*, char*, Field* );

pro_Data GetconfigData(char*);
#endif // GETPROFILE_H_INCLUDED
