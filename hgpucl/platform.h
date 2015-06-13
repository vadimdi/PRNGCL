/******************************************************************************
 * @file     platform.h
 * @author   Vadim Demchik <vadimdi@yahoo.com>
 * @version  1.0.2
 *
 * @brief    [HGPU library]
 *           Interface for OpenCL AMD APP & nVidia SDK environment
 *
 *
 * @section  LICENSE
 *
 * Copyright (c) 2013-2015 Vadim Demchik
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

#ifndef platform_H
#define platform_H

#include <stdio.h>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #include <direct.h>
  #define GetCurrentDir _getcwd
  static const char slash[]="\\"; 
  #define snprintf(b,size,fmt,...) _snprintf_s((b),(size),_TRUNCATE,(fmt),##__VA_ARGS__)
#else
  #include <string.h>
  #include <cstdlib>
  #include <unistd.h>
  #define GetCurrentDir getcwd
  static const char slash[]="/";
  
  #define	sprintf_s(b,size,fmt,...) sprintf((b),(fmt),##__VA_ARGS__)
  #define	fopen_s(stream, buffer, mode) (*stream = fopen(buffer, mode))
  #define   _dupenv_s(value, len, variable) (*value = getenv(variable))
  #define	sscanf_s(buffer, format, ...) (sscanf(buffer, format,##__VA_ARGS__))
  #define	_getch() (getchar())
  #define	ctime_s(timebuf,timez,ltime) (timebuf=ctime(ltime))
  #define	_strnset_s(Replace,replength,ch,replength2) (memset(Replace, ch, replength2))
  #define	strncpy_s(newtext, replength, resorc, lenz) (strlen(resorc)-snprintf(newtext, replength, "%s", resorc))
  #define	strncat_s(newtext, replength, resorc, lenz) (snprintf(newtext, lenz, "%s%s", newtext, resorc))
  #define   _snprintf_s(newtext, replength, flag, format, val1, val2 ) snprintf(newtext,replength, format, val1, val2)
  #define	strcpy_s(newtext, replength, resorc) (strcpy(newtext, resorc))
  #define	memcpy_s(destination, destination_length, source, len) (memcpy(destination, source, len))
  #define	strcat_s(newtext, replength, resorc, lenz) (strcat(newtext, resorc))
  #define	Sleep(tm) { for(int i=1;i<=(int) (tm/CLOCKS_PER_SEC);i++) sleep(1); }
#endif

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <time.h>
#include <malloc.h>

#endif
