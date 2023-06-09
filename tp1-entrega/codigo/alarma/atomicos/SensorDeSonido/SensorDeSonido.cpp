#ifndef __SENSOR_DE_SONIDO_CPP__
#define __SENSOR_DE_SONIDO_CPP__
// si esta en 0, las seniales solo se envian ante estimulos
// si esta en 1, las seniales se envian periodicamente
#define __MANTENER_SENIAL__ 0

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico SensorDeSonido
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
#include "SensorDeSonido.h"  		   // base header
#include "Auxiliares.h"
#include "Debugger.h"

#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )

/*******************************************************************
* Function Name: SensorDeSonido
* Description: constructor
********************************************************************/
SensorDeSonido::SensorDeSonido( const string &name )
: Atomic( name )
, sonido_in( addInputPort( "sonido_in" ) )
, cmd_sonido_in( addInputPort( "cmd_sonido_in" ) )
, senial_sonido_out( addOutputPort( "senial_sonido_out" ) )
{
	string filename = "SensorDeSonido.dbg";
	debugger = new Debugger(filename);
}

Model &SensorDeSonido::initFunction()
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
Model &SensorDeSonido::externalFunction( const ExternalMessage &msg )
{
	string message = "[SensorDeSonido][externalFunction][B] (" + msg.time().asString()+ ") ::-::\n" + getEstadoActual();
	debugger->write(message);	
	if (msg.port() == sonido_in)
	{
		if (activado)
		{
			sonido = msg.value();
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
	else if (msg.port() == cmd_sonido_in)
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
	message = "[SensorDeSonido][externalFunction][E] ("+msg.time().asString()+") ::-::\n" +getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);	
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &SensorDeSonido::internalFunction( const InternalMessage &msg )
{
	string message = "[SensorDeSonido][internalFunction][B] ("+msg.time().asString()+") ::-::\n"+getEstadoActual();
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
			}
			if (esComandoNiveles())
			{
				niveles = getValorNiveles();
				senial = calcularSenial();
			}
			break;
		default:
			// esto no deberia ocurrir nunca, asi que si llego a este
			// estado, me quedo aca para siempre
			estado = estado;
			passivate();
			break;
	}
	message = "[SensorDeSonido][internalFunction][E] ("+msg.time().asString()+") ::-::\n"+getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);	
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &SensorDeSonido::outputFunction( const InternalMessage &msg )
{
	if (estado == ENVIAR_SENIAL)
	{
		if (activado)
		{
			sendOutput(msg.time(), senial_sonido_out, senial);
		}
	}
	else
	{
		// en cualquier otro estado, no hago nada
	}
	return *this;
}

SensorDeSonido::~SensorDeSonido()
{
	delete debugger;
}


double SensorDeSonido::calcularSenial()
{
	int resultado = sonido * niveles;
	resultado /= rango;
	return resultado;
}

bool SensorDeSonido::esComandoActivacion()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '1');
}

bool SensorDeSonido::esComandoRango()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '2');
}

bool SensorDeSonido::esComandoNiveles()
{
	string comandoString = doubleToString(comando);
	return (comandoString[0] == '3');	
}

double SensorDeSonido::getValorActivacion()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());
}

double SensorDeSonido::getValorRango()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());
}

double SensorDeSonido::getValorNiveles()
{
	string comandoString = doubleToString(comando);
	return atof(comandoString.substr(1).c_str());	
}

string SensorDeSonido::getEstadoActual()
{
	string estadoString;
	
	estadoString += ".activado = " + doubleToString(activado) + "\n";
	estadoString += ".sonido = " + doubleToString(sonido) + "\n";
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