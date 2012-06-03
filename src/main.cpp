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

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "codfile/codfile.h"

using namespace std;


void ShowCODInfo(CODFile *cod);
void SaveIcon(CODFile *cod);

int main(int argc, char *argv[])
{
    cout << endl << "  COD Extractor - created by sc0ty 2011 - http://sc0ty.pl";
    if (argc < 2)
    {
        cout << endl << endl << "use:" << endl;
        cout << "  codextract <file.cod> <file-2.cod> ... <file-n.cod>" << endl << endl;
        return 0;
    }

    CODFile cod;

    for (int i=1; i<argc; i++)
    {
        try
        {
            // load file and show filename
            cout << endl << endl << "  File \"" << argv[i] << "\"";
            cod.LoadFile(argv[i]);

            // load parent file and show filename
            if (cod.LoadParent())
            {
                cout << ", parent file \"" << cod.getFileName() << "\"";
            }

            // show info
            cout << endl << endl;
            ShowCODInfo(&cod);
            SaveIcon(&cod);
        }

        catch (const char *ex)
        {
            cout << endl << endl << "\tError: " << ex;
            cout << " (" << cod.getFileName() << ")" << endl;
        }

        cod.Close();
    }

    cout << endl;
    return 0;
}


void ShowCODInfo(CODFile *cod)
{
    string actime, aname, adesc, avend, aver;
    actime = ctime(cod->getCreateTimestamp());

    try
    {
        aname = cod->getAppName();
    }
    catch (...)
    {
        aname = "<none>";
    }

    try
    {
        adesc = cod->getDescription();
    }
    catch (...)
    {
        adesc = "<none>";
    }

    try
    {
        avend = cod->getVendor();
    }
    catch (...)
    {
        avend = "<none>";
    }

    try
    {
        aver = cod->getVersion();
    }
    catch (...)
    {
        aver = "<none>";
    }

    cout << "\t    Created: " << actime;
    cout << "\t    AppName: " << aname << endl;
    cout << "\t Version no: " << aver << endl;
    cout << "\tDescription: " << adesc << endl;
    cout << "\t     Vendor: " << avend << endl;
} // ShowCODInfo(CODFile *cod)


void SaveIcon(CODFile *cod)
{
    u16 size;
    char *data = cod->getIcon(size);

    // there is no icon in resources
    if (data == NULL)
    {
        cout << "\t  Icon file: <there is no icon>" << endl;
        return;
    }

    // generate icon file name
    string fn = cod->getFileName();
    size_t sz = fn.find_last_of('.');
    if (sz != string::npos) fn.resize(fn.find_last_of('.'));
    fn += "-icon.png";

    cout << "\t  Icon file: " << fn << endl;

    // open file to write
    ofstream of(fn.c_str(), ios::binary);
    if (!of.is_open()) throw "Cannot write to file";

    // write icon
    of.write(data, size);
    of.close();
} // SaveIcon(CODFile *cod);
