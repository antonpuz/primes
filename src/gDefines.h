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

static std::string blackList = "http://newsnow.in/news/tunisians-head-to-elect-president-in-runoff-u-t-san-diego";
static std::string blackList2 = "http://consortiumnews.com/2014/10/30/russiaskey-role-in-iran-nuke-deal/";
//static std::string blackList3 = "http://newsnow.in/news/terrorist-don-t-deserve-any-concession-pak-presiden";
static std::string blackList4 = "http://consortiumnews.com/2014/04/29/kerry-grovels-over-israeli-apartheid/";
static std::string blackList5 = "http://www.newsnow.in/news/bangladesh-president-to-arrive-in-india-tomorrow-on-six-day-visit-zee-news";
//static std::string blackList6 = "http://newsnow.in/news/-india-china-ties-need-to-focus-on-investment-";
static std::string blackList7 = "http://newsnow.in/news/fm-meets-modi-discusses-economic-performance";
static std::string blackList8 = "http://www.siemens.com/press/political-dialogue";
//http://www.newsnow.in/news/bangladesh-president-to-arrive-in-india-tomorrow-on-six-day-visit-zee-news


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
		{"december", 12},
		{"jan", 1},
		{"feb", 2},
		{"mar", 3},
		{"apr", 4},
		{"may", 5},
		{"jun", 6},
		{"jul", 7},
		{"aug", 8},
		{"sep", 9},
		{"oct", 10},
		{"nov", 11},
		{"dec", 12}
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
