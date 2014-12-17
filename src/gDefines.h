/*
 * gDefines.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef GDEFINES_H_
#define GDEFINES_H_

#include <string.h>
#include <map>

#define Debug 1

#define DBG(str) if(Debug) cout << str << endl;
#define PRINT(str) cout << str << endl;

typedef enum
{
	ok,
	error
}retVal;

static std::map<std::string, int> MonthToNumber = {
		{"january", 1},
		{"february", 2},
		{"march", 3},
		{"april", 4},
		{"may", 5},
		{"june", 6},
		{"july", 7},
		{"august", 8},
		{"september", 9},
		{"october", 10},
		{"november", 11},
		{"december", 12}
};

static retVal stringToLower(std::string& str)
{
	for(std::string::iterator it=str.begin() ; it!=str.end() ; ++it)
	{
		*it = tolower(*it);
	}
	return ok;
}

#endif /* GDEFINES_H_ */
