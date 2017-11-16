//  Financial Computing 1, Fall 2016
//  Homework 1.1
//  File:  hw1.1.cpp
//  Authors:  Jingyi Guo
//  Description:  Reads cme.20160826.c.pa2 as its input file, and produces CL_and_NG_expirations_and_settlements.txt as its output file

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

bool TimeInRange(string line, int startnum)
{
	string year_str = line.substr(startnum, 4);
	int year = atoi(year_str.c_str());
	string month_str = line.substr(startnum+4, 2);
	int month = atof(month_str.c_str());
	if ((year <= 2016 && month < 10) || (year > 2018))
		return false;
	else
		return true;
}


int main()
{
	ifstream fin("cme.20160826.c.pa2");
	ofstream fout("CL_and_NG_expirations_and_settlements.txt");
	string line;
	bool before8 = true;
	fout << "Futures   Contract   Contract   Futures     Options   Options\n";
	fout << "Code      Month      Type       Exp Date    Code      Exp Date\n";
	fout << "-------   --------   --------   --------    -------   --------\n";
	while (getline(fin, line))
	{
		if (line[0] == 'B')//Type B records
		{
			if ((line[99] == 'C' && line[100] == 'L' && line[101] == ' ') || (line[99] == 'N' && line[100] == 'G' && line[101] == ' '))//CL or NG
			{
				if (line[15] == 'F' && line[16] == 'U' && line[17] == 'T' && TimeInRange(line, 18))//Futures
				{
					fout << line[99] << line[100] << "        ";//futures code
					//futures contract date
					fout << line[18] << line[19] << line[20] << line[21] << "-" << line[22] << line[23] << "    ";
					fout << "Fut        ";//contract type
					//expiration date
					fout << line[91] << line[92] << line[93] << line[94] << "-" << line[95] << line[96] << "-" << line[97] << line[98] << "\n";
				} 
				if (line[15] == 'O' && line[16] == 'O' && line[17] == 'F' && ((line[5] == 'L' && line[6] == 'O') || (line[5] == 'O' && line[6] == 'N')) && TimeInRange(line,27))// LO or ON Options
				{
					fout << line[99] << line[100] << "        ";//futures code
					//options contract date
					fout << line[27] << line[28] << line[29] << line[30] << "-" << line[31] << line[32] << "    ";
					fout << "Opt                    ";//contract type
					fout << line[5] << line[6] << "        ";//options code
					//expiration date
					fout << line[91] << line[92] << line[93] << line[94] << "-" << line[95] << line[96] << "-" << line[97] << line[98] << "\n";
				}

			}
		}
		if (line[0] == '8' && line[1] == '1' && line[2]== 'N' && line[3]=='Y' && line[4]=='M')//Type 8 Records
		{
			if (before8)
			{
				fout << "\n";
				fout << "Futures   Contract   Contract   Strike   Settlement\n";
				fout << "Code      Month      Type       Price    Price\n";
				fout << "-------   --------   --------   ------   ----------\n";
				before8 = false;
			}
			if ((line[15] == 'C' && line[16] == 'L' && line[17] == ' ') || (line[15] == 'N' && line[16] == 'G'&& line[17] == ' '))//CL or NG
			{
				if (line[25] == 'F' && line[26] == 'U' && line[27] == 'T' && TimeInRange(line,29))//Futures
				{
					fout << line[15] << line[16] << "        ";//futures code[
					//futures contract date
					fout << line[29] << line[30] << line[31] << line[32] << "-" << line[33] << line[34] << "    ";
					fout << "Fut                 ";//contract type
					if (line[15] == 'C' && line[16] == 'L')
					{
						fout << line[118] << line[119] << "." << line[120] << line[121] << "\n";//settlement price
					}
					if (line[15] == 'N' && line[16] == 'G')
					{
						fout << line[116] << "." << line[117] << line[118] << line[119] << "\n";//settlement price
					}
					
				}
				if (line[25] == 'O' && line[26] == 'O' && line[27] == 'F' && TimeInRange(line,38) && ((line[5]=='L' && line[6]=='O')|| (line[5] == 'O' && line[6] == 'N')))//American Options
				{
					fout << line[15] << line[16] << "        ";//futures code
					//options contract date
					fout << line[38] << line[39] << line[40] << line[41] << "-" << line[42] << line[43] << "    ";
					if (line[28] == 'C')
					{
						fout << "Call       ";
					}
					if (line[28] == 'P')
					{
						fout << "Put        ";
					}
					if (line[15] == 'N' && line[16] == 'G')
					{
						fout << line[50] << "." << line[51] << line[52] << line[53] << "     ";//strike price
						fout << line[118] << "." << line[119] << line[120] << line[121] << "\n";//settlement price
					}
					if (line[15] == 'C' && line[16] == 'L')
					{
						fout << line[51] << "." << line[52] << line[53] << "     ";//strike price
						if (line[28] == 'P')
						{
							fout << " " << line[119] << "." << line[120] << line[121] << "\n";//settlement price
						}
						if (line[28] == 'C')
						{
							if (line[118] != '0')
							{
								fout << line[118] << line[119] << "." << line[120] << line[121] << "\n";//settlement price
							}
							else 
							    fout << " " << line[119] << "." << line[120] << line[121] << "\n";//settlement price
						}

					}
				}
			}
		}
	}
	return 0;
}
