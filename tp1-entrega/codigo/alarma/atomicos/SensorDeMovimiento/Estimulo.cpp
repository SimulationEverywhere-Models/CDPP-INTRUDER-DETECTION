#ifndef __ESTIMULO_CPP__
#define __ESTIMULO_CPP__


/*******************************************************************
*
*  DESCRIPTION: Clase Estimulo
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 01/10/04
*
*******************************************************************/

#include "Estimulo.h"
#include <stdlib.h>
#include <string>
#include <sstream>
using std::string;

Estimulo::Estimulo()
{
}

Estimulo::Estimulo(TipoDeEstimulo tipo,double valor)
{
	tipo = tipo;
	valor = valor;
}

Estimulo::~Estimulo()
{
}
	
TipoDeEstimulo Estimulo::getTipo()
{
	return tipo;
}

double Estimulo::getValor()
{
	return valor;
}

double Estimulo::toDouble()
{
	string tipoString = tipoToString();
	string valorString = valorToString();
	string estimuloString = tipoString + valorString;
	return atof(estimuloString.c_str());
}
	
void Estimulo::fromDouble(double estimuloDouble)
{
	string estimuloString = doubleToString(estimuloDouble);
	string tipoString = getTipoString(estimuloString);
	string valorString = getValorString(estimuloString);
	tipo = getTipoValue(tipoString);
	valor = getValorValue(valorString);
}

string Estimulo::doubleToString(double d)
{
	ostringstream ostr;	
	ostr << d;
	return ostr.str();
}

string Estimulo::tipoToString()
{
	ostringstream ostr;
	ostr << tipo;
	return ostr.str();
}

string Estimulo::valorToString()
{
	ostringstream ostr;
	ostr << valor;
	return ostr.str();
}

string Estimulo::getTipoString(string estimuloString)
{
	return estimuloString.substr(0,1);
}

string Estimulo::getValorString(string estimuloString)
{
	return estimuloString.substr(1);
}

TipoDeEstimulo Estimulo::getTipoValue(string tipoString)
{
	int tipoInt = (atoi(tipoString.c_str())-1);
	TipoDeEstimulo t;
	switch (tipoInt)
	{
		case 0:
			t = MOVIMIENTO;
			break;
		case 1:
			t = SONIDO;
			break;
		case 2:
			t = APERTURA;
			break;
	}
	return t;
}

double Estimulo::getValorValue(string valorString)
{
	return atof(valorString.c_str());
}

#endif