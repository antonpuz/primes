#include "gDefines.h"
#include "parser.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

parser::parser(const char* fileName)
{
	if(!fileName)
	{
		perror("no file name inserted - no instance will be made");
		pFileName = NULL;
	}

	string strTmp(fileName);
	pFileName = new char[strTmp.length() + 1]();
	strncpy(pFileName, fileName, strTmp.length() + 1);
}

parser::~parser()
{
	cout << "destructing the file parser " << endl;
	if(pFileName)
	{
		delete[] pFileName;
		pFileName = NULL;
	}
}

retVal parser::removeEmptyLinesInFile(const char* sourceFile, const char* destFile)
{
	ifstream ifs(sourceFile, ifstream::in);
	ofstream of(destFile, ofstream::out);

	if(!ifs || !of)
	{
		PRINT("Could not open the files");
		return error;
	}
	char lineBuff[8000];
	while( !ifs.eof() )
	{
		ifs.getline(lineBuff, 8000);
		if(ifs.fail()){
			return error;
		}
		if((lineBuff[0] != '\0'))
		{
			of << lineBuff;
			of << endl;
		}
	}
	ifs.close();
	of.close();
	return ok;
}



//retVal parser::parseFile(std::string startPhase, std::string endPhase, retVal (*handleFunc)(std::string phrase))
//{
//	return ok;
//}
