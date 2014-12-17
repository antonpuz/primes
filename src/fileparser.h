/*
 * fileparser.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "parser.h"
#include "gDefines.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>


class fileparser : public parser
{
public:
	fileparser(const char* fileName);
	~fileparser();

	retVal tokenizeStringToVector(std::string startPhase, std::string endPhase, std::string mustAppear, std::string special ,std::vector< std::vector<std::string> >& dataCollector);
	retVal parseFile(std::string startPhase, std::string endPhase, retVal (*handleFunc)(std::string phrase));
	static retVal parsePhrase(std::string phrase);
	static retVal dummyFunc(std::string phrase);
	static retVal handleTable(std::string phrase, std::string specialPhrase, std::vector< std::vector<std::string> >& dataCollector);
	/*
	//this function is used in order to iterate over the vector containing the interesting information that was retrieved from the
	//wikipedia page and store it in the vector.
	 * The type here indicates the kind of wikipedia page we got:
	 * 1 - Table is devided by years, each year appears at the header of the table
	 */
	static retVal cropVector(std::vector<std::string>& data, int type, std::string extraData);
	static retVal getDates (std::string fileName);


private:

};

#endif /* FILEPARSER_H_ */
