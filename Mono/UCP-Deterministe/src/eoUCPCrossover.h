#ifndef eoUCPCrossover_H
#define eoUCPCrossover_H

#include <ctime>
#include <cstdlib>

/*!
 * \file eoUCPCrossover.h
 * \brief Crossover operator
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPCrossover<GenotypeT>
 * \brief Class that represents the crossover operator
 */
template<class GenotypeT>
class eoUCPCrossover: public eoQuadOp<GenotypeT>
{
 public: 

  /*!
   * \brief Constructor
   *
   * eoUCPCrossover<GenotypeT> class consctructor 
   */
  eoUCPCrossover()
    {
    }


  /*!
   * \brief Operator () which is cross two individuals
   *
   * \param _genotype1 : individual to cross with other individual
   * \param _genotype2 : individual to cross with other individual
   * \return true
   */
  bool operator()(GenotypeT& _genotype1, GenotypeT & _genotype2)
  {
    //time_t debut, fin;
    //double duree;
    //debut=time(NULL);

    bool oneAtLeastIsModified(true);
    bool tmp;

    //std::vector<bool>  mbits1 = _genotype1.get_mbits();
    //std::vector<bool>  mbits2 = _genotype2.get_mbits();

    std::vector<bool> * mbits1 = _genotype1.get_p_mbits();
    std::vector<bool> * mbits2 = _genotype2.get_p_mbits();

    //int mSize=mbits1.size(), point = rand()%mSize;
    int mSize=(*mbits1).size(), point = rand()%mSize;

    for(unsigned i = point;i<mSize;i++)
      {
	tmp = (*mbits1)[i];
        (*mbits1)[i]=(*mbits2)[i];
        (*mbits2)[i]=tmp;
      }

    _genotype1.invalidate();
    _genotype2.invalidate();

   // fin=time(NULL);
   // duree=difftime(fin, debut);
   // printf("Duree crossover : %f\n",duree);
    
    return oneAtLeastIsModified;
  }

 private:
	
};

#endif
