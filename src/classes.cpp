//============================================================================
// Name        : classes.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "connector.h"
#include "webparser.h"
#include "gDefines.h"

using namespace std;

int main() {
	retVal rt;
	string name = "out1.txt";

	webparser crwl;

	crwl.init(name.c_str());
	name = "www.ynet.co.il";
	crwl.setLink(name.c_str());

	rt = crwl.execGet();

	if(rt == ok)
		cout << "DONE!" << endl;
}
