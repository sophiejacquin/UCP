#ifndef _eoUCP_h
#define _eoUCP_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/*!
 * \file eoUCP.h
 * \brief UCP solution
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCP<FitT>
 * \brief Class that represents an individual, solution for the UCP
 */
template< class FitT>
class eoUCP : public EO<FitT>{
 public:
  /*!
   * \brief Constructor
   *
   * eoUCP<FitT> class consctructor 
   */
  eoUCP()
    {
      startUpCost=0.;
      fuelCost=0.;
    }

  /*!
   * \brief Destructor 
   *
   * eoUCP<FitT> class destructor
   */
  virtual ~eoUCP()
    {
    }

  /*!
   * ClassName
   *
   * \return a string which represents the name of this class 
   */
  virtual string className() const
  {
    return "eoUCP";
  }

  /*!
   * \brief Print the solution
   *
   * \param ostream : output stream which writes the description of the solution
   */
	void readFrom(istream& is)
      {
	// of course you should read the fitness first!
	EO<FitT>::readFrom(is);
    // START Code of input
	for(unsigned i=0;i<mSize;i++)
	{
		bool r;
		is>> r;
		mbits[i]=r;
	}
	/** HINTS
	 * remember the eoUCP object will come from the default ctor
	 * this is why having the sizes written out is useful
	 */

    // END   Code of input
      }
  void printOn(ostream& os)const
  {
    std::stringstream ss;
   
    EO<FitT>::printOn(os);
    

    for(unsigned i=0;i<mSize;i++)
      {
	ss << mbits[i];
	if((i+1)%nbHours==0 && i<mSize-1)
	  ss<< "|";
      }

    ss << std::endl;
 
  
       ss<<std::endl<<"\nProduction per hour\n";
	 for(unsigned i=0;i<nbHours;i++)
	 {
	 ss<< "p[" << i << "]=" << prod_hour(i)  << " ";
	 }
      
	 ss<<"\n\nProduction per unit\n";

    int indice;
    for(unsigned i=0;i<nbUnits;i++)
      {
	if(i+1<10)
	  ss << "\nUnit  " << i+1 << " : ";
	else
	  ss << "\nUnit " << i+1 << " : ";
	for(unsigned j=0;j<nbHours;j++)
	  {
	    indice = i*nbHours+j;
	    ss << production[indice] << " ";
	  }
      }
    ss<<std::endl;
    os << ss.str();    
  }
  
 

  /*!
   * \brief Length of the binary string
   *
   * \return unsigned int : size of the genotype 
   */
  unsigned int get_mSize()
  {
    return mSize;
  }

  /*!
   * \brief Accessor to the n-th element in the genotype
   *
   * \return true or false  
   * \param index : Index of the element in the genotype
   */
  bool  operator[](unsigned int index)const
  {
    return mbits[index]; 
  }

  /*!
   * \brief Set a boolean value at index i in the genotype
   *
   * \param index : Index of the element to be set in the binary-string 
   * \param boolValue : New value of the element in the genotype
   */
  void setBit(unsigned int index,bool boolValue)
  {
    mbits[index]=boolValue;
  }

  /*!
   * \brief Number of units
   *
   * \return the number of units to be scheduled
   */
  unsigned int get_nbUnits()const
  {
    return nbUnits;
  }

  /*!
   * \brief Number of hours
   *
   * \return the number of hours while the state of units is scheduled
   */
  unsigned int get_nbHours()const
  {
    return nbHours;
  }

  /*!
   * \brief Set the number of units and hours
   *
   * \param _nbUnits : number of units 
   * \param _nbHours : number of hours
   */
  void set(unsigned int _nbUnits,unsigned int _nbHours)
  {
    nbUnits = _nbUnits;
    nbHours = _nbHours;
    mSize = nbUnits * nbHours;
    mbits.resize(mSize);
    production.resize(mSize);
  }


  /*!
   * \brief Set a production value 
   * \param index : Index in the production matrix
   * \param productionValue : amount of energy produced
   */
  void insert_prod(unsigned int index, double productionValue)
  {    
    production[index]=productionValue;
  }


 /*!
   * \brief index in the genotype for a unit at one hour
   *
   * \param i :number of the unit
   * \param h : hour 
   * \return index of the unit i at time h in the binary-string
   */
  unsigned int indiceUnitAtTime(unsigned int i, unsigned int h)const
  {
    return i*nbHours+h;
  }


  /*!
   * \brief Get a production value
   *
   * \param index : Index in the production matrix
   * \return amount of energy produced
   */
  double get_production(unsigned int index) const
  {
    return production[index];
  }

  /*!
   * \brief Compute the amount of energy produced at hour h
   *
   * \param h : hour at which get the amount of energy produced
   * \return amount of energy produced						\
   */
  double prod_hour(unsigned int h) const
  {
    double res=0;
    for(unsigned i=h;i<mSize;i=i+nbHours)
      {
	res += production[i];
      }
    return res;
  }

  /*!
   * \brief Reset the production matrix values
   */
  void clear_prod()
  {
    production.clear();
    production.resize(mSize);
  }

  /*!
   * Set the fuelCost to the value _fuelCost
   */
  void setFuelCost(double _fuelCost)
  {
    fuelCost=_fuelCost;
  }


  /*!
   * \brief Set the starUpCost
   *
   * \param _startUpCost :  new value of startUpCost
   */
  void setStartUpCost(double _startUpCost)
  {
    startUpCost=_startUpCost;
  }

 

  /*!
   * \brief Get the penaltyCost
   *
   * \return penaltyCost value of the solution
   */
  double get_penaltyCost()
  {
    return penaltyDemand+minUpDownPenalty;
  }


    /*!
   * \brief Get the fuelCost
   *
   * \return fuelCost value of the solution
   */
  double get_fuelCost()
  {
    return fuelCost;
  }


   /*!
   * \brief Get the startUpCost
   *
   * \return startUpCost value of the solution
   */
  double get_startUpCost() 
  {
    return startUpCost;
  }

 
 /*!
   * \brief Add a penalty concerning minimum up/down time violated constraints to the solution
   *
   * \param _penalty : penalty value to be added
   */
  void addMinUpDownPenalty(double _minUpDownPenalty)
  {
    minUpDownPenalty += _minUpDownPenalty;
  }

  /*!
   * \brief Set the penalty concerning the minimum up/down time violated constraints to the solution
   *
   * \param _penalty : new value of penaltyDemand
   */
  void setMinUpDownPenalty(double _minUpDownPenalty)
  {
    minUpDownPenalty = _minUpDownPenalty;
  }

 /*!
   * \brief Add a penalty concerning the demand to the solution
   *
   * \param _penalty : penalty value to be added
   */
  void addDemandPenalty(double _penalty)
  {
    penaltyDemand +=_penalty;
  }

  /*!
   * \brief Set the penalty concerning the demand to the solution
   *
   * \param _penalty : new value of penaltyDemand
   */
  void setDemandPenalty(double _penalty)
  {
    penaltyDemand =_penalty;
  }


  /*!
   * \brief Get the production matrix
   *
   * \return std::vector<double> *
   */
   const std::vector<double> & get_production() const{
    return production;
  }

  /*!
   * \brief Get the genotype matrix
   *
   * \return std::vector<double> *
   */
  std::vector<bool> * get_p_mbits() {
    return &mbits;
  }
 private:

  std::vector<bool> mbits; /*!< Binary-string that represents the genotype of the individual*/

  std::vector<double> production; /*!< Matrix production*/

  unsigned int mSize; /*!< Genotype size*/

  unsigned int nbUnits; /*!< Number of units to schedule*/

  unsigned int nbHours; /*!< Number of hours while the units will be scheduled*/

  double fuelCost; /*!< fuelCost value of the solution*/

  double startUpCost; /*!< startUpCost value of the solution*/

  double penaltyDemand; /*!< penaltyCost value of the solution*/
  
  double minUpDownPenalty;
};


#endif
