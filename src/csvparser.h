/*
 * csvparser.h
 *
 *  Created on: Dec 26, 2014
 *      Author: anton
 */

#ifndef CSVPARSER_H_
#define CSVPARSER_H_

#include "parser.h"
#include <stdlib.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "gDefines.h"

class csvparser : public parser
{
public:
	csvparser (const char* fileName);
	~csvparser ();

	//retVal parseFile(std::string startPhase, std::string endPhase, retVal (*handleFunc)(std::string phrase));
	retVal parseFile(std::map <int, std::string>& countriesMap, std::map <int , std::vector<std::string> >& yearToLeadersMap);

private:
	retVal line2vector(std::string line, std::vector<std::string>& data);
};



#endif /* CSVPARSER_H_ */
