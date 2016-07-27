/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutation4_H
#define eoUCPMutation4_H


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
class eoUCPMutation4: public eoMonOp<GenotypeT>
{
private:
// START Private data of an eoUCPMutation object
  //  varType anyVariable;	
std::vector<eoUCPUnit> * units; /*!< Pointer to the vector that contains the units*/
  std::vector<double> * load; /*!< Pointer to the vector that contains the hourly demand*/
	std::vector<double> * reserve;
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;

// END   Private data of an eoUCPMutation object
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPMutation4 (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,eoUCPData * _data)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
		if(_data!=NULL){
     		 units=_data->get_units();
      		load=_data->get_load();
		reserve=_data->get_reserve();
		}
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
      //cout<<"deb mut4"<<endl;
 bool isModified(true);

	for(int inex=0; inex<1;inex++)
{
eoUniformGenerator<int> hazare(0,5);
if(true){

	  eoUniformGenerator<int> choixBinaire(0,2);
	int nbHeures=_genotype.getNbHeures();
	int nbUnits= _genotype.getNbUnits();
	int h,e,j;
//      bool isModified(true);
    // START code for mutation of the _genotype object
	//choix état à muter :
	eoUniformGenerator<int> choixHeure(1,nbHeures);
	h=choixHeure();
	
	vector< vector<int> > temps= _genotype.getTemps();
	//verifier si mutable
	vector<int> allumable;
	vector<int> eteignable;
	bool mut=false;
	int dep=0;
	for(j=0;j<nbUnits;j++)
	{
//		bool mut=false;

		if(temps[h][j]==tUp[j]||temps[h][j]==1)
		{
			if(temps[h-1][j]==tUp[j]||temps[h][j]==1)//si tUp est mutable
			{mut=true;
			eteignable.push_back(j);}
			else
			{
				if(h+1<nbHeures){dep=1;
				e=j;}
			}

		}
		else if ((temps[h][j]<=tDown[j]&&temps[h-1][j]<=tDown[j])||temps[h][j]==-1)
		{
			mut=true;
			allumable.push_back(j);
		}
		else
		{
			if(temps[h][j]>0)
			{
				if((tUp[j]-temps[h][j]+1>dep && dep>0)||(tUp[j]-temps[h][j]+1>-dep && -dep>0)|| dep==0)
				{
					if(h+(tUp[j]-temps[h][j])+1<nbHeures){
					dep=(tUp[j]-temps[h][j]+1 );
					e=j;}
				}
				if((temps[h][j]-1<dep && dep>0) || (temps[h][j]-1<-dep && dep<0))//se mefier
				{
					if(h-temps[h][j]+1>0)
					{
						dep=-temps[h][j]-1;
						e=j;
					}
				}
			}
			else
			{
				if((tDown[j]-temps[h][j]-1>-dep && dep>0)||(tDown[j]-temps[h][j]-1>dep && dep<0)|| dep==0)
				{
					if(h-(tDown[j]-temps[h][j])+1<nbHeures)
					{
					dep=-(tDown[j]-temps[h][j])+1;
					e=j;}
				}
				if((temps[h][j]+1>dep && dep<0)||(temps[h][j]+1>-dep && dep>0))//se mefier
				{
					if(h+temps[h][j]+1>0)
					{
						dep=temps[h][j]+1;
						e=j;
					}
				}
			}
		}
	}
	//sinon changer :

	if(mut==false){ h=h+dep;
//cout<<h<<" on ajoute "<<dep<<endl;
}
	else //choix de e :
	{
		eoUniformGenerator<int> choixEteignage(0,eteignable.size());
		eoUniformGenerator<int> choixAlumage(0,allumable.size());
		
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
		
/*		if((*load)[h]+(*reserve)[h]<sumPmin && eteignable.size()>0)
		{
//cout<<"ici"<<endl;
			int choix= choixEteignage();
			//cout<<choix<<endl;
			e=eteignable[choix];
		}
		else{
			if ((*load)[h]+(*reserve)[h]>sumPmax && allumable.size()>0 )
			{
//cout<<"là"<<endl;
				int choix=choixAlumage();
				e=allumable[choix];
			}
			else
			{*/
			//cout<<"da"<<endl;
				int choixB=choixBinaire();
				if(choixB && eteignable.size()==0) choixB=0;
				if(choixB==0 &&  allumable.size()==0) choixB=1;
				//cout<<choixB<<endl;
				if(choixB)
				{
				//cout<<"eteindre"<<endl;
					int choix= choixEteignage();
					e=eteignable[choix];
				}
				else
				{
				//cout<<"alumer"<<endl;
					int choix=choixAlumage();
					e=allumable[choix];
				}
		//	}
//		}
	}
	//if(temps[h][e]==tDown[e]&&temps[h-1][e]>tDown[e]&&tDown[e]<-1) cout<<"pb dans mut "<<h<<" "<<e<<endl;
	//Tout est choisi, mutons :
	//cout<<"On mute l'usine "<<e<<" à l'heure "<<h<<endl;
//	_genotype.printOn(cout);
// Cas -1=> tUp :
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
			//	if(temps[h+j][e]<0)
			//	{
					_genotype.setModif(h+j,true);
			//	}
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

		}
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
//verification genome
		/*for( h=1;h<nbHeures;h++)
		{
			int pred=_genotype.getTemps(h-1,e);
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

}}
      
    return isModified;
	
    // END code for mutation of the _genotype object
  }
  


	
};

#endif
