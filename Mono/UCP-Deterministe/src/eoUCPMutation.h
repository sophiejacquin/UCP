#ifndef eoUCPMutation_H
#define eoUCPMutation_H


/*!
 * \file eoUCPMutation.h
 * \brief Mutation operator
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPMutation<GenotypeT>
 * \brief Class that represents the mutation operator
 */
template<class GenotypeT>
class eoUCPMutation: public eoMonOp<GenotypeT>
{
 public:

  
  /*!
   * \brief Constructor
   *
   * eoUCPMutation<GenotypeT> class consctructor 
   */
  eoUCPMutation()
    {  
    }

 
  /*!
   * \brief Operator () which is applied to a genotype for the mutation
   *
   * \param _genotype : individual to be muted
   * \return true
   */
  bool operator()(GenotypeT & _genotype)
  {
    //time_t debut, fin;
    //double duree;
    //debut=time(NULL);

    bool isModified(true);
  
    std::vector<bool> * mbits = _genotype.get_p_mbits();

    int indMut = rand()%(*mbits).size();
      
    //_genotype.setBit(indMut,!_genotype[indMut]);
    (*mbits)[indMut]=!(*mbits)[indMut];
    _genotype.invalidate();

    //fin=time(NULL);
    //duree=difftime(fin, debut);
    //printf("Duree mutation : %f\n",duree);

    return isModified;
  }

 private:
 
};

#endif