#include "connector.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "gDefines.h"

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


connector::connector(int num)
{
	k = num;
}
