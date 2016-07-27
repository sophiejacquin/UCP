/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for evaluator in EO, a functor that computes the fitness of an EO
==========================================================================
*/

#ifndef _eoUCPEvalFunc_h
#define _eoUCPEvalFunc_h

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
class moeoUCPEvalFunc : public moeoEvalFunc<EOT>
{
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  moeoUCPEvalFunc(vector<eoUCPData> _scenarios)
{
 	scenarios=_scenarios;
}

  /** Actually compute the fitness
   *
   * @param EOT & _eo the EO object to evaluate
   *                  it should stay templatized to be usable
   *                  with any fitness type
   */
  void lambda_delta(double & lambda,double & delta,unsigned int h,EOT & _eo,int numS)
   {
     double res=0.0;

     int Pmax=0;
     int Pmin=0;

     int nbUnits=scenarios[numS].units_size();
  //   std::vector<bool> * mbits = _eo.get_p_mbits();

     double a1=0;
     double a2=0;

     double lmin=numeric_limits<double>::max();
     double lmax=0;

     double lminTmp=0;
     double lmaxTmp=0;

     for(unsigned  i=0;i<nbUnits;i++)
       {
 	eoUCPUnit & unite = scenarios[numS].get_unite(i);

 	if(_eo.getTemps(h,i)>0)
 	  {
 	    Pmax = unite.get_Pmax();
 	    Pmin = unite.get_Pmin();

 	    a1 = unite.get_a1();
 	    a2 = unite.get_a2();

 	    lminTmp = a1+(2*a2*Pmin);
 	    lmaxTmp = a1+(2*a2*Pmax);
 	    if(lminTmp<lmin)
 	      lmin=lminTmp;
 	    if(lmaxTmp>lmax)
 	      lmax=lmaxTmp;
 	  }
       }
     lambda=(lmax+lmin)/2;
     delta= (lmax-lmin)/2;
   }
  double penalityHour(EOT & _eo,int h,int numS)
  {
	  //Test existence :
	  //retour :

	      double Pmax,Pmin,sumPmin=0, sumPmax=0,demandReserve=0;
	      int nbUnits=scenarios[numS].units_size();
	      for(unsigned i=0;i<nbUnits;i++)
	        {
	  	eoUCPUnit & unite = scenarios[numS].get_unite(i);
	  	Pmax = unite.get_Pmax();
	  	Pmin = unite.get_Pmin();


	  		if(_eo.getTemps(h,i)>0)
	  		{
	  			sumPmax+=Pmax;
	  			sumPmin+=Pmin;
	  		}
	        }

	      demandReserve= (*scenarios[numS].get_load)[h-1]+(*scenarios[numS].get_reserve)[h-1];
	     if (sumPmax >=demandReserve && sumPmin<=(*scenarios[numS].get_load)[h-1])
	     {
	    	 return 0;
	     }
//	     else if(sumPmax<demandReserve) return 1000000*(demandReserve-sumPmax)*(demandReserve-sumPmax);
		else return 5000000;//*getNbGenerations()/1000;//*(sumPmin- demandReserve)*(sumPmin- demandReserve);

  }
  double StartUpCostHour(EOT & _eo, int h,int numS)
  {
	  int nbUnits=scenarios[numS].units_size();
	  unsigned i;
	  double cost=0;
	  for(i=0;i<nbUnits;i++)
	  {
		  if(_eo.getTemps(h,i)==1 && _eo.getTemps(h-1,i)<0)
		  {
			  if(_eo.getTemps(h-1,i)>-scenarios[numS].get_unite(i).get_tColdStart())
			  {
				  cost=cost+scenarios[numS].get_unite(i).get_sHot();
			  }
			  else cost=cost+scenarios[numS].get_unite(i).get_sCold();
		  }
	  }
	  return cost;
  }
  double fuelCost(EOT & _eo,int h,int numS)
  {
	  double lamb,delt;
	  lambda_delta(lamb,delt,h,_eo);
	  int nbUnits=scenarios[numS].units_size();
	  double prod[nbUnits];
	  double a1,a2,a0,pi,diff,sum=0;
	  	//int indice;
	  	do
	  	  {
	  	    sum=0;

	  	    for(unsigned int i=0;i<nbUnits;i++)
	  	      {
	  		//indice = _eo.indiceUnitAtTime(i,h);
	  		if(_eo.getTemps(h,i)>0)
	  		  {
	  		    eoUCPUnit & unite =scenarios[numS].get_unite(i);
	  		    a1=unite.get_a1();
	  		    a2=unite.get_a2();


	  		    pi=0.5*(lamb-a1)/(a2);

	  		    if(pi<unite.get_Pmin())
	  		      pi=unite.get_Pmin();

	  		    else if(pi>unite.get_Pmax())
	  		      pi=unite.get_Pmax();

	  		    prod[i]=pi;
	  		    sum += pi;
	  		  }
	  	      }

	  	    diff=(*scenarios[numS].get_load)[h-1]-sum;

	  	    if(diff<0)
	  	      lamb=lamb-delt;
	  	    else
	  	      lamb=lamb+delt;

	  	    delt=delt*0.5;
	  	  }
	  	while(fabs(diff)>0.0001);
	  	double res=0;
	  	for(unsigned int i=0;i<nbUnits;i++)
	   {
	  		if(_eo.getTemps(h,i)>0)
	  		{
	  			 eoUCPUnit & unite =scenarios[numS].get_unite(i);
	  		     a1=unite.get_a1();
	  		    a2=unite.get_a2();
	  			a0=unite.get_a0();
	  			double p=prod[i];
	  			double fc=(a2*p+a1)*p+a0;
	  			res += fc;
	  		}
	   }
	  	return res;

  }
  void operator()(EOT & _eo)
  {
    // test for invalid to avoid recomputing fitness of unmodified individuals
    if (_eo.invalid())
      {
	 UCPObjectiveVector objVec;
	vector<double> results;
	objVec[0] = 0;
	for(int i=0;i<scenarios.size();i++)
	{
	double fit=0;		   // to hold fitness value
	int nbHeures=_eo.getNbHeures();
	int h;
	double totFc=0;
	for(h=1;h<nbHeures;h++)
	{
		if(_eo.getModif(h))
		{
			double penalites;
			double fC=0;
			double sC;
			penalites=penalityHour(_eo,h,i);
			if(penalites==0)
			{
				fC=fuelCost(_eo,h,i);
				totFc+=fC;
			}
			
			sC=StartUpCostHour(_eo,h,i);
			_eo.setEval(h,penalites+fC+sC);
			_eo.setModif(h,false);
		}
		fit=fit+_eo.getEval(h);
		objVec[0] +=_eo.getEval(h) ;
	}
	results.push_back(fit);
	
	
	}
	
	objVec[0]=objVec[0]/scenarios.size();
	objVec[1]=0;
	for(int i=0;i<scenarios.size();i++)
	{
		objVec[1]+=(results[i]-objVec[0])*(results[i]-objVec[0]);
	}
	objVec[1]=objVec[1]/scenarios.size();
	

	//if(fit<=564058 ) cout<<"fc "<<totFc<<endl;
	_moeo.objectiveVector(objVec);
      }
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
  vector<eoUCPData> scenarios;
// END   Private data of an eoUCPEvalFunc object
};


#endif