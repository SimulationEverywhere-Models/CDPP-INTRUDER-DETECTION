#ifndef __UNIDAD_DE_CONTROL_CPP__
#define __UNIDAD_DE_CONTROL_CPP__

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico UnidadDeControl
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
#include "UnidadDeControl.h"  		   // base header
#include "Auxiliares.h"
#include "Debugger.h"

#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )

/*******************************************************************
* Function Name: UnidadDeControl
* Description: constructor
********************************************************************/
UnidadDeControl::UnidadDeControl( const string &name )
: Atomic( name )
, comando_in( addInputPort( "comando_in" ) )
, senial_sonido_in( addInputPort( "senial_sonido_in" ) )
, senial_movimiento_in( addInputPort( "senial_movimiento_in" ) )
, senial_apertura_in( addInputPort( "senial_apertura_in" ) )
, cmd_sonido_out( addOutputPort( "cmd_sonido_out" ) )
, cmd_movimiento_out( addOutputPort( "cmd_movimiento_out" ) )
, cmd_apertura_out( addOutputPort( "cmd_apertura_out" ) )
, sensores_out( addOutputPort ("sensores_out") )
, alarma_out( addOutputPort ("alarma_out") )
{
	string filename = "UnidadDeControl.dbg";
	debugger = new Debugger(filename);
}

Model &UnidadDeControl::initFunction()
{
	// definimos los tiempos
	tiempoReaccion = Time(0,0,0,10);
	tiempoReaccionInterna = Time(0,0,0,2);
	// inicializamos las variables internas
	umbralAlarma = 0.75;
	nivelesApertura = 2;
	nivelesMovimiento = 2;
	nivelesSonido = 2;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &UnidadDeControl::externalFunction( const ExternalMessage &msg )
{
	string message = "[UnidadDeControl][externalFunction][B] (" + msg.time().asString() + ") ::-::\n" + getEstadoActual();
	debugger->write(message);
	if (msg.port() == senial_apertura_in)
	{
		apertura.push_back(msg.value());
		estado = RECIBIENDO_SENIALES;
		holdIn(this->state(), tiempoReaccion);
	}
	else if (msg.port() == senial_movimiento_in)
	{
		movimiento.push_back(msg.value());
		estado = RECIBIENDO_SENIALES;
		holdIn(this->state(), tiempoReaccion);
	}
	else if (msg.port() == senial_sonido_in)
	{
		sonido.push_back(msg.value());
		estado = RECIBIENDO_SENIALES;
		holdIn(this->state(), tiempoReaccion);
	}
	else if (msg.port() == comando_in)
	{
		comando.push_back(msg.value());
		estado = RECIBIENDO_COMANDOS;
		holdIn(this->state(), tiempoReaccion);
	}
	else
	{
		// no deberia llegar a este punto.. ignoramos
		estado = estado;
		passivate();
	}
	message = "[UnidadDeControl][externalFunction][E] (" + msg.time().asString() + ") ::-::\n" + getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);	
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &UnidadDeControl::internalFunction( const InternalMessage &msg )
{
	string message = "[UnidadDeControl][internalFunction][B] (" + msg.time().asString() +  ") ::-::\n" + getEstadoActual();
	debugger->write(message);
	switch (estado)
	{
		case RECIBIENDO_SENIALES:
			if (llegaronTodasLasSeniales())
			{
				armarEvento();
				estado = ANALIZANDO_EVENTO;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else
			{
				estado = estado;
				passivate();
			}
			break;
		case RECIBIENDO_COMANDOS:
			if (esComandoConfiguracion())
			{
				estado = CONFIGURANDO;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else if (esComandoSeniales())
			{
				getCantidadSeniales();
				estado = RECIBIENDO_SENIALES;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else if (esComandoInfo())
			{
				sensores = getSensores();
				estado = ENVIANDO_SENSORES;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else
			{
				// mensaje incorrecto.. ignoramos
				estado = estado;
				passivate();
			}
			break;
		case ANALIZANDO_EVENTO:
			alarma = generarAlarma();
			estado = ENVIANDO_ALARMA;
			holdIn(this->state(), tiempoReaccionInterna);
			break;
		case ENVIANDO_ALARMA:
			borrarSenialesUsadas();
			borrarComandoActual();
			if (haySeniales())
			{
				estado = RECIBIENDO_SENIALES;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else
			{
				estado = ESPERANDO;
				passivate();
			}
			break;
		case ENVIANDO_CMD_APERTURA:
		case ENVIANDO_CMD_MOVIMIENTO:
		case ENVIANDO_CMD_SONIDO:
			borrarComandoActual();
			if (hayComandos())
			{
				estado = RECIBIENDO_COMANDOS;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else
			{
				estado = ESPERANDO;
				passivate();
			}
			break;
		case ENVIANDO_SENSORES:
			sensores.clear();
			estado = ESPERANDO;
			passivate();
			break;
		case CONFIGURANDO:
			if (esCmdApertura())
			{
				borrarComandoActual();
				comandoActivo = getValorCmdApertura();
				estado = ENVIANDO_CMD_APERTURA;
				holdIn(this->state(), tiempoReaccionInterna);
			}
			else if (esCmdMovimiento())
			{
				borrarComandoActual();
				comandoActivo = getValorCmdMovimiento();
				estado = ENVIANDO_CMD_MOVIMIENTO;
				holdIn(this->state(), tiempoReaccionInterna);				
			}
			else if (esCmdSonido())
			{
				borrarComandoActual();
				comandoActivo = getValorCmdSonido();
				estado = ENVIANDO_CMD_SONIDO;
				holdIn(this->state(), tiempoReaccionInterna);				
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
	message = "[UnidadDeControl][internalFunction][E] (" + msg.time().asString() + ") ::-::\n" + getEstadoActual() + "\n---------------------------------------------------------------------\n";
	debugger->write(message);	
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &UnidadDeControl::outputFunction( const InternalMessage &msg )
{
	switch(estado)
	{
		case ENVIANDO_ALARMA:
			sendOutput(msg.time(), alarma_out, alarma);
			break;
		case ENVIANDO_CMD_APERTURA:
			sendOutput(msg.time(), cmd_apertura_out, comando.front());
			break;
		case ENVIANDO_CMD_MOVIMIENTO:
			sendOutput(msg.time(), cmd_movimiento_out, comando.front());
			break;
		case ENVIANDO_CMD_SONIDO:
			sendOutput(msg.time(), cmd_sonido_out, comando.front());
			break;
		case ENVIANDO_SENSORES:
			sendOutput(msg.time(), sensores_out, vectorToDouble(sensores));
			break;
		default:
			// en cualquier otro estado, no hago nada
			break;
	}
	return *this;
}

UnidadDeControl::~UnidadDeControl()
{
	delete debugger;
}

bool UnidadDeControl::esComandoConfiguracion()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[0] == '1');
}

bool UnidadDeControl::esComandoSeniales()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[0] == '2');
}

bool UnidadDeControl::esComandoInfo()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[0] == '3');
}

void UnidadDeControl::getCantidadSeniales()
{
	string comandoString = doubleToString(comando.front());
	cantidadApertura.push_back(atof(comandoString.substr(1,1).c_str()));
	cantidadMovimiento.push_back(atof(comandoString.substr(2,1).c_str()));
	cantidadSonido.push_back(atof(comandoString.substr(3,1).c_str()));
}

vector<double> UnidadDeControl::getSensores()
{
	///FIXME: para hacer andar esto, hay que modificar los sensores, y
	///        los estados, etc.
	return vector<double>();	
}

bool UnidadDeControl::llegaronTodasLasSeniales()
{
	bool llegoApertura = (apertura.size() >= cantidadApertura.front());
	bool llegoMovimiento = (movimiento.size() >= cantidadMovimiento.front());
	bool llegoSonido = (sonido.size() >= cantidadSonido.front());	
	return (llegoApertura && llegoMovimiento && llegoSonido);
}

void UnidadDeControl::armarEvento()
{
}

double UnidadDeControl::generarAlarma()
{
	double activacionApertura = 0;
	double activacionMovimiento = 0;
	double activacionSonido = 0;		
	if (cantidadApertura.front() == 1)
	{
		activacionApertura = apertura.front()/nivelesApertura;
	}
	if (cantidadMovimiento.front() == 1)
	{
		activacionMovimiento = movimiento.front()/nivelesMovimiento;
	}
	if (cantidadSonido.front() == 1)
	{
		activacionSonido = sonido.front()/nivelesSonido;	
	}
	return (activacionApertura+activacionMovimiento+activacionSonido) >= umbralAlarma;
}

bool UnidadDeControl::haySeniales()
{
	bool hayApertura = apertura.size();
	bool hayMovimiento = movimiento.size();
	bool haySonido = sonido.size();
	return (hayApertura || hayMovimiento || haySonido);
}

bool UnidadDeControl::hayComandos()
{
	return (comando.size() > 0);
}

bool UnidadDeControl::esCmdApertura()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[1] == '1');
}

bool UnidadDeControl::esCmdMovimiento()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[1] == '2');
}

bool UnidadDeControl::esCmdSonido()
{
	string comandoString = doubleToString(comando.front());
	return (comandoString[1] == '3');
}

double UnidadDeControl::getValorCmdApertura()
{
	string comandoString = doubleToString(comando.front());
	return (atof(comandoString.substr(2).c_str()));
}

double UnidadDeControl::getValorCmdMovimiento()
{
	string comandoString = doubleToString(comando.front());
	return (atof(comandoString.substr(2).c_str()));
}

double UnidadDeControl::getValorCmdSonido()
{
	string comandoString = doubleToString(comando.front());
	return (atof(comandoString.substr(2).c_str()));
}

void UnidadDeControl::borrarSenialesUsadas()
{
	vector<double>::iterator iterador;
	if (cantidadApertura.front() == 1)
	{
		iterador = apertura.begin();
		apertura.erase(iterador);
	}
	iterador = cantidadApertura.begin();
	cantidadApertura.erase(iterador);
	if (cantidadMovimiento.front() == 1)
	{
		iterador = movimiento.begin();
		movimiento.erase(iterador);
	}
	iterador = cantidadMovimiento.begin();
	cantidadMovimiento.erase(iterador);
	if (cantidadSonido.front() == 1)
	{
		iterador = sonido.begin();
		sonido.erase(iterador);
	}
	iterador = cantidadSonido.begin();
	cantidadSonido.erase(iterador);
}

string UnidadDeControl::getEstadoActual()
{
	string estadoString;
	
	estadoString += ".sonido = " + vectorToString(sonido) + "\n";
	estadoString += ".apertura = " + vectorToString(apertura) + "\n";
	estadoString += ".movimiento = " + vectorToString(movimiento) + "\n";
	estadoString += ".sensores = " + vectorToString(sensores) + "\n";	
	estadoString += ".comando = " + vectorToString(comando) + "\n";
	estadoString += ".comandoActivo = " + doubleToString(comandoActivo) + "\n";
	estadoString += ".alarma = " + doubleToString(alarma) + "\n";
	estadoString += ".cantidadApertura = " + vectorToString(cantidadApertura) + "\n";
	estadoString += ".cantidadMovimiento = " + vectorToString(cantidadMovimiento) + "\n";
	estadoString += ".cantidadSonido = " + vectorToString(cantidadSonido) + "\n";	
	estadoString += ".rangoApertura = " + doubleToString(rangoApertura) + "\n";
	estadoString += ".nivelesApertura = " + doubleToString(nivelesApertura) + "\n";
	estadoString += ".rangoMovimiento = " + doubleToString(rangoMovimiento) + "\n";
	estadoString += ".nivelesMovimiento = " + doubleToString(nivelesMovimiento) + "\n";
	estadoString += ".rangoSonido = " + doubleToString(rangoSonido) + "\n";
	estadoString += ".nivelesSonido = " + doubleToString(nivelesSonido) + "\n";
	switch(estado)
	{	
		case RECIBIENDO_SENIALES:
			estadoString += ".estado = RECIBIENDO_SENIALES\n";
			break;
		case RECIBIENDO_COMANDOS:
			estadoString += ".estado = RECIBIENDO_COMANDOS\n";
			break;
		case CONFIGURANDO:
			estadoString += ".estado = CONFIGURANDO\n";
			break;
		case ESPERANDO:
			estadoString += ".estado = ESPERANDO\n";
			break;
		case ANALIZANDO_EVENTO:
			estadoString += ".estado = ANALIZANDO_EVENTO\n";
			break;
		case ENVIANDO_ALARMA:
			estadoString += ".estado = ENVIANDO_ALARMA\n";
			break;
		case ENVIANDO_CMD_APERTURA:
			estadoString += ".estado = ENVIANDO_CMD_APERTURA\n";
			break;	
		case ENVIANDO_CMD_MOVIMIENTO:
			estadoString += ".estado = ENVIANDO_CMD_MOVIMIENTO\n";
			break;
		case ENVIANDO_CMD_SONIDO:
			estadoString += ".estado = ENVIANDO_CMD_SONIDO\n";
			break;
		case ENVIANDO_SENSORES:
			estadoString += ".estado = ENVIANDO_SENSORES\n";
			break;
		default:
			estadoString += ".estado = <<ESTADO INVALIDO>>\n";
			break;
	}
	
	return estadoString;
}

void UnidadDeControl::borrarComandoActual()
{
	vector<double>::iterator iter = comando.begin();
	comando.erase(iter);
}


#endif