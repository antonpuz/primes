/*
 * connector.h
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "gDefines.h"

class connector
{
public:
	void setK(int newK);
	int getK();
	void printK();

	connector(int num);

private:
	int k;

};

#endif /* CONNECTOR_H_ */
