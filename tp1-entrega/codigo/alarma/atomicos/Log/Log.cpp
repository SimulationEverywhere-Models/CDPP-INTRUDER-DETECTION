#ifndef __LOG_CPP__
#define __LOG_CPP__

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico Log
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 29/09/04
*
*******************************************************************/

/** include files **/
#include "Log.h"  		   // base header
#include "..\..\codigo\Auxiliares.cpp"
#include <vector>		   // class vector

#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )

/*******************************************************************
* Function Name: Log
* Description: constructor
********************************************************************/
Log::Log( const string &name )
: Atomic( name )
, estimulos_in( addInputPort( "estimulos_in" ) )
, alarma_in( addInputPort( "alarma_in" ) )
, reaccion_out( addOutputPort( "reaccion_out" ) )
{
}

Model &Log::initFunction()
{
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Log::externalFunction( const ExternalMessage &msg )
{
	if ( msg.port() == alarma_in )
	{
		// agregamos el valor recibido
		alarma.push_back(msg.value());

		if (estado == TENGO_ESTIMULOS)
		{
			estado = TENGO_UN_PAR;
			holdIn(this->state(), 0);
		}
		else
		{
			estado = TENGO_ALARMA;
			passivate();
		}
	} 
	else 
	{
		if ( msg.port() == estimulos_in )
		{
			// agregamos el valor recibido
			estimulos.push_back(msg.value());
		
			if (estado == TENGO_ALARMA)
			{
				estado = TENGO_UN_PAR;
				holdIn(this->state(), 0);
			}
			else
			{
				estado = TENGO_ESTIMULOS;
				passivate();
			}
		}
		else
		{
			// no deberia llegar nunca aca, asi que si sucede
			// me quedo en este estado para siempre
			estado = estado;
			passivate();
		}
	}
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Log::internalFunction( const InternalMessage &msg )
{
	vector<double>::iterator startIterator;
	
	switch (estado)
	{
		case TENGO_UN_PAR:
			// armamos la reaccion
			reaccion.push_back(armarReaccion(alarma.front(), estimulos.front()));
			
			// borramos la alarma y los estimulos que la generaron

			startIterator = alarma.begin();
			alarma.erase(startIterator);
			startIterator = estimulos.begin();
			estimulos.erase(startIterator);
			
			estado = ENVIAR_REACCION;
			holdIn(this->state(), 0);
			break;
		case ENVIAR_REACCION:
			if (alarma.size() == 0)
			{
				if (estimulos.size() == 0)
				{
					estado = NO_TENGO_NADA;
				}
				else
				{
					estado = TENGO_ESTIMULOS;
				}
			}
			else
			{
				if (estimulos.size() == 0)
				{
					estado = TENGO_ALARMA;
				}
				else
				{
					estado = TENGO_UN_PAR;
				}
			}
			// borramos el elemento que acabamos de enviar
			startIterator = reaccion.begin();
			reaccion.erase(startIterator);
			
			holdIn(this->state(), 0);
			break;
		default:
			// esto no deberia ocurrir nunca, asi que si llego a este
			// estado, me quedo aca para siempre
			estado = estado;
			passivate();
			break;
	}
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Log::outputFunction( const InternalMessage &msg )
{
	if ( estado == ENVIAR_REACCION )
	{
		sendOutput(msg.time(), reaccion_out, reaccion.front());
	}
	// en cualquier otro estado, no hago nada
	return *this;
}

Log::~Log()
{
}


double Log::armarReaccion(double alarma, double estimulos)
{
	string marcador = "888";
	string alarmaString = doubleToString(alarma);
	string estimulosString = doubleToString(estimulos);
	alarmaString = charStuff(alarmaString, marcador);
	estimulosString = charStuff(estimulosString, marcador);
	string reaccion = alarmaString + marcador + estimulosString;
	return atof(reaccion.c_str());
}

#endif