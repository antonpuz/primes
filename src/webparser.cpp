#include "webparser.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include "gDefines.h"

using namespace std;

webparser::webparser()
{
	curl = NULL;
	res = CURLE_OK;
	filePtr = NULL;
}

retVal webparser::init()
{
	init("dummy.txt");
}

retVal webparser::init(const char* outputFileNameStr)
{
	filePtr = fopen(outputFileNameStr, "wb");
	if(filePtr == NULL)
	{
		perror("error opening file");
		return error;
	}

	curl = curl_easy_init();
	if(curl) //creation of the curl has succeded
	{
		curl_easy_setopt(curl, CURLOPT_URL, "www.walla.co.il"); // should be changed later with setLink()
		/* example.com is redirected, so we tell libcurl to follow redirection */
	    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	    /* set the agent - to mock a browser */
	    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:31.0) Gecko/20100101 Firefox/31.0");

	    /* send all data to this function  */
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, filePtr);
	    //curl_easy_setopt(curl,   CURLOPT_HEADERDATA, header);

	    return ok;
	}
	return error;
}

size_t webparser::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	  return written;
}

retVal webparser::execGet()
{
	if(!curl || !filePtr)
		return error;

	res = curl_easy_perform(curl);
	fclose(filePtr);

    /* Check for errors */
    if(res != CURLE_OK)
    {
    	perror("curl_easy_perform() failed");
    	return error;
    }

    return ok;
}

retVal webparser::setLink(const char* webSiteLinkStr)
{
	if(!curl || !webSiteLinkStr)
		return error;

	curl_easy_setopt(curl, CURLOPT_URL, webSiteLinkStr); // should be changed later with setLink()
	return ok;
}
