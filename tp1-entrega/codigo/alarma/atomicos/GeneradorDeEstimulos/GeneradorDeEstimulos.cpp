#ifndef __GENERADOR_DE_ESTIMULOS_CPP__
#define __GENERADOR_DE_ESTIMULOS_CPP__

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico GeneradorDeEstimulos
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*
*
*  DATE: 01/10/04
*
*******************************************************************/

/** include files **/
#include "GeneradorDeEstimulos.h"  		   // base header
#include "..\..\codigo\Estimulo.cpp"
#include "..\..\codigo\Evento.cpp"
#include "..\..\codigo\Auxiliares.cpp"

#include <vector>		   // class vector
#include <algorithm>
#include <sstream>

#include "distri.h"
#include "time.h"
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )

/*******************************************************************
* Function Name: GeneradorDeEstimulos
* Description: constructor
********************************************************************/
GeneradorDeEstimulos::GeneradorDeEstimulos( const string &name )
: Atomic( name )
, eventos_in( addInputPort( "estimulos_in" ) )
, estabilidad_in( addInputPort( "alarma_in" ) )
, movimiento_out( addOutputPort( "movimiento_out" ) )
, sonido_out( addOutputPort( "sonido_out" ) )
, apertura_out( addOutputPort( "apertura_out" ) )
, control_out( addOutputPort( "control_out" ) )
, estimulos_out( addOutputPort( "estimulos_out" ) )
{
}

Model &GeneradorDeEstimulos::initFunction()
{
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &GeneradorDeEstimulos::externalFunction( const ExternalMessage &msg )
{
	if ( msg.port() == eventos_in )
	{
		if (estado == NO_CONFIGURADO)
		{
			///TODO: ver como hacer para extraer la configuracion del generador
			configurarGenerador(msg.value());
			if (esFinConfiguracion(msg.value()))
			{
				estado = EMPEZADO;
				holdIn(this->state(),0);
			}
			else
			{
				estado = estado;
				passivate();
			}
		}
		else
		{
			///TODO: que hacemos en caso de un evento externo una vez ya configurado?
			estado = estado;
			// cuanto tiempo me tengo que quedar en este estado?
			//Time tiempo (0,0,0,0);
			//holdIn(this->state,tiempo);
			passivate();
		}
	}
	if (msg.port() == estabilidad_in)
	{
		esEstable = msg.value();
		if (esEstable)
		{
			estado = TERMINADO;
			passivate();
		}
		else
		{
			estado = estado;
			///TODO: que hacemos en este caso (inesperado)??
			//Tiempo tiempo (0,0,0,0);
			//holdIn(this->state, tiempo);
			passivate();
		}
		
	}
		
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &GeneradorDeEstimulos::internalFunction( const InternalMessage &msg )
{
	
	switch (estado)
	{
		case EMPEZADO:
			cantidadEventos = getCantidadEventos();
			estado = GENERANDO_EVENTOS;
			holdIn(this->state(), 0);
			break;
		case GENERANDO_EVENTOS:
			evento = getEvento();
			estado = GENERANDO_ESTIMULOS;
			holdIn(this->state(), 0);
			break;
		case GENERANDO_ESTIMULOS:
			estimulos = getEstimulos(evento);
			estado = ENVIANDO_ESTIMULOS;
			holdIn(this->state(),0);
			break;
		case ENVIANDO_ESTIMULOS:
			cantidadEventos = cantidadEventos - 1;
			if (cantidadEventos == 0)
			{
				estado = TERMINADO;
			}
			else
			{
				estado = GENERANDO_EVENTOS;
			}
			holdIn(this->state(),0);
			break;
		case TERMINADO:
			if (!esEstable)
			{
				estado = EMPEZADO;
				holdIn(this->state(),0);
			}
			else
			{
				estado = estado;
				passivate();
			}
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
Model &GeneradorDeEstimulos::outputFunction( const InternalMessage &msg )
{
	if ( estado == ENVIANDO_ESTIMULOS )
	{
		for (int i=0;i<estimulos.size();i++)
		{
			Estimulo e = estimulos[i];
			if (esMovimiento(e))
			{
				sendOutput(msg.time(), movimiento_out, e.toDouble());
			}
			if (esSonido(e))
			{
				sendOutput(msg.time(), sonido_out, e.toDouble());
			}
			if (esApertura(e))
			{
				sendOutput(msg.time(), apertura_out, e.toDouble());
			}
		}
		double estimulosDouble = vectorToDouble(estimulos);
		sendOutput(msg.time(), estimulos_out, estimulosDouble);
	}
	if (estado == EMPEZADO)
	{
		sendOutput(msg.time(), control_out, 1);
	}
	if (estado == TERMINADO)
	{
		sendOutput(msg.time(), control_out, 0);
	}
	// en cualquier otro estado, no hago nada
	return *this;
}

GeneradorDeEstimulos::~GeneradorDeEstimulos()
{
}

int GeneradorDeEstimulos::getCantidadEventos()
{
	// generamos una cantidad random de eventos para entrenar
	return rand();
}

Evento GeneradorDeEstimulos::getEvento()
{
	Evento e;
	
	if (proxEventos.size() > 0)
	{
		// ya hay eventos precalculados. devolvemos esos
	}
	else
	{
		// calculamos los proximos eventos
		vector<Time> tiempos(defEventos.size());
		Distribution* dist;
			
		// calculamos los tiempos de los proximos eventos
		for (int i=0; i<defEventos.size();i++)
		{
			e = defEventos[i];
			dist = e.getDistribucion();
			
			tiempos.push_back(Time(static_cast<float>(fabs(dist->get()))));
		}
		// ordenamos los eventos por tiempo de ejecucion
		
		vector<Time>::iterator tiemposIter;

		while (tiempos.size() > 0)
		{
			// 1. buscar el menor tiempo
			Time min = tiempos[0];
			int index_min = 0;
			for (int i=0;i<tiempos.size();i++)
			{
				if (tiempos[i] < min)
				{
					min = tiempos[i];
					index_min = i;
				}
			}
			// 2. poner el evento correspondiente a ese tiempo al final del vector
			proxEventos.push_back(defEventos[index_min]);
			// 3. eliminar el evento y el tiempo correspondiente de la lista
			tiemposIter = tiempos.begin();
			tiemposIter += index_min;
			tiempos.erase(tiemposIter);
			// 4. repetir
		}
		
	}
	e = proxEventos.front();
	proxEventos.erase(0);
	return e;
}

vector<Estimulo> GeneradorDeEstimulos::getEstimulos(Evento ev)
{
	///TODO: completar esta funcion
	vector<Estimulo> v;
	return v;
}

bool GeneradorDeEstimulos::esMovimiento(Estimulo e)
{
	return (e.getTipo() == MOVIMIENTO);
}

bool GeneradorDeEstimulos::esSonido(Estimulo e)
{
	return (e.getTipo() == SONIDO);
}

bool GeneradorDeEstimulos::esApertura(Estimulo e)
{
	return (e.getTipo() == APERTURA);
}

void GeneradorDeEstimulos::configurarGenerador(double configuracion)
{
	string configString = doubleToString(configuracion);
	int movimiento, sonido, apertura;
	if (configString[0] == '1')
	{
		// es un mensaje de configuracion, ok
		movimiento = configString[1];
		sonido = configString[2];
		apertura = configString[3];

		// leemos la distribucion que sera usada para los eventos
		// tiene que ser para todos los eventos la misma
		Distribution* dist;
		try
		{
			dist = Distribution::create(MainSimulator::Instance().getParameter(description(),"distribution"));
	
			MASSERT(dist);
	
			for (int i = 0;i<dist->varCount();i++)
			{
				string parameter(MainSimulator::Instance().getParameter(description(),dist->getVar(i)));
				dist->setVar(i,str2float(parameter));
			}
		}catch(InvalidDistribution &e)
		{
			e.addText("The model " + description() + " has distribution problems!");
			e.print(cerr);
			MTHROW(e);
		}catch(MException &e)
		{
			MTHROW(e);
		}	
	
		
		// guardamos la definicion del evento
		Evento e(movimiento,sonido,apertura,dist);
		defEventos.push_back(e);
	}
	else
	{
		// mensaje de formato incorrecto, lo ignoramos
	}

}

bool GeneradorDeEstimulos::esFinConfiguracion(double configuracion)
{
	return (configuracion == 2);
}

#endif