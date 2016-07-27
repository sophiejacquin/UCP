#include <iostream>
using namespace std;
#include <stdio.h>
#include <moeo>
#include "eo"
#include<addToParadisEO/moeo/moeoSteadyHyperVolumeContinue.h>
#include "eoUCPData.h"
#include "eoUCPInit.h"
#include <addToParadisEO/eo/eoPropCombineVectOp.h>
#include "eoUCPParser.h"
#include "eoUCPQuadCrossover.h"
#include "eoUCPMutation.h"
#include "eoUCPMutation2.h"
#include "eoUCPMutation3.h"
#include "eoUCPMutation4.h"
#include "eoUCPMutation5.h"
#include "eoUCPSecondQuadCrossover.h"
//#include "eoUCPThirdQuadCrossover.h"
#include"eoUCPHyperMut2.h"
#include"eoUCPHyperMut2Dirigee.h"
#include"eoUCPMutationMO.h"
#include "moeoUCPObjectiveVectorTraits.h"
#include "moeoUCPX.h"
#include "moeoUCPS.h"
#include "moeoUCPEvalFunc.h"
#include "moeoUCPDecoder.h"
typedef moeoUCPS IndiS;
typedef moeoUCPX IndiX;
#include <make_pop.h>
#include<addToParadisEO/moeo/algo/moeoDecodedNSGAII.h>
#include<addToParadisEO/moeo/algo/moeoDecodedIBEA.h>
#include <metric/moeoHyperVolumeDifferenceMetric.h>
#include <metric/moeoHyperVolumeMetric.h>
#include"eoUCPMutation5MO.h"
#include<addToParadisEO/moeoIHDMetric.h>
#include<metric/moeoVecVsVecAdditiveEpsilonBinaryMetric.h>
eoPop<IndiS>&  make_pop(eoParser& _parser, eoState& _state, eoInit<IndiS> & _init)
{
  return do_make_pop(_parser, _state, _init);
}

// checks for help demand, and writes the status file
// and make_help; in libutils
void make_help(eoParser & _parser);


// main
int main (int argc, char *argv[])
{
    
    eoParser parser(argc, argv);  // for user-parameter reading

    /** Lecture des informations d entree a partir du fichier mentionné dans le fichier "fichier.param" **/

    std::string inputFile = parser.getORcreateParam(std::string(" "), "inputFile", "File which describes input information", 'I', "Input " ).value();
     std::string outputFile = parser.getORcreateParam(std::string("nomOutPut"), "outputFile", "File which describes output information", 'U', "Output " ).value();
    const char * c = outputFile.c_str();
     ofstream output(c, ios::out | ios::trunc);

    eoUCPParser p(inputFile);

    eoState state;                // to keep all things allocated*/

    // parameters
    unsigned int MAX_GEN = parser.createParam((unsigned int)(100), "maxGen", "Maximum number of generations",'G',"Param").value();

   
    // objective functions evaluation
    //moeoUCPEval eval;
    //moeoUCPEvalFunc< Indi > plainEval;

    // turn that object into an evaluation counter
   // eoEvalFuncCounter<Indi> eval(plainEval);
    unsigned int n=parser.createParam((unsigned int)10,"nbLambda","number of solution evaluated in a pareto front of the sub problem",'n',"Param").value();
    eoUCPInit<IndiS> init(n);

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
	init.set(tUp,tDown,tColdStart);
    //plainEval.setData(&data);

    // A (first) crossover (possibly use the parser in its Ctor)
   // eoUCPQuadCrossover<IndiS> cross1 /* (eoParser parser) */;
    
    // IF MORE THAN ONE:
    
    // read its relative rate in the combination
	//double cross1Rate = parser.createParam(1.0, "cross1Rate", "Relative rate for crossover 1", 'x', "Variation Operators").value();
    
    // create the combined operator with the first one (rename it cross1 !!!)
    //eoPropCombinedQuadOp<IndiS> cross(cross1, cross1Rate);
    
    // and as many as you want the following way:
    // 1- write the new class by mimicking eoUCPQuadCrossover.h
    // 2- include that file here together with eoUCPQuadCrossover above
    // 3- uncomment and duplicate the following lines:
    //
    eoUCPSecondQuadCrossover<IndiS> cross2(tUp, tDown,tColdStart,&data);
    double cross2Rate = parser.createParam(1.0, "cross2Rate", "Relative rate for crossover 2", 'y', "Variation Operators").value();
     eoPropCombinedQuadOp<IndiS> cross(cross2, cross2Rate);
    
   /* eoUCPThirdQuadCrossover<IndiS> cross3(tUp, tDown,tColdStart,&data);
    double cross3Rate = parser.createParam(1.0, "cross3Rate", "Relative rate for crossover 2", 'v', "Variation Operators").value();
    cross.add(cross3, cross3Rate);*/
    
    // NOTE: if you want some gentle output, the last one shoudl be like
    //  cross.add(cross, crossXXXRate, true);
    
    /////////////// Same thing for MUTATION
    
    // a (first) mutation   (possibly use the parser in its Ctor)
    eoUCPMutation<IndiS> mut1(tUp, tDown,tColdStart,&data) /* (parser) */;
    
    // IF MORE THAN ONE:
    
    // read its relative rate in the combination
    double mut1Rate = parser.createParam(1.0, "mut1Rate", "Relative rate for mutation 1", '1', "Variation Operators").value();
    
    // create the combined operator with the first one (rename it cross1 !!!)
    eoPropCombinedMonOp<IndiS> mut(mut1, mut1Rate);
    
    // and as many as you want the following way:
    // 1- write the new class by mimicking eoUCPMutation.h
    // 2- include that file here together with eoUCPMutation above
    // 3- uncomment and duplicate the following lines:
    //
    int lng = parser.createParam(23, "long", "Longueur periode mutée", 'l', "Variation Operators").value();
    eoUCPMutation2<IndiS> mut2(tUp, tDown,tColdStart,&data,lng+2);
    eoUCPMutationMO<IndiS> mutMO(tUp, tDown,tColdStart,&data,lng+2);
    
    double mutMORate = parser.createParam(0.1, "mutMORate", "Relative rate for mutation 2", '7', "Variation Operators").value();
    eoPropCombinedVectOp<IndiS> vectMut(mutMO,mutMORate);
    
    double mut2Rate = parser.createParam(1.0, "mut2Rate", "Relative rate for mutation 2", '2', "Variation Operators").value();
    mut.add(mut2, mut2Rate);
    int repet = parser.createParam(3,"repet", "nb de repetition de mut2", 'z', "Variation Operators").value();
    eoUCPHyperMut2<IndiS> Hmut2(repet,nbUnits,mut2);
    eoUCPHyperMut2Dirigee<IndiS> Hmut2D(repet,nbUnits,mutMO);
    double mut2HRate = parser.createParam(1.0, "mut2HRate", "Relative rate for hyper mutation 2 scalarizée", '6', "Variation Operators").value();
    double mut2HDRate = parser.createParam(1.0, "mut2HDRate", "Relative rate for hyper mutation 2 dirigee", '8', "Variation Operators").value();
    mut.add(Hmut2, mut2HRate);
    vectMut.add(Hmut2D,mut2HDRate);
    eoUCPMutation3<IndiS> mut3(tUp, tDown,tColdStart,&data);
    double mut3Rate = parser.createParam(1.0, "mut3Rate", "Relative rate for mutation 3", '3', "Variation Operators").value();
    mut.add(mut3, mut3Rate);
    eoUCPMutation4<IndiS> mut4(tUp, tDown,tColdStart,&data);
    double mut4Rate = parser.createParam(1.0, "mut4Rate", "Relative rate for mutation 3", '4', "Variation Operators").value();
    mut.add(mut4, mut4Rate);
    
   int nbU = parser.createParam(8, "nbU", "nb unites modifées par mut5", 'u', "Variation Operators").value();
    eoUCPMutation5<IndiS> mut5(tUp, tDown,tColdStart,&data,lng,nbU);
    double mut5Rate = parser.createParam(1.0, "mut5Rate", "Relative rate for mutation 1", '5', "Variation Operators").value();
     mut.add(mut5, mut5Rate);
    eoUCPMutation5MO<IndiS> mut5MO(tUp, tDown,tColdStart,&data,lng,5);
     double mut5RateMO = parser.createParam(1.0, "mut5RateMO", "Relative rate for mutation 1", '9', "Variation Operators").value();
    vectMut.add(mut5MO,mut5RateMO);
    // NOTE: if you want some gentle output, the last one shoudl be like
    //  mut.add(mut, mutXXXRate, true);
    
    // now encapsulate your crossover(s) and mutation(s) into an eoGeneralOp
    // so you can fully benefit of the existing evolution engines
    
    // First read the individual level parameters
    double pCross = parser.createParam(0.6, "pCross", "Probability of Crossover", 'C', "Variation Operators" ).value();
    // minimum check
    if ( (pCross < 0) || (pCross > 1) )
        throw runtime_error("Invalid pCross");
    
    double pMut = parser.createParam(0.1, "pMut", "Probability of Mutation", 'M', "Variation Operators" ).value();
     double pVectMut = parser.createParam(0.1, "pVectMut", "Probability of Mutation vectorielle", 'V', "Variation Operators" ).value();
    // minimum check
    if ( (pMut < 0) || (pMut > 1) )
        throw runtime_error("Invalid pMut");
    
    // now create the generalOp
    eoSGAGenOp<IndiS> op(cross, pCross, mut, pMut);
    
    // // initialize the population
     eoPop<IndiS>& pop   = make_pop(parser, state, init);

    eoFileMonitor   *myFileMonitor;
 
    //int algo = parser.getORcreateParam(0, "algo", "Choose algorithm 0 = NSGA-II, 1 = IBEA", 'A', "Algorithm").value();

    // printing of the initial population
    /*cout << "Initial Population\n";
    pop.sortedPrintOn(cout);
    cout << endl;*/
    cout<<"creation decoder"<<endl;
	moeoUCPDecoder<IndiS,IndiX> decoder(&data);

    cout << "IBEA setup" << endl;
    moeoIHDMetric< moeoUCPObjectiveVector > metric(false,1.1);
   // moeoDecodedIBEA<IndiS, IndiX> ibea(MAX_GEN, decoder, cross, pCross, mut, pMut, metric); // kappa = 0.05

   
    //calcul point de ref :
    moeoUCPObjectiveVector pointRef(2);
    pointRef[0]=0;
    pointRef[1]=0;
    std::vector<eoUCPUnit>  * unites=data.get_units();
    for(int i=0;i<unites->size();i++)
    {
        
        double Pmax=(*unites)[i].get_Pmax();
        pointRef[0]+=((*unites)[i].get_a0()+(*unites)[i].get_a1()*Pmax+(*unites)[i].get_a2()*Pmax*Pmax)*((data.get_load())->size())+(*unites)[i].get_sCold()*((data.get_load())->size());
        pointRef[1]+=((*unites)[i].get_b0()+(*unites)[i].get_b1()*Pmax+(*unites)[i].get_b2()*Pmax*Pmax)*((data.get_load())->size());
    }
    //moeoHyperVolumeDifferenceMetric<moeoUCPObjectiveVector> metric(true,1.1);
    moeoHyperVolumeMetric<moeoUCPObjectiveVector> hyperVol(false,pointRef);
  //  moeoSteadyHyperVolumeContinue<IndiX> continuator( 0, 100, hyperVol);
    int time_max = parser.getORcreateParam(120, "time_max", "time max", 'T', "Algorithm").value();
    eoTimeContinue<IndiX> continuator(time_max);
   // moeoDecodedNSGAII<IndiS, IndiX> nsgaII(continuator, decoder, cross,pCross,mut, pMut,vectMut,pVectMut); 
   //moeoDecodedNSGAII<IndiS, IndiX> nsgaII(MAX_GEN, decoder, cross,pCross,mut, pMut); 
    //moeoDecodedIBEA <IndiS, IndiX> ibea(MAX_GEN, decoder, cross,pCross,mut, pMut, metric) ;
    moeoDecodedIBEA <IndiS, IndiX> ibea(MAX_GEN, decoder, cross,pCross,mut, pMut,vectMut,pVectMut, metric) ;
     cout<<"avant lancement NSGA2"<<endl;
   // nsgaII(pop);
    moeoUnboundedArchive < IndiX > arch;

    ibea(pop,arch);
    pop.printOn(cout);
   /* if ( algo == 0 ) {
   	 // build NSGA-II
   	 moeoNSGAII < Indi > nsgaII (MAX_GEN, eval, cross, pCross, mut, pMut);
    	// run the algo
    	nsgaII (pop);
    } else {
    	moeoAdditiveEpsilonBinaryMetric<moeoUCPObjectiveVector> indicator;
    	moeoIBEA<Indi> ibea (MAX_GEN, eval, cross, pCross, mut, pMut, indicator);
    	ibea (pop);
    }*/

    // help ?
    make_help(parser);
    eoPop <IndiX> popX;
    decoder.decodePop(pop, popX);
      cout << "Finalpopopoppo Population\n";
    popX.printOn(cout);    
    
    // extract first front of the final population using an moeoArchive (this is the output of nsgaII)
       arch(popX);

    // printing of the final population
    cout << "Final Population\n";
    popX.printOn(cout);
    cout << endl;

    // printing of the final archive
    //std::cout << "Final Archive " << std::endl;
    std::cout << "# ";
    arch.sortedPrintOn(cout);//(output);
    if(arch.size()>1)
    {
    vector<moeoUCPObjectiveVector> res;
    for(int i=0;i<arch.size();i++)
        res.push_back(arch[i].objectiveVector());
    
    double valHyperVol=hyperVol(res);
    if(arch[0].objectiveVector()[0]>pointRef[0] || arch[1].objectiveVector()[1]>pointRef[1] ) valHyperVol=0;
    cout<<" point de ref =("<<pointRef[0]<<","<<pointRef[1]<<")"<<endl;
    cout<<"air de l'hypervolume :"<<-1*valHyperVol<<endl;
    }
    else
       cout<<"air de l'hypervolume :"<<0; 
   // std::cout << std::endl;

    return EXIT_SUCCESS;
}
