#ifndef __LOG_H__
#define __LOG_H__

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
#include "atomic.h"  // class Atomic
#include <vector>  // class vector

/** declarations **/
class Log: public Atomic
{
public:
	Log( const string &name = "Log" ) ;	 // Default constructor

	~Log();					// Destructor

	virtual string className() const
		{return "Log";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &alarma_in ;
	const Port &estimulos_in ;	
	Port &reaccion_out ;

	enum Estado { NO_TENGO_NADA, TENGO_ALARMA, TENGO_ESTIMULOS, TENGO_UN_PAR, ENVIAR_REACCION };

    Estado estado;
	vector<double> alarma;
	vector<double> estimulos;
    vector<double> reaccion;
    
    double armarReaccion(double,double);

};


#endif
