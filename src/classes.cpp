//============================================================================
// Name        : classes.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include "connector.h"
#include "webparser.h"
#include "fileparser.h"
#include "gDefines.h"
#include "mysql.h"

using namespace std;

int main() {
	retVal rt;
	string name = "out1.txt";

	/*webparser crwl;

	crwl.init(name.c_str());
	name = "http://en.wikipedia.org/wiki/List_of_state_visits_made_by_Queen_Elizabeth_II";
	crwl.setLink(name.c_str());

	rt = crwl.execGet();

	if(rt == ok)
		cout << "DONE!" << endl;
	*/
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
	fileparser psr(name.c_str());
	psr.parseFile();
}
