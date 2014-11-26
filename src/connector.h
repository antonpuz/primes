/*
 * connector.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "gDefines.h"
#include "mysql.h"

class connector
{
public:
	void setK(int newK);
	int getK();
	void printK();

	connector();

	retVal initANDconnect(const char* DBName);
	MYSQL_RES* runQuery(const char* query);

private:
	int k;
    MYSQL            *connection;
    MYSQL_RES        *resource;
};

#endif /* CONNECTOR_H_ */
