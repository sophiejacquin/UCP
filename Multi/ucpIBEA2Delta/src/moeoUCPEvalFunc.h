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
class moeoUCPEvalFunc : public eoEvalFunc<EOT>
{
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  moeoUCPEvalFunc(eoUCPData * _data)
{
  if(_data!=NULL){
    units=_data->get_units();
    load=_data->get_load();
    reserve=_data->get_reserve();
  }
}
    moeoUCPEvalFunc()
    {
    }
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
  void lambda_delta(double & lambda,double & delta,unsigned int h,EOT & _eo,double alpha)
   {
     double res=0.0;

     int Pmax=0;
     int Pmin=0;

     int nbUnits=(*units).size();
  //   std::vector<bool> * mbits = _eo.get_p_mbits();

     double a1=0;
     double a2=0;
     double b1=0;
     double b2=0;

     double lmin=numeric_limits<double>::max();
     double lmax=0;

     double lminTmp=0;
     double lmaxTmp=0;

     for(unsigned  i=0;i<nbUnits;i++)
       {
 	eoUCPUnit & unite = (*units)[i];

 	if(_eo.getTemps(h,i)>0)
 	  {
 	    Pmax = unite.get_Pmax();
 	    Pmin = unite.get_Pmin();

 	    a1 = unite.get_a1()/(_eo.getPointNadir()[0]-_eo.getPointIdeal()[0]);
 	    a2 = unite.get_a2()/(_eo.getPointNadir()[0]-_eo.getPointIdeal()[0]);
        b1 = unite.get_b1()/(_eo.getPointNadir()[1]-_eo.getPointIdeal()[1]);
        b2 = unite.get_b2()/(_eo.getPointNadir()[1]-_eo.getPointIdeal()[1]);

        lminTmp = alpha * (a1+(2*a2*Pmin)) + (1-alpha) * (b1+(2*b2*Pmin));
        lmaxTmp = alpha * (a1+(2*a2*Pmax)) + (1-alpha) * (b1+(2*b2*Pmax));
 	    if(lminTmp<lmin)
 	      lmin=lminTmp;
 	    if(lmaxTmp>lmax)
 	      lmax=lmaxTmp;
 	  }
       }
     lambda=(lmax+lmin)/2;
     delta= (lmax-lmin)/2;
   }
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
  void fuelEmissionCost(EOT & _eo,int h, double & fc, double & ec,double alpha)
  {
     
      fc=0;
      ec=0;
	  double lamb,delt;
	  lambda_delta(lamb,delt,h,_eo,alpha);
	  int nbUnits=(*units).size();
	  double prod[nbUnits];
	  double a1,a2,a0,b1,b2,b0,pi,diff,sum=0;
	  	//int indice;
	  	do
	  	  {
	  	    sum=0;

	  	    for(unsigned int i=0;i<nbUnits;i++)
	  	      {
	  		//indice = _eo.indiceUnitAtTime(i,h);
	  		if(_eo.getTemps(h,i)>0)
	  		  {
	  		    eoUCPUnit & unite =(*units)[i];
                  a1=unite.get_a1()/(_eo.getPointNadir()[0]-_eo.getPointIdeal()[0]);
	  		    a2=unite.get_a2()/(_eo.getPointNadir()[0]-_eo.getPointIdeal()[0]);
                
                  b1 = unite.get_b1()/(_eo.getPointNadir()[1]-_eo.getPointIdeal()[1]);
                  b2 = unite.get_b2()/(_eo.getPointNadir()[1]-_eo.getPointIdeal()[1]);

	  		    pi=(lamb - (alpha * a1 + (1 - alpha) * b1)) / (2*a2*alpha + (1-alpha) * 2 * b2) ;

	  		    if(pi<unite.get_Pmin())
	  		      pi=unite.get_Pmin();

	  		    else if(pi>unite.get_Pmax())
	  		      pi=unite.get_Pmax();

	  		    prod[i]=pi;
	  		    sum += pi;
	  		  }
	  	      }

	  	    diff=(*load)[h-1]-sum;

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
	  			 eoUCPUnit & unite =(*units)[i];
	  		     a1=unite.get_a1();
	  		    a2=unite.get_a2();
	  			a0=unite.get_a0();
                b1=unite.get_b1();
	  		    b2=unite.get_b2();
	  			b0=unite.get_b0();
	  			double p=prod[i];
                fc+=(a2*p+a1)*p+a0;
	  			ec+=(b2*p+b1)*p+b0;
	  		}
	   }
     
   
	  	

  }
  void operator()(EOT & _eo)
  {
     // cout<<"deb éval"<<endl;
          double alpha =  _eo.getAlpha();
                moeoUCPObjectiveVector objVec;
          double fit=0;		   // to hold fitness value
          int nbHeures=_eo.getNbHeures();
          int h;
         // double totFc=0;
          objVec[0]=0;
          objVec[1]=0;
            
          for(h=1;h<nbHeures;h++)
          {
              if(_eo.getModif(h))
		{
			//double penalites;
			double fC=0, ec=0;
			//double sC;
			//penalites=penalityHour(_eo,h);
			//if(penalites==0)
			//{
				fuelEmissionCost(_eo,h,fC,ec,alpha);
				//totFc+=fC;
			//}
            /*if(!(_eo.getModif(h)) && (fC!=_eo.getEval1(h) || ec!=_eo.getEval2(h))){
                cout<<"probleme à h "<<h<<" alpha "<<alpha<<" fC "<<fC<<" eval1 "<<_eo.getEval1(h)<<" ec "<<ec<<" eval2 "<<_eo.getEval2(h)<<endl;
                exit(1);
                
            }*/

			//sC=StartUpCostHour(_eo,h);
			_eo.setEval1(h,fC);
            _eo.setEval2(h,ec);
			//_eo.setModif(h,false);
		//
        }
              /*else
              {
                  cout<<h<<" "<<_eo.getEval1(h)<<" "<<_eo.getEval2(h)<<endl;
              }*/
              objVec[0]=objVec[0]+_eo.getEval1(h);
              objVec[1]=objVec[1]+_eo.getEval2(h);
          }
           _eo.objectiveVector(objVec);
     // cout<<"fin éval X"<<endl;
	
      //}
  }
/*void setGen(eoIncrementorParam<unsigned> * gen)
  {
    nbGenerations = gen;
  }*/

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

   // eoIncrementorParam<unsigned>  * nbGenerations; /*!< number of generations */
// END   Private data of an eoUCPEvalFunc object
};


#endif
