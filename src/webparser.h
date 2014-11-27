/*
 * webparser.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef WEBPARSER_H_
#define WEBPARSER_H_

#include <curl/curl.h>
#include <unistd.h>
#include "gDefines.h"
#include <string>

class webparser
{
public:
	webparser();
	retVal init();
	retVal init(const char* outputFileNameStr);
	retVal setLink(const char* webSiteLinkStr);
	retVal execGet();

private:
	  CURL *curl;
	  CURLcode res;
	  FILE* filePtr;
	  std::string urlCpy;

	  static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
};



#endif /* WEBPARSER_H_ */
