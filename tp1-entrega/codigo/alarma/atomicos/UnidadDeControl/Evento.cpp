#ifndef __EVENTO_CPP__
#define __EVENTO_CPP__

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

#include "Evento.h"

Evento::Evento()
{
}

Evento::Evento(int movimiento, int sonido, int apertura, Distribution* distribucion)
{
	this->movimiento = movimiento;
	this->sonido = sonido;
	this->apertura = apertura;
	this->distribucion = distribucion;
}

Evento::~Evento()
{
}

int Evento::getMovimiento()
{
	return movimiento;
}

int Evento::getSonido()
{
	return sonido;
}

int Evento::getApertura()
{
	return apertura;
}

Distribution* Evento::getDistribucion()
{
	return distribucion;
}

#endif