/*     Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.
*/

/* CODExtractor - created by sc0ty 2011 - http://sc0ty.pl */

#ifndef CODHEADER_H_INCLUDED
#define CODHEADER_H_INCLUDED

#include "../types.h"

/* COD File header struct */
typedef struct
{
    u32 flashid;
    u32 section_number; //always 0
    u32 vtable_pointer; //always 0
    u32 timestamp;
    u32 user_version;
    u32 fieldref_pointer;
    u16 maxtype_list_size;
    u16 reserved;       //always 0xFF
    u32 data_section;   //always 0xFFFF
    u32 module_info;    //always 0xFFFF
    u16 version;
    u16 code_size;
    u16 data_size;
    u16 flags;
} TCODFILEHEADER;

#endif // CODHEADER_H_INCLUDED
