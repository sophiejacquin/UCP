/*
* <moeoDecodedNSGAII.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2008
* (C) OPAC Team, LIFL, 2002-2008
*
* Igor Machado & Sophie Jacquin
*
* This software is governed by the CeCILL license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".
*
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*
* ParadisEO WebSite : http://paradiseo.gforge.inria.fr
* Contact: paradiseo-help@lists.gforge.inria.fr
*
*/
//-----------------------------------------------------------------------------

#ifndef MOEODECODEDSMS_H_
#define MOEODECODEDSMmS_H_

#include <eoBreed.h>
#include <eoCloneOps.h>
#include <eoContinue.h>
#include <eoEvalFunc.h>
#include <eoGenContinue.h>
#include <eoGeneralBreeder.h>
#include <eoGenOp.h>
#include <eoPopEvalFunc.h>
#include <eoSGAGenOp.h>
#include <algo/moeoEA.h>
#include <diversity/moeoFrontByFrontCrowdingDiversityAssignment.h>
#include <fitness/moeoDominanceDepthFitnessAssignment.h>
#include <replacement/moeoElitistReplacement.h>
#include <selection/moeoDetTournamentSelect.h>
#include <addToParadisEO/moeo/core/moeoDecoder.h>
#include <addToParadisEO/eo/eoTransformePopTailleVariable.h>
#include <addToParadisEO/moeo/fitness/moeoDecodedFitnessAssignment.h>
//#include <addToParadisEO/moeo/diversity/moeoDecodedDiversityAssignment.h> // TODO: make abstract
#include <addToParadisEO/moeo/diversity/moeoDecodedFrontByFrontCrowdingDiversityAssignment.h>
#include <addToParadisEO/moeo/replacement/moeoNoFitnessDiversityAssignmentElitistReplacementSMS.h>
#include <comparator/moeoStrictObjectiveVectorComparator.h>

/**
 * NSGA-II (Non-dominated Sorting Genetic Algorithm II) -- decoder version.
 * Deb, K., S. Agrawal, A. Pratap, and T. Meyarivan. A fast elitist non-dominated sorting genetic algorithm for multi-objective optimization: NSGA-II. IEEE Transactions on Evolutionary Computation, Vol. 6, No 2, pp 182-197 (2002).
 * This class builds the NSGA-II algorithm only by using the fine-grained components of the ParadisEO-MOEO framework.
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedSMS: public moeoEA < SMOEOT >
{
public:

    /**
     * Ctor with a crossover, a mutation and their corresponding rates.
     * @param _maxGen maximum number of generations before stopping
     * @param _eval evaluation function
     * @param _crossover crossover
     * @param _pCross crossover probability
     * @param _mutation mutation
     * @param _pMut mutation probability
     */
    moeoDecodedSMS (unsigned int _maxGen, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut) :
        defaultGenContinuator(_maxGen), decoder(_decoder), continuator(defaultGenContinuator), select (2),
        selectMany(select,0.0), selectTransform(defaultSelect, defaultTransform), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut),transVar(_crossover, _pCross, _mutation, _pMut), genBreed (select, defaultSGAGenOp), breed (genBreed), fitnessAssignment(false, nsgaiiFA)
    {}

    moeoDecodedSMS (eoContinue <XMOEOT> & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut) :
     defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select (2),
    selectMany(select,0.0), selectTransform(defaultSelect, defaultTransform), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut),transVar(_crossover, _pCross, _mutation, _pMut), genBreed (select, defaultSGAGenOp), breed (genBreed), fitnessAssignment(false, nsgaiiFA)
    {}
    
    moeoDecodedSMS (eoContinue <XMOEOT> & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, eoVectOp<SMOEOT> & _mutVec, double _pMutVec) :
    defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select (2),
    selectMany(select,1.0), transVar(_crossover, _pCross, _mutation, _pMut,_mutVec,_pMutVec), selectTransform(selectMany,transVar ), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed(select, defaultSGAGenOp), breed(selectTransform),nsgaiiFA(comp), fitnessAssignment(false, nsgaiiFA)
    {}
  
    
    moeoDecodedSMS (eoContinue <XMOEOT> & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, eoVectOp<SMOEOT> & _mutVec, double _pMutVec,double nbOffspring, bool isRate) :
    defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select (2),
    selectMany(select,nbOffspring,isRate), transVar(_crossover, _pCross, _mutation, _pMut,_mutVec,_pMutVec), selectTransform(selectMany,transVar ), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed(select, defaultSGAGenOp), breed(selectTransform),nsgaiiFA(comp), fitnessAssignment(false, nsgaiiFA)
    {}
    moeoDecodedSMS (unsigned int _maxGen, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, eoVectOp<SMOEOT> & _mutVec, double _pMutVec,double nbOffspring, bool isRate) :
    defaultGenContinuator(_maxGen), continuator(defaultGenContinuator), decoder(_decoder), select (2),
    selectMany(select,nbOffspring,isRate), transVar(_crossover, _pCross, _mutation, _pMut,_mutVec,_pMutVec), selectTransform(selectMany,transVar ), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed(select, defaultSGAGenOp), breed(selectTransform),nsgaiiFA(comp), fitnessAssignment(false, nsgaiiFA)
    {}
    /**
     * Ctor with a eoContinue and a eoGenOp.
     * @param _continuator stopping criteria
     * @param _eval evaluation function
     * @param _op variation operators
     */
    moeoDecodedSMS (eoContinue < XMOEOT > & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoGenOp < SMOEOT > & _op) :
        defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select(2),transVar(defaultQuadOp, 1.0, defaultMonOp, 1.0),
        selectMany(select,0.0), selectTransform(defaultSelect, defaultTransform), defaultSGAGenOp(defaultQuadOp, 1.0, defaultMonOp, 1.0), genBreed(select, _op), breed(genBreed), fitnessAssignment(false,nsgaiiFA)
    {}


    /**
     * Ctor with a eoContinue and a eoTransform.
     * @param _continuator stopping criteria
     * @param _eval evaluation function
     * @param _transform variation operator
     */
    moeoDecodedSMS (eoContinue < XMOEOT > & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoTransform < SMOEOT > & _transform) :
        defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select(2),
        selectMany(select, 1.0), selectTransform(selectMany, _transform), defaultSGAGenOp(defaultQuadOp, 0.0, defaultMonOp, 0.0), genBreed(select, defaultSGAGenOp), breed(selectTransform), fitnessAssignment(nsgaiiFA)
    {}



    typedef typename XMOEOT::ObjectiveVector ObjectiveVector;

    /** update vector of objectives 'vObj' and return if some improvement is made
     */
    bool updateMinObj(std::vector<double>& vObj, eoPop < XMOEOT >& px)
    {
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();

        bool change = false;
        for(unsigned obj=0; obj<nObjectives; ++obj)
            for(unsigned x=0; x<px.size(); ++x)
                
            {
                if(px[x].objectiveVector()[obj] < vObj[obj])
                {
                    change = true;
                    vObj[obj] = px[x].objectiveVector()[obj];
                  
                }
            }
        return change;
    }

    /** update vector of objectives 'vObj'
     */
    void updateMaxObj(std::vector<double>& vObj, eoPop < XMOEOT >& px)
    {

        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
      
        bool change = false;
        for(unsigned obj=0; obj<nObjectives; ++obj)
        {
            double max = 0;
          
            for(unsigned x=0; x<px.size(); ++x)
                if(px[x].objectiveVector()[obj] > max)
                {
                 
                    change = true;
                    max = px[x].objectiveVector()[obj];
                }
      
            vObj[obj] = max;
        }
    }

    /** update vector of objectives 'vObj' and return if some improvement is made
     */
    void calcAvgObj(std::vector<double>& vObj, eoPop < XMOEOT >& px)
    {
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();

        for(unsigned obj=0; obj<nObjectives; ++obj)
        {
            vObj[obj] = 0;
            for(unsigned x=0; x<px.size(); ++x)
                vObj[obj] += px[x].objectiveVector()[obj];
            vObj[obj] = vObj[obj]/px.size();
        }
    }

    /** print values of objectives
     */
    void printObj(string title, std::vector<double>& vObj)
    {
        cout << title << ": [ ";
        for(unsigned i=0; i<vObj.size(); ++i)
            cout << vObj[i] << (i==vObj.size()-1?" ":" , ");
        cout << "]" << endl;
    }


    /**
     * Apply a the algorithm to the population _pop until the stopping criteria is satified.
     * @param _pop the population
     */
    virtual void operator () (eoPop < SMOEOT >& popS)
    {
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        std::vector<double> vObjMin(nObjectives, std::numeric_limits<double>::max());
        std::vector<double> vObjMax(nObjectives, std::numeric_limits<double>::min());
        std::vector<double> vObjAvg(nObjectives, 0.0);

        eoPop < SMOEOT > offspringS;

        eoPop < XMOEOT > popX;
        unsigned sumX = 0;

        for(unsigned s=0; s<popS.size(); s++)
        {
            popS[s].origin(s);

            decoder(popS[s], popX);
            for(unsigned x=sumX; x<popX.size(); x++)
                popX[x].origin(s);

            sumX = popX.size();
        }
  

        unsigned countGen = 0;

       // cout<<"av fitness assignment"<<endl;
        fitnessAssignment(popS, popX);
       // cout<<"apres fitness ass"<<endl;
        diversityAssignment(popS, popX);
        updateMaxObj(vObjMax, popX);
   
        if(updateMinObj(vObjMin, popX))
        {
             cout << "countGen=" << countGen << endl;
            printObj("min", vObjMin);
            calcAvgObj(vObjAvg, popX);
            printObj("avg", vObjAvg);
            printObj("max", vObjMax);
        }
     

        do
        {
            //cout<<"deb do"<<endl;
            countGen++;
            // generate offspring, worths are recalculated if necessary
            breed (popS, offspringS);
            //cout<<"ap breed"<<endl;
            int sumS = popS.size();
            for(unsigned s=0; s<offspringS.size(); s++)
                offspringS[s].origin(sumS + s);

            eoPop < SMOEOT > unionS;
            unionS.reserve (popS.size () + offspringS.size ());
            std::copy (popS.begin (), popS.end (), back_inserter (unionS));
            std::copy (offspringS.begin (), offspringS.end (), back_inserter (unionS));

            unsigned sumX = popX.size();
            cout<<"sms av decodage"<<endl;
            for(unsigned s=0; s<offspringS.size(); s++)
            {
                decoder(offspringS[s], popX);

                for(unsigned x=sumX; x<popX.size(); x++)
                    popX[x].origin(offspringS[s].origin());
                sumX = popX.size();
            }
           // cout<<"ap decodage"<<endl;

            fitnessAssignment(unionS, popX);
            //cout<<"ap fitness"<<endl;
            diversityAssignment(unionS, popX);

            // safety! re-enter values of fitness and diversity
            for(unsigned s=0; s<sumS; s++)
            {
                
                popS[s].fitness(unionS[s].fitness());
                if(popS[s].origin()!= unionS[s].origin())
                    cout<<"*:%****************/*********************ATTENTION PROOOOOOOOOOOOOOBLEME ORIGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**************************************"<<endl;
                //popS[s].diversity(unionS[s].diversity());
            }

            for(unsigned s=sumS; s<unionS.size(); s++)
            {
                //cout<< offspringS[s-sumS].origin()<<"   "<<unionS[s].origin()<<endl;
                if(offspringS[s-sumS].origin()!= unionS[s].origin())
                    cout<<"DEUXIEME *:%****************/*********************ATTENTION PROOOOOOOOOOOOOOBLEME ORIGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**************************************"<<endl;
                offspringS[s-sumS].fitness(unionS[s].fitness());
               offspringS[s-sumS].diversity(unionS[s].diversity());
            }
            updateMaxObj(vObjMax, popX);
            if(updateMinObj(vObjMin, popX))
            {
                cout << "countGen=" << countGen << endl;
                printObj("min", vObjMin);
                calcAvgObj(vObjAvg, popX);
                printObj("avg", vObjAvg);
                printObj("max", vObjMax);
            }
           
           
            replace (popS, offspringS);
            


            for(unsigned x=0; x<popX.size(); x++)
            {
                
                bool ok=false;
                for(unsigned s=0; s<popS.size() && !ok; s++)
                {

                    if(popX[x].origin()==popS[s].origin())
                    {

                        popX[x].origin(s);
                        ok=true;
                        break;
                    }

                }
                if(!ok)
                {
                    popX[x].origin(-1);
                }

            }
            for(unsigned x=0; x<popX.size(); ++x)
            {
                if(popX[x].origin()==-1)
                {
                    popX.erase(popX.begin()+x);
                    --x;
                }
            }

            sumX=popX.size();
            
            //popX.sort();
            for(unsigned s=0; s<popS.size(); s++)
                popS[s].origin(s);
                      // if(popX[sumX-1].objectiveVector()[0]>vObjMin[0] && popX[sumX-1].objectiveVector()[1]>vObjMin[1])
                //exit(1);
          
        }
        while (continuator (popX));
    }


    virtual void operator () (eoPop < SMOEOT >& popS,moeoUnboundedArchive < XMOEOT > & arch)
    {
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        std::vector<double> vObjMin(nObjectives, std::numeric_limits<double>::max());
        std::vector<double> vObjMax(nObjectives, std::numeric_limits<double>::min());
        std::vector<double> vObjAvg(nObjectives, 0.0);
        
        eoPop < SMOEOT > offspringS;
        
        eoPop < XMOEOT > popX;
        unsigned sumX = 0;
        
        for(unsigned s=0; s<popS.size(); s++)
        {
            popS[s].origin(s);
            
            decoder(popS[s], popX);
            for(unsigned x=sumX; x<popX.size(); x++)
                popX[x].origin(s);
            
            sumX = popX.size();
        }
        
        arch(popX);
        unsigned countGen = 0;
        
        // cout<<"av fitness assignment"<<endl;
        fitnessAssignment(popS, popX);
        // cout<<"apres fitness ass"<<endl;
        diversityAssignment(popS, popX);
        updateMaxObj(vObjMax, popX);
        
        if(updateMinObj(vObjMin, popX))
        {
            cout << "countGen=" << countGen << endl;
            printObj("min", vObjMin);
            calcAvgObj(vObjAvg, popX);
            printObj("avg", vObjAvg);
            printObj("max", vObjMax);
        }
        
        
        do
        {
            //cout<<"deb do"<<endl;
            countGen++;
            // generate offspring, worths are recalculated if necessary
            breed (popS, offspringS);
            //cout<<"ap breed"<<endl;
            int sumS = popS.size();
            for(unsigned s=0; s<offspringS.size(); s++)
                offspringS[s].origin(sumS + s);
            
            eoPop < SMOEOT > unionS;
            unionS.reserve (popS.size () + offspringS.size ());
            std::copy (popS.begin (), popS.end (), back_inserter (unionS));
            std::copy (offspringS.begin (), offspringS.end (), back_inserter (unionS));
            
            unsigned sumX = popX.size();
            cout<<"sms av decodage"<<endl;
            for(unsigned s=0; s<offspringS.size(); s++)
            {
                decoder(offspringS[s], popX);
                
                for(unsigned x=sumX; x<popX.size(); x++)
                    popX[x].origin(offspringS[s].origin());
                sumX = popX.size();
            }
            arch(popX);
            // cout<<"ap decodage"<<endl;
            
            fitnessAssignment(unionS, popX);
            //cout<<"ap fitness"<<endl;
            diversityAssignment(unionS, popX);
            
            // safety! re-enter values of fitness and diversity
            for(unsigned s=0; s<sumS; s++)
            {
                
                popS[s].fitness(unionS[s].fitness());
                if(popS[s].origin()!= unionS[s].origin())
                    cout<<"*:%****************/*********************ATTENTION PROOOOOOOOOOOOOOBLEME ORIGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**************************************"<<endl;
                //popS[s].diversity(unionS[s].diversity());
            }
            
            for(unsigned s=sumS; s<unionS.size(); s++)
            {
                //cout<< offspringS[s-sumS].origin()<<"   "<<unionS[s].origin()<<endl;
                if(offspringS[s-sumS].origin()!= unionS[s].origin())
                    cout<<"DEUXIEME *:%****************/*********************ATTENTION PROOOOOOOOOOOOOOBLEME ORIGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**************************************"<<endl;
                offspringS[s-sumS].fitness(unionS[s].fitness());
                offspringS[s-sumS].diversity(unionS[s].diversity());
            }
            updateMaxObj(vObjMax, popX);
            if(updateMinObj(vObjMin, popX))
            {
                cout << "countGen=" << countGen << endl;
                printObj("min", vObjMin);
                calcAvgObj(vObjAvg, popX);
                printObj("avg", vObjAvg);
                printObj("max", vObjMax);
            }
            
            
            replace (popS, offspringS);
            
            
            
            for(unsigned x=0; x<popX.size(); x++)
            {
                
                bool ok=false;
                for(unsigned s=0; s<popS.size() && !ok; s++)
                {
                    
                    if(popX[x].origin()==popS[s].origin())
                    {
                        
                        popX[x].origin(s);
                        ok=true;
                        break;
                    }
                    
                }
                if(!ok)
                {
                    popX[x].origin(-1);
                }
                
            }
            for(unsigned x=0; x<popX.size(); ++x)
            {
                if(popX[x].origin()==-1)
                {
                    popX.erase(popX.begin()+x);
                    --x;
                }
            }
            
            sumX=popX.size();
            
            //popX.sort();
            for(unsigned s=0; s<popS.size(); s++)
                popS[s].origin(s);
            // if(popX[sumX-1].objectiveVector()[0]>vObjMin[0] && popX[sumX-1].objectiveVector()[1]>vObjMin[1])
            //exit(1);
            
        }
        while (continuator (popX));
    }
    

    
protected:

    /** decoder function */
    moeoDecoder< SMOEOT, XMOEOT >& decoder;

    /** a continuator based on the number of generations (used as default) */
    eoGenContinue < XMOEOT > defaultGenContinuator;
    /** stopping criteria */
    eoContinue < XMOEOT > & continuator;

    /** default select */
    class DummySelect : public eoSelect < SMOEOT >
    {
    public :
        void operator()(const eoPop<SMOEOT>&, eoPop<SMOEOT>&) {}
    }
    defaultSelect;
    /** binary tournament selection */
    moeoDetTournamentSelect < SMOEOT > select;
    /** default select many */
    eoSelectMany < SMOEOT >  selectMany;
    /** select transform */
    eoSelectTransform < SMOEOT > selectTransform;
    /** a default crossover */
    eoQuadCloneOp < SMOEOT > defaultQuadOp;
    /** a default mutation */
    eoMonCloneOp < SMOEOT > defaultMonOp;
    /** an object for genetic operators (used as default) */
    eoSGAGenOp < SMOEOT > defaultSGAGenOp;
    /** default transform */
    class DummyTransform : public eoTransform < SMOEOT >
    {
    public :
        void operator()(eoPop<SMOEOT>&) {}
    }
    defaultTransform;
    eoTransformePopTailleVariable<SMOEOT> transVar;
    /** general breeder */
    eoGeneralBreeder < SMOEOT > genBreed;
    /** breeder */
    eoBreed < SMOEOT > & breed;
    /** fitness assignment used in NSGA */
    moeoStrictObjectiveVectorComparator<typename XMOEOT::ObjectiveVector> comp;
    moeoDominanceDepthFitnessAssignment < XMOEOT > nsgaiiFA;
    //moeoDominanceRankFitnessAssignment < XMOEOT > nsgaiiFA;
    /** elitist replacement */
    moeoNoFitnessDiversityAssignmentElitistReplacementSMS < SMOEOT,XMOEOT > replace;
    /** SX fitness assignment */
    moeoDecodedFitnessAssignment< SMOEOT , XMOEOT > fitnessAssignment;
    /** SX diversity assignment */
    moeoDecodedFrontByFrontCrowdingDiversityAssignment  < SMOEOT , XMOEOT > diversityAssignment;

};

#endif /*MOEODECODEDNSGAII_H_*/
