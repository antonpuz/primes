//============================================================================
// Name        : classes.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "connector.h"
#include "htmlparser.h"
#include "webparser.h"
#include "fileparser.h"
#include "gDefines.h"
#include "mysql.h"
#include <sstream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main() {
	retVal rt;
	string name = "out1.txt";
	char buffer[1024];

	vector< vector<string> > tokenVector;

//Download & cache a web page
	webparser crwl;
	crwl.init(name.c_str());
	string baseName = "https://www.google.co.il/search?q=Shinzo+Abe+Tony+Abbott+meets+OR+met+OR+visit&client=ubuntu&tbs=cdr%3A1%2Ccd_min%3A1.1.2013%2Ccd_max%3A31.12.2014&num=60";
	crwl.setLink(baseName.c_str());
	rt = crwl.execGet();


//	string baseName = "https://www.google.co.il/search?hl=en&q=Hu+Jintao+visits+india&tbas=0&start=";
//	for(int j=0;j<=20;j+=20)
//	{
//		webparser crwl;
//		crwl.init(name.c_str());
//
//		stringstream link;
//		link << baseName;
//		link << j;
//		PRINT(link.str().c_str())
//		crwl.setLink(link.str().c_str());
//		rt = crwl.execGet();
//		if(rt == ok)
//		{
//			cout << "DONE!" << endl;
//
			fileparser psr(name.c_str());
			vector <string> links;
			psr.parseFile("<h3 class=\"r\"><a href=\"", "</h3>", fileparser::getLinksFromGoogleSearch, links);
			for(int i=0;i<links.size();i++)PRINT(links[i]);
//		}
//
//
//	}

	//Iterate over google links loop
	std::vector< vector<int> > v;
	std::vector< std::string > names = {"Shinzo", "Abe", "Tony", "Abbott"};

	for(int linkNum = 0 ; linkNum < links.size() ; linkNum++)
	{
		PRINT("Checking link:");
		PRINT(links[linkNum]);

		webparser tmpCrwl;

//		string aa("http://www.independent.co.uk/incoming/tony-abbott-embarrasses-australia-by-praising-japanese-wwii-military-getting-on-the-sake-and-posing-for-crotchshot-photo-opportunity-9596793.html");

		tmpCrwl.init(name.c_str());
		tmpCrwl.setLink(links[linkNum].c_str());
//		tmpCrwl.setLink(aa.c_str());
		rt = tmpCrwl.execGet();
		if(rt == ok) // if the crawl succeded
		{
			htmlparser parser(name.c_str());
			parser.parseFile();
			parser::removeEmptyLinesInFile("out4.txt", "out5.txt");

			rt = fileparser::getDates("out5.txt", v, names);
			if(rt == error)
			{
				PRINT("Getting dates was stopped in:");
				PRINT(links[linkNum]);
			}
		}
		else
		{
			PRINT("Bad");
		}

	}


	//input data into mysql database
	connector cnt;
	MYSQL_RES* res;
	string db = "anton";
	cnt.initANDconnect(db.c_str());

	//Parse the file with the html parser
//	name = "out1.txt";
//	htmlparser parser(name.c_str());
//	parser.parseFile();
//	parser::removeEmptyLinesInFile("out4.txt", "out5.txt");


//	std::vector< vector<int> > v;
//	fileparser::getDates("out5.txt", v);
//

	PRINT("Number of found dates:");
	PRINT(v.size());

//	for(int i = 0 ; i < v.size() ; i++)
//	{
//		cout << "year: " << v[i][2] << " month: " << v[i][1] << " day: " << v[i][0] << endl;
//	}

	std::vector< vector<int> > aggregatedDates;
	for(std::vector< std::vector<int> >::iterator it = v.begin() ; it != v.end() ; ++it)
	{
		//it point to an internal vector which represents the dates
		cout << "year: " << (*it)[2] << " month: " << (*it)[1] << " day: " << (*it)[0] << endl;
		bool found = false;
		for(std::vector< std::vector<int> >::iterator aggit = aggregatedDates.begin() ; (aggit != aggregatedDates.end()) && !found ; ++aggit)
		{
			//check if the visit was on the same year and month and at most 4 days apart
			if(((*it)[2] == (*aggit)[2]) && ((*it)[1] == (*aggit)[1]) && ((*it)[0]-(*aggit)[0] >= -7) && ((*it)[0]-(*aggit)[0] <= 7))
			{
				(*aggit)[3]++;
				found = true;
			}
		}
		if (!found)
		{
			std::vector<int> tmp(*it);
			tmp.push_back(1);
			aggregatedDates.push_back(tmp);
		}
	}

	PRINT("Printing only the meeting dates");
	for(std::vector< std::vector<int> >::iterator it = aggregatedDates.begin() ; it != aggregatedDates.end() ; ++it)
	{
		//check if the visit was on the same year and month and at most 4 days apart
		if( (*it)[3] >= 4)
		{
			cout << "year: " << (*it)[2] << " month: " << (*it)[1] << " day: " << (*it)[0] << endl;
		}
	}

	//The wikipedia parser
//	name = "out1.txt";
//	fileparser psr(name.c_str());
//	psr.tokenizeStringToVector("<td><span class=\"flagicon\">", "</table>", "wikitable","<span class=\"mw-headline" ,tokenVector);
//
//	int i=0;
//	for(int i=0;i<tokenVector.size();i++)
//	{
//		fileparser::cropVector(tokenVector[i], 1, "2009");
//
//		//insert into vistits values(0,'Vatican City','Queen Elizabeth II',1,4,2014,'England');
//		if(tokenVector[i][1][0] == ' ')tokenVector[i][1].erase(tokenVector[i][1].begin());
//		string qur = "insert into visits values(0,";
//		qur += "'United States','Barack Obama',";
//		qur += tokenVector[i][4];
//		qur += ",";
//		qur += tokenVector[i][3];
//		qur += ",";
//		qur += tokenVector[i][0];
//		qur += ",'";
//		qur += tokenVector[i][1];
//		qur += "')";
//		PRINT(qur);
//		MYSQL_ROW          result;
//		cnt.runQuery(qur.c_str());
//		cout << endl;
//	}


//	vector<string> tmp;
//	tmp.push_back(string("banana"));
//	tmp.push_back(string("apple"));
//	tmp.push_back(string("peach"));
//	tokenVector.push_back(tmp);
//	tmp.clear();
//	tmp.push_back(string("anton"));
//	tmp.push_back(string("anton1"));
//	tmp.push_back(string("anton2"));
//	tokenVector.push_back(tmp);
//	for(int i=0; i<tokenVector.size() ;i++)
//	{
//		for(int j=0; j<tokenVector[i].size() ;j++)
//		{
//			cout << tokenVector[i][j];
//		}
//		cout << endl;
//	}

}
