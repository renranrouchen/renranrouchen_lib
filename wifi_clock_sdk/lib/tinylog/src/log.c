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
#include <stdarg.h>
#include <stdio.h>

#include "log/log.h"
#include "log/log_str.h"
#include "log/log_port.h"


#ifdef ENABLE_COLOR
#define CSI_START                      "\033["
#define CSI_END                        "\033[0m"
/* output log front color */
#define F_BLACK                        "30;"
#define F_RED                          "31;"
#define F_GREEN                        "32;"
#define F_YELLOW                       "33;"
#define F_BLUE                         "34;"
#define F_MAGENTA                      "35;"
#define F_CYAN                         "36;"
#define F_WHITE                        "37;"
/* output log background color */
#define B_NULL
#define B_BLACK                        "40;"
#define B_RED                          "41;"
#define B_GREEN                        "42;"
#define B_YELLOW                       "43;"
#define B_BLUE                         "44;"
#define B_MAGENTA                      "45;"
#define B_CYAN                         "46;"
#define B_WHITE                        "47;"
/* output log fonts style */
#define S_BOLD                         "1m"
#define S_UNDERLINE                    "4m"
#define S_BLINK                        "5m"
#define S_NORMAL                       "22m"

#ifndef COLOR_ASSERT
#define COLOR_ASSERT              (F_MAGENTA B_NULL S_NORMAL)
#endif
#ifndef COLOR_ERROR
#define COLOR_ERROR               (F_RED B_NULL S_NORMAL)
#endif
#ifndef COLOR_WARN
#define COLOR_WARN                (F_YELLOW B_NULL S_NORMAL)
#endif
#ifndef COLOR_INFO
#define COLOR_INFO                (F_CYAN B_NULL S_NORMAL)
#endif
#ifndef COLOR_DEBUG
#define COLOR_DEBUG               (F_GREEN B_NULL S_NORMAL)
#endif
#ifndef COLOR_VERBOSE
#define COLOR_VERBOSE             (F_BLUE B_NULL S_NORMAL)
#endif
#endif

static const size8_t* levelName[LOG_LEVEL_NUM] = {
    "A: ",
    "D: ",
    "W: ",
    "E: ",
    "I: ",
    "V: ",
};

const size8_t* outputColorTb[LOG_LEVEL_NUM] = {
    COLOR_ASSERT,
    COLOR_DEBUG,
    COLOR_WARN,
    COLOR_ERROR,
    COLOR_INFO,
    COLOR_VERBOSE,
};

inline usize8_t getLoglevel(struct logcat* log)
{
    return log->outfilter.outLevel;
}

void outbuffDirectory(char* buff,int n)
{
    outPort(buff,n);
}

void packageLog	\
(const char* tag,usize8_t level,const int line,const char* file,const char* func,char* buff,int* len,char* fmt,va_list* vList)
{
    usize8_t format = log.outfilter.outFormat;
#ifdef ENABLE_COLOR
    *len += mymemcpy(buff + *len,CSI_START);
    *len += mymemcpy(buff + *len,outputColorTb[level - 1]);
#endif

    if(format & LOG_FORMAT_LEVEL){
        *len += mymemcpy(buff + *len,levelName[level - 1]);
    }
    if(format & LOG_FORMAT_TAG){
        *len += mymemcpy(buff + *len,tag);
    }
    if (format & LOG_FORMAT_DIR){
        *len += mymemcpy(buff + *len," Dir: ");
        *len += mymemcpy(buff + *len,file);
    }
    if(format & LOG_FORMAT_FUNC){
        *len += mymemcpy(buff + *len," Function: ");
        *len += mymemcpy(buff + *len,func);
    }
    if (format & LOG_FORMAT_LINE){
        /* add line message */
        *len += sprintf(buff + *len," line: %ld",line);
    }
#ifdef OS
    if (format & LOG_FORMAT_TIME){
        *len += mymemcpy(buff + *len," time: ");
        *len += mymemcpy(buff + *len,GETTIME);
    }

    if (format & LOG_FORMAT_PID){
        /* add pid message */
        *len += sprintf(buff + *len," PID: %ld",pid);
    }
#endif
    /* package rest string */
    *len += vsprintf(buff + *len,fmt,*vList);

#ifdef ENABLE_COLOR
    *len += mymemcpy(buff + *len,CSI_END);
#endif
}
void logOutput \
(const char* tag,LOG_LVL level,const int line,const char* file,const char* func,char* format,...)
{
    char logbuf[512];
	char* bp = logbuf;
    int loglen = 0;
    va_list args;
    va_start(args,format);
    if(!log.logEnbale)
        return;
	
    /* parse out */
    if(level > log.outfilter.outLevel)
        return;
    logLock();
    packageLog(tag,(usize8_t)level,line,file,func,logbuf,&loglen,format,&args);
    /* out buff directory*/
    outbuffDirectory(bp,loglen);
	bp = NULL;
	logUnlock();
	
    va_end(args);
}

void setLogLevel(LOG_LVL level)
{
    log.outfilter.outLevel = level;
}

inline void disableLogcat()
{
	log.logEnbale = 0;
}

inline void enableLogcat()
{
	log.logEnbale = 1;
}
void logcatInit()
{
	lockInit();
    log.logEnbale = 1;
    log.outfilter.outFormat = 0xFF;
    log.outfilter.outLevel = 6;
}
