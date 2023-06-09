/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Ricardo Kirkner
*
*  EMAIL: rkirkner@dc.uba.ar
*         
*
*  DATE: 02/10/04
*
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "SensorDeSonido.h"		      



void MainSimulator::registerNewAtomics()
{
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SensorDeSonido>() , "SensorDeSonido" ) ;
}
