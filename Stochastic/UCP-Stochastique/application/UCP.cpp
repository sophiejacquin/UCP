#include <iostream>
using namespace std;
//#define HAVE_GNUPLOT
#include "eo"
#include"moeo"
#include "UCPObjectiveVectorTraits.h"
#include "eoUCP.h"
#include "eoUCPData.h"
#include "eoUCPInit.h"
#include"addToParadisEO/eo/eoPropCombineVectOp.h"
#include "eoUCPStochasticEvalFunc.h"
#include"addToParadisEO/eo/eoTransformePopTailleVariable.h"
#include "moeoUCPStochasticEvalFunc.h"
#include "eoUCPSecondQuadCrossover.h"
#include "eoUCPMutation.h"
#include "eoUCPParser.h"
#include "eoUCPMutation3.h"
#include "eoUCPMutation2.h"
#include "eoUCPMutation5MO.h"
typedef eoMinimizingFitness MyFitT ;

typedef eoUCP<MyFitT> Indi; 

#include <make_pop.h>
eoPop<Indi >&  make_pop(eoParser& _parser, eoState& _state, eoInit<Indi> & _init)
{
  return do_make_pop(_parser, _state, _init);
}

// the stopping criterion
#include <make_continue.h>
eoContinue<Indi>& make_continue(eoParser& _parser, eoState& _state, eoEvalFuncCounter<Indi> & _eval)
{
  return do_make_continue(_parser, _state, _eval);
}

// outputs (stats, population dumps, ...)
#include <make_checkpoint.h>
eoCheckPoint<Indi>& make_checkpoint(eoParser& _parser, eoState& _state, eoEvalFuncCounter<Indi>& _eval, eoContinue<Indi>& _continue)
{
  return do_make_checkpoint(_parser, _state, _eval, _continue);
}

// evolution engine (selection and replacement)
#include <make_algo_scalar.h>
eoAlgo<Indi>&  make_algo_scalar(eoParser& _parser, eoState& _state, eoEvalFunc<Indi>& _eval, eoContinue<Indi>& _continue, eoGenOp<Indi>& _op, eoDistance<Indi> *_dist = NULL)
{
  return do_make_algo_scalar(_parser, _state, _eval, _continue, _op, _dist);
}

// simple call to the algo. stays there for consistency reasons
// no template for that one
#include <make_run.h>
#include <eoScalarFitness.h>
void run_ea(eoAlgo<Indi>& _ga, eoPop<Indi>& _pop)
{
  do_run(_ga, _pop);
}

// checks for help demand, and writes the status file
// and make_help; in libutils
void make_help(eoParser & _parser);

int main(int argc, char** argv){
 
  try{
 std::cout << "# deb try"<<endl;
    eoParser parser(argc, argv);  // for user-parameter reading
    /** Lecture des informations d entree a partir du fichier mentionné dans le fichier "fichier.param" **/
    std::string inputFile = parser.getORcreateParam(std::string("/home/jacquin/Bureau/UCP-Stochastique/input/probleme10.tsp"), "inputFile", "File which describes input information", 'I', "Input " ).value();
    eoState state;    // keeps all things allocated
    eoUCPParser p(inputFile);


    
     

    // turn that object into an evaluation counter
   

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
	init.set(tUp,tDown,tColdStart);
    /*
    *
    * STOCHASTIC PROBLEM STARTS HERE.
    *
    */
    int number_of_scenarios = parser.getORcreateParam(0, "scenariosNum", "Count of scenarios", 'N', "Variation Operators" ).value();
    std::vector<eoUCPData> scenarios;
    scenarios.push_back(data);
    std::ostringstream o;
    if(number_of_scenarios>0){
          eoUCPInit<Indi> init2;
          eoUCPData data2;

      std::string scenarioFolder = parser.getORcreateParam(std::string("/home/jacquin/Bureau/UCP-Stochastique/input/scenarios"), "scenariosFolder", "Folder containing ", 'I', "Input " ).value();
      for(int i=1;i<=number_of_scenarios*3;i++){
        o.str("");
        o  <<scenarioFolder<<"/scenario_"<< i <<".tsp";
        eoUCPParser stochastic_parser(o.str());
        stochastic_parser(init2, data2);
        scenarios.push_back(data2);
      }
    }else{
      exit(EXIT_FAILURE);
    }
// The fitness
    //////////////
	moeoUCPEvalFunc< Indi > plainEval(scenarios);
	 eoEvalFuncCounter<Indi> eval(plainEval);
    //plainEval.setData(scenarios);


    // A (first) crossover (possibly use the parser in its Ctor)
    eoUCPSecondQuadCrossover<Indi> cross1(tUp, tDown,tColdStart);
    eoUCPMutation<Indi> mut1(tUp, tDown,tColdStart,&data);
    eoUCPMutation3<Indi> mut2(tUp, tDown,tColdStart,scenarios);
    int longueur = parser.getORcreateParam(24, "longueur", "longueur hybridation", 'l', "Variation Operators" ).value();
    eoUCPMutation2<Indi> mut3(tUp, tDown,tColdStart,scenarios,longueur);
     eoUCPMutation5MO<Indi> mut5MO (tUp, tDown, tColdStart,23,5,scenarios) ; 
    double pCross = parser.getORcreateParam(0.6, "pCross", "Probability of Crossover", 'C', "Variation Operators" ).value();
	double pVec = parser.getORcreateParam(1, "pVec", "Relative Probability of Mutation vectorielle 1", 'v', "Variation Operators" ).value();
   
    // minimum check
    if ( (pCross < 0) || (pCross > 1) )
      throw runtime_error("Invalid pCross");

    double pMut = parser.getORcreateParam(0.1, "pMut", "Probability of Mutation", 'M', "Variation Operators" ).value();
    double pMut1 = parser.getORcreateParam(1, "pMut1", "Relative Probability of Mutation 1", '1', "Variation Operators" ).value();
    double pMut2 = parser.getORcreateParam(1, "pMut2", "Relative Probability of Mutation 1", '2', "Variation Operators" ).value();
    double pMut3 = parser.getORcreateParam(1, "pMut3", "Relative Probability of Mutation 1", '3', "Variation Operators" ).value();
	double pMut5 = parser.getORcreateParam(1, "pMut5", "Relative Probability of Mutation 1", '5', "Variation Operators" ).value();
    // minimum check
    if ( (pMut < 0) || (pMut > 1) )
      throw runtime_error("Invalid pMut");

    eoPropCombinedQuadOp<Indi> cross(cross1,pCross);


    eoPropCombinedMonOp<Indi> mut(mut1, pMut1);
    mut.add(mut2,pMut2);
   eoPropCombinedVectOp<Indi> vec(mut3,pMut3);
		vec.add(mut5MO,pMut5);  
    eoSGAGenOp<Indi> op(cross, pCross, mut, pMut);
	cout<<"ici av init pop"<<endl;
    // // initialize the population
     eoPop<Indi>& pop   = make_pop(parser, state, init);
	cout<<"apres "<<endl;
     // stopping criteria
     eoContinue<Indi> & term = make_continue(parser, state, eval);
     // output
    eoCheckPoint<Indi> & checkpoint = make_checkpoint(parser, state, eval, term);

    eoIncrementorParam<unsigned> generationCounter("Gen.");

//    plainEval.setGen(& generationCounter);

    checkpoint.add(generationCounter);
    // need to get the name of the redDir param (if any)
    std::string dirName =  parser.getORcreateParam(std::string("Res"), "resDir", "Directory to store DISK outputs", '\0', "Output - Disk").value() + "/";

    // those need to be pointers because of the if's
    eoStdoutMonitor *myStdOutMonitor;
    eoFileMonitor   *myFileMonitor;
#ifdef HAVE_GNUPLOT
    eoGnuplot1DMonitor *myGnuMonitor;
#endif

    unsigned int MAX_GEN = parser.createParam((unsigned int)(100), "maxGen", "Maximum number of generations",'G',"Param").value();
	eoTransformePopTailleVariable<Indi> trans(cross, pCross, mut, pMut, vec, pVec);
    // should we write it on StdOut ?
 

   int algo = parser.getORcreateParam(0, "algo", "Choose algorithm 0 = NSGA-II, 1 = IBEA", 'A', "Algorithm").value();

    // printing of the initial population
    /*cout << "Initial Population\n";
    pop.sortedPrintOn(cout);
    cout << endl;*/
	eoGenContinue<Indi> continuator(MAX_GEN);
 std::cout << "# av algo"<<endl;
    if ( algo == 0 ) {
   	 // build NSGA-II
   	 //moeoNSGAII < Indi > nsgaII (MAX_GEN, eval, cross, pCross, mut, pMut);
	
	moeoNSGAII < Indi > nsgaII (continuator, eval,trans);
    	// run the algo
    	nsgaII (pop);
    } else {
    	moeoAdditiveEpsilonBinaryMetric<UCPObjectiveVector> indicator;
    	//moeoIBEA<Indi> ibea (MAX_GEN, eval, cross, pCross, mut, pMut, indicator);
	moeoIBEA<Indi> ibea(continuator, eval,trans,indicator);
    	ibea (pop);
    }
 
    make_help(parser);

   moeoUnboundedArchive < Indi > arch;
    arch(pop);

    // printing of the final population
    /*cout << "Final Population\n";
    pop.sortedPrintOn(cout);
    cout << endl;*/

    // printing of the final archive
    //std::cout << "Final Archive " << std::endl;
    std::cout << "# FINI";
    arch.sortedPrintOn(std::cout);
    std::cout << std::endl;
  }
  catch(exception& e)
    {
      cout << e.what() << endl;
    }
       
  return 0;
}
