/*
 * htmlparser.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: anton
 */

#include "htmlparser.h"
#include <sstream>
#include "connector.h"
#include <fstream>      // std::ifstream
#include <iostream>
#include <tidy.h>
#include <buffio.h>

using namespace std;

htmlparser::htmlparser(const char* fileName) : parser(fileName)
{

	if(!fileName)
		perror("no file name inserted - no instance will be made");

}

htmlparser::~htmlparser()
{
	cout << "destructing the file parser " << endl;
}


retVal htmlparser::parseFile()
{
	char * ptr;
	if(!pFileName)
	{
		cout << "No file was registered please delete and recreate the parser. " << endl;
		return error;
	}
	//open the file for reading
	ifstream is (pFileName, ifstream::in | ifstream::binary);
	if(!is)
	{
		perror("No file descriptor");
		return error;
	}

	DBG("start of seeking");
	//load the file into memory
	// get length of file:
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);

	DBG("end of seeking");

	char * buffer = new char [length + 1];
	if(!buffer)
	{
		perror("error with allocation of memory");
		return error;
	}
	is.read (buffer,length); //buffer contains the file
	buffer[length] = '\0';
	is.close();


	  TidyBuffer output = {0};
	  TidyBuffer errbuf = {0};
	  int rc = -1;
	  Bool okk;

	  TidyDoc tdoc = tidyCreate();                     // Initialize "document"
	  //printf( "Tidying:\t%s\n", buffer );

	  okk = tidyOptSetBool( tdoc, TidyDropPropAttrs, yes );  // Convert to XHTML
	  okk = tidyOptSetBool( tdoc, TidyDropFontTags, yes );  // Convert to XHTML
	  //okk = tidyOptSetBool( tdoc, TidySortAttributes, yes);  // Convert to XHTML
	  okk = tidyOptSetBool( tdoc, TidyXmlOut, yes );  // Convert to XHTML
	  if ( okk )
	    rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics

	  if ( rc >= 0 )
	    rc = tidyParseString( tdoc, buffer );           // Parse the input
	  if ( rc >= 0 )
	    rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
	  if ( rc >= 0 )
	    rc = tidyRunDiagnostics( tdoc );               // Kvetch
	  if ( rc > 1 )                                    // If error, force output.
	    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	  if ( rc >= 0 )
	    rc = tidySaveBuffer( tdoc, &output );          // Pretty Print

	  if ( rc >= 0 )
	  {
	   // if ( rc > 0 )
	      //printf( "\nDiagnostics:\n\n%s", errbuf.bp );
	    //printf( "\nAnd here is the result:\n\n%s", output.bp );
	  }
	  //else
	    //printf( "A severe error (%d) occurred.\n", rc );
PRINT("place 2");
		ptr = (char *)malloc(output.size);
	  if(!ptr)
	  {
		  PRINT("could not alloc");
		  exit(0);
	  }
	  memcpy(ptr, output.bp, output.size);
	  printf( "\nAnd here is the result:\n\n%s", ptr);

	  tidyBufFree( &output );
	  tidyBufFree( &errbuf );
	  tidyRelease( tdoc );
	/*
	char c;
	int htmlCounter = 0;

	is.get(c);
	while(!is.eof())
	{
		switch (c)
		{
		case '<':
			htmlCounter++;
			break;
		case '>':
			htmlCounter--;
			break;
		default:
			if(htmlCounter == 0)cout << c;

			break;
		}

		is.get(c);
	}
	*/
	FILE * pFile;
	//ofstream of ("out3.txt", ofstream::out);
	PRINT("starting crop");
//	pFile = fopen("out4.txt", "w");
//	if(!pFile)
//	{
//		perror("No file descriptor");
//		return error;
//	}
	int index = 0;
	char c;
	int htmlCounter = 0;
	char* bodyPtr = strstr(ptr, "<body");
	if(bodyPtr)
	{
		ofstream of("out4.txt", ofstream::out);
		c = bodyPtr[index];
		while(c != '\0')
		{
			//cout << (int)htmlCounter << " ";
			switch (c)
			{
			case '<':
				htmlCounter++;
				break;
			case '>':
				if(htmlCounter > 0)
					htmlCounter--;
				break;
			default:
				if(htmlCounter == 0)
				{
					//fprintf(pFile, "%c", c);
					of << c;
				}
				else
				{
					//fprintf(pFile, "+%c+", c);
					//cout << "a";

				}
				break;
			}


			c = bodyPtr[++index];
		}
		of.close();
	}
	delete[] ptr;
	cout << endl;
	//fclose(pFile);
	cout << "exited on index" << index << " and html: " << htmlCounter << endl;
	PRINT("ANTON");






	return ok;
}
