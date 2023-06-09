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



void MainSimulator::registerNewAtomics()
{
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<SensorDeApertura>() , "SensorDeApertura" ) ;
}
