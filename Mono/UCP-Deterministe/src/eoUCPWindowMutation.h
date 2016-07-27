#ifndef eoUCPWindowMutation_H
#define eoUCPWindowMutation_H


/*!
 * \file eoUCPWindowMutation.h
 * \brief Window-mutation operator
 * \author Yanis NAIT ABDELAZIZ
 */


/*!
 * \class eoUCPWindowMutation<GenotypeT>
 * \brief Class that represents the window-mutation operator
 */
template<class GenotypeT>
class eoUCPWindowMutation: public eoMonOp<GenotypeT>
{
 public:

  /*!
   * \brief Constructor
   *
   * eoUCPWindowMutation<GenotypeT> class consctructor 
   */
  eoUCPWindowMutation()
    {
  
    }


  /*!
   * \brief Operator () which is applied to a genotype for the window-mutation
   *
   * \param _genotype : individual to be muted
   * \return true
   */
  bool operator()(GenotypeT & _genotype)
  {
    bool isModified(true);
    std::vector<bool> * mbits = _genotype.get_p_mbits();
    int nbHours = _genotype.get_nbHours();
    int nbUnits = _genotype.get_nbUnits();

    int unit = rand()%nbUnits;
    int windowSize = rand()%nbHours;
    int h1 = rand()%(nbHours-windowSize);

    int debut = unit*nbHours+h1,fin=debut+windowSize;

    for(unsigned i=debut;i<fin;i++)
      {
	//_genotype.setBit(i,!_genotype[i]);
	(*mbits)[i]=!(*mbits)[i];
      }
    _genotype.invalidate();
    return isModified;
  }

 private:
  
};

#endif
