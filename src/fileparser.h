/*
 * fileparser.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "gDefines.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>


class fileparser
{
public:
	fileparser(const char* fileName);
	~fileparser();

	retVal parseFile();
	static retVal parsePhrase(std::string phrase);

private:
	char* pFileName;
};

#endif /* FILEPARSER_H_ */
