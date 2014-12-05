//============================================================================
// Name        : classes.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include "connector.h"
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

//
	webparser crwl;
	crwl.init(name.c_str());
	string baseName = "http://tsamfamily.blogspot.co.il/2012/04/tsam-protest-against-hu-jin-tao-visit.html";
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


	/*
	connector cnt;
	MYSQL_RES* res;
	string db = "anton";
	cnt.initANDconnect(db.c_str());
	string qur = "SELECT * FROM aa1 ORDER BY a1 ASC";
	MYSQL_ROW          result;
	res = cnt.runQuery(qur.c_str());

    while((result = mysql_fetch_row(res))) {

       cout << result[0] << " " << result[1] << endl;
    }
    */
	name = "out1.txt";
	htmlparser parser(name.c_str());
	parser.parseFile();
	parser::removeEmptyLinesInFile("out4.txt", "out5.txt");
//	fileparser psr(name.c_str());
//	//psr.tokenizeStringToVector("<h3 class=\"r\"><a href=\"", "</h3>", tokenVector);
//	psr.tokenizeStringToVector("<td><span class=\"flagicon\">", "</table>", "wikitable","mw-headline" ,tokenVector);
//
//	for(int i=0;i<tokenVector.size();i++)
//	{
//		PRINT("This sub vector size:");
//		PRINT(tokenVector[i].size());
//		//if(tokenVector[i].size() < 3) continue;
//		for(int j=0;j<tokenVector[i].size();j++)
//		{
//			cout << tokenVector[i][j] << "&&&&&&&&&   ";
//		}
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
