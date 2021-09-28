/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/
#include <common.h>
#include <malloc.h>
#include <MsString.h>
#include <ShareType.h>
#include <linux/ctype.h>

char* strlower(char* str)
{
    int i = 0;
    if(str == NULL)
    {
       return NULL;
    }
    while(str[i]) 
    {
       str[i] = tolower(str[i]);
       i++;
    }
    return str;
}

char* array_reverse(char * array,int ArraySize)
{
   int i=0;
   char tmp;
   if(array == NULL)
       return NULL;
   for(i=0;i<(ArraySize/2);i++)
   {
       tmp=array[ArraySize-1-i];
       array[ArraySize-1-i]=array[i];
       array[i]=tmp;
   }
   return array; 
}

char* strrps(char* source,char* findstr, char *repstr)
{
    int i= 0;      
    int Replenth = 0;   
    char *tmp = NULL;   
    char *substr = NULL;

    substr=strstr(source,findstr);
    if(substr == NULL)
        return NULL;
    tmp=(char *)malloc(strlen(substr)+1); 
    if(tmp == NULL)
        return NULL;
    strcpy(tmp,substr+strlen(findstr));
    Replenth = strlen(repstr);
    for(i=0 ; i < Replenth; i++)
    {
        substr[i]=repstr[i];             
    }
    substr[Replenth]='\0';
    strcat(substr,tmp);
    free(tmp);
    
    return source;
}

char* ParaReplace(char *str,int argc,char * const argv[])
{
    int index = 0;
    static char tmpstr[SCRIPT_BUF_SIZE];
    char *strptr = NULL;
    char findstr[3];
    findstr[0]='$';
    findstr[2]='\0';
    memset(tmpstr,0,sizeof(tmpstr));
    strcpy(tmpstr,str);
    strptr = tmpstr;
    while((strptr=strstr(strptr,"$"))!= NULL)
    {
        if( *(strptr+1)>'0' && *(strptr+1) < '9') //check the parameter is between 0 to 9.
        {
            index = *(strptr+1) - '0' ;
            if(index > argc)
            {
                printf("Error: Parameters is bigger than argc!\n");
                return NULL;
            }
            else
            {
                findstr[1]=*(strptr+1);
                strrps(tmpstr,findstr,argv[index]);
            }
        }
        else
        {
            strptr=strptr+1;
        }
    }
    return tmpstr;
}
