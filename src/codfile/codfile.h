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

#ifndef CODFILE_H
#define CODFILE_H

#include <list>
#include <string>

#include "codheader.h"
#include "dataheader.h"
#include "codresource.h"

using namespace std;


const u32 ZIPSignature = 0x04034B50;


class CODFile
{
    private:
        string FName;   // file name
        TCODFILEHEADER *HeadSeg;     // header section
        char *CodeSeg;  // code section
        char *DataSeg;  // data section
        char *FootSeg;  // footer section
        bool isOpen;    // file was open
        list<CODResource> Resources; // resources

    public:
        CODFile();
        virtual ~CODFile();

        // load cod from file
        void LoadFile(const char *fname);

        // load parent cod file
        // return false if parent is already loaded
        bool LoadParent();

        // free resources
        void Close();

        // find resource by type, if can't - return NULL
        CODResource *FindResource(CODRESType type);

        // return true if this is child cod file
        bool isChildCOD();

        // return file name
        string getFileName();

        // return cod file creat timestamp
        time_t *getCreateTimestamp();

        // return version number of parent cod
        string getVersion();

        // read application name, description and vendor from resources
        string getAppName();
        string getDescription();
        string getVendor();

        // get icon from resources, return icon data or NULL if there is no icon
        char *getIcon(u16 &size);
};

#endif // CODFILE_H
