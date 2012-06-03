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

#ifndef CODRESOURCE_H_INCLUDED
#define CODRESOURCE_H_INCLUDED

#include "../types.h"

using namespace std;


/* COD resource data format */
typedef struct
{
    u16 type_ptr;
    u16 size;
    u16 data_ptr;
} TCODRESOURCE;


/* COD Resource types */
enum CODRESType
{
    CODRESTUndefined,
    CODRESTUnknown,
    CODRESTAppName,
    CODRESTDescription,
    CODRESTVendor,
    CODRESTIcon
};


/* COD resource struct */
class CODResource
{
    private:
        char    *type;
        u16     size;
        char    *data;

        CODRESType itype;

    public:
        CODResource(TCODRESOURCE *res, char *DataSeg, u16 DataSZ);

        // return resource type
        CODRESType getType();

        // return resource data
        char *getData();

        // return resource data size
        u16 getSize();

        // get u16 number from givven data offset as reversed byte order
        u16 getReversedU16(u16 offset);
};


/* Types of resources */
const u8 CODRESAppName[] = {0x5f, 0x18, 0x70, 0x4e, 0x2c, 0x65, 0x73};
const u8 CODRESDescription[] = {0x5f, 0x06, 0x73, 0x63, 0x72, 0x69, 0x88};
const u8 CODRESVendor[] = {0x5f, 0x76, 0x0c9, 0x19};
const u8 CODRESIcon[] = {0x5f, 0x18, 0x70, 0x49, 0x63, 0x10, 0x73};


#endif // CODRESOURCE_H_INCLUDED
