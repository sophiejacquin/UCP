/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutation3_H
#define eoUCPMutation3_H


#include <eoOp.h>
#include <iostream>
using namespace std;
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPMutation3: public eoMonOp<GenotypeT>
{
private:
// START Private data of an eoUCPMutation object
  //  varType anyVariable;	
 /*!< Pointer to the vector that contains the units*/
std::vector<eoUCPData> scenarios;
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;

// END   Private data of an eoUCPMutation object
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPMutation3 (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,vector<eoUCPData>  _scenarios)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
		scenarios=_scenarios;
	}
	
    // END   Code of Ctor of an eoUCPEvalFunc object
  

  /// The class name. Used to display statistics
  string className() const { return "eoUCPMutation"; }

  /**
   * modifies the parent
   * @param _genotype The parent genotype (will be modified)
   */
  bool operator()(GenotypeT & _genotype)
  {

 bool isModified(true);

//choix scenario et construction vecteurs units load et reserve, puis pas de changement :
int nbScenarios=scenarios.size();
eoUniformGenerator<int> choixScenario(0,nbScenarios);
int scenario=choixScenario();
vector<eoUCPUnit>* units =scenarios[scenario].get_units() ;
std::vector<double>  * load=scenarios[scenario].get_load() ;
std::vector<double>  * reserve=scenarios[scenario].get_reserve() ;
//cout<<"debut mut"<<endl;
	  eoUniformGenerator<int> choixBinaire(0,2);
	int nbHeures=_genotype.getNbHeures();
	int nbUnits= _genotype.getNbUnits();
	int h,e,j;
//      bool isModified(true);
    // START code for mutation of the _genotype object
	//choix état à muter :
	eoUniformGenerator<int> choixHeure(1,nbHeures);
	h=choixHeure();
	//cout<<"h :"<<h<<endl;
	vector< vector<int> > temps= _genotype.getTemps();
	//verifier si mutable
	bool mut=true;
	int dep=0;
	 double sumPmin=0;
                double sumPmax=0;
                for(j=0;j<nbUnits;j++)
                {
                        if(temps[h][j]>0)
                        {
                                sumPmin=sumPmin+ (*units)[j].get_Pmin();
                                sumPmax=sumPmax+ (*units)[j].get_Pmax();

                        }
                }

	while(mut &&((*load)[h-1]+(*reserve)[h]>sumPmax||(*load)[h-1]<sumPmin)){
	//cout<<mut<<endl;
	  vector<int> allumable;
        vector<int> eteignable;

	for(j=0;j<nbUnits;j++)
	{
//		bool mut=false;

		if(temps[h][j]==tUp[j]||temps[h][j]==1)
		{
			if(temps[h-1][j]==tUp[j]||temps[h][j]==1)//si tUp est mutable
			{mut=true;
			eteignable.push_back(j);}
			

		}
		else if ((temps[h][j]<=tDown[j]&&temps[h-1][j]<=tDown[j])||temps[h][j]==-1)
		{
			mut=true;
			allumable.push_back(j);
		}
	}
	//sinon changer :


		eoUniformGenerator<int> choixEteignage(0,eteignable.size());
		eoUniformGenerator<int> choixAlumage(0,allumable.size());
		//cout<<"taille vecteurs "<<eteignable.size()<<" "<<allumable.size()<<endl;
	
	
		//cout<<"ici"<<endl;
		if((*load)[h-1]<sumPmin && eteignable.size()>0)
		{
//cout<<"ici "<<sumPmin<<" "<<(*load)[h]<<" "<<h<<endl;
			int choix= choixEteignage();
//			cout<<choix<<endl;
			e=eteignable[choix];
		}
		else{
			if ((*load)[h-1]+(*reserve)[h-1]>sumPmax && allumable.size()>0 )
			{
//cout<<"là"<<endl;
				int choix=choixAlumage();
				e=allumable[choix];
			}
			else
			{
				mut=false;
			//cout<<"da"<<endl;
				
			}
		}
	
	//if(temps[h][e]==tDown[e]&&temps[h-1][e]>tDown[e]&&tDown[e]<-1) cout<<"pb dans mut "<<h<<" "<<e<<endl;
	//Tout est choisi, mutons :
	//cout<<"On mute l'usine "<<e<<" à l'heure "<<h<<endl;
//	_genotype.printOn(cout);
// Cas -1=> tUp :
if(mut){
	if(temps[h][e]==-1 && temps[h-1][e]!=-1)
	{
		//cout<<"-1=>tUp"<<endl;
		//on mute e :
		_genotype.setTemps(h,e,tUp[e]);
		_genotype.setModif(h,true);
		//On raccorde :
		if(h-tColdStart[e]<nbHeures&&temps[h-tColdStart[e]][e]==tColdStart[e])
		{
			//cout<<"si1"<<endl;
			for(j=1;j<-tColdStart[e]+1;j++)
			{
				_genotype.setTemps(h+j,e,-j);
				 _genotype.setModif(h+j,true);
			}
			if(h-tColdStart[e]+1<nbHeures)_genotype.setModif(h-tColdStart[e]+1,true);
		}
		else if(h-tDown[e]<nbHeures &&temps[h-tDown[e]][e]>0)
		{
			//cout<<"si2"<<endl;
			j=1;
			while(h+j<nbHeures && temps[h+j][e]!=tUp[e])
			{
				_genotype.setTemps(h+j,e,tUp[e]);
				_genotype.setModif(h+j,true);
			
				j++;
			}
			if(h+j<nbHeures)_genotype.setModif(h+j,true);
		}
		else
		{
			//cout<<"sinon"<<endl;
			j=1;
			while(h+j<nbHeures &&( temps[h+j][e]!=1&&(-j>tColdStart[e]||temps[h+j][e]>tColdStart[e])) )
			{
				_genotype.setModif(h+j,true);
				if(-j>tColdStart[e])_genotype.setTemps(h+j,e,-j);
				else _genotype.setTemps(h+j,e,tColdStart[e]);
				j++;

			}if(h+j<nbHeures)_genotype.setModif(h+j,true);

		}
	}
	//Cas tUp=> -1 :
	else if(temps[h][e]==tUp[e] && temps[h-1][e]==tUp[e])
	{
		//cout<<"tUp=>-1"<<endl;
		j=0;
		while(j<-tDown[e]&& h+j<nbHeures)
		{

			 _genotype.setModif(h+j,true);
			_genotype.setTemps(h+j,e,-j-1); j++;

		}if(h+j<nbHeures)_genotype.setModif(h+j,true);
		if(h-tDown[e]<nbHeures&&temps[h-tDown[e]][e]<0)
		{
			j=-tDown[e];
			while(h+j<nbHeures&&temps[h+j][e]!=1&&(temps[h+j][e]!=tColdStart[e]||-j-1<tColdStart[e]))
			{
				 _genotype.setModif(h+j,true);
			    if(-j-1<tColdStart[e])_genotype.setTemps(h+j,e,tColdStart[e]);
				else _genotype.setTemps(h+j,e,-j-1);
				j++;

			}if(h+j<nbHeures)_genotype.setModif(h+j,true);
		}
		else{ if(h-tDown[e]+tUp[e]-1<nbHeures &&temps[h-tDown[e]+tUp[e]-1][e]>0)
		{
			int k=1;
			j=h-tDown[e];
			while(j<nbHeures&& temps[j][e]!=k)
			{
				_genotype.setModif(j,true);
				_genotype.setTemps(j,e,k);
				if(k<tUp[e])k++;
				j++;
			}if(j<nbHeures)_genotype.setModif(j,true);
		}
		else
		{
			j=-tDown[e];
			while(h+j<nbHeures && temps[h+j][e]!=1&&(temps[h+j][e]!=tColdStart[e]||-j-1<tColdStart[e]))
			{
				_genotype.setModif(h+j,true);
				if(-j-1<tColdStart[e])_genotype.setTemps(h+j,e,tColdStart[e]);
				else _genotype.setTemps(h+j,e,-j-1);
				j++;
				

			}if(h+j<nbHeures)_genotype.setModif(h+j,true);
		}}
	}
	//1=> [tDown,tCS]
	else if(temps[h][e]==1 && temps[h-1][e]!=1)
	{
		//cout<<"1=>tD"<<endl;
		//on mute :
		if(temps[h-1][e]>tColdStart[e])
		_genotype.setTemps(h,e,temps[h-1][e]-1);
		else _genotype.setTemps(h,e,tColdStart[e]);
		_genotype.setModif(h,true);
		//On accorde :
		if(h+tUp[e]<nbHeures&&temps[h+tUp[e]][e]==tUp[e])
		{
			for(j=1;j<tUp[e];j++)
			{
				
				_genotype.setModif(h+j,true);
				_genotype.setTemps(h+j,e,j);
			}
			_genotype.setModif(h+tUp[e],true);
		}
		else
		{
			j=1;
			while(h+j<nbHeures&&temps[h+j][e]!=1 && temps[h+j][e]!=tColdStart[e])
			{
				_genotype.setModif(h+j,true);
				
				if(temps[h-1][e] -j-1>tColdStart[e])_genotype.setTemps(h+j,e,temps[h-1][e]-j-1);
				else _genotype.setTemps(h+j,e,tColdStart[e]);
				
				j++;
			}
			if(h+j<nbHeures)_genotype.setModif(h+j,true);
		}
	}
	//1<= [tDown,tCS]
	else{
		//cout<<"1<=tD"<<endl;
		//on mute :
		_genotype.setTemps(h,e,1);
		_genotype.setModif(h,true);
		//On accorde :
		j=1;
		while(j<tUp[e]&&h+j<nbHeures)
		{
			_genotype.setModif(h+j,true);
			_genotype.setTemps(h+j,e,j+1);
			j++;
		}
		if(h+j<nbHeures)_genotype.setModif(h+j,true);
		//si il existe j dans [tUp+h-1,tUp+h-1+tDown] tq t(j)>0
		bool exist=false;
		j=tUp[e]+h-1;
		while(j<tUp[e]+h-tDown[e] && j<nbHeures&&exist==false)
		{
			if(temps[j][e]>0) exist=true;
			j++;
		}
		//alors on rejoins à tUp
		if (exist)
		{
			j=tUp[e]+h;
			while( j<nbHeures && temps[j][e]!=tUp[e])
			{
				_genotype.setModif(j,true);
				_genotype.setTemps(j,e,tUp[e]);
				j++;
			}
			if(j<nbHeures)_genotype.setModif(j,true);
		}
		//Sinon on rejoin à tCS ou 1
		else
		{
			j=tUp[e]+h;
			int k=-1;
			while(j<nbHeures && (temps[j][e]!=1&&temps[j][e]!=k) )
			{
				_genotype.setModif(j,true);
				_genotype.setTemps(j,e,k);
				j++;
				if(k>tColdStart[e])k--;
			}
			if(j<nbHeures)_genotype.setModif(j,true);
		}
	
	
	
	}
	       sumPmin=0;
               sumPmax=0;
                for(j=0;j<nbUnits;j++)
                {
                        if(temps[h][j]>0)
                        {
                                sumPmin=sumPmin+ (*units)[j].get_Pmin();
                                sumPmax=sumPmax+ (*units)[j].get_Pmax();

                        }
                }
		temps= _genotype.getTemps();
		if(h+1<nbHeures) _genotype.setModif(h+1,true);
		//for(j=1;j<nbHeures;j++) _genotype.setModif(j,true);
	}//fin si mut
//verification genome
		//cout<<nbHeures<<endl;
		/*for( h=1;h<nbHeures;h++)
		{
			/*cout<<h<<endl;
			cout<<_genotype.getTemps(h,e)<<endl;
			cout<<_genotype.getModif(h)<<endl;*/
			//_genotype.setModif(h,true);
			//cout<<"ici apres truc que je crois "<<endl;
			/*int pred=_genotype.getTemps(h-1,e);
			int actu=_genotype.getTemps(h,e);
			if(pred>0)
			{
				if(!((pred<tUp[e] && actu==pred+1)||(pred==tUp[e] &&(actu==-1 || actu==tUp[e]))))
				{cout<<"ERREURE Mutation"<<endl; _genotype.printOn(cout);cout<<_genotype.getTemps(-1,e);}
				
			}
			else
			{
				if(!((pred>tColdStart[e]&&actu==pred-1)||(pred<=tDown[e] && actu==1)||(pred==tColdStart[e] && actu==tColdStart[e])))
				{cout<<"ERREURE Mutation"<<endl; _genotype.printOn(cout); cout<<_genotype.getTemps(-1,e);}
			}
		}*/
		



       /** Requirement
	* if (_genotype has been modified)
	*     isModified = true;
	* else
	*     isModified = false;
	*/
//cout<<"fin mut"<<endl;
}
    return isModified;
	
    // END code for mutation of the _genotype object
  }
  


	
};

#endif
