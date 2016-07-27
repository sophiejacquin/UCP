/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for evaluator in EO, a functor that computes the fitness of an EO
==========================================================================
*/

#ifndef _moeoUCPEvalFuncS_h
#define _moeoUCPEvalFuncS_h

// include whatever general include you need
#include <stdexcept>
#include <fstream>

// include the base definition of eoEvalFunc
#include "eoEvalFunc.h"

/**
  Always write a comment in this format before class definition
  if you want the class to be documented by Doxygen
*/
template <class EOT>
class moeoUCPEvalFuncS : public eoEvalFunc<EOT>
{
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  moeoUCPEvalFuncS()
    {}
void setData(eoUCPData * _data)
{
  if(_data!=NULL){
    units=_data->get_units();
    load=_data->get_load();
    reserve=_data->get_reserve();
  }
}

  /** Actually compute the fitness
   *
   * @param EOT & _eo the EO object to evaluate
   *                  it should stay templatized to be usable
   *                  with any fitness type
   */
   double penalityHour(EOT & _eo,int h)
  {
	  //Test existence :
	  //retour :

	      double Pmax,Pmin,sumPmin=0, sumPmax=0,demandReserve=0;
	      int nbUnits=(*units).size();
	      for(unsigned i=0;i<nbUnits;i++)
	        {
	  	eoUCPUnit & unite = (*units)[i];
	  	Pmax = unite.get_Pmax();
	  	Pmin = unite.get_Pmin();


	  		if(_eo.getTemps(h,i)>0)
	  		{
	  			sumPmax+=Pmax;
	  			sumPmin+=Pmin;
	  		}
	        }

	      demandReserve= (*load)[h-1]+(*reserve)[h-1];
	     if (sumPmax >=demandReserve && sumPmin<=(*load)[h-1])
	     {
	    	 return 0;
	     }
//	     else if(sumPmax<demandReserve) return 1000000*(demandReserve-sumPmax)*(demandReserve-sumPmax);
		else return 5000000;//*getNbGenerations()/1000;//*(sumPmin- demandReserve)*(sumPmin- demandReserve);

  }
  double StartUpCostHour(EOT & _eo, int h)
  {
	  int nbUnits=(*units).size();
	  unsigned i;
	  double cost=0;
	  for(i=0;i<nbUnits;i++)
	  {
		  if(_eo.getTemps(h,i)==1 && _eo.getTemps(h-1,i)<0)
		  {
			  if(_eo.getTemps(h-1,i)>-(*units)[i].get_tColdStart())
			  {
				  cost=cost+(*units)[i].get_sHot();
			  }
			  else cost=cost+(*units)[i].get_sCold();
		  }
	  }
	  return cost;
  }
    void operator()(EOT & _eo)
  {
    // test for invalid to avoid recomputing fitness of unmodified individuals
   // if (_eo.invalid())
     // {
           moeoUCPObjectiveVector objVec;
	double fit=0;		   // to hold fitness value
	int nbHeures=_eo.getNbHeures();
	int h;
          double penalites=0;
          double sC=0;

	for(h=1;h<nbHeures;h++)
	{
		
			penalites+=penalityHour(_eo,h);
            sC+=StartUpCostHour(_eo,h);

	}
          objVec[0]=penalites+sC;
          objVec[1]=penalites;
          _eo.objectiveVector(objVec);

      //}
  }


  /*!
   * \brief Get the number of generations
   *
   * \return current number of generations
   
  int getNbGenerations()const
  { 
    return atoi(nbGenerations->getValue().c_str());
  }*/
private:
// START Private data of an eoUCPEvalFunc object
  //  varType anyVariable;		   // for example ...
  std::vector<eoUCPUnit> * units; /*!< Pointer to the vector that contains the units*/
    std::vector<double> * load; /*!< Pointer to the vector that contains the hourly demand*/
    std::vector<double> * reserve; /*!< Pointer to the vector that contains the hourly reserve*/

  //  eoIncrementorParam<unsigned>  * nbGenerations; /*!< number of generations */
// END   Private data of an eoUCPEvalFunc object
};


#endif
