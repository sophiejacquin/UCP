#include "eo"
#include <iostream>
using namespace std;
#include "utils/eoRealVectorBounds.h"
#include "eoUCP.h"
#include "eoUCPData.h"
#include "eoUCPUnit.h"
#include "eoUCPInit.h"
#include "eoUCPParser.h"
#include "eoUCPMutation.h"
//#include "eoUCPMutation5.h"
#include "eoUCPEvalFunc.h"
#include "eoUCPQuadCrossover.h"
#include "eoUCPSecondQuadCrossover.h"
typedef eoMinimizingFitness MyFitT ;	// type of fitness
// END fitness type
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Then define your EO objects using that fitness type
typedef eoUCP<MyFitT> Indi; 
main(){
	//eoParser parser(argc, argv);  // for user-parameter reading

    //std::string inputFile = parser.getORcreateParam(std::string(" "), "inputFile", "File which describes input information", 'I', "Input " ).value();

    eoUCPParser p("/home/jacquin/Bureau/UCP/input/probleme4.tsp");
	eoUCPInit<Indi> init;

    eoUCPData data;
   	  
    p(init,data);
	vector<int> tUp;
	vector<int> tColdStart;
	vector<int> tDown;
	int nbUnits= data.units_size();
	int i;
	for(i=0;i<nbUnits;i++)
	{
		tUp.push_back(data.get_unite(i).get_tUp());
		tDown.push_back(-data.get_unite(i).get_tDown());
		tColdStart.push_back(-data.get_unite(i).get_tColdStart());
	}
	//declaration des operateurs :
	init.set(tUp,tDown,tColdStart);
	eoUCPEvalFunc<Indi> eval(& data);
	 eoUCPQuadCrossover<Indi> cross1;
	eoUCPSecondQuadCrossover<Indi> cross2(tUp, tDown,tColdStart,&data);
	//test mutation :
	Indi genotype1;
	init(genotype1);
	eval(genotype1);
	genotype1.printOn(cout,&data);
	//eoUCPMutation5<Indi> mut(tUp, tDown,tColdStart,&data,7,4);
	//mut(genotype1);
	genotype1.invalidate();
	eval(genotype1);
	cout<<"APRES MUTATION :"<<endl;
	genotype1.printOn(cout,&data);
	cout<<"là"<<endl;
	//test croisement 1:
	/*Indi genotype2;
	init(genotype2);
	eval(genotype2);
	cout<<"Genotype 2:"<<endl;
	genotype2.printOn(cout);
	cross2(genotype1,genotype2);
	genotype2.invalidate();
	cout<<"APRES CROSSOVER"<<endl;
	eval(genotype2);
	genotype2.printOn(cout);*/
}
