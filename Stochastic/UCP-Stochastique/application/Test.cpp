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
#include "eoUCPEvalFunc.h"
#include <fstream>  
typedef eoMinimizingFitness MyFitT ;
typedef eoUCP<MyFitT> Indi; 
main(){
	//eoParser parser(argc, argv);  // for user-parameter reading

    //std::string inputFile = parser.getORcreateParam(std::string(" "), "inputFile", "File which describes input information", 'I', "Input " ).value();

    eoUCPParser p("/home/jacquin/Bureau/UCP/input/probleme100bis.tsp");
	eoUCPInit<Indi> init;

    eoUCPData data;
   	  
    p(init,data);
	cout<<"là"<<endl;
	 eoUCPEvalFunc<Indi> eval;
	eval.setData(&data);
	//eval.setGen(1);
	Indi genotype1;
	genotype1.set(100,24);
	  std::filebuf fb;
	fb.open ("testM100",std::ios::in);
	std::istream is(&fb);
	cout<<"ici"<<endl;
	genotype1.readFrom(is);
	cout<<"lu"<<endl;
	genotype1.invalidate();
	genotype1.printOn(cout);
	cout<<"on evalu"<<endl;
	eval(genotype1);
	cout<<"évalué"<<endl;
	genotype1.printOn(cout);
	vector<double> prod=genotype1.get_production();
	vector<bool> mbits=*(genotype1.get_p_mbits());
	double nbHeures=(data.get_load())->size();
	double prix=0;
	for(int i=0;i<data.units_size();i++)
	{
		double a0=(*(data.get_units()))[i].get_a0();
		double a1=(*(data.get_units()))[i].get_a1();
		double a2=(*(data.get_units()))[i].get_a2();
		for(int j=0;j<(data.get_load())->size();j++)
		{
			double p=prod[j+i*nbHeures];
			if(mbits[j+i*nbHeures])prix+=a2*p*p+a1*p+a0;
		}

		
	}
	cout<<prix<<endl;
}
	