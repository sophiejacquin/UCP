/*
* <moeoDecodedIBEA.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
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

#ifndef MOEO_DECODED_IBEA_H_
#define MOEO_DECODED_IBEA_H_

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
#include <diversity/moeoDummyDiversityAssignment.h>
#include <fitness/moeoExpBinaryIndicatorBasedFitnessAssignment.h>
#include <metric/moeoMetric.h>
#include <replacement/moeoEnvironmentalReplacement.h>
#include <selection/moeoDetTournamentSelect.h>
#include <addToParadisEO/moeo/fitness/moeoDecodedExpBinaryIndicatorBasedFitnessAssignment.h>
#include<addToParadisEO/moeo/replacement/moeoDecodedEnvironmentalReplacement.h>
#include <addToParadisEO/eo/eoTransformePopTailleVariable.h>
#include <addToParadisEO/eo/eoVectOp.h>
/**
 * IBEA (Indicator-Based Evolutionary Algorithm).
 * E. Zitzler, S. Künzli, "Indicator-Based Selection in Multiobjective Search", Proc. 8th International Conference on Parallel Problem Solving from Nature (PPSN VIII), pp. 832-842, Birmingham, UK (2004).
 * This class builds the IBEA algorithm only by using the fine-grained components of the ParadisEO-MOEO framework (WITH A DECODED FUNCTION)
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedIBEA : public moeoEA < SMOEOT >
{
public:

    /** The type of objective vector */
    typedef typename XMOEOT::ObjectiveVector ObjectiveVector;


    /**
      * Ctor with a crossover, a mutation and their corresponding rates.
      * @param _maxGen maximum number of generations before stopping
      * @param _decoder decoding function
      * @param _crossover crossover
      * @param _pCross crossover probability
      * @param _mutation mutation
      * @param _pMut mutation probability
      * @param _metric metric
      * @param _kappa scaling factor kappa
      */
    moeoDecodedIBEA (unsigned int _maxGen, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
            defaultGenContinuator(_maxGen), decoder(_decoder), continuator(defaultGenContinuator), select (2), selectMany(select,0.0), selectTransform(defaultSelect, defaultTransform), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed (select, defaultSGAGenOp), breed (genBreed), fitnessAssignment(_metric, _kappa), replace (fitnessAssignment, diversityAssignment),transVar(_crossover, _pCross, _mutation, _pMut)
    {}


    /**
     * Ctor with a eoContinue and a eoGenOp.
     * @param _continuator stopping criteria
     * @param _eval evaluation function
     * @param _op variation operators
     * @param _metric metric
     * @param _kappa scaling factor kappa
     */
    moeoDecodedIBEA (eoContinue < XMOEOT > & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoGenOp < SMOEOT > & _op, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
            defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select(2),
            selectMany(select,0.0), selectTransform(defaultSelect, defaultTransform), defaultSGAGenOp(defaultQuadOp, 1.0, defaultMonOp, 1.0), genBreed(select, _op), breed(genBreed), fitnessAssignment(_metric, _kappa), replace (fitnessAssignment, diversityAssignment)
    {}
    moeoDecodedIBEA (unsigned int _maxGen, moeoDecoder< SMOEOT, XMOEOT >& _decoder,  eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, eoVectOp<SMOEOT> & _mutVec, double _pMutVec, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
    defaultGenContinuator(_maxGen), continuator(defaultGenContinuator), decoder(_decoder), select(2),
    selectMany(select,0.0),transVar(_crossover, _pCross, _mutation, _pMut,_mutVec,_pMutVec), selectTransform(defaultSelect, transVar), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed(select, defaultSGAGenOp), breed(genBreed), fitnessAssignment(_metric, _kappa), replace (fitnessAssignment, diversityAssignment)
    {}
    
    moeoDecodedIBEA (eoContinue < XMOEOT > & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder,  eoQuadOp < SMOEOT > & _crossover, double _pCross, eoMonOp < SMOEOT > & _mutation, double _pMut, eoVectOp<SMOEOT> & _mutVec, double _pMutVec, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
     defaultGenContinuator(0), continuator(_continuator), decoder(_decoder), select(2),
    selectMany(select,0.0),transVar(_crossover, _pCross, _mutation, _pMut,_mutVec,_pMutVec), selectTransform(defaultSelect, transVar), defaultSGAGenOp(_crossover, _pCross, _mutation, _pMut), genBreed(select, defaultSGAGenOp), breed(genBreed), fitnessAssignment(_metric, _kappa), replace (fitnessAssignment, diversityAssignment)
    {}

    


    /**
     * Ctor with a eoContinue and a eoTransform.
     * @param _continuator stopping criteria
     * @param _eval evaluation function
     * @param _transform variation operator
     * @param _metric metric
     * @param _kappa scaling factor kappa
     */
    moeoDecodedIBEA (eoContinue < XMOEOT > & _continuator, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoTransform < SMOEOT > & _transform, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
            defaultGenContinuator(0), decoder(_decoder), continuator(_continuator),
            select(2),  selectMany(select, 1.0), selectTransform(selectMany, _transform), defaultSGAGenOp(defaultQuadOp, 0.0, defaultMonOp, 0.0), genBreed(select, defaultSGAGenOp), breed(selectTransform), fitnessAssignment(_metric, _kappa), replace(fitnessAssignment, diversityAssignment)
    {}
    moeoDecodedIBEA (unsigned int _maxGen, moeoDecoder< SMOEOT, XMOEOT >& _decoder, eoTransform < SMOEOT > & _transform, moeoVectorVsVectorBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa=0.05) :
    defaultGenContinuator(_maxGen), decoder(_decoder), continuator(defaultGenContinuator),
    select(2),  selectMany(select, 1.0), selectTransform(selectMany, _transform), defaultSGAGenOp(defaultQuadOp, 0.0, defaultMonOp, 0.0), genBreed(select, defaultSGAGenOp), breed(selectTransform), fitnessAssignment(_metric, _kappa), replace(fitnessAssignment, diversityAssignment)
    {}

    
    void printObj(string title, std::vector<double>& vObj)
    {
        cout << title << ": [ ";
        for(unsigned i=0; i<vObj.size(); ++i)
            cout << vObj[i] << (i==vObj.size()-1?" ":" , ");
        cout << "]" << endl;
    }

    
    bool updateMinObj(std::vector<double>& vObj, eoPop < XMOEOT >& px)
    {
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        
        bool change = false;
        for(unsigned obj=0; obj<nObjectives; ++obj)
            for(unsigned x=0; x<px.size(); ++x)
                
            {
                //cout<<px[x].objectiveVector()[obj] <<" "<< vObj[obj]<<endl;
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


    /**
     * Apply the algorithm to the population _pop until the stopping criteria is satified.
     * @param _pop the population
     */
    virtual void operator () (eoPop < SMOEOT > & popS)
    {
        
        cout<<"dans IBEA"<<endl;
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        std::vector<double> vObjMin(nObjectives, std::numeric_limits<double>::max());
        std::vector<double> vObjMax(nObjectives, std::numeric_limits<double>::min());
        std::vector<double> vObjAvg(nObjectives, 0.0);

        eoPop < SMOEOT > offS;

      

        
        
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
        
        updateMaxObj(vObjMax, popX);
        if(updateMinObj(vObjMin, popX))
        {
            cout << "countGen=" << countGen << endl;
            printObj("min", vObjMin);
            calcAvgObj(vObjAvg, popX);
            printObj("avg", vObjAvg);
            printObj("max", vObjMax);
        }

       
        // evaluate fitness and diversity
        fitnessAssignment(popS);
        diversityAssignment(popS);
        do
        {
            countGen++;
            
            int sumS = popS.size();
            // generate offspring, worths are recalculated if necessary
                      breed (popS, offS);
            
              
           
            unsigned sumX = popX.size();
            //cout<<"deb decodage :"<<endl;
            for(unsigned s=0; s<offS.size(); s++)
            {
               // cout<<"deb decoder"<<endl;
                decoder(offS[s], popX);
                //cout<<"fin decoder"<<endl;
                offS[s].origin(sumS+s);
               
                for(unsigned x=sumX; x<popX.size(); x++)
                    popX[x].origin(offS[s].origin());
             
                sumX = popX.size();
                
            }
           // cout<<"fin decodage"<<endl;
  
         
           // if()
           // {
           // cout<<"replace:"<<endl;
             replace (popS, offS);
            //cout<<"fin replace"<<endl;
          
            for(unsigned x=0; x<popX.size(); ++x)
            {
                bool ok=false;
                for(unsigned s=0; s<popS.size(); ++s)
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
            //popX.printOn(cout);
            
            sumX=popX.size();
            
            updateMaxObj(vObjMax, popX);
            if(true)
            {
                updateMinObj(vObjMin, popX);
                cout << "countGen=" << countGen << endl;
                printObj("min", vObjMin);
                calcAvgObj(vObjAvg, popX);
                printObj("avg", vObjAvg);
                printObj("max", vObjMax);
            }
            
            for(unsigned s=0; s<popS.size(); s++)
                popS[s].origin(s);
         
            //cout<<popS[0].children()[0];
        }
        while (continuator (popX));
    }


     void operator () (eoPop < SMOEOT > & popS,moeoUnboundedArchive < IndiX > & arch)
    {
        
        cout<<"dans IBEA"<<endl;
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        std::vector<double> vObjMin(nObjectives, std::numeric_limits<double>::max());
        std::vector<double> vObjMax(nObjectives, std::numeric_limits<double>::min());
        std::vector<double> vObjAvg(nObjectives, 0.0);
        
        eoPop < SMOEOT > offS;
        
        
        
        
        
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
        arch(popX);
        updateMaxObj(vObjMax, popX);
        if(updateMinObj(vObjMin, popX))
        {
            cout << "countGen=" << countGen << endl;
            printObj("min", vObjMin);
            calcAvgObj(vObjAvg, popX);
            printObj("avg", vObjAvg);
            printObj("max", vObjMax);
        }
        
        
        // evaluate fitness and diversity
        fitnessAssignment(popS);
        diversityAssignment(popS);
        do
        {
            countGen++;
            
            int sumS = popS.size();
            // generate offspring, worths are recalculated if necessary
            breed (popS, offS);
            
            
            
            unsigned sumX = popX.size();
            //cout<<"deb decodage :"<<endl;
            for(unsigned s=0; s<offS.size(); s++)
            {
                // cout<<"deb decoder"<<endl;
                decoder(offS[s], popX);
                //cout<<"fin decoder"<<endl;
                offS[s].origin(sumS+s);
                
                for(unsigned x=sumX; x<popX.size(); x++)
                    popX[x].origin(offS[s].origin());
                
                sumX = popX.size();
                
            }
            arch(popX);
            // cout<<"fin decodage"<<endl;
            
            
            // if()
            // {
            // cout<<"replace:"<<endl;
            replace (popS, offS);
            //cout<<"fin replace"<<endl;
            
            for(unsigned x=0; x<popX.size(); ++x)
            {
                bool ok=false;
                for(unsigned s=0; s<popS.size(); ++s)
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
            //popX.printOn(cout);
            
            sumX=popX.size();
            
            updateMaxObj(vObjMax, popX);
            if(true)
            {
                updateMinObj(vObjMin, popX);
                cout << "countGen=" << countGen << endl;
                printObj("min", vObjMin);
                calcAvgObj(vObjAvg, popX);
                printObj("avg", vObjAvg);
                printObj("max", vObjMax);
            }
            
            for(unsigned s=0; s<popS.size(); s++)
                popS[s].origin(s);
            
            //cout<<popS[0].children()[0];
        }
        while (continuator (popX));
    }
    

    
    
protected:

    /** a decoding function */
    moeoDecoder< SMOEOT, XMOEOT >& decoder;

    /** a continuator based on the number of generations (used as default) */
    eoGenContinue < XMOEOT > defaultGenContinuator;
    /** stopping criteria */
    eoContinue < XMOEOT > & continuator;
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
    /** fitness assignment used in IBEA */
    moeoDecodedExpBinaryIndicatorBasedFitnessAssignment < SMOEOT >  fitnessAssignment;
    /** dummy diversity assignment */
    moeoDummyDiversityAssignment < SMOEOT > diversityAssignment;
    /** environmental replacement */
    moeoDecodedEnvironmentalReplacement < SMOEOT > replace;

};

#endif /*MOEO_DECODED_IBEA_H_*/
