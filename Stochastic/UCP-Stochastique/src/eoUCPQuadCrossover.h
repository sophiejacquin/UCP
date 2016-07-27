/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for simple quadratic crossover operators
=================================================

Quadratic crossover operators modify the both genotypes
*/

#ifndef eoUCPQuadCrossover_H
#define eoUCPQuadCrossover_H

#include <eoOp.h>

/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPQuadCrossover: public eoQuadOp<GenotypeT>
{
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPQuadCrossover()
  //  eoUCPQuadCrossover( varType  _anyVariable) : anyVariable(_anyVariable)
// END eventually add or modify the anyVariable argument
  {
    // START Code of Ctor of an eoUCPEvalFunc object
    // END   Code of Ctor of an eoUCPEvalFunc object
  }

  /// The class name. Used to display statistics
  string className() const { return "eoUCPQuadCrossover"; }

  /**
   * eoQuad crossover - _genotype1 and _genotype2 are the (future)
   *       offspring, i.e. _copies_ of the parents, to be modified
   * @param _genotype1 The first parent
   * @param _genotype2 The second parent
   */
  bool operator()(GenotypeT& _genotype1, GenotypeT & _genotype2)
  {
cout<<"deb cross"<<endl;
      bool oneAtLeastIsModified(true);
    // START code for crossover of _genotype1 and _genotype2 objects
	int nbHeures=_genotype1.getNbHeures();
	int nbUnits= _genotype1.getNbUnits();
	int i,j,k;
	int der=-1;
	double sum1=0;
	double sum2=0;
	vector< vector<int> > temps1 =_genotype1.getTemps();
	vector< vector<int> > temps2 =_genotype2.getTemps();
	for(i=0;i<nbHeures;i++)
	{
		//verifier si on est sur un état commun :
		bool commun=true;
		j=0;
		while(commun && j<nbUnits)
		{
			if(temps1[i][j]!=temps2[i][j])
			{
				commun=false;
			} j++;
		}
		if(commun)
		{
			
			if(sum2<sum1)
			{
				for(k=der+1;k<i;k++)
				{
					//on echange :
					for(j=0;j<nbUnits;j++)
					{
						_genotype1.setTemps(k,j,temps2[k][j]);
					}
					_genotype1.setEval1(k,_genotype2.getEval1(k));
					_genotype2.setEval2(k,_genotype1.getEval2(k));
					
					
				}
				if(der+1!=i)
				{_genotype1.setModif(der+1,true);_genotype1.setModif(i,true); }
				//on remet sum à zero et on change der
				sum1=0; sum2=0;
				
			}
			der=i;
		}//fin si(commun)
		else
		{
			sum1=sum1+_genotype1.getEval(i);
			sum2=sum2+_genotype2.getEval(i);
		}	
	}
	//derniere portion :
	if(der>0 && sum2<sum1)
	{
		
		for(k=der+1;k<nbHeures;k++)
				{
					//on echange :
					for(j=0;j<nbUnits;j++)
					{
						_genotype1.setTemps(k,j,temps2[k][j]);
					}
					_genotype1.setEval1(k,_genotype2.getEval1(k));
					_genotype2.setEval2(k,_genotype1.getEval2(k));
					
				}
		if(der+1!=nbHeures)
				{_genotype1.setModif(der+1,true);//_genotype1.setModif(nbHeures-1,true);
				 }
	}
	else if(der<1) oneAtLeastIsModified=false;
	cout<<"der = "<<der<<endl;
       /** Requirement
	* if (at least one genotype has been modified) // no way to distinguish
	*     oneAtLeastIsModified = true;
	* else
	*     oneAtLeastIsModified = false;
	*/
    return oneAtLeastIsModified;
    // END code for crossover of _genotype1 and _genotype2 objects
  }

private:
// START Private data of an eoUCPQuadCrossover object
  //  varType anyVariable;		   // for example ...
// END   Private data of an eoUCPQuadCrossover object
};

#endif
