#ifndef __UNIDAD_DE_CONTROL_H__
#define __UNIDAD_DE_CONTROL_H__

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
#include "atomic.h"  // class Atomic
#include <vector>
#include "Debugger.h"

/** declarations **/
class UnidadDeControl: public Atomic
{
public:
	UnidadDeControl( const string &name = "UnidadDeControl" ) ;	 // Default constructor

	~UnidadDeControl();					// Destructor

	virtual string className() const
		{return "UnidadDeControl";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &comando_in ;
	const Port &senial_apertura_in ;		
	const Port &senial_movimiento_in ;		
	const Port &senial_sonido_in ;	
	Port &cmd_apertura_out ;
	Port &cmd_movimiento_out ;	
	Port &cmd_sonido_out ;
	Port &sensores_out;
	Port &alarma_out;

	enum Estado { ESPERANDO, RECIBIENDO_SENIALES, RECIBIENDO_COMANDOS, CONFIGURANDO, ANALIZANDO_EVENTO, ENVIANDO_ALARMA, \
		ENVIANDO_CMD_APERTURA, ENVIANDO_CMD_MOVIMIENTO, ENVIANDO_CMD_SONIDO, ENVIANDO_SENSORES };

    Estado estado;
	vector<double> sonido;
	vector<double> apertura;
	vector<double> movimiento;
    vector<double> sensores;
    vector<double> comando;
    double comandoActivo;
    double alarma;
    vector<double> cantidadApertura;
    vector<double> cantidadMovimiento;
    vector<double> cantidadSonido;
    
    	
	Time tiempoReaccion;
	Time tiempoReaccionInterna;
	
	
	bool esComandoConfiguracion();
	bool esComandoSeniales();
	bool esComandoInfo();
	void getCantidadSeniales();
	vector<double> getSensores();
	bool llegaronTodasLasSeniales();
	void armarEvento();
	double generarAlarma();
	bool haySeniales();
	bool hayComandos();
	bool esCmdApertura();
	bool esCmdMovimiento();
	bool esCmdSonido();
	double getValorCmdApertura();
	double getValorCmdMovimiento();
	double getValorCmdSonido();
	void borrarSenialesUsadas();
	void borrarComandoActual();
	
	double rangoApertura;
	double nivelesApertura;
	double rangoMovimiento;
	double nivelesMovimiento;
	double rangoSonido;
	double nivelesSonido;
	double umbralAlarma;
	
	Debugger* debugger;
	string getEstadoActual();
};


#endif
