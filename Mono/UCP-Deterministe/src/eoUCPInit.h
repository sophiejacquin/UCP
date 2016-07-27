#ifndef _eoUCPInit_h
#define _eoUCPInit_h

#include <cstdlib>
#include<ga.h>
#include <ctime>


/*!
 * \file eoUCPInit.h
 * \brief Random UCP solution initilizer 
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPInit<GenotypeT>
 * \brief Class that represents the solution initilizer
 *
 * Initialize randomly the genotype of the soltuion
 */
template <class GenotypeT>
class eoUCPInit: public eoInit<GenotypeT> {
 public:



  /*!
   * \brief Constructor
   *
   * eoUCPInit<GenotypeT> class consctructor 
   */
  eoUCPInit()
    {
    }

  /*!
   * \brief  Set the number of units and the number of hours
   *
   * \param _nbUnits : number of units
   * \param _nbHours : number of hours
   */
  void set(unsigned int _nbUnits, unsigned int _nbHours)
  {
    nbHours = _nbHours;
    nbUnits = _nbUnits;
  }

  /*!
   * \brief Operator () which is applied to a genotype for the random initialization
   */
  void operator()(GenotypeT & _genotype)
  {   
    _genotype.set(nbUnits,nbHours);
    std::vector<bool> * mbits = _genotype.get_p_mbits();
    int mSize=/*_genotype.get_mSize();*/(*mbits).size();
    for(unsigned i=0;i<mSize;i++)
      {
	//_genotype.setBit(i,generator());
        (*mbits)[i]=generator();
      }
    _genotype.invalidate();
  }

 private:
  eoBooleanGenerator  generator; /*!< Random boolean generator*/

  unsigned int nbHours;/*!< number of hours*/

  unsigned int nbUnits;/*!< number of units*/

};

#endif


