#include "eo"
using namespace std;
#include "utils/eoRealVectorBounds.h"
#include "eoUCP.h"
#include "eoUCPData.h"
#include "eoUCPUnit.h"
#include "eoUCPInit.h"
#include "eoUCPParser.h"
#include "eoUCPMutation.h"
//#include "eoUCPEvalFunc.h"
#include <iostream>
#include <string>
#include <fstream>

#define N 2 
typedef eoMinimizingFitness MyFitT ;
typedef eoUCP<MyFitT> Indi; 
main(){
	//eoParser parser(argc, argv);  // for user-parameter reading

    //std::string inputFile = parser.getORcreateParam(std::string(" "), "inputFile", "File which describes input information", 'I', "Input " ).value();

    eoUCPParser p("/home/jacquin/Bureau/UCP/input/probleme10.tsp");
	eoUCPInit<Indi> init;

    eoUCPData data;
   	  
    p(init,data);
	
	// eoUCPEvalFunc<Indi> eval;
	//eval.setData(&data);
	//eval.setGen(1);
	 ofstream fichier("nouvelleDonne.tsp", ios::out | ios::trunc);
	if(fichier)
	{
		fichier<<"--unites="<<10*N<<endl;
		fichier<<"--heures=24"<<endl<<endl<<endl;
		fichier<<"#Unites"<<endl;
		for(int i=0; i<N*10;i++)
		{
			fichier<<"@"<<i+1<<", ";
			fichier<<data.get_unite(i%10).get_Pmax()<<", "<<data.get_unite(i%10).get_Pmin()<<", "<<data.get_unite(i%10).get_a0()<<", "<<data.get_unite(i%10).get_a1()<<", "<<data.get_unite(i%10).get_a2()<<", "<<data.get_unite(i%10).get_tUp()<<", "<<data.get_unite(i%10).get_tDown()<<", "<<data.get_unite(i%10).get_sHot()<<", "<<data.get_unite(i%10).get_sCold()<<", "<<data.get_unite(i%10).get_tColdStart()<<", "<<data.get_unite(i%10).get_initialState()<<endl;
		}
		fichier<<endl<<endl;
		fichier<<"#Load"<<endl;
		fichier<<"%l ";
		for(int i=0;i<23;i++) 
			fichier<<data.get_load(i)*N<<",";
		
		fichier<<data.get_load(23)*N<<endl;
		fichier<<endl<<"#Reserve"<<endl;
		fichier<<"%r ";
		for(int i=0;i<23;i++) 
			fichier<<data.get_load(i)*N*0.1<<",";
		fichier<<data.get_load(23)*N*0.1<<endl;
		fichier.close();
	} 
}
	