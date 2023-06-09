/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*         
*
*  DATE: 29/09/04
*
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "SensorDeApertura.h"		      
#include "SensorDeMovimiento.h"		      
#include "SensorDeSonido.h"		      
#include "UnidadDeControl.h"



void MainSimulator::registerNewAtomics()
{
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SensorDeApertura>() , "SensorDeApertura" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SensorDeMovimiento>() , "SensorDeMovimiento" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SensorDeSonido>() , "SensorDeSonido" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<UnidadDeControl>() , "UnidadDeControl" ) ;            
}
