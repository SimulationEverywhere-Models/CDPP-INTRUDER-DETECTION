#ifndef __EVENTO_H__
#define __EVENTO_H__

/*******************************************************************
*
*  DESCRIPTION: Clase Evento
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 01/10/04
*
*******************************************************************/

#include "distri.h"

class Evento
{
public:

	Evento();
	Evento(int,int,int,Distribution*);
	~Evento();
	
	Distribution* getDistribucion();
	int getMovimiento();
	int getSonido();
	int getApertura();
	
private:

	int movimiento;
	int sonido;
	int apertura;
	Distribution* distribucion;
	
};


#endif