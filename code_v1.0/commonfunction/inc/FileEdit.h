#ifndef _FILE_EDIT_H_
#define _FILE_EDIT_H_
#include <rtthread.h>

rt_int8_t create_file(const char* path,const char* filename);
rt_int8_t Read_file(const char* path,const char* filename,rt_int8_t* buf,rt_int16_t len);
rt_int8_t Write_file(const char* path,const char* filename,rt_int8_t* buf,rt_int16_t len);

#endif