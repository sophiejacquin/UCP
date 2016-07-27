#ifndef eoUCPWindowCrossover_H
#define eoUCPWindowCrossover_H

#include <ctime>
#include <cstdlib>

/*!
 * \file eoUCPWindowCrossover.h
 * \brief Crossover operator
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPWindowCrossover<GenotypeT>
 * \brief Class that represents the window-crossover operator
 */
template<class GenotypeT>
class eoUCPWindowCrossover: public eoQuadOp<GenotypeT>
{
 public:


   /*!
   * \brief Constructor
   *
   * eoUCPWindowCrossover<GenotypeT> class consctructor 
   */
  eoUCPWindowCrossover()
    {
    }

  
  /*!
   * \brief Operator () which is cross(window) two individuals
   *
   * \param _genotype1 : individual to cross with other individual
   * \param _genotype2 : individual to cross with other individual
   * \return true
   */
  bool operator()(GenotypeT& _genotype1, GenotypeT & _genotype2)
  {
    bool oneAtLeastIsModified(true);

    bool tmp;
    int mSize = _genotype1.get_mSize();
    int windowSize = rand()%mSize;
    int r = rand();
    int debut = r%(mSize-windowSize),fin=debut+windowSize;

    GenotypeT child;
    child.set(_genotype1.get_nbUnits(),_genotype1.get_nbHours());

    double fit1,fit2;
    fit1 = _genotype1.fitness();
    fit2 = _genotype2.fitness();

    GenotypeT * worseParent  = &_genotype2;
    GenotypeT * betterParent = &_genotype1;    

    if(fit1>fit2)
      {
	worseParent  = &_genotype1;
	betterParent = &_genotype2;	
      }
  
   
    if(windowSize<mSize-windowSize)
      {
	for(unsigned i=0;i<mSize;i++)
	  {
	    if(i<debut || i>fin)	      
	      child.setBit(i,(*betterParent)[i]);
	    else 
	      child.setBit(i,(*worseParent)[i]);
	  }
      }

    else
      {
	for(unsigned i=0;i<mSize;i++)
	  {
	    if(i<debut || i>debut+windowSize)	      
	      child.setBit(i,(*worseParent)[i]);
	    else 
	      child.setBit(i,(*betterParent)[i]);
	  }      
      }
 
    (*worseParent) = child;
    worseParent=NULL;
    betterParent=NULL;

    return oneAtLeastIsModified;
  }

 private:

};

#endif
