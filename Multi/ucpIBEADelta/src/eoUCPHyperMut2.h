#ifndef eoUCPHMutation2_H
#define eoUCPHMutation2_H

#include"eoUCPMutation2.h"
#include <eoOp.h>
#include <iostream> 
#include <algorithm>  
using namespace std;
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPHyperMut2: public eoMonOp<GenotypeT>
{
private :
	int nbUnitsaModifier;
	int nbUnits;
	eoUCPMutation2<GenotypeT> mut2;
public :
	//constructeur
		eoUCPHyperMut2(int _nbUnitsaModifier, int _nbUnits,eoUCPMutation2<GenotypeT> & _mut2)
{
			nbUnitsaModifier=_nbUnitsaModifier;
			nbUnits=_nbUnits;
			mut2=_mut2;
}
//opérateur :
 bool operator()(GenotypeT & _genotype)
{

	 //ordonancement
	 vector<int> ordre;
	 int i;
	 for(i=0;i<nbUnits;i++)
		 ordre.push_back(i);
	 std::random_shuffle ( ordre.begin(), ordre.end());
	 //applications consecutives de mut2 :
	 for(i=0;i<nbUnitsaModifier;i++)
	 {
		 mut2(_genotype,ordre[i]);
	 }
	 return true;
} 

};

#endif
