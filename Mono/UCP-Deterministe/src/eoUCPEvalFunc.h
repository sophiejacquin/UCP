#ifndef _eoUCPEvalFunc_h
#define _eoUCPEvalFunc_h

#include <fstream>
#include <limits>
#include <math.h>
#include <time.h>
#include <sys/time.h>

/*!
 * \file eoUCPEvalFunc.h
 * \brief Evaluation of a UCP solution
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPEvalFunc<EOT>
 * \brief Class that represents the evaluation function of a UCP solution
 */
template <class EOT>
class eoUCPEvalFunc : public eoEvalFunc<EOT>
{
 public:

  /*!
   * \brief Constructor
   *
   * eoUCPEvalFunc<EOT> class consctructor 
   */
  eoUCPEvalFunc()
    {
    }

  /*!
   * \brief Destructor 
   *
   * eoUCPEvalFunc<EOT> class destructor
   */
  virtual ~eoUCPEvalFunc()
    {
    }
    
  /*!
   * \brief Compute the lambda and delta initial values of economic dispatch prodedure
   *
   * \param lambda : variable in which the lambda initial value is stored
   * \param delta : variable in which the delta initial value is stored
   * \param h : hour 
   * \param _eo : a UCP solution
   */
  void lambda_delta(double & lambda,double & delta,unsigned int h,EOT & _eo)
  {
    double res=0.0;
    int indice;
    int Pmax=0;
    int Pmin=0;
    
    int nbUnits=(*units).size();
    std::vector<bool> * mbits = _eo.get_p_mbits();

    double a1=0;
    double a2=0;

    double lmin=numeric_limits<double>::max();
    double lmax=0;

    double lminTmp=0;
    double lmaxTmp=0;

    for(unsigned  i=0;i<nbUnits;i++)
      {
	eoUCPUnit & unite = (*units)[i];
	indice = _eo.indiceUnitAtTime(i,h);
	if((*mbits)[indice])
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

  /*!
   * \brief Compute the total fuel-cost
   *
   * \param _eo : a UCP solution
   * \return fuel-cost value of the solution _eo
   */
  double fuelCost(EOT & _eo)
  {   
    std::vector<bool> * mbits = _eo.get_p_mbits(); 
    double res=0;
    double fc;
    int indice,nbUnits=(*units).size(),nbHours=(*load).size();
    double a0,a1,a2,p;

    for(unsigned i=0;i<nbUnits;i++)
      {
	eoUCPUnit & unite = (*units)[i];
	a0=unite.get_a0();
	a1=unite.get_a1();
	a2=unite.get_a2();
	
	for(unsigned j=0;j<nbHours;j++)
	  {
   	    indice = _eo.indiceUnitAtTime(i,j);
	 
	    p = _eo.get_production(indice);
	    if((*mbits)[indice])
	      {
		
		//cout<<p<<" a2 "<<a2<<" a1 "<<a1<<" a0 "<<a0<<endl;
		res += (a2*p+a1)*p+a0;
	      }	    
	  }
      }    
    return res;
  }

   /*!
   * \brief How many hours the unit i has been ON at hour h
   *
   * \param i : number of the unit
   * \param h : hour
   * \param  _eo : a UCP solution
   * \return number of hours 
   */
  int xitOn(unsigned int i,unsigned int h,EOT & _eo){
    std::vector<bool> * mbits = _eo.get_p_mbits();
    eoUCPUnit & unite =(*units)[i];
    int res=0;
    if(h==0)
      {
	if(unite.get_initialState()>0)
	  return  unite.get_initialState();
      }
    else
      {
	int indice = _eo.indiceUnitAtTime(i,h);
	if((*mbits)[indice-1])
	  res += 1 + xitOn(i,h-1,_eo);
      }

    return res;
  }  

  /*!
   * \brief How many hours the unit i has been OFF at hour h
   *
   * \param i : number of the unit
   * \param h : hour
   * \param  _eo : a UCP solution
   * \return number of hours
   */
  int xitOff(unsigned int i,unsigned int h,EOT & _eo){

    std::vector<bool> * mbits = _eo.get_p_mbits();
    eoUCPUnit & unite =(*units)[i];
    int res=0;
    if(h==0)
      {
	if(unite.get_initialState()<0)
	  return -(unite.get_initialState());
      }
    else{
      int indice = _eo.indiceUnitAtTime(i,h);
      if(!(*mbits)[indice-1])
	res += 1 + xitOff(i,h-1,_eo);
    }

    return res;
  }

   /*!
   * \brief Compute the start-up cost for a unit i at hour h
   * 
   * \param i : number of the unit
   * \param h : hour
   * \param _eo : a UCP solution
   * \return unit i startUpCost at hour h value
   */
  double startUpCostUnitHour(unsigned int i, unsigned int h, EOT & _eo)
  {
    double res=0;
    std::vector<bool> * mbits = _eo.get_p_mbits();
    int indice = _eo.indiceUnitAtTime(i,h);
    int xt;
    bool set=false;
    eoUCPUnit & unite = (*units)[i];
 
    if((h>0 && (*mbits)[indice] && !(*mbits)[indice-1]) || (h==0 && (*mbits)[indice]))
      {
	xt = xitOff(i,h,_eo);
	if(xt>0)
	  set=true;
      }  
    if(set)
      {
 	
	if(xt<=unite.get_tColdStart()+unite.get_tDown())
	  res += unite.get_sHot();
	else
	  res += unite.get_sCold();
      }  
	//if(i==3 && h==2) cout<<"res "<<res<<" set "<<set<<" xt "<<xt<<unite.get_tColdStart()+unite.get_tDown()<<" "<<unite.get_sCold()<<" "<<unite.get_sHot()<<endl;
    return res;
  }

   /*!
   * \brief Compute start-up cost of a solution
   *
   * \param _eo : a UCP solution
   * \return start-up cost value of _eo
   */
  double startUpCost(EOT & _eo)
  {
    double cs=0;
    int nbUnits=(*units).size(),nbHours=(*load).size();
    for(unsigned i=0;i<nbUnits;i++)
      { 
	for(unsigned j=0;j<nbHours;j++)
	  {
	    cs += startUpCostUnitHour(i,j,_eo); 
	  }
	
      }
    return cs;
  }



  /*!
   * \brief Check if exists any solution at time h 
   *
   * \param _eo : a UCP solution
   * \param h : hour
   * \return true if the solution is feasible at hour h, false otherwise
   */
  bool existeSolution(EOT & _eo,int h)
  {
    std::vector<bool> * mbits = _eo.get_p_mbits();
    int indice=0, nbUnits=(*units).size();
    double Pmax,Pmin,sumPmin=0, sumPmax=0,demandReserve=0;
  
    for(unsigned i=0;i<nbUnits;i++)
      {
	eoUCPUnit & unite = (*units)[i];
	Pmax = unite.get_Pmax();
	Pmin = unite.get_Pmin();

	indice = _eo.indiceUnitAtTime(i,h);
	if((*mbits)[indice])
	  {
	    sumPmax+=Pmax;
	    sumPmin+=Pmin;
	  }	
      }
 
    demandReserve= (*load)[h]+(*reserve)[h];
    return (sumPmax >=demandReserve && sumPmin<=(*load)[h]);
  }


  /*!
   * \brief Economic dispatch at  hour h
   *
   * \param _eo : a UCP solution
   * \param h : hour
   * \param correction : bool value which indicates if a solution is corrected when the   * solution is indeasible
   */
  void lambda_iteration_hour(EOT & _eo,unsigned int h,bool correction)
  {
    double lamb,delt;
    int nbUnits =(*units).size();
    std::vector<bool> * mbits = _eo.get_p_mbits();

    if(existeSolution(_eo,h)) 
      {
	lambda_delta(lamb,delt,h,_eo);

	double a1,a2,pi,diff,sum=0;
	int indice;
	do
	  {
	    sum=0;
	    
	    for(unsigned int i=0;i<nbUnits;i++)
	      {
		indice = _eo.indiceUnitAtTime(i,h);
		if((*mbits)[indice])
		  {
		    eoUCPUnit & unite =(*units)[i];
		    a1=unite.get_a1();
		    a2=unite.get_a2();
		    

		    pi=(lamb-a1)/(2*a2);			

		    if(pi<unite.get_Pmin())
		      pi=unite.get_Pmin();		   
			
		    else if(pi>unite.get_Pmax())
		      pi=unite.get_Pmax();
				
		    _eo.insert_prod(indice,pi);		
		    sum += pi;
		  }
	      }

	    diff=(*load)[h]-sum;
	
	    if(diff<0)	
	      lamb=lamb-delt;	
	    else	
	      lamb=lamb+delt;
	
	    delt/=2;	 
	  }
	while(fabs(diff)>0.0001);
      }

    else
      {	
	if(correction)
	  correct(_eo,h);
	else
	  _eo.addDemandPenalty(1000000);
      }

  }


  /*!
   * \brief Proceed to the hourly economic dispatch
   *
   * \param _eo : a UCP solution
   */
  void lambda_iteration(EOT & _eo)
  {    
    double penaltyMinUD=0;
    penaltyMinUD=penalize(_eo);
	//cout<<"penalise"<<endl;
    if(penaltyMinUD>0)
      setMinimumUpDown(_eo);

    else
      {
	bool correction=false;
	_eo.clear_prod();
	int nbHeures = (*load).size();
	if(eo::rng.uniform()<=0.95)
	  correction=true;
   
	for(unsigned int i=0;i<nbHeures;i++)
	  {
	    lambda_iteration_hour(_eo,i,correction);
	  } 
      }       
  }  

  /*!
   * \brief Operator () which evaluates a candidate solution
   *
   * \param _eo : a UCP solution
   */
  void operator()(EOT & _eo)
  {    	

    if (_eo.invalid())
      {
	_eo.setDemandPenalty(0.0);
	_eo.setMinUpDownPenalty(0.0);
       	double fit=0,penalite=0,fc=0,cs=0; 	
	
	lambda_iteration(_eo);
	//cout<<"lambda"<<endl;
	fc += fuelCost(_eo);
	cs += startUpCost(_eo);

	_eo.setFuelCost(fc);
	_eo.setStartUpCost(cs);
	penalize(_eo);
	penalite = _eo.get_penaltyCost();
	//cout<<penalite<<endl;
	//cout<<cs<<" "<<fc<<endl;
	fit += cs+fc+penalite;
	//cout<<fit<<endl;
	_eo.fitness(fit);
      }
  }


  /*!
   * \brief Correct if the solution is infeasible at hour h
   *
   * \param _eo : a UCP solution
   * \param h : hour
   */
  void correct(EOT & _eo, unsigned int h)
  {
    std::vector<bool> * mbits = _eo.get_p_mbits();

    int indice,xoff,xon,j,nbUnits=(*units).size();
    bool excesPmin=false,deficitPmax=false;
    double sumPmin=0, sumPmax=0,demandReserve=0;
    _eo.setDemandPenalty(0.0);
   
    if(!existeSolution(_eo,h))
      {
	for(unsigned i=0;i<nbUnits;i++)
	  {
	    eoUCPUnit & unite =(*units)[i];	  

	    indice = _eo.indiceUnitAtTime(i,h);
	    if((*mbits)[indice])
	      {
		sumPmax+=unite.get_Pmax();
		sumPmin+=unite.get_Pmin();
	      }	
	  }

	demandReserve = (*load)[h]+(*reserve)[h];
	
	if(sumPmax < demandReserve)
	  {deficitPmax=true;}//cout<<"sumPmax "<<sumPmax<<" demandReserve "<<demandReserve<<" h "<<h<<endl;}
	if(sumPmin>demandReserve)
	  excesPmin=true;

	j=rand()%nbUnits;
	indice = _eo.indiceUnitAtTime(j,h);

	if(deficitPmax)
	  {
	    if(!(*mbits)[indice])
	      {
		(*mbits)[indice]=true;
		if(!existeSolution(_eo,h))
		  {
		    correct(_eo,h);
		  }
	      }
	  }      
	
	if(excesPmin)
	  {  
	    if((*mbits)[indice])
	      {
		(*mbits)[indice]=false;
		if(!existeSolution(_eo,h))
		  {
		    correct(_eo,h);
		  }
	      }
	  }
	lambda_iteration_hour(_eo,h,true);
	
      }
  }  

  /*!
   * \brief Get the penalty cost if the minimum up/down time constraint is violated
   *
   * \param _eo : a UCP solution
   * \return penalty value 
   */
  double penalize(EOT & _eo)
  {
    int n=nMinUpDownVC(_eo);
	//if(n>0) cout<<"attention n ="<<n<<endl;
    double penalite =100000*n;//*sqrt(getNbGenerations());
//cout<<"penalité à remetre"<<endl;
    _eo.setMinUpDownPenalty(penalite);
    return penalite;
  }

  /*!
   * \brief number of minimum up/down time violated constraints
   *
   * \param _eo : a UCP solution 
   * \return number of violated constraints
   */
  int nMinUpDownVC(EOT & _eo)
  {
    std::vector<bool> * mbits = _eo.get_p_mbits();
    int initialState,j=0,n=0,indice,xtOn,xtOff,xt=0,nbUnits=(*units).size(),nbHours=(*load).size(),tUp,tDown;
    bool continu=true;
    for(unsigned int i=0;i<nbUnits;i++)
      {	
	eoUCPUnit & unite =(*units)[i];
	tUp = unite.get_tUp();
	tDown = unite.get_tDown();
	//cout<< i<<" "<<tDown<<endl;
	for(unsigned j=0;j<nbHours;j++)
	  {

	    indice = _eo.indiceUnitAtTime(i,j);
	    if(j==0)
	      {		
		initialState =unite.get_initialState();		

		if((*mbits)[indice])
		  {
		    if(initialState<0)
		      {
			xtOff=xitOff(i,0,_eo);
			if(xtOff<tDown && xtOff>0)
			  {
				//cout<<"pb unit "<<i<<" heure "<<j<<endl;
			    n++;
			  }
		      }
		  }
		
		else
		  {
		    if(initialState>0)
		      {
			xtOn=xitOn(i,0,_eo);
			if(xtOn<tUp && xtOn>0)
			  {
			    n++;
				//cout<<"pb unit "<<i<<" heure "<<j<<endl;
			  }
		      }
		  }

	      }
	    else
	      {		
		if((*mbits)[indice]!=(*mbits)[indice-1])
		  {
		    if((*mbits)[indice])
		      {
			//if(i==1)cout<<"ici "<<j<<endl;
			xtOff=xitOff(i,j,_eo);
			//cout<<xtOff<<endl;
			if(xtOff<tDown && xtOff>0)
			  {
			    n++;//
			//cout<<"pb unit "<<i<<" heure "<<j<<endl;
				
			  }
		      }
		    else
		      {
			xtOn=xitOn(i,j,_eo);
			if(xtOn<tUp && xtOn>0)
			  {
			    n++;
			//   cout<<"pb unit "<<i<<" heure "<<j<<endl;
			  }
		      }
		  }
	      }
	  }
      }   
    return n;
  }

  /*!
   * \brief Repair the solution, to satisfy the minimum up/down time of the units
   *
   * \param _eo : a UCP solution
   */
  void setMinimumUpDown(EOT & _eo)    
  {
    //_eo.setMinUpDownPenalty(0.0);
    std::vector<bool> * mbits = _eo.get_p_mbits();
    int nbUnits=(*units).size(),nbHours=(*load).size();
    int xoff=0;
    int xon=0;
    int indice;
    for(unsigned i=0;i<nbUnits;i++)
      {
	for(unsigned j=0;j<nbHours;j++)
	  {	  
	    indice =_eo.indiceUnitAtTime(i,j);
	    if((*mbits)[indice])
	      {
		xoff=xitOff(i,j,_eo);
		if(xoff>0 && xoff<(*units)[i].get_tDown())
		  {
		    (*mbits)[indice]=false;
		  }		
	      }
	    else
	      {
		xon=xitOn(i,j,_eo);
		if(xon>0 && xon<(*units)[i].get_tUp())
		  {
		    (*mbits)[indice]=true;
		  }
	      }	    
	  }    
      }
	
    lambda_iteration(_eo);
  }

  /*!
   * \brief Set the input data of the problem
   *
   * \param _data : pointer to the problem  test data
   */
  void setData(eoUCPData * _data)
  {
    if(_data!=NULL){
      units=_data->get_units();
      load=_data->get_load();
      reserve=_data->get_reserve();
    }
  }
  

   /*!
   * \brief Set the number of generations
   *
   * \param _gen : Pointer to the value of current number of generations
   */
  void setGen(eoIncrementorParam<unsigned> * gen)
  {
    nbGenerations = gen;
  }

  /*!
   * \brief Get the number of generations
   *
   * \return current number of generations
   */
  int getNbGenerations()const
  { 
    return atoi(nbGenerations->getValue().c_str());
  }

 private:
  std::vector<eoUCPUnit> * units; /*!< Pointer to the vector that contains the units*/
  std::vector<double> * load; /*!< Pointer to the vector that contains the hourly demand*/
  std::vector<double> * reserve; /*!< Pointer to the vector that contains the hourly reserve*/ 
  
  eoIncrementorParam<unsigned>  * nbGenerations; /*!< number of generations */
  
  
};

#endif
