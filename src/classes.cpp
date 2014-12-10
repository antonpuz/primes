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

using namespace std;

int main() {
	retVal rt;
	string name = "out1.txt";
	char buffer[1024];

	vector< vector<string> > tokenVector;



//Download & cache a web page
	webparser crwl;
	crwl.init(name.c_str());
	string baseName = "http://en.wikipedia.org/wiki/List_of_presidential_trips_made_by_Barack_Obama";
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
//			fileparser psr(name.c_str());
//			psr.parseFile("<h3 class=\"r\"><a href=\"", "</h3>", fileparser::dummyFunc);
//		}
//
//
//	}



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

	//The wikipedia parser
	name = "out1.txt";
	fileparser psr(name.c_str());
	psr.tokenizeStringToVector("<td><span class=\"flagicon\">", "</table>", "wikitable","<span class=\"mw-headline" ,tokenVector);

	int i=0;
	//for(int i=0;i<tokenVector.size();i++)
	//{
		fileparser::cropVector(tokenVector[i], 1, "2009");
		char month[20];
		int day;
//		for (int j=0;j<tokenVector[i].size();j++)
//			cout << tokenVector[i][j] << "&&&&&&&&&   ";

		//insert into vistits values(0,'Vatican City','Queen Elizabeth II',1,4,2014,'England');
		if(tokenVector[i][1][0] == ' ')tokenVector[i][1].erase(tokenVector[i][1].begin());
		string qur = "insert into visits values(0,";
		qur += "'United States','Barack Obama',";
		qur += tokenVector[i][4];
		qur += ",";
		qur += tokenVector[i][3];
		qur += ",";
		qur += tokenVector[i][0];
		qur += ",'";
		qur += tokenVector[i][1];
		qur += "')";
		PRINT(qur);
		MYSQL_ROW          result;
		cnt.runQuery(qur.c_str());
		cout << endl;
	//}


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
