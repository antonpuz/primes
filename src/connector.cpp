#include "connector.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "gDefines.h"
#include "mysql.h"

using namespace std;

void connector::setK(int newK)
{
	k = newK;
}

int connector::getK()
{
	return k;
}

void connector::printK()
{
	cout << k << endl;
}

connector::connector()
{
	k = 0;
	connection = NULL;
	resource = NULL;
}

retVal connector::initANDconnect(const char* DBName)
{
    char            *servername      = "localhost";
    char            *user            = "root";
    char            *password        = "ol13an28";
    char            *socket            = "/opt/lampp/var/mysql/mysql.sock";

    /* Intitialize connection to database, and MYSQL structure. */
    connection = mysql_init(connection);

    /* Connect to database */
    if(!mysql_real_connect(connection, servername, user,
	password, DBName, 0, NULL, 0)) {
		cout << mysql_error(connection) << endl;
		connection = NULL;
		return error;
	}

    return ok;
}

MYSQL_RES* connector::runQuery(const char* query)
{
	if(!query || !connection)
		return NULL;

	static MYSQL_RES* pResourceReply;
	mysql_query(connection, query);

	pResourceReply = mysql_use_result(connection);
	return pResourceReply;
}
