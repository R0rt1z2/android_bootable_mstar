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

#include <common.h>
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <MsOS.h>
#include "language.h"
#include "LoaderCharset.h"
#include "LoaderCharset_Eng.h"


#define STRBUFF_LENGTH       256
#define OSD_LANGUAGE         "osd_language"
#define LANGSTR(ID,OFFSET)   pLangArray[(ID)*6+(OFFSET)]
#define LANGSTRLEN(ID)       (LANGSTR(ID,0)<<8)|(LANGSTR(ID,1))
#define LANGSTRADDR(ID)      (LANGSTR(ID,2)<<24)|(LANGSTR(ID,3)<<16)|(LANGSTR(ID,4)<<8)|LANGSTR(ID,5)
#define ENGLISH              0
#define CHINESE              1
#define RUSSIAN              2

typedef struct{
  char *language;
  MS_U8 languagenum;
}languageID;

languageID languageid[]={
    {"English",ENGLISH},
    {"Chinese",CHINESE},
    {"Russian",RUSSIAN},
};
static MS_U8 CHAR_BUFFER[STRBUFF_LENGTH];
const MS_U8* pLoaderCharacterset = NULL;
const MS_U8* pLoaderFont = NULL;
const MS_U8* pLoaderWidthData=NULL;
extern int strcmp(const char * cs,const char * ct);

MS_U8 get_languageID(void)
{
    int  i,size;
    MS_U8* language=NULL;
    MS_BOOL env=FALSE;
    language=(MS_U8*)getenv(OSD_LANGUAGE);
    if(language == NULL)
    {
        printf("Failed to get the language from the environment,please check");
        return 0;
    }

    size=sizeof(languageid)/sizeof(languageID);
    for(i=0;i<size;i++)
    {
        if(strcmp((const char *)language,(const char *)languageid[i].language) == 0)
        {
            env= TRUE;
            break;
        }
        else
        {
            env=FALSE;
        }
    }
    if(env == TRUE)
    {
        return languageid[i].languagenum;
    }
    else
    {
        return languageid[0].languagenum;
    }
}

MS_U8* OSD_GetString(MS_U32 id)
{
    const MS_U8* pLangArray=NULL;
    MS_U8 LangID;
    MS_U16 strlength;
    MS_U32 straddr;
    MS_U16 i=0;
    memset(CHAR_BUFFER,0,STRBUFF_LENGTH);
    id=id&0xffff;
    LangID=get_languageID();
    switch(LangID)
    {
        case 0:
            pLangArray=language0;
            break;
        case 1:
            pLangArray=language1;
            pLoaderCharacterset=LoaderCharacterset1;
            pLoaderFont=LoaderFont1;
            pLoaderWidthData=LoaderWidthData1;
            break;
        case 2:
            pLangArray=language2;
            pLoaderCharacterset=LoaderCharacterset2;
            pLoaderFont=LoaderFont2;
            pLoaderWidthData=LoaderWidthData2;
            break;
        case 3:
            pLangArray=language3;
            pLoaderCharacterset=LoaderCharacterset3;
            pLoaderFont=LoaderFont3;
            pLoaderWidthData=LoaderWidthData3;
            break;
        case 4:
            pLangArray=language4;
            pLoaderCharacterset=LoaderCharacterset4;
            pLoaderFont=LoaderFont4;
            pLoaderWidthData=LoaderWidthData4;
            break;
        case 5:
            pLangArray=language5;
            pLoaderCharacterset=LoaderCharacterset5;
            pLoaderFont=LoaderFont5;
            pLoaderWidthData=LoaderWidthData5;
            break;
        case 6:
            pLangArray=language6;
            pLoaderCharacterset=LoaderCharacterset6;
            pLoaderFont=LoaderFont6;
            pLoaderWidthData=LoaderWidthData6;
            break;
        case 7:
            pLangArray=language7;
            pLoaderCharacterset=LoaderCharacterset7;
            pLoaderFont=LoaderFont7;
            pLoaderWidthData=LoaderWidthData7;
            break;
        default:
            printf("no more language");
            break;
    }
    strlength=LANGSTRLEN(id);
    straddr=LANGSTRADDR(id);
    for(i=0;i<strlength;i++)
    {
        CHAR_BUFFER[i]=pLangArray[straddr+i];
    }
    CHAR_BUFFER[i]=' '; //space key

    return CHAR_BUFFER;

}

