/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        Csv.cpp

  Description: Helper to access a CSV file using VimbaCPP.
               

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#include <vector>

#include <Csv.h>

#include <VimbaCPP/Include/VimbaCPP.h>

namespace AVT {
namespace VmbAPI {
namespace Examples {

Csv::Csv()
{
    m_VD = ';';
    m_RD = '\n';
    m_bHasHeader = false;
}

void Csv::GetVD ( char& VD )
{
    VD = m_VD;
}

void Csv::SetVD ( char VD )
{
    m_VD = VD;
}

void Csv::GetRD ( char& RD )
{
    RD = m_RD;
}

void Csv::GetHasHeader ( bool& bHasHeader )
{
    bHasHeader = m_bHasHeader;
}

void Csv::SetHasHeader ( bool bHasHeader )
{
    m_bHasHeader = bHasHeader;
}

void Csv::CsvRow::GetLineText ( string& lineText )
{
    lineText = m_lineText;
}

void Csv::CsvRow::SetLineText ( string lineText )
{
    m_lineText = lineText;
}

// Class to write data to a CSV file
Csv::CsvSave::CsvSave ( string filename, Csv& instance ) : m_instance ( instance ) 
{
    open( filename.c_str() , ofstream::out );
}

// Writes a single row to a CSV file.
bool Csv::CsvSave::Row ( Csv::CsvRow row )
{
    string lineText;
    row.GetLineText(lineText);    

    if ( 0 == lineText.size() )
    {
        return false;
    }
    char RD[2];
    RD[1] = 0;
    this->m_instance.GetRD(RD[0]);
    lineText.append((const char*)RD);

    write(lineText.c_str(), lineText.length());

    return true;
}

// Class to load data from CSV
Csv::CsvLoad::CsvLoad ( string filename, Csv& instance ): m_instance ( instance )
{
    open( filename.c_str() , ifstream::in );
}

// Reads a row of data from CSV
bool Csv::CsvLoad::Row ( Csv::CsvRow* row )
{
    char pLineText[256];
    getline(pLineText, 256);

    string lineText = string(pLineText);
    row->SetLineText(lineText);

    if ( 0 == lineText.size() )
    {
        return false;
    }

    int pos = 0;
    int rows = 0;

    while (pos < (int)lineText.length())
    {
        string value;
        int start = pos;

        while (pos < (int)lineText.length() && lineText[pos] != m_instance.m_VD)
        {
            pos++;
        }

        value = lineText.substr(start, pos - start);

        // Add field to list
        if (rows < (int)row->size())
        {
            row->at(rows) = value;
        }
        else
        {
            row->push_back(value);
        }
        rows++;

        if (pos < (int)lineText.length())
        {
            pos++;
        }
    }

    // Delete unused items
    while ((int)row->size() > rows)
    {
        row->erase(row->begin()+rows);
    }

    return (row->size() > 0);
}

}}} // namespace AVT::VmbAPI::Examples