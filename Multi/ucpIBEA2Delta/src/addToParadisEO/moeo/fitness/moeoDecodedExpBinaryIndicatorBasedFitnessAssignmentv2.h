/*
* <moeoDecodedExpBinaryIndicatorBasedFitnessAssignment.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
*
* Igor Machado, Sophie Jacquin, Arnaud Liefooghe
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

#ifndef MOEO_DECODED_EXPBINARYINDICATORBASEDFITNESSASSIGNMENT_H_
#define MOEO_DECODED_EXPBINARYINDICATORBASEDFITNESSASSIGNMENT_H_

#include <math.h>
#include <vector>
#include <eoPop.h>
#include <fitness/moeoBinaryIndicatorBasedFitnessAssignment.h>
#include <metric/moeoNormalizedSolutionVsSolutionBinaryMetric.h>
#include <utils/moeoConvertPopToObjectiveVectors.h>

/**
 * Fitness assignment sheme based on an indicator proposed in:
 * E. Zitzler, S. Künzli, "Indicator-Based Selection in Multiobjective Search", Proc. 8th International Conference on Parallel Problem Solving from Nature (PPSN VIII), pp. 832-842, Birmingham, UK (2004).
 * This strategy is, for instance, used in IBEA.
 */
template < class MOEOT > class moeoDecodedExpBinaryIndicatorBasedFitnessAssignment : public moeoBinaryIndicatorBasedFitnessAssignment < MOEOT >
  {
  public:

    /** The type of objective vector */
    typedef typename MOEOT::ObjectiveVector ObjectiveVector;


    /**
     * Ctor.
     * @param _metric the quality indicator
     * @param _kappa the scaling factor
     */
    moeoDecodedExpBinaryIndicatorBasedFitnessAssignment(moeoNormalizedSolutionVsSolutionBinaryMetric < ObjectiveVector, double > & _metric, const double _kappa = 0.05) : metric(_metric), kappa(_kappa)
    {}
     


    /**
     * Sets the fitness values for every solution contained in the population _pop
     * @param _pop the population
     */
    void operator()(eoPop < MOEOT > & ps)
    {
        //cout<<"deb fit ass "<<ps[0].children().size()<<endl;
      // 1 - setting of the bounds
        values.resize(ps.size());
       setup(ps);
      // 2 - computing every indicator values

      setFitnesses(ps);
        //cout<<"fin fitness Assignement"<<endl; 
    }


    /**
     * Updates the fitness values of the whole population _pop by taking the deletion of the objective vector _objVec into account.
     * @param _pop the population
     * @param _objVec the objective vector
     */
    void updateByDeletingV(eoPop < MOEOT > & _pop, vector<ObjectiveVector> & _vobjVec,int idx)
    {
        cout<<"deb update by deleting"<<endl;
        for(int i=0;i<values[idx].size();i++)
            values[idx][i]=values[_pop.size()-1][i];
        for(int i=0;i<_pop.size();i++)
        {
            double max=-9999999999999999;
            for(unsigned int j=0;j<_pop[i].children().size();j++)
            {
                double fit=values[i][j];
                for(unsigned int k=0;k<_vobjVec.size();k++)
                {
                    fit+=exp(-metric(_vobjVec[k],_pop[i].children()[j])/kappa);
                    
                    
                }
                if(max<fit){
                    max=fit;
                    
                }
                values[i][j]=fit;
            }
            _pop[i].fitness(max);
        }
       // cout<<"fin update by deleting"<<endl;
            
        
    }


    /**
     * Updates the fitness values of the whole population _pop by taking the adding of the objective vector _objVec into account
     * and returns the fitness value of _objVec.
     * @param _pop the population
     * @param _objVec the objective vector
     */
      double updateByAdding(eoPop < MOEOT > & _pop, ObjectiveVector & _objVec)
      {
          cout<<"not defined!! B)"<<endl;
      }
      void updateByDeleting(eoPop < MOEOT > & _pop, ObjectiveVector & _objVec)
      {
          cout<<"just to loose pureté"<<endl;
      }
      double updateByAddingV(eoPop < MOEOT > & _pop, vector<ObjectiveVector> & _vobjVec)
    {
        for(int i=0;i<_pop.size();i++)
        {
             double max=-999999999999999;
            for(unsigned int j=0;j<_pop[i].children().size();j++)
            {
                double fit=values[i][j];
                for(unsigned int k=0;k<_vobjVec.size();k++)
                {
                    fit-=exp(-metric(_vobjVec[k],_pop[i].children()[j])/kappa);
                    
                             
                }
                if(max<fit){
                   max=fit;
                    
                }
                values[i][j]=fit;
            }
            _pop[i].fitness(max);
            
        }
        //compute result :
        vector<double> v;
        double result=-99999999999999999;
        for(int i=0;i<_vobjVec.size();i++)
        {
            double fit=0;
            for(unsigned int j=0;j<_pop.size();j++)
            {
                for(int k=0;k<_pop[j].children().size();k++)
                {
                    fit-=exp(-metric(_pop[i].children()[j],_vobjVec[i])/kappa);
                }
            }
            _vobjVec.push_back(fit);
            if(result<fit) result=fit;
            
                             
        }
        values.push_back(v);
    
          return result;
    }


  protected:

    /** the quality indicator */
    moeoNormalizedSolutionVsSolutionBinaryMetric < ObjectiveVector, double > & metric;
    /** the scaling factor */
    double kappa;
    /** the computed indicator values */
    std::vector < std::vector<double> > values;


    /**
     * Sets the bounds for every objective using the min and the max value for every objective vector of _pop
     * @param _pop the population
     */
    void setup(const eoPop < MOEOT > & _pop)
    {
        //cout<<"deb setup"<<endl;
        double min,max;
        for (unsigned int i=0; i<ObjectiveVector::Traits::nObjectives(); i++)
        {
            min=_pop[0].children()[0][i];
            max=_pop[0].children()[0][i];
            for(unsigned int j=0;j<_pop.size();j++)
            {
                for(unsigned int k=0;k<_pop[j].children().size();k++)
                {
                    if(min> _pop[j].children()[k][i])
                        min=_pop[j].children()[k][i];
                   if(max< _pop[j].children()[k][i])
                       max=_pop[j].children()[k][i];
                                 
                }
            }
            metric.setup(min,max,i);
            // cout<<"fin setup min:"<<min<<" max : "<<max<<endl;
        }
       
    }


    /**
     * Compute every indicator value in values (values[i] = I(_v[i], _o))
     * @param _pop the population
     */
  


    /**
     * Sets the fitness value of the whple population
     * @param _pop the population
     */
    void setFitnesses(eoPop < MOEOT > & _pop)
    {
        //cout<<"deb setFit"<<endl;
      for (unsigned int i=0; i<_pop.size(); i++)
        {
          _pop[i].fitness(computeFitness(i,_pop));
        }
        //cout<<"fin seFit"<<endl;
    }


    /**
     * Returns the fitness value of the _idx th individual of the population
     * @param _idx the index
     */
    double computeFitness(const unsigned int _idx,eoPop < MOEOT > & _pop)
    {
        //cout<<"deb computeFit idx="<<_idx<<"popp size : "<<_pop.size()<<" "<<_pop[_idx].children().size()<<endl;
        values[_idx].resize(_pop[_idx].children().size());
       // cout<<"resize fait"<<endl;
      double result = 999999999999999999;
        for(unsigned int i=0;i<_pop[_idx].children().size();i++)
        {
            double fit=0;
            for(unsigned int j=0;j<_pop.size();j++)
            {
                if(j!=_idx)
                {
                    for(unsigned k=0; k<_pop[j].children().size();k++)
                    {
                        fit-=exp(-metric(_pop[j].children()[k],_pop[_idx].children()[i])/kappa);
                        //cout<<"metric "<<metric(_pop[j].children()[k],_pop[_idx].children()[i])/kappa<<"  "<<_pop[j].children()[k]<<"  "<<_pop[_idx].children()[i]<<endl;
                       
                    }
                }
            }
            if(result>0||result<fit)
                result=fit;
            values[_idx][i]=fit;
            
            
            
        }
       // cout<<"end compute fit"<<endl;
      //  cout<<"result="<<result<<endl;
      return result;
    }

  };

#endif /*MOEO_DECODED_EXPBINARYINDICATORBASEDFITNESSASSIGNMENT_H_*/
