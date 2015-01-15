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
#include "csvparser.h"
#include "fileparser.h"
#include "gDefines.h"
#include "mysql.h"
#include <sstream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
void processGoogleLink (string query, vector<string> mustAppear, std::vector< vector<int> >& v);
void printDates(std::vector< vector<int> >& v, std::vector< vector<int> >& aggreagationVector);
void runGoogleQuery(string url, string filename, vector<string>& links);

int main() {
	retVal rt;
	string name = "out1.txt";
	char buffer[1024];
	string baseName = "https://www.google.co.il/search?q=Shinzo+Abe+Tony+Abbott+meets+OR+met+OR+visit&client=ubuntu&tbs=cdr%3A1%2Ccd_min%3A1.1.2014%2Ccd_max%3A31.12.2014&num=60";
	string queryStart = "https://www.google.co.il/search?q=";
	string queryEnd = "meets+OR+met+OR+visit&client=ubuntu&tbs=cdr%3A1%2Ccd_min%3A1.1.2014%2Ccd_max%3A31.12.2014&num=60";
	vector< vector<string> > tokenVector;

	//the search link
	//https://www.google.co.il/search?q=Pranab+Mukherjee+Giorgio+Napolitano+meets+OR+met+OR+visit&client=ubuntu&tbs=cdr%3A1%2Ccd_min%3A1.1.2014%2Ccd_max%3A31.12.2014&num=60
	//problem with the link:
//	string linkssss = "http://newsnow.in/news/-india-china-ties-need-to-focus-on-investment-";
//	std::vector< std::string > namessss = {"Mamnoon", "Hussain", "Joachim", "Gauck"};
//	std::vector< vector<int> > vsss;
//
//	processGoogleLink(linkssss, namessss, vsss);
//
//	exit(1);

//Download & cache a web page

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

//		}
//
//
//	}

	map <int, string> countriesMap;
	map <int , vector<string> > yearToLeadersMap;

	csvparser csvparse("primes.csv");
	csvparse.parseFile(countriesMap, yearToLeadersMap);

	boost::char_separator<char> sep(" ");

	int firstCountryID = 0;
	int secondCountryID = 0;

	bool firstRun = true;
	bool firstRunFlagForSecond = false;
	for(map<int, vector<string>>::iterator it = yearToLeadersMap.begin() ; it!=yearToLeadersMap.end() ; ++it)
	{
		if(it->first != 2014) continue;
		for(vector<string>::iterator vit=it->second.begin() ; vit != it->second.end() ; ++vit,firstCountryID++)
		{
			if(firstRun)
			{
				firstRun = false;
				vit+=58;
				firstCountryID+=58;
				//firstRunFlagForSecond = true;
			}
			string leader(*vit);
			if(leader.compare("1") == 0)continue;

			secondCountryID=firstCountryID+1;
			for(vector<string>::iterator innerIt=vit+1 ; innerIt != it->second.end() ; ++innerIt,secondCountryID++)
			{
//				if(firstRunFlagForSecond)
//				{
//					innerIt+=30;
//					secondCountryID+=30;
//					firstRunFlagForSecond = false;
//				}

				string innerLeader(*innerIt);
				if(innerLeader.compare("1") == 0)continue;
				if((firstCountryID/2) == (secondCountryID/2))continue; //if its the same country - continue

				/**** Break the leader names into strings and store them in vectors ****/
				vector<string> leader1;
				vector<string> leader2;

				boost::tokenizer<boost::char_separator<char>> tok(leader, sep);
				for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg){
					string tmp(*beg); // delete the preceding space
					if(tmp[0] == ' ')tmp.erase(0,1);
					leader1.push_back(tmp);
				}

				boost::tokenizer<boost::char_separator<char>> tok2(innerLeader, sep);
				for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok2.begin(); beg!=tok2.end();++beg){
					string tmp(*beg); // delete the preceding space
					if(tmp[0] == ' ')tmp.erase(0,1);
					leader2.push_back(tmp);
				}
				/***********************************************************************/
				/**** Create the google search query ****/
				std::vector< std::string > names;
				string query(queryStart);

				for(vector<string>::iterator it = leader1.begin() ; it != leader1.end() ; ++it)
				{
					names.push_back(*it);
					query += *it;
					query += "+";
				}

				for(vector<string>::iterator it = leader2.begin() ; it != leader2.end() ; ++it)
				{
					names.push_back(*it);
					query += *it;
					query += "+";
				}

				query += queryEnd;
				/***********************************************************************/
				PRINT(query);

				/**** Run the query ****/
				std::vector< vector<int> > v;
				vector <string> links;
				runGoogleQuery(query, name, links);

				for(int linkNum = 0 ; linkNum < links.size() ; linkNum++)
				{
					PRINT("Checking link:");
					PRINT(links[linkNum]);
					processGoogleLink(links[linkNum], names, v);
				}

				std::vector< vector<int> > aggregationVector;
				printDates(v, aggregationVector);

				PRINT("Data for meetings between");
				PRINT(countriesMap[firstCountryID/2]);
				PRINT("And");
				PRINT(countriesMap[secondCountryID/2]);

				//input data into mysql database
				connector cnt;
				MYSQL_RES* res;
				string db = "anton";
				cnt.initANDconnect(db.c_str());

				for(std::vector< std::vector<int> >::iterator it = aggregationVector.begin() ; it != aggregationVector.end() ; ++it)
				{
					//check if the visit was on the same year and month and at most 4 days apart
					if( ((*it)[3] >= 4) && ((*it)[2]==2014))
					{
						//insert into vistits values(0,'Vatican City','Queen Elizabeth II',1,4,2014,'England');
						stringstream stream;
						stream << "insert into idvisits values(0,";
						stream << (firstCountryID/2);
						stream << ",";
						stream << (secondCountryID/2);
						stream << ",";
						stream << (*it)[2];
						stream << ",";
						stream << (*it)[1];
						stream << ",";
						stream << (*it)[0];
						stream << ")";
						PRINT(stream.str());
						MYSQL_ROW          result;
						cnt.runQuery(stream.str().c_str());
					}
				}

				cnt.closeSession();

				sleep(5);
				/***********************************************************************/
			}
		}
	}

	exit(1);

	// CREATE TABLE idvisits(id INT PRIMARY KEY AUTO_INCREMENT, ctry1 INT, cntry2 INT, year INT, month INT, day INT);

	//Iterate over google links loop
//	std::vector< vector<int> > v;
//	std::vector< std::string > names = {"Shinzo", "Abe", "Tony", "Abbott"};
#if 0
	vector <string> links;
	runGoogleQuery(baseName, name, links);

	for(int linkNum = 0 ; linkNum < links.size() ; linkNum++)
	{
		PRINT("Checking link:");
		PRINT(links[linkNum]);
		processGoogleLink(links[linkNum], names, v);
	}

	printDates(v);


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
#endif
}


//accepts google link with related search fields and dates, and the must appear vector and prints the relevant filtered dates
//v is the vector where all the data is stored
void processGoogleLink (string query, vector<string> mustAppear, std::vector< vector<int> >& v)
{
	retVal rt;
	string name = "out1.txt";
	char buffer[1024];
	webparser tmpCrwl;


//	string aa("http://www.independent.co.uk/incoming/tony-abbott-embarrasses-australia-by-praising-japanese-wwii-military-getting-on-the-sake-and-posing-for-crotchshot-photo-opportunity-9596793.html");
	tmpCrwl.init(name.c_str());
	PRINT(query.c_str());
	rt = tmpCrwl.setLink(query.c_str());
	if(rt != ok)
	{
		PRINT("Link in black list");
		return;
	}
//	tmpCrwl.setLink(aa.c_str());
	rt = tmpCrwl.execGet();
	if(rt == ok) // if the crawl succeded
	{
		htmlparser parser(name.c_str());
		parser.parseFile();
		rt = parser::removeEmptyLinesInFile("out4.txt", "out5.txt");
		if(rt == error)
			return;

		rt = fileparser::getDates("out5.txt", v, mustAppear);
		if(rt == error)
		{
			PRINT("Getting dates was stopped in:");
			PRINT(query);
		}
	}
	else
	{
		PRINT("Bad");
	}
}

void printDates(std::vector< vector<int> >& v, std::vector< vector<int> >& aggreagationVector)
{

	PRINT("Number of found dates:");
	PRINT(v.size());

//	for(int i = 0 ; i < v.size() ; i++)
//	{
//		cout << "year: " << v[i][2] << " month: " << v[i][1] << " day: " << v[i][0] << endl;
//	}

	for(std::vector< std::vector<int> >::iterator it = v.begin() ; it != v.end() ; ++it)
	{
		//it point to an internal vector which represents the dates
		cout << "year: " << (*it)[2] << " month: " << (*it)[1] << " day: " << (*it)[0] << endl;
		bool found = false;
		for(std::vector< std::vector<int> >::iterator aggit = aggreagationVector.begin() ; (aggit != aggreagationVector.end()) && !found ; ++aggit)
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
			aggreagationVector.push_back(tmp);
		}
	}

	PRINT("Printing only the meeting dates");
	for(std::vector< std::vector<int> >::iterator it = aggreagationVector.begin() ; it != aggreagationVector.end() ; ++it)
	{
		//check if the visit was on the same year and month and at most 4 days apart
		if( (*it)[3] >= 4)
		{
			cout << "year: " << (*it)[2] << " month: " << (*it)[1] << " day: " << (*it)[0] << endl;
		}
	}

}

void runGoogleQuery(string url, string filename, vector<string>& links)
{
	retVal rt;
	webparser crwl;
	crwl.init(filename.c_str());
	crwl.setLink(url.c_str());
	rt = crwl.execGet();

	fileparser psr(filename.c_str());

	psr.parseFile("<h3 class=\"r\"><a href=\"", "</h3>", fileparser::getLinksFromGoogleSearch, links);
	for(int i=0;i<links.size();i++)PRINT(links[i]);
}
