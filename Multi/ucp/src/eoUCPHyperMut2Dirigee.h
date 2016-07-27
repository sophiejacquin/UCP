#ifndef eoUCPHMutation2Dirigee_H
#define eoUCPHMutation2Dirigee_H

#include"eoUCPMutationMO.h"
#include <eoOp.h>
#include <iostream> 
#include <algorithm>  
#include <vector>
#include<addToParadisEO/eo/eoVectOp.h>

using namespace std;
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPHyperMut2Dirigee: public eoVectOp<GenotypeT>
{
private :
	int nbUnitsaModifier;
	int nbUnits;
	eoUCPMutationMO<GenotypeT> mut2;
public :
	//constructeur
		eoUCPHyperMut2Dirigee(int _nbUnitsaModifier, int _nbUnits,eoUCPMutationMO<GenotypeT> & _mut2)
{
			nbUnitsaModifier=_nbUnitsaModifier;
			nbUnits=_nbUnits;
			mut2=_mut2;
}
//opérateur :
 virtual vector<GenotypeT> operator()(GenotypeT & _genotype)
{

	 //ordonancement
    std::vector<GenotypeT> result;
	 vector<int> ordre;
	 int i;
	 for(i=0;i<nbUnits;i++)
		 ordre.push_back(i);
	 std::random_shuffle ( ordre.begin(), ordre.end());
	 //applications consecutives de mut2 :
	 for(i=0;i<nbUnitsaModifier;i++)
	 {
		 vector<GenotypeT> temp=mut2(_genotype,ordre[i]);
         result.reserve(result.size()+temp.size());
         result.insert(result.end(),temp.begin(),temp.end());
	 }
	 return result;
} 

};

#endif
