/*
 * htmlparser.h
 *
 *  Created on: Dec 3, 2014
 *      Author: anton
 */

#ifndef HTMLPARSER_H_
#define HTMLPARSER_H_

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gDefines.h"

class htmlparser : public parser
{
public:
	htmlparser(const char* fileName);
	~htmlparser();

	//retVal parseFile(std::string startPhase, std::string endPhase, retVal (*handleFunc)(std::string phrase));
	retVal parseFile();

private:

};



#endif /* HTMLPARSER_H_ */
