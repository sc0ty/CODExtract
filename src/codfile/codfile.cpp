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

#include <fstream>
#include <sstream>

#include "codfile.h"

CODFile::CODFile()
{
    HeadSeg = NULL;
    CodeSeg = NULL;
    DataSeg = NULL;
    FootSeg = NULL;
    isOpen = false;
} // CODFile::CODFile()


CODFile::~CODFile()
{
    Close();
} // CODFile::~CODFile()


void CODFile::LoadFile(const char *fname)
{
    Close();
    FName = fname;

    // open file
    ifstream is;
    is.open(fname, ios::binary);
    if (!is.is_open()) throw "Cannot read file";

    // read header
    HeadSeg = new TCODFILEHEADER;
    is.read((char *)HeadSeg, sizeof(TCODFILEHEADER));

    // check for zip file
    if (HeadSeg->flashid == ZIPSignature)
    {
        is.close();
        throw "Compressed ZIP file - extract it first";
    }

    // sanity check
    if (is.eof()) throw "Incorrect file format (file too short)";
    if (HeadSeg->data_size < sizeof(TDATASEGHEADER)) throw "Incorrect file format";

    // read code segment
    CodeSeg = new char[HeadSeg->code_size];
    is.read((char *)CodeSeg, HeadSeg->code_size);
    if (is.eof()) throw "Incorrect file format";

    // read data segment
    DataSeg = new char[HeadSeg->data_size];
    is.read((char *)DataSeg, HeadSeg->data_size);
    if (is.eof()) throw "Incorrect file format";

    is.close();
    isOpen = true;

    /* read resources */

    // data segment header
    TDATASEGHEADER *dh = (TDATASEGHEADER *)DataSeg;

    // sanity check
    if ((dh->exported_string_offset > dh->data_bytes_offset) ||
        (dh->data_bytes_offset > HeadSeg->data_size))
        throw "Incorrect file format";

    // resources array
    TCODRESOURCE *res = (TCODRESOURCE *)(DataSeg + dh->exported_string_offset);

    // resources array size
    int ressz = (dh->data_bytes_offset - dh->exported_string_offset) / sizeof(TCODRESOURCE);

    CODResource *r;

    // read resources from data segment
    for (int i=0; i<ressz; i++)
    {
        r = new CODResource(&res[i], DataSeg, HeadSeg->data_size);
        Resources.push_back( *r );
    }
} // CODFile::LoadFile(char *fname)


bool CODFile::LoadParent()
{
    if (!isOpen) throw "Load file first";

    // parent already loaded
    if (!isChildCOD()) return false;

    /* extract parent filename from cod file */

    // data segment header
    TDATASEGHEADER *dh = (TDATASEGHEADER *)DataSeg;

    // sanity check
    if (dh->exported_string_offset > HeadSeg->data_size)
        throw "Incorrect file format";

    // export names ptr array
    u16 *expn = (u16 *)(DataSeg + sizeof(TDATASEGHEADER) +
                        sizeof(u16)*dh->num_classes +
                        sizeof(u16)*2*dh->num_modules);

    // check if ptr is in data section
    if ( *expn >= HeadSeg->data_size)
        throw "Incorrect file format";

    /* create parent filename */

    string fn = FName;
    fn = &DataSeg[*expn];
	size_t s;

	// find every $ characters
	while ((s = fn.find('$')) != string::npos)
	{
		int c;
		istringstream iss(fn.substr(s+1, 2));
		iss >> hex >> c;
		fn.replace(s, 3, 1, (char)c);
	}

	// extract directory from path
    size_t sz = FName.find_last_of("\\/");
	if (sz != string::npos) fn = FName.substr(0, sz+1) + fn;
    fn += ".cod";

    // load parent cod file
    LoadFile(fn.c_str());

    return true;
} // CODFile::LoadParent()


void CODFile::Close()
{
    if (HeadSeg) delete HeadSeg;
    if (CodeSeg) delete [] CodeSeg;
    if (DataSeg) delete [] DataSeg;
    if (FootSeg) delete [] FootSeg;

    HeadSeg = NULL;
    CodeSeg = DataSeg = FootSeg = NULL;
    Resources.clear();

    isOpen = false;
} // CODFile::Close()


CODResource *CODFile::FindResource(CODRESType type)
{
    if (!isOpen) throw "Load file first";

    list<CODResource>::iterator it;

    for(it=Resources.begin(); it!=Resources.end(); it++)
    {
        if (type == it->getType()) return &*it;
    }

    // can't find
    return NULL;
} // CODFile::FindResource(CODRESType type)


string CODFile::getFileName()
{
    return FName;
} // *CODFile::getFileName()


time_t *CODFile::getCreateTimestamp()
{
    if (!isOpen) throw "Load file first";
    return (time_t *)&(HeadSeg->timestamp);
} // CODFile::getCreateTimestamp()


bool CODFile::isChildCOD()
{
    if (!isOpen) throw "Load file first";

    return HeadSeg->flags & 1;
} // CODFile::isChildCOD()


string CODFile::getVersion()
{
    if (!isOpen) throw "Load file first";

    // data segment header
    TDATASEGHEADER *dh = (TDATASEGHEADER *)DataSeg;

    // sanity check - classes and modules list
    u16 tmp = dh->num_classes + 2*dh->num_modules;
    tmp *= sizeof(u16);
    tmp += sizeof(TDATASEGHEADER);
    if (tmp > dh->exported_string_offset) throw "Incorrect file format";

    // there is no modules on the list
    if (dh->num_modules == 0) throw "No version string in module";

    // module version ptr array
    u16 *modv = (u16 *)(DataSeg + sizeof(TDATASEGHEADER) +
                        sizeof(u16)*dh->num_classes +
                        sizeof(u16)*dh->num_modules);

    // check if ptr are in data section
    if (*modv >= HeadSeg->data_size)
            throw "Incorrect file format";

    return &DataSeg[ *modv ];
} // CODFile::getVersion()


string CODFile::getAppName()
{
    if (!isOpen) throw "Load file first";

    // find resource with name
    CODResource *r = FindResource(CODRESTAppName);

    // can't find
    if (r == NULL) throw "No AppName in resources";

    return *( new string(r->getData()+2, r->getReversedU16(0)) );

} // CODFile::getAppName()


string CODFile::getDescription()
{
    if (!isOpen) throw "Load file first";

    // find resource with name
    CODResource *r = FindResource(CODRESTDescription);

    // can't find
    if (r == NULL) throw "No Description in resources";

    return *( new string(r->getData()+2, r->getReversedU16(0)) );
} // CODFile::getDescription()


string CODFile::getVendor()
{
    if (!isOpen) throw "Load file first";

    // find resource with name
    CODResource *r = FindResource(CODRESTVendor);

    // can't find
    if (r == NULL) throw "No Vendor in resources";

    return *( new string(r->getData()+2, r->getReversedU16(0)) );
} // CODFile::getVendor()


char *CODFile::getIcon(u16 &size)
{
    if (!isOpen) throw "Load file first";

    // find resource with icon
    CODResource *r = FindResource(CODRESTIcon);

    // can't find
    if (r == NULL) return NULL;

    size = r->getReversedU16(2);
    return r->getData() + 4;

} // CODFile::getIcon(ushort *size)

