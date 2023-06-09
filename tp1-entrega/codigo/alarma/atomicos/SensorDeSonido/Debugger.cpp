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
#include "Debugger.h"
#include <fstream>
#include <string>

/*******************************************************************
* Function Name: Debugger
* Description: constructor
********************************************************************/
Debugger::Debugger()
{
}

Debugger::Debugger(string fileName)
{
	// en este archivo vamos a guardar todos los 
	// mensajes de debugging
	output = new ofstream (fileName.c_str(), ios::out);
	if (!output)
	{
		//error al abrir el archivo....
		cout << "Error al abrir el archivo " << fileName <<endl;
		exit(1);
	}
}

/*******************************************************************
* Function Name: ~Debugger
* Description: destructor
********************************************************************/
Debugger::~Debugger()
{
	// cerramos el archivo
	output->close();
	delete output;
}

/*******************************************************************
* Function Name: write
* Description: writes a message into the defined file
********************************************************************/
void Debugger::write(string message)
{
	*output << message << endl;
	output->flush();
}