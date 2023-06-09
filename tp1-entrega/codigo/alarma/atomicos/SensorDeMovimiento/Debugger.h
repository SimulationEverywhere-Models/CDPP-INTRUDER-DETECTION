#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

/*******************************************************************
*
*  DESCRIPTION: Debugger
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 27/09/2004
*
*******************************************************************/

/** include files **/
#include <fstream>
#include <string>

/** declarations **/
class Debugger
{
public:
	Debugger();
	Debugger(string fileName);
	~Debugger();
	
	void write(string message);

private:

	ofstream* output;

};


#endif