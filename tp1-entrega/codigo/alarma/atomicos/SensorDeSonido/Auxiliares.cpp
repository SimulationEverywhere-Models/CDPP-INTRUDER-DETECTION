#ifndef __AUXILIARES_CPP__
#define __AUXILIARES_CPP__

#include "Auxiliares.h"
#include "Estimulo.h"

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

    
    
string doubleToString(double d)
{
	ostringstream ostr;	
	ostr << d;
	return ostr.str();
}

string charStuff(string estimuloString, string marcador)
{
	string temp = estimuloString;
	string resultado = "";
	
	// buscamos la presencia del marcador
	unsigned int index = temp.find(marcador,0);
	while (index != string::npos)
	{
		// encontramos el marcador, entonces hay que hacer stuffing
		string primeraParte = temp.substr(0,index+marcador.length()-1);
		string segundaParte = temp.substr(index+marcador.length());
		resultado += primeraParte + marcador;
		temp = segundaParte;
		index = temp.find(marcador,0);
	}
	resultado += temp;
	return resultado;
}


double vectorToDouble(vector<Estimulo> v)
{
	Estimulo e;
	double estimuloDouble;
	string estimuloString;

	string marcador = "999";	
	// los primeros n digitos indican la longitud del vector
	ostringstream ostr;
	ostr << setiosflags(ios::fixed) << setprecision(marcador.length()) << v.size();
	string resultado = ostr.str();
	
	// cada estimulo va seguido de un marcador de terminacion
	for (int i=0; i<v.size();i++)
	{
		e = v[i];
		estimuloDouble = e.toDouble();
		estimuloString = doubleToString(estimuloDouble);
		// para evitar confundir el marcador, se hace
		// character stuffing
		estimuloString = charStuff(estimuloString,marcador);
		resultado += estimuloString + marcador;
	}
	
	double vectorDouble = atof(resultado.c_str());
	return vectorDouble;
}

double vectorToDouble(vector<double> v)
{
	///FIXME: hacer que este metodo funcione bien
	return 0;
}

string vectorToString(vector<double> v)
{
	string resultado = "<";
	for (int i=0;i<v.size()-1;i++)
	{
		resultado += doubleToString(v[i]) + ",";
	}
	resultado += doubleToString(v[v.size()-1]) + ">";
	
	return resultado;
}

#endif