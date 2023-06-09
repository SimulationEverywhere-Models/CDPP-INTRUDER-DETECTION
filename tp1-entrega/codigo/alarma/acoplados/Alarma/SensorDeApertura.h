#ifndef __SENSOR_DE_APERTURA_H__
#define __SENSOR_DE_APERTURA_H__

/*******************************************************************
*
*  DESCRIPTION: Modelo Atómico SensorDeApertura
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
#include "atomic.h"  // class Atomic

#include "Debugger.h"

/** declarations **/
class SensorDeApertura: public Atomic
{
public:
	SensorDeApertura( const string &name = "SensorDeApertura" ) ;	 // Default constructor

	~SensorDeApertura();					// Destructor

	virtual string className() const
		{return "SensorDeApertura";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &apertura_in ;
	const Port &cmd_apertura_in ;	
	Port &senial_apertura_out ;

	enum Estado { ESPERANDO, ESTIMULO_RECIBIDO, INACTIVO, ENVIAR_SENIAL, MANTENER_SENIAL, CONFIGURANDO, ACTIVO };

    Estado estado;
	double apertura;
	double senial;
	double activado;
	double comando;
	
	Time tiempoReaccion;
	Time tiempoMuestreo;
    
    double calcularSenial();
    bool esComandoActivacion();
    double getValorActivacion();

    Debugger* debugger;
    string getEstadoActual();

};


#endif
