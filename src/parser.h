/*
 * parser.h
 *
 *  Created on: Dec 3, 2014
 *      Author: anton
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gDefines.h"

class parser{
public:
	parser(const char* fileName);
	~parser();

	static retVal removeEmptyLinesInFile(const char* sourceFile, const char* destFile);
	//retVal parseFile(std::string startPhase, std::string endPhase, retVal (*handleFunc)(std::string phrase));

protected:
	char* pFileName;


};



#endif /* PARSER_H_ */
