/*
* <moeoHyperVolumeDifferenceMetric.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2007
* (C) OPAC Team, LIFL, 2002-2007
*
* Jeremie Humeau
* Arnaud Liefooghe
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

#ifndef MOEOIHD_H_
#define MOEOIHD_H_

#include <metric/moeoMetric.h>
#include <metric/moeoHyperVolumeDifferenceMetric.h>
#include<iostream>

/**
 * The contribution metric evaluates the proportion of non-dominated solutions given by a Pareto set relatively to another Pareto set
 * (Meunier, Talbi, Reininger: 'A multiobjective genetic algorithm for radio network optimization', in Proc. of the 2000 Congress on Evolutionary Computation, IEEE Press, pp. 317-324)
 */
template < class ObjectiveVector >
class moeoIHDMetric : public moeoVectorVsVectorBinaryMetric < ObjectiveVector, double >
  {
	public:

    /**
     * Constructor with a coefficient (rho)
     * @param _normalize allow to normalize data (default true)
     * @param _rho coefficient to determine the reference point.
     */
    moeoIHDMetric(bool _normalize=true, double _rho=1.1): normalize(_normalize), rho(_rho), ref_point(NULL){
       
    }

    /**
     * Constructor with a reference point
     * @param _normalize allow to normalize data (default true)
     * @param _ref_point the reference point
     */
    moeoIHDMetric(bool _normalize=true, ObjectiveVector& _ref_point=NULL): normalize(_normalize), rho(0.0), ref_point(_ref_point){
      
    }

    /**
     * calculates and returns the HyperVolume value of a pareto front
     * @param _set1 the vector contains all objective Vector of the first pareto front
     * @param _set2 the vector contains all objective Vector of the second pareto front
     */
    double operator()(const std::vector < ObjectiveVector > & _set1, const std::vector < ObjectiveVector > & _set2)
    {
        double result;
        bool set2DomineSet1=true;
        int i=0;
        int tailleSet1=_set1.size();
        int tailleSet2=_set2.size();
        unsigned int nObjectives = ObjectiveVector::nObjectives();
        moeoHyperVolumeDifferenceMetric<  ObjectiveVector > metric(normalize,rho);
     
        while(set2DomineSet1 && i<tailleSet1)
        {
            int j=0;
            bool domine=false;
            while( !domine && j<tailleSet2)
            {
                int k=0;
                bool  dominateur=true;
                while(dominateur && k<nObjectives)
                {
                    if(ObjectiveVector::Traits::minimizing(k))
                    {
                        if(_set1[i][k]<_set2[j][k])
                            dominateur=false;
                        
                    }
                    else
                    {
                        if(_set1[i][k]>_set2[j][k])
                            dominateur=false;

                    }
                    k++;
                }
                if(dominateur)
                    domine=true;
                j++;
                
            }
            if(!domine)
                set2DomineSet1=false;
            i++;
        }
        
        if(set2DomineSet1)
        {
            
            result=metric(_set2,_set1);
           
        }
        else
        {
            //check if set1 domine set2 :
            bool set1DomineSet2 = true;
            i=0;
            while(set1DomineSet2 && i<tailleSet2)
            {
                int j=0;
                bool domine=false;
                while( !domine && j<tailleSet1)
                {
                    int k=0;
                    bool  dominateur=true;
                    while(dominateur && k<nObjectives)
                    {
                        if(ObjectiveVector::Traits::minimizing(k))
                        {
                            if(_set2[i][k]<_set1[j][k])
                                dominateur=false;
                            
                        }
                        else
                        {
                            if(_set2[i][k]>_set1[j][k])
                                dominateur=false;
                            
                        }
                        k++;
                    }
                    if(dominateur)
                        domine=true;
                    j++;
                    
                }
                if(!domine)
                    set1DomineSet2=false;
                i++;
            }

            if(set1DomineSet2)
            {
                result= -metric(_set1,_set2);
            }
            else
            {
                vector < ObjectiveVector > unionSet;
                unionSet.reserve(_set2.size()+_set1.size());
                unionSet.insert(unionSet.end(),_set1.begin(),_set1.end());
                unionSet.insert(unionSet.end(), _set2.begin(),_set2.end());
       
                result= metric(unionSet,_set1);
            }
         
        }
        return result;
    }

      bool normalize;
      double rho;
      const ObjectiveVector& ref_point;

  	

  };

#endif /*MOEOHYPERVOLUMEMETRIC_H_*/
