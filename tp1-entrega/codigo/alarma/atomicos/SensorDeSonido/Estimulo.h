#ifndef __ESTIMULO_H__
#define __ESTIMULO_H__

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

#include <string>
using std::string;

enum TipoDeEstimulo { MOVIMIENTO, SONIDO, APERTURA };

class Estimulo
{
public:

	Estimulo();
	Estimulo(TipoDeEstimulo,double valor);
	~Estimulo();
	
	TipoDeEstimulo getTipo();
	double getValor();
	
	double toDouble();
	void fromDouble(double);

private:


	TipoDeEstimulo tipo;
	double valor;	
	string doubleToString(double);
	string tipoToString();
	string valorToString();
	string getTipoString(string);
	string getValorString(string);
	TipoDeEstimulo getTipoValue(string);
	double getValorValue(string);

	
};


#endif