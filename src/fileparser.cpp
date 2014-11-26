/*
 * fileparser.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#include "fileparser.h"
#include "mysql.h"
#include <sstream>
#include "connector.h"
#include <fstream>      // std::ifstream

using namespace std;

fileparser::fileparser(const char* fileName)
{
	if(!fileName)
		perror("no file name inserted - no instance will be made");

	string strTmp(fileName);
	pFileName = new char[strTmp.length() + 1]();
	strncpy(pFileName, fileName, strTmp.length() + 1);
}

fileparser::~fileparser()
{
	cout << "destructing the file parser " << endl;
	delete[] pFileName;
	pFileName = NULL;
}

retVal fileparser::parseFile()
{
	ifstream is (pFileName, ifstream::in | ifstream::binary);
	if(!is)
	{
		perror("No file descriptor");
		return error;
	}
	DBG("start of seeking");
	//load the file into memory
	// get length of file:
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);

	DBG("end of seeking");

	char * buffer = new char [length];
	if(!buffer)
	{
		perror("error with allocation of memory");
		return error;
	}
	is.read (buffer,length); //buffer contains the file
	is.close();

	string data(buffer);
	string startPhase = "<tr";
	string endPhase = "</tr>";
	string cuttedData;
	size_t location = -5;
	size_t endLocation;

	while((location = data.find(startPhase, location+5)) != string::npos) //iterate through the file
	{
		if((endLocation = data.find(endPhase, location)) != string::npos)
		{
			cuttedData = data.substr(location, endLocation - location);
			//cout << cuttedData << endl;
			parsePhrase(cuttedData);
		}
		else
		{
			break;
		}
	}

	delete[] buffer;
	return ok;
}

retVal fileparser::parsePhrase(string phrase)
{
	char debugBuff[100];
	string mustAppear = "flagicon";
	string startPhase = "<td";
	string endPhase = "</td>";
	string innerStartPhase = "</span>";
	string innerEndPhase = "<";
	string startEndPlacePhrase = ">";
	string cuttedData;
	string innerCuttedData;
	size_t start;
	size_t startEndPlace;
	size_t end;
	size_t innerStart;
	size_t innerEnd;

	//Data that will be used to make the query
	string vst_cntry = "United Kingdom";
	string vst_person = "Queen Elizabeth II";
	int day;
	char month[15];
	int year;
	string destination;
	char yearBuff[30];

	if((start = phrase.find(mustAppear)) == string::npos)//check for validity of input
		return error;

	start = phrase.find(startPhase);
	if(start != string::npos)
	{
		startEndPlace = phrase.find(startEndPlacePhrase, start);
		end = phrase.find(endPhase);
		if(end != string::npos)
		{
			cuttedData = phrase.substr(startEndPlace+startEndPlacePhrase.length(), end-startEndPlace-startEndPlacePhrase.length()); //cuttedData has the internal data segment
			size_t tmpLoc = 0;
			//cout << "The date: " << cuttedData << endl;
			sscanf(cuttedData.c_str(), "%d", &day);

			char * pch;
			char* pch2;
			char* ptr = new char[cuttedData.length() + 1];
			strncpy(ptr, cuttedData.c_str(), cuttedData.length() + 1);
			DBG(ptr);
			pch = strtok (ptr," ");
			while (pch != NULL)
			{
				pch2 = pch;

				int k = 0;
				while(pch[k] != 0)
				{
					pch[k] = tolower(pch[k]);
					k++;
				}
				if((strcmp(pch, "january") == 0) || (strcmp(pch, "february") == 0) || (strcmp(pch, "march") == 0) ||
				   (strcmp(pch, "april") == 0) || (strcmp(pch, "may") == 0) || (strcmp(pch, "june") == 0) ||
				   (strcmp(pch, "july") == 0) || (strcmp(pch, "august") == 0) || (strcmp(pch, "september") == 0) ||
				   (strcmp(pch, "october") == 0) || (strcmp(pch, "november") == 0) || (strcmp(pch, "december") == 0))
				{
					strcpy(month, pch);
				}

				pch = strtok (NULL, " ");

				if(!pch){
					strncpy(yearBuff, pch2, 4);
					yearBuff[4] = '\0';
				}

			}
			delete (ptr);

			sscanf(yearBuff, "%d", &year);
			string m(month);
			sprintf(debugBuff, "day: %u month: %d year: %u", day, MonthToNumber[m], year);
			//PRINT(debugBuff);


		}
	}

	start = phrase.find(startPhase, start + 5);
	if(start != string::npos)
	{
		end = phrase.find(endPhase, start + 5);
		if(end != string::npos)
		{
			cuttedData = phrase.substr(start+startPhase.length(), end-start-startPhase.length()+innerEndPhase.length()); //cuttedData has the internal data segment
			innerStart = cuttedData.find(innerStartPhase); //cut the inner data
			if(innerStart != string::npos)
			{
				innerEnd = cuttedData.find(innerEndPhase, innerStart+innerStartPhase.length());
				string tmp = cuttedData.substr(innerStart+innerStartPhase.length());
				//DBG(tmp);
				if(innerEnd != string::npos)
				{
					destination = cuttedData.substr(innerStart+innerStartPhase.length(), innerEnd-innerStart-innerStartPhase.length());
//					cout << "The state is: " << destination << endl;
				}else PRINT("could not find the state end breaker phrase");
			} else PRINT("could not find the state breaker phrase");
		}
	}

	connector cnt;
	MYSQL_RES* res;
	string db = "anton";
	if(cnt.initANDconnect(db.c_str()) == ok)
	{
		stringstream dima("");
		dima << "insert into visits values(0,'";
		dima << destination;
		dima << "',";
		dima << "'Queen Elizabeth II',";
		if(day>0 && day<31) dima << day; else dima << 2;
		dima << ",";
		dima << MonthToNumber[month];
		dima << ",";
		dima << year;
		dima << ",'England')";
		PRINT(dima.str());
		//string qur = "SELECT * FROM aa1 ORDER BY a1 ASC";
		MYSQL_ROW          result;
		//res = cnt.runQuery(dima.str().c_str());


		//while((result = mysql_fetch_row(res))) {

		   //cout << result[0] << endl;
		//}

	}else
	{
		PRINT("could not connect");
	}


	return ok;
}
