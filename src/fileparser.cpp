/*
 * fileparser.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#include "fileparser.h"
#include "mysql.h"
#include <sstream>
#include "connector.h"
#include <fstream>      // std::ifstream
#include <boost/regex.hpp>

using namespace std;

fileparser::fileparser(const char* fileName) : parser(fileName)
{

	if(!fileName)
		perror("no file name inserted - no instance will be made");

}

fileparser::~fileparser()
{
	cout << "destructing the file parser " << endl;
}

retVal fileparser::parseFile(string startPhase, string endPhase, retVal (*handleFunc)(string phrase))
{
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

	string data(buffer);
	string cuttedData;
	size_t location = -5;
	size_t endLocation;

	while((location = data.find(startPhase, location+5)) != string::npos) //iterate through the file
	{
		if((endLocation = data.find(endPhase, location)) != string::npos)
		{
			cuttedData = data.substr(location, endLocation - location);
			//cout << cuttedData << endl;
			handleFunc(cuttedData);
		}
		else
		{
			break;
		}
	}

	delete[] buffer;
	return ok;
}

retVal fileparser::parsePhrase(string phrase)
{
	char debugBuff[100];
	string mustAppear = "flagicon";
	string startPhase = "<td";
	string endPhase = "</td>";
	string innerStartPhase = "</span>";
	string innerEndPhase = "<";
	string startEndPlacePhrase = ">";
	string cuttedData;
	string innerCuttedData;
	size_t start;
	size_t startEndPlace;
	size_t end;
	size_t innerStart;
	size_t innerEnd;

	//Data that will be used to make the query
	string vst_cntry = "United Kingdom";
	string vst_person = "Queen Elizabeth II";
	int day;
	char month[15];
	int year;
	string destination;
	char yearBuff[30];

	if((start = phrase.find(mustAppear)) == string::npos)//check for validity of input
		return error;

	start = phrase.find(startPhase);
	if(start != string::npos)
	{
		startEndPlace = phrase.find(startEndPlacePhrase, start);
		end = phrase.find(endPhase);
		if(end != string::npos)
		{
			cuttedData = phrase.substr(startEndPlace+startEndPlacePhrase.length(), end-startEndPlace-startEndPlacePhrase.length()); //cuttedData has the internal data segment
			size_t tmpLoc = 0;
			//cout << "The date: " << cuttedData << endl;
			sscanf(cuttedData.c_str(), "%d", &day);

			char * pch;
			char* pch2;
			char* ptr = new char[cuttedData.length() + 1];
			strncpy(ptr, cuttedData.c_str(), cuttedData.length() + 1);
			DBG(ptr);
			pch = strtok (ptr," ");
			while (pch != NULL)
			{
				pch2 = pch;

				int k = 0;
				while(pch[k] != 0)
				{
					pch[k] = tolower(pch[k]);
					k++;
				}
				if((strcmp(pch, "january") == 0) || (strcmp(pch, "february") == 0) || (strcmp(pch, "march") == 0) ||
				   (strcmp(pch, "april") == 0) || (strcmp(pch, "may") == 0) || (strcmp(pch, "june") == 0) ||
				   (strcmp(pch, "july") == 0) || (strcmp(pch, "august") == 0) || (strcmp(pch, "september") == 0) ||
				   (strcmp(pch, "october") == 0) || (strcmp(pch, "november") == 0) || (strcmp(pch, "december") == 0))
				{
					strcpy(month, pch);
				}

				pch = strtok (NULL, " ");

				if(!pch){
					strncpy(yearBuff, pch2, 4);
					yearBuff[4] = '\0';
				}

			}
			delete (ptr);

			sscanf(yearBuff, "%d", &year);
			string m(month);
			sprintf(debugBuff, "day: %u month: %d year: %u", day, MonthToNumber[m], year);
			//PRINT(debugBuff);


		}
	}

	start = phrase.find(startPhase, start + 5);
	if(start != string::npos)
	{
		end = phrase.find(endPhase, start + 5);
		if(end != string::npos)
		{
			cuttedData = phrase.substr(start+startPhase.length(), end-start-startPhase.length()+innerEndPhase.length()); //cuttedData has the internal data segment
			innerStart = cuttedData.find(innerStartPhase); //cut the inner data
			if(innerStart != string::npos)
			{
				innerEnd = cuttedData.find(innerEndPhase, innerStart+innerStartPhase.length());
				string tmp = cuttedData.substr(innerStart+innerStartPhase.length());
				//DBG(tmp);
				if(innerEnd != string::npos)
				{
					destination = cuttedData.substr(innerStart+innerStartPhase.length(), innerEnd-innerStart-innerStartPhase.length());
//					cout << "The state is: " << destination << endl;
				}else PRINT("could not find the state end breaker phrase");
			} else PRINT("could not find the state breaker phrase");
		}
	}

	connector cnt;
	MYSQL_RES* res;
	string db = "anton";
	if(cnt.initANDconnect(db.c_str()) == ok)
	{
		stringstream dima("");
		dima << "insert into visits values(0,'";
		dima << destination;
		dima << "',";
		dima << "'Queen Elizabeth II',";
		if(day>0 && day<31) dima << day; else dima << 2;
		dima << ",";
		dima << MonthToNumber[month];
		dima << ",";
		dima << year;
		dima << ",'England')";
		PRINT(dima.str());
		//string qur = "SELECT * FROM aa1 ORDER BY a1 ASC";
		MYSQL_ROW          result;
		//res = cnt.runQuery(dima.str().c_str());


		//while((result = mysql_fetch_row(res))) {

		   //cout << result[0] << endl;
		//}

	}else
	{
		PRINT("could not connect");
	}


	return ok;
}

retVal fileparser::tokenizeStringToVector(string startPhase, string endPhase, string mustAppear, std::string special, vector< vector<string> >& dataCollector)
{
	ifstream is (pFileName, ifstream::in | ifstream::binary);
	if(!is)
	{
		perror("No file descriptor");
		return error;
	}
	retVal ans;
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

	string data(buffer);
	string cuttedData;
	string specialData;
	size_t location = -5;
	size_t endLocation;
	size_t specialLocation;
	int ctr = 0;

	if(special != "")
	{
		specialLocation = data.find(special);
		if(specialLocation != string::npos)
			specialData = data.substr(specialLocation, 60); // TODO: The 40 size should be changed
	}
	else
	{
		specialData = "";
	}

	while((location = data.find(startPhase, location+5)) != string::npos) //iterate through the file
	{
		if((endLocation = data.find(endPhase, location)) != string::npos)
		{
			if((special != "") && (specialLocation != string::npos) && (specialLocation < location))
			{
				specialData = data.substr(specialLocation, 60); // TODO: The 40 size should be changed
			}

			cuttedData = data.substr(location, endLocation - location);
			//cout << cuttedData << endl;
			//vector<string> tmp;
			if(cuttedData.find(mustAppear) != string::npos)
				fileparser::handleTable(cuttedData, specialData, dataCollector);

			//if(ctr++ == 5)break; //DEBUG!!!!!!
			if(special != "")
			{
				specialLocation = data.find(special, location);
			}
		}
		else
		{
			break;
		}
	}

	delete[] buffer;
	return ok;



	return ok;
}

retVal fileparser::handleTable(std::string phrase, std::string specialPhrase, std::vector< std::vector<std::string> >& dataCollector)
{
	string cuttedData;
	size_t location = -5;
	size_t endLocation;
	string startPhase("<td><span class=\"flagicon\">");
	string endPhase("</tr>");

	while((location = phrase.find(startPhase, location+5)) != string::npos) //iterate through the file
	{
		if((endLocation = phrase.find(endPhase, location)) != string::npos)
		{
			cuttedData = phrase.substr(location, endLocation - location); // cutted data is the entire TR area
			vector< string > tmp;
			if(specialPhrase != "")
				tmp.push_back(specialPhrase);

			size_t secondLocation = -5;
			size_t secondEndLocation;

			cout << cuttedData << endl;

			while((secondLocation = cuttedData.find("<td>", secondLocation+5)) != string::npos) //iterate through the file
			{
				if((secondEndLocation = cuttedData.find("</td>", secondLocation)) != string::npos)
				{
					PRINT("########################");
					string tempStr = cuttedData.substr(secondLocation+4, secondEndLocation - secondLocation - 4);
					PRINT(tempStr);
					PRINT("$$$$$$$$$$$$$$$$$$$$$$$$");
					tmp.push_back(tempStr);
				}
			}
			dataCollector.push_back(tmp);
			tmp.clear();
		}
		else
		{
			//break;
		}
	}



	return ok;
}

retVal fileparser::dummyFunc(std::string phrase)
{
	string pre = "<h3 class=\"r\"><a href=\"";
	int loc = phrase.find_first_of("\"", pre.length() + 1);
	PRINT(phrase.substr(pre.length(), loc-pre.length()));
	return ok;
}


retVal fileparser::cropVector(std::vector<std::string>& data, int type, std::string extraData)
{
	string tmp;
	size_t beginning;
	size_t end;

	for(int j=0;j<data.size();j++)
	{
		string cut = "";
		int status = 0;
		for (int i=0;i<data[j].size();i++)
		{
			switch (data[j][i])
			{
			case '<':
				status++;
				break;
			case '>':
				status--;
				break;
			default:
				if(status == 0)cut.push_back(data[j][i]);
				break;
			}
		}
		data[j] = cut;
	}

	char month[20];
	int monthNum;
	int day;
	int year;
	switch (type)
	{
	case 1:
		//get the month and day of the visit
		sscanf(data[3].c_str(), "%s %d", month, &day);
		int k = 0;
		while(month[k] != 0)
		{
			month[k] = tolower(month[k]);
			k++;
		}
		monthNum = MonthToNumber[month];
		stringstream mon;
		mon << monthNum;
		data[3] = mon.str();
		stringstream ss;
		ss << day;
		data.push_back(ss.str());
		//check the year of the visit
		k = sscanf(data[0].c_str(), "%d", &year);
		if(k == 0)data[0] = extraData;
		break;
	}

	return ok;
}

retVal fileparser::getDates (std::string fileName)
{
	ifstream is (fileName, ifstream::in | ifstream::binary);
	if(!is)
	{
		perror("No file descriptor");
		return error;
	}
	//load the file into memory
	// get length of file:
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);

	char * buffer = new char [length + 1];
	if(!buffer)
	{
		perror("error with allocation of memory");
		return error;
	}
	is.read (buffer,length); //buffer contains the file
	buffer[length] = '\0';
	is.close();

	PRINT("First run");
	boost::regex r("([[:digit:]]{4})/([[:digit:]]{1,2})/([[:digit:]]{1,2})");
	//boost::regex r("([[:digit:]]{4})");
	string str(buffer);
	string tmp;
	tmp = str;
	boost::smatch m;
	while (boost::regex_search (tmp,m,r)) {
		//for (auto x:m) std::cout << x << " ";
		//std::cout << std::endl;
		cout << "Date: " << m[0].str() << endl;
		cout << "The year " << m[1].str() << " The month " << m[2].str() << " The day " << m[3].str() << endl;
		tmp = m.suffix().str();
	}
	PRINT("Second run");
	tmp = str;
	r.set_expression("([[:digit:]]{1,2})/([[:digit:]]{1,2})/([[:digit:]]{4})");
	while (boost::regex_search (tmp,m,r)) {
		//for (auto x:m) std::cout << x << " ";
		//std::cout << std::endl;
		cout << "Date: " << m[0].str() << endl;
		cout << "The year " << m[3].str() << " The month " << m[1].str() << " The day " << m[2].str() << endl;
		tmp = m.suffix().str();
	}
	PRINT("Third run");
	tmp = str;
	r.set_expression("([[:alpha:]]+) ([[:digit:]]{1,2})([, ]*)([[:digit:]]{4})");
	while (boost::regex_search (tmp,m,r)) {
//		for (auto x:m) std::cout << x << " ";
//		std::cout << std::endl;
//		cout << m[0].str() << endl;
		cout << "Date: " << m[0].str() << endl;
		cout << "The year " << m[4].str() << " The month " << m[1].str() << " The day " << m[2].str() << endl;
		tmp = m.suffix().str();
	}

	return ok;
}
