#ifndef MODBUS_H_INCLUDED
#define MODBUS_H_INCLUDED

#include"GetProfile.h"
#define SIZE_BUF 256

short CRC16_Modbus (char*, int);

int write_data(int ,char* ,size_t );

int read_data(int , char *, size_t , unsigned int );

float analysis(char ,char );

int modbus_data(int ,pro_Data );
#endif // MODBUS_H_INCLUDED
