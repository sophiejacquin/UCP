/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for EO objects initialization in EO
============================================
*/

#ifndef _eoUCPInit_h
#define _eoUCPInit_h

// include the base definition of eoInit
#include <eoInit.h>

/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * There is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO (e.g. to initialize
 *    atoms of an eoVector you will need an eoInit<AtomType>)
 */
template <class GenotypeT>
class eoUCPInit: public eoInit<GenotypeT> {
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  eoUCPInit(int _n)
  //  eoUCPInit( varType  _anyVariable) : anyVariable(_anyVariable)
// END eventually add or modify the anyVariable argument
  {
    // START Code of Ctor of an eoUCPInit object
	nbHeures=0;
	nbUnits=0;
      n=_n;
    // END   Code of Ctor of an eoUCPInit object
  }


  /** initialize a genotype
   *
   * @param _genotype  generally a genotype that has been default-constructed
   *                   whatever it contains will be lost
   */
  void operator()(GenotypeT & _genotype)
  {
    // START Code of random initialization of an eoUCP object
	int h,i;
      _genotype.setN(n);
	_genotype.setNbUnits(nbUnits);
	eoUniformGenerator<int> changeIni(0,2);
		vector<int> dure;
	//etat ini:
	for(i=0;i<nbUnits;i++)
	{
		dure.push_back(initialStates[i]);
	}
	_genotype.adPeriode(dure);
	//etat 0 :
	dure.clear();
	for(i=0;i<nbUnits;i++)
	{
		eoUniformGenerator<int> choixOff(0,-tDown[i]+1);
		eoUniformGenerator<int> choixOn(0,tUp[i]+1);
		if(initialStates[i]>0)
		{
			if(initialStates[i]<tUp[i])
			{
				dure.push_back(initialStates[i]+1);
			}
			else
			{
				int choix=choixOff();//cout<<choix<<endl;
				if(choix>0)
					dure.push_back(tUp[i]);
				else dure.push_back(-1);
			}
		}
		else
		{
			if(initialStates[i]>tDown[i])
			{
				dure.push_back(initialStates[i]-1);
			}
			else
			{
				int choix=choixOn();//cout<<choix<<endl;
				if(choix==1) dure.push_back(1);
				else
				{
					if(initialStates[i]==tColdStart[i]) dure.push_back(tColdStart[i]);
					else dure.push_back(initialStates[i]-1);
				}
			}
		}
	}
	_genotype.adPeriode(dure);
	eoUniformGenerator<int> change(0,2);
	for(h=2;h<nbHeures+1;h++)
	{
		dure.clear();
		for(i=0;i<nbUnits;i++)
		{
		 eoUniformGenerator<int> choixOff(0,i-tDown[i]+1);
                eoUniformGenerator<int> choixOn(0,tUp[i]+1);

			if(_genotype.getTemps(h-1,i)>0)
			{
				if(_genotype.getTemps(h-1,i)<tUp[i])
				{
					dure.push_back(_genotype.getTemps(h-1,i)+1);
				}
				else
				{
					int choix=choixOff();
						//cout<<choix<<endl;
					if(choix>0)
						dure.push_back(tUp[i]);
					else dure.push_back(-1);
				}
			}
			else
			{
				if(_genotype.getTemps(h-1,i)>tDown[i])
				{
					dure.push_back(_genotype.getTemps(h-1,i)-1);
				}
				else
				{
					int choix=choixOn();//cout<<choix<<endl;
					if(choix==1) dure.push_back(1);
					else
					{
						if(_genotype.getTemps(h-1,i)==tColdStart[i]) dure.push_back(tColdStart[i]);
						else dure.push_back(_genotype.getTemps(h-1,i)-1);
					}
				}
			}
		}
		_genotype.adPeriode(dure);
		
	}
	
      _genotype.IniEval();
    // END   Code of random initialization of an eoUCP object
    _genotype.invalidate();	   // IMPORTANT in case the _genotype is old
  }
  void set(int _nbUnits, int _nbHeures,vector<int> _initialStates)
	{
		nbUnits=_nbUnits;
		nbHeures=_nbHeures;
		initialStates=_initialStates;
	}
 void set (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
	}
	
private:
// START Private data of an eoUCPInit object
  //  varType anyVariable;		   // for example ...
	int nbHeures;
	int nbUnits;
    int n;
	vector<int> initialStates;
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;
	
// END   Private data of an eoUCPInit object
};

#endif
