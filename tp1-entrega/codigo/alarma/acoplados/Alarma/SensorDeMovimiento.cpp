#ifndef __SENSOR_DE_MOVIMIENTO_CPP__
#define __SENSOR_DE_MOVIMIENTO_CPP__
// si esta en 0, las seniales solo se envian ante estimulos
// si esta en 1, las seniales se envian periodicamente
#define __MANTENER_SENIAL__ 0

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico SensorDeMovimiento
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 02/10/04
*
*******************************************************************/

/** include files **/
#include "SensorDeMovimiento.h"  		   // base header
#include "Auxiliares.h"
#include "Debugger.h"

#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )

/*******************************************************************
* Function Name: SensorDeMovimiento
* Description: constructor
********************************************************************/
SensorDeMovimiento::SensorDeMovimiento( const string &name )
: Atomic( name )
, movimiento_in( addInputPort( "movimiento_in" ) )
, cmd_movimiento_in( addInputPort( "cmd_movimiento_in" ) )
, senial_movimiento_out( addOutputPort( "senial_movimiento_out" ) )
{
	string filename = "SensorDeMovimiento.dbg";
	debugger = new Debugger(filename);
}

Model &SensorDeMovimiento::initFunction()
{
	// definimos los tiempos
	tiempoReaccion = Time(0,0,0,10);
	tiempoMuestreo = Time(0,0,0,50);
	// inicializamos las variables internas
	activado = 1;
	niveles = 2;
	rango = 10;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &SensorDeMovimiento::externalFunction( const ExternalMessage &msg )
{
	string message = "[SensorDeMovimiento][externalFunction][B] ("+msg.time().asString()+ ") ::-::\n"+getEstadoActual();
	debugger->write(message);	
	if (msg.port() == movimiento_in)
	{
		if (activado)
		{
			movimiento = msg.value();
			estado = ESTIMULO_RECIBIDO;
			holdIn(this->state(), tiempoReaccion);
		}
		else
		{
			// como estoy desactivado, ignoro el mensaje
			estado = estado;
			passivate();
		}
	}
	else if (msg.port() == cmd_movimiento_in)
	{
		comando = msg.value();
		estado = CONFIGURANDO;
		holdIn(this->state(), tiempoReaccion);
	}
	else
	{
		// en otro caso, no hacemos nada
		estado = estado;
		passivate();
	}
	message = "[SensorDeMovimiento][externalFunction][E] ("+msg.time().asString()+") ::-::\n"+getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &SensorDeMovimiento::internalFunction( const InternalMessage &msg )
{
	string message = "[SensorDeMovimiento][internalFunction][B] ("+msg.time().asString()+") ::-::\n"+getEstadoActual();
	debugger->write(message);	
	switch (estado)
	{
		case ESTIMULO_RECIBIDO:
			senial = calcularSenial();
			estado = ENVIAR_SENIAL;
			holdIn(this->state(), 0);
			break;
		case ENVIAR_SENIAL:
#if __MANTENER_SENIAL__
			estado = MANTENER_SENIAL;
			holdIn(this->state(), tiempoMuestreo);
#else
			estado = ESPERANDO;
			passivate();
#endif
			break;
		case MANTENER_SENIAL:
			estado = ENVIAR_SENIAL;
			holdIn(this->state(),0);
			break;
		case INACTIVO:
			if (activado)
			{
				estado = ENVIAR_SENIAL;
				holdIn(this->state(),tiempoReaccion);
			}
			else
			{
				estado = estado;
				passivate();
			}
			break;
		case CONFIGURANDO:
			if (esComandoActivacion())
			{
				activado = getValorActivacion();
				if (!activado)
				{
					estado = INACTIVO;
					passivate();
				}
				else
				{
					estado = ACTIVO;
					holdIn(this->state(), 0);
				}
			}
			if (esComandoRango())
			{
				rango = getValorRango();
				senial = calcularSenial();
#if __MANTENER_SENIAL__
			estado = MANTENER_SENIAL;
			holdIn(this->state(), tiempoMuestreo);
#else
			estado = ESPERANDO;
			passivate();
#endif
			}
			if (esComandoNiveles())
			{
				niveles = getValorNiveles();
				senial = calcularSenial();
#if __MANTENER_SENIAL__
			estado = MANTENER_SENIAL;
			holdIn(this->state(), tiempoMuestreo);
#else
			estado = ESPERANDO;
			passivate();
#endif
			}
			break;
		default:
			// esto no deberia ocurrir nunca, asi que si llego a este
			// estado, me quedo aca para siempre
			estado = estado;
			passivate();
			break;
	}
	message = "[SensorDeMovimiento][internalFunction][E] ("+msg.time().asString()+") ::-::\n"+getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);	
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &SensorDeMovimiento::outputFunction( const InternalMessage &msg )
{
	if (estado == ENVIAR_SENIAL)
	{
		if (activado)
		{
			sendOutput(msg.time(), senial_movimiento_out, senial);
		}
	}
	else
	{
		// en cualquier otro estado, no hago nada
	}
	return *this;
}

SensorDeMovimiento::~SensorDeMovimiento()
{
	delete debugger;
}


double SensorDeMovimiento::calcularSenial()
{
	int resultado = movimiento * niveles;
	resultado /= rango;
	return resultado;
}

bool SensorDeMovimiento::esComandoActivacion()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '1');
}

bool SensorDeMovimiento::esComandoRango()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '2');
}

bool SensorDeMovimiento::esComandoNiveles()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '3');	
}

double SensorDeMovimiento::getValorActivacion()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());
}

double SensorDeMovimiento::getValorRango()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());
}

double SensorDeMovimiento::getValorNiveles()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());	
}

string SensorDeMovimiento::getEstadoActual()
{
	string estadoString;
	
	estadoString += ".activado = " + doubleToString(activado) + "\n";
	estadoString += ".movimiento = " + doubleToString(movimiento) + "\n";
	estadoString += ".senial = " + doubleToString(senial) + "\n";
	estadoString += ".comando = " + doubleToString(comando) + "\n";
	estadoString += ".rango = " + doubleToString(rango) + "\n";
	estadoString += ".niveles = " + doubleToString(niveles) + "\n";
	switch(estado)
	{	
		case ESPERANDO:
			estadoString += ".estado = ESPERANDO\n";
			break;
		case ESTIMULO_RECIBIDO:
			estadoString += ".estado = ESTIMULO_RECIBIDO\n";
			break;
		case INACTIVO:
			estadoString += ".estado = INACTIVO\n";
			break;
		case ENVIAR_SENIAL:
			estadoString += ".estado = ENVIAR_SENIAL\n";
			break;
		case MANTENER_SENIAL:
			estadoString += ".estado = MANTENER_SENIAL\n";
			break;
		case CONFIGURANDO:
			estadoString += ".estado = CONFIGURANDO\n";
			break;
		case ACTIVO:
			estadoString += ".estado = ACTIVO\n";
			break;	
	}
	
	return estadoString;
}


#endif