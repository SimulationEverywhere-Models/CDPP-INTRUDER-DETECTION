#ifndef __SENSOR_DE_SONIDO_H__
#define __SENSOR_DE_SONIDO_H__

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
#include "atomic.h"  // class Atomic
#include "Debugger.h"

/** declarations **/
class SensorDeSonido: public Atomic
{
public:
	SensorDeSonido( const string &name = "SensorDeSonido" ) ;	 // Default constructor

	~SensorDeSonido();					// Destructor

	virtual string className() const
		{return "SensorDeSonido";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &sonido_in ;
	const Port &cmd_sonido_in ;	
	Port &senial_sonido_out ;

	enum Estado { ESPERANDO, ESTIMULO_RECIBIDO, INACTIVO, ENVIAR_SENIAL, MANTENER_SENIAL, CONFIGURANDO, ACTIVO };

    Estado estado;
	double sonido;
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
