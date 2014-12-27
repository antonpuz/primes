/*
 * csvparser.cpp
 *
 *  Created on: Dec 26, 2014
 *      Author: anton
 */

#include "csvparser.h"
#include <sstream>
#include "connector.h"
#include <fstream>      // std::ifstream
#include <iostream>
#include <tidy.h>
#include <buffio.h>
#include <boost/tokenizer.hpp>

using namespace std;

csvparser::csvparser(const char* fileName) : parser(fileName)
{

	if(!fileName)
		perror("no file name inserted - no instance will be made");

}

csvparser::~csvparser()
{
	cout << "destructing the file parser " << endl;
}


retVal csvparser::parseFile(std::map <int, std::string>& countriesMap, std::map <int , std::vector<std::string> >& yearToLeadersMap)
{
	if(!pFileName)
	{
		PRINT("No file name has entered for csv parser");
		return error;
	}

	ifstream ifs(pFileName, ifstream::in);
	if(!ifs)
	{
		PRINT("Could not open the files");
		return error;
	}

	char lineBuff[8000];
	vector <string> data;

	//read the countries in to the country map
	ifs.getline(lineBuff, 8000);
	if((lineBuff[0] != '\0'))
	{
		line2vector(lineBuff, data);
		int id = 0;
		for(vector<string>::iterator it=data.begin()+1 ; it != data.end() ; it+=2)
		{
			countriesMap.insert( pair<int, string>(id, *it) );
			id++;
		}

		for (map<int, string>::iterator it = countriesMap.begin() ; it!=countriesMap.end() ; ++it)
		{
			cout << "The country " << it->second << " has id " << it->first << endl;
		}

	}

	//pass over the prime minister - president vector
	ifs.getline(lineBuff, 8000);
	//

	while( !ifs.eof() )
	{
		data.clear();
		ifs.getline(lineBuff, 8000);
		if((lineBuff[0] != '\0'))
		{
			line2vector(lineBuff, data);
			int year = atoi(data[0].c_str());
			//delete the year entry
			data.erase(data.begin());
			yearToLeadersMap.insert( pair<int, std::vector<std::string> >(year, data) );
		}
	}
	ifs.close();

	return ok;
}

retVal csvparser::line2vector(std::string line, std::vector<std::string>& data)
{
	boost::char_separator<char> sep(",");
	boost::tokenizer<boost::char_separator<char>> tok(line, sep);
	for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg){
		string tmp(*beg); // delete the preceding space
		if(tmp[0] == ' ')tmp.erase(0,1);
		data.push_back(tmp);
	}
	return ok;
}
