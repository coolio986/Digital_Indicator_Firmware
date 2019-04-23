/*
* Error.cpp
*
* Created: 4/23/2019 9:44:09 AM
*  Author: Anthony
*/

#include "Error.h"
#include <Vector.h>

Error errorArray[15];
Vector<Error> errorVector;

void startErrorHandler(void)
{
	errorVector.setStorage(errorArray);
}

void AddError(Error *eError)
{
	Error *err;


	if (!errorVector.full())
	{
		bool itemExists;
		for (int i = 0; i < errorVector.max_size(); i++)
		{
			err = &errorVector.at(i);
			itemExists = err->errorCode == eError->errorCode;
			if (itemExists){break;}
		}

		if (!itemExists){
			errorVector.push_back(*eError);
		}
	}

	//Error ev = errorVector.at(0);
	//Serial.print("");

}

void ClearError(byte ErrorCode)
{
	Error *err;

	

	for (int i = 0; i < errorVector.max_size(); i++)
	{
		err = &errorVector.at(i);
		if (err->errorCode == ErrorCode){
			errorVector.remove(i);
			err = &errorVector.at(i);
			err->errorCode = 0;
			err->errorLevel = 0;
			err->hardwareType = 0;
			Serial.print("");
		}
		
		
	}

}

bool HasErrors(void)
{
	
	for (int i = 0; i < errorVector.max_size(); i++)
	{
		if (errorVector.at(i).errorCode > 0)
		{
			return true;
		}
	}
	
	
	return false;

}

bool HasErrorCode(byte code)
{

for (int i = 0; i < errorVector.max_size(); i++)
{
	if (errorVector.at(i).errorCode == code)
	{
		return true;
	}
}


return false;
}
