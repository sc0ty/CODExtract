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

#ifndef DATAHEADER_H_INCLUDED
#define DATAHEADER_H_INCLUDED

#include "../types.h"

/* COD Data segment header struct */
typedef struct
{
    u8  flags;
    u8  version;
    u16 num_icalls;
    u8  num_modules;
    u8  num_classes ;
    u16 exported_string_offset;
    u16 data_bytes_offset;
    u16 empty_field;
    u16 class_definitions;
    u8  unknow1[14];
    u16 aliases;
    u8  unknow2[22];
} TDATASEGHEADER;

#endif // DATAHEADER_H_INCLUDED
