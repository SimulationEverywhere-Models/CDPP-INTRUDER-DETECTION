#ifndef __SENSOR_DE_MOVIMIENTO_H__
#define __SENSOR_DE_MOVIMIENTO_H__

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
#include "atomic.h"  // class Atomic
#include "Debugger.h"

/** declarations **/
class SensorDeMovimiento: public Atomic
{
public:
	SensorDeMovimiento( const string &name = "SensorDeMovimiento" ) ;	 // Default constructor

	~SensorDeMovimiento();					// Destructor

	virtual string className() const
		{return "SensorDeMovimiento";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &movimiento_in ;
	const Port &cmd_movimiento_in ;	
	Port &senial_movimiento_out ;

	enum Estado { ESPERANDO, ESTIMULO_RECIBIDO, INACTIVO, ENVIAR_SENIAL, MANTENER_SENIAL, CONFIGURANDO, ACTIVO };

    Estado estado;
	double movimiento;
	double senial;
	double activado;
    double comando;
    double rango;
    double niveles;
    	
	Time tiempoReaccion;
	Time tiempoMuestreo;
    

    double calcularSenial();
	bool esComandoActivacion();
	bool esComandoRango();
	bool esComandoNiveles();
	double getValorActivacion();
	double getValorRango();
	double getValorNiveles();

	Debugger* debugger;
	string getEstadoActual();
};


#endif
