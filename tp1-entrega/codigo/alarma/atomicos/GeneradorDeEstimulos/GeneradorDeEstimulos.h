#ifndef __GENERADOR_DE_ESTIMULOS_H__
#define __GENERADOR_DE_ESTIMULOS_H__

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
#include "atomic.h"  // class Atomic
#include "..\..\codigo\Evento.h"
#include "..\..\codigo\Estimulo.h"

/** forward declaration **/
class Distribution;

#include <vector>  // class vector

/** declarations **/
class GeneradorDeEstimulos: public Atomic
{
public:
	GeneradorDeEstimulos( const string &name = "GeneradorDeEstimulos" ) ;	 // Default constructor

	~GeneradorDeEstimulos();					// Destructor

	virtual string className() const
		{return "GeneradorDeEstimulos";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &eventos_in ;
	const Port &estabilidad_in ;	
	Port &movimiento_out ;
	Port &sonido_out ;
	Port &apertura_out ;
	Port &control_out ;
	Port &estimulos_out ;				

	enum Estado { NO_CONFIGURADO, EMPEZADO, TERMINADO, GENERANDO_EVENTOS, GENERANDO_ESTIMULOS, ENVIANDO_ESTIMULOS };

	// variables descriptivas
    Estado estado;
    vector<Estimulo> estimulos;
    vector<Evento> defEventos;
    vector<Evento> proxEventos;
    Evento evento;

	// variables auxiliares
    int esEstable;
    int cantidadEventos;

	// funciones auxiliares
    int getCantidadEventos();
    Evento getEvento();
    vector<Estimulo> getEstimulos(Evento);
    void configurarGenerador(double);
    bool esMovimiento(Estimulo);
    bool esSonido(Estimulo);
    bool esApertura(Estimulo);
	bool esFinConfiguracion(double);


};


#endif
