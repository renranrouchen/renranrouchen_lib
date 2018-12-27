/*
 * Copyright (C) 2016 zwxf
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LOG_H
#define LOG_H
#include "logcfg.h"
#include <stdbool.h>

#ifndef LOG_TAG
#define LOG_TAG     "zwxf"
#endif

#define ARRAY_SIZE(arr)     (sizeof(arr)/sizeof(arr[0]))

typedef unsigned long   usize32_t;
typedef unsigned int    usize16_t;
typedef unsigned char   usize8_t;
typedef char   size8_t;
typedef bool            BOOL;

#define LOG_LEVEL_NUM   6
/* log level */
typedef enum{
	LOG_NONE   = 0x00,
    LOG_ASSERT = 0x01,
    LOG_DEBUG  = 0x02,
    LOG_WARN   = 0x03,
    LOG_ERRO   = 0x04,
    LOG_INFO   = 0x05,
    LOG_VERBOSE= 0x06,
}LOG_LVL;

/* out format */

enum{
    LOG_FORMAT_LEVEL= 0x01 << 0,
    LOG_FORMAT_TAG =  0x01 << 1,
    LOG_FORMAT_DIR =  0x01 << 2,
    LOG_FORMAT_FUNC=  0x01 << 3,
    LOG_FORMAT_LINE=  0x01 << 4,
#ifdef OS
    LOG_FORMAT_PID =  0x01 << 5;
    LOG_FORMAT_TIME=  0x01 << 6,
#endif
    LOG_FORMAT_NUM =  0x01 << 7,
};


#define GETLINE     __LINE__
#define GETTIME     __TIME__
#define GETFILE     __FILE__
#define GETFUNC     __FUNCTION__

void logOutput(const char* tag,LOG_LVL level,const int line,const char* file,const char* func,char* format,...);

#ifdef LOG_ENABLE

#define log_assert(tag,...) \
    logOutput(tag,LOG_ASSERT,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

#define log_debug(tag,...) \
    logOutput(tag,LOG_DEBUG,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

#define log_warn(tag,...) \
    logOutput(tag,LOG_WARN,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

#define log_erro(tag,...) \
    logOutput(tag,LOG_ERRO,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

#define log_info(tag,...) \
    logOutput(tag,LOG_INFO,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

#define log_verbose(tag,...) \
    logOutput(tag,LOG_VERBOSE,GETLINE,GETFILE,GETFUNC,__VA_ARGS__)

/* short API */
#ifndef DEFAULT_TAG 
#define assert(tag,...)  log_assert(tag,__VA_ARGS__)
#define debug(tag,...)   log_debug(tag,__VA_ARGS__)
#define warn(tag,...)    log_warn(tag,__VA_ARGS__)
#define erro(tag,...)    log_erro(tag,__VA_ARGS__)
#define info(tag,...)    log_info(tag,__VA_ARGS__)
#define verbose(tag,...) log_verbose(tag,__VA_ARGS__)
#else
#define assert(...)  log_assert(LOG_TAG,__VA_ARGS__)
#define debug(...)   log_debug(LOG_TAG,__VA_ARGS__)
#define warn(...)    log_warn(LOG_TAG,__VA_ARGS__)
#define erro(...)    log_erro(LOG_TAG,__VA_ARGS__)
#define info(...)    log_info(LOG_TAG,__VA_ARGS__)
#define verbose(...) log_verbose(LOG_TAG,__VA_ARGS__)
#endif

#else
#define assert(...)
#define debug(...)
#define warn(...)
#define erro(...)
#define info(...)
#define verbose(...)

#endif


typedef struct logFilter{
    usize8_t outLevel;
    usize8_t outFormat;
}filter;

typedef struct logcat{
    struct logFilter outfilter;
    BOOL logEnbale;
}Tlogcat;
static Tlogcat log;

void logcatInit();
void setLogLevel(LOG_LVL level);
inline void disableLogcat();
inline void enableLogcat();


#endif
