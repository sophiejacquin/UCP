#include<fstream>
#include<fstream>
#include<vector>
using namespace std;
#include "eoUCPInit.h"
#include "eoUCPUnit.h"
#include "eoUCPData.h"
#include "eoUCPParser.h"
#include "eoUCP.h"
#include <sstream>
#include "truncated_normal.cpp"
#define NBS 20
#define DEV 1
#define PPROD 20
typedef eoMinimizingFitness MyFitT ;

typedef eoUCP<MyFitT> Indi; 
vector<double> TransformePmax(vector<eoUCPUnit> * units, vector<double> * reserve)
{
	vector<double> pmax;
	// il faut somme Pmax > reserve	
	double sum=0;
	for(int i=0; i< units->size(); i++)
	{
		eoUniformGenerator<int> choixSeed(0,345629897);
		int seed =choixSeed();
		double r=(*units)[i].get_Pmax()-truncated_normal_ab_sample( 0, 1, -1, 1,seed  )*(*units)[i].get_Pmax()*PPROD/100;
		pmax.push_back(r);
		sum+=pmax[i];
		
	}
	for(int t=0; t<reserve->size();t++)
	{
		if (sum<(*reserve)[t])
		{
			eoUniformGenerator<int> choixUnit(0,units->size());
			int i=choixUnit();
			pmax[i]+=(*reserve)[t]-sum;
			sum=(*reserve)[t];
		}
	}
	return pmax;
}
vector<double> TransformePmin(vector<eoUCPUnit> * units, vector<double> * Pmax, vector<double> * reserve)
{
	
}
vector<double> TransformeLoad(vector<double> * load, vector<double> * Pmax,vector<double> * reserve,vector<eoUCPUnit> * units)
{
	vector<double> newLoads;
	for(int t=0; t<reserve->size();t++)
	{
		double borneSup= -(*reserve)[t];
		double borneInf=0;
		vector<int> index;
		for(int i=0;i<units->size();i++)
			index.push_back(i);
		while((borneInf>borneSup)|| ((*load)[t]> borneSup)&&index.size()>0)
		{
			eoUniformGenerator<int> choixUnit(0,index.size());
			int i =choixUnit();
			borneInf+=(*units)[index[i]].get_Pmin();
			borneSup+=(*Pmax)[index[i]];
			index.erase (index.begin()+i);
		}
		if((borneInf>borneSup)|| ((*load)[t]> borneSup)) cout<< borneInf<<" "<<borneSup<<" load : "<<(*load)[t]<<endl;
		eoUniformGenerator<int> choixSeed(0,345629897);
		int seed =choixSeed();
		
		newLoads.push_back(truncated_normal_ab_sample((*load)[t] , 0.1*(*load)[t], borneInf, borneSup,seed));
		
	}
	return newLoads;
}
main(){
	
    eoUCPParser p("/home/jacquin/Bureau/UCP-Stochastique/input/probleme10.tsp");
	eoUCPInit<Indi> init;

    eoUCPData data;
   	  
    p(init,data);
	vector<eoUCPUnit>  * unitsBase=data.get_units();
	std::vector<double>  *  loadBase=data.get_load();
    	vector<double> * reserve =data.get_reserve();
	for( int i=0;i<NBS;i++)
	{
		ostringstream os;
		os<<i;
		string nom="/home/jacquin/Bureau/UCP-Stochastique/input/Scenarios_10/scenario_"+os.str();
		ofstream fichier(nom.c_str(),ios::out| ios:: trunc);
		if(fichier)
		{
			fichier<<"--unites="<<unitsBase->size()<<"\n";
			fichier<<"--nbHeures="<<loadBase->size()<<"\n\n#Unites \n";
			vector<double> Pmax= TransformePmax(unitsBase,reserve);
			cout<<"ap Pmax"<<endl;
			vector<double> load=TransformeLoad(loadBase, &Pmax,reserve,unitsBase);
			cout<<"apLoad"<<endl;
			for(int j=0;j<unitsBase->size();j++)
			{
				fichier<<"@"<<j+1<<", "<<Pmax[j]<<", "<<(*unitsBase)[j].get_Pmin()<<", "<<(*unitsBase)[j].get_a0()<<", "<<(*unitsBase)[j].get_a1()<<", "<<(*unitsBase)[j].get_a2()<<", "<<(*unitsBase)[j].get_tUp()<<", "<<(*unitsBase)[j].get_tDown()<<", "<<(*unitsBase)[j].get_sHot()<<", "<<(*unitsBase)[j].get_sCold()<<", "<<(*unitsBase)[j].get_tColdStart()<<", "<<(*unitsBase)[j].get_initialState()<<"\n";
			}
			fichier<<"\n#Load\n %l ";
			for(int t=0;t<load.size()-1;t++)
			fichier<<load[t]<<", ";
			fichier<<load[load.size()-1]<<"\n\n#Reserve\n%r";
			for(int t=0;t<load.size()-1;t++)
			fichier<<(*reserve)[t]<<", ";
			fichier<<(*reserve)[load.size()-1]<<"\n";
			fichier.close();

		}
		else
			cerr<<"erreur à l'ouverture !!"<<endl;
	}
	

}


