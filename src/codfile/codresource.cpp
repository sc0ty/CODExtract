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

#include <cstring>

#include "codresource.h"
#include "dataheader.h"


CODResource::CODResource(TCODRESOURCE *res, char *DataSeg, u16 DataSZ)
{
    // set data
    type = (char *)(DataSeg + res->type_ptr);
    size = res->size;
    data = (char *)(DataSeg + res->data_ptr);
    itype = CODRESTUndefined;

    // sanity check
    if ( (res->type_ptr + (strlen(type)) >= DataSZ) ||
         (res->data_ptr + size >= DataSZ))
        throw "Incorrect file format";

	#define CMPRES(res) ((strlen(type) == sizeof(res)) && \
						 (memcmp(res, (u8 *)type, sizeof(res)) == 0))

    // decode resource type
    if      (CMPRES(CODRESAppName))     itype = CODRESTAppName;
    else if (CMPRES(CODRESDescription)) itype = CODRESTDescription;
    else if (CMPRES(CODRESVendor))      itype = CODRESTVendor;
    else if (CMPRES(CODRESIcon))        itype = CODRESTIcon;
    else itype = CODRESTUnknown;

} // CODResource(TCODRESOURCE *res, char *DataSeg, char *DataSZ)


CODRESType CODResource::getType()
{
    return itype;
} // CODResource::getType()


char *CODResource::getData()
{
    return data;
} // CODResource::getData()


u16 CODResource::getSize()
{
    return size;
} // CODResource::getSize()


u16 CODResource::getReversedU16(u16 offset)
{
    // sanity check
    if (offset+1 >= size) throw "Exceeded end of resource data";

    // get two bytes from data
    u8 b1 = data[offset];
    u8 b2 = data[offset + 1];

    // revers byte order and return
    return (u16)(b1 * 256 + b2);
} // CODResource::getReversedU16(u16 offset)
