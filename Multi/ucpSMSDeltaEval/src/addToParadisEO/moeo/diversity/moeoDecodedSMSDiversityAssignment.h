/*
* <moeoDecodedCrowdingDiversityAssignment.h>
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

#ifndef MOEO_DECODED_SMSDIVERSITYASSIGNMENT_H_
#define MOEO_DECODED_SMSDIVERSITYASSIGNMENT_H_

#include <eoPop.h>
#include <comparator/moeoOneObjectiveComparator.h>
#include <metric/moeoHyperVolumeDifferenceMetric.h>
//#include <addToParadisEO/moeo/diversity/moeoDecodedDiversityAssignment.h>  // TODO: create abstract class for this

/**
 * Diversity assignment sheme based on crowding proposed in:
 * K. Deb, A. Pratap, S. Agarwal, T. Meyarivan, "A Fast and Elitist Multi-Objective Genetic Algorithm: NSGA-II", IEEE Transactions on Evolutionary Computation, vol. 6, no. 2 (2002).
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedSMSDiversityAssignment :public moeoDiversityAssignment < SMOEOT >
//public moeoDecodedDiversityAssignment < SMOEOT, XMOEOT > // TODO: use abstract class for this
{
public:

    /** the objective vector type of the solutions */
    typedef typename XMOEOT::ObjectiveVector ObjectiveVector;


    /**
     


    /**
     * Computes diversity values for every solution contained in the population _pop
     * @param _pop the population
     */
    void operator()(eoPop < SMOEOT > & ps)
    {
        //cout<<"deb diversity"<<endl;
        vector<moeoUCPObjectiveVector> setPopX;
        moeoUCPObjectiveVector pointRef(2);
        moeoUCPObjectiveVector pointIdeal(2);
        pointIdeal[0]=ps[0].children()[0][0];
        pointIdeal[1]=ps[0].children()[0][1];
        pointRef[0]=0;
        pointRef[1]=0;
        vector<int> indiceExtern;
        //indiceExtern.push_back(0);indiceExtern.push_back(0);
        for(int i=0;i<ps.size();i++)
        {
            setPopX.insert(setPopX.end(),ps[i].children().begin(), ps[i].children().end());
            for(int j=0;j<ps[i].children().size();j++)
            {
                if(ps[i].children()[j][0]>pointRef[0])
                {
                    pointRef[0]=ps[i].children()[j][0];
                 
                    
                }
                if(ps[i].children()[j][0]<pointIdeal[0])
                {
                    
                    pointIdeal[0]=ps[i].children()[j][0];
                    
                }
                if(ps[i].children()[j][1]<pointIdeal[1])
                {
                    
                    pointIdeal[1]=ps[i].children()[j][1];
                    
                }
                if(ps[i].children()[j][1]>pointRef[1])
                {
                    pointRef[1]=ps[i].children()[j][1];
                    
                    
                }
            }
        }
        for(int i=0;i<ps.size();i++)
        {
            bool trouve1= false;
            bool trouve2=false;
            for(int j=0;j<ps[i].children().size() &&(!trouve1||!trouve2);j++)
            {
                if(!trouve1 && ps[i].children()[j][0]==pointIdeal[0] )
                {
                    indiceExtern.push_back(i);
                    trouve1=true;
                }
                else if(!trouve2 && ps[i].children()[j][1]==pointIdeal[1] )
                {
                    indiceExtern.push_back(i);
                    trouve2=true;
                }


            }

        }
        
        pointRef[0]+=1;//(pointRef[0]-pointIdeal[0]);
        pointRef[1]+=1;//(pointRef[1]-pointIdeal[1]);
        //calcul des différences : 
        moeoHyperVolumeDifferenceMetric<moeoUCPObjectiveVector> hyperVolDif(false,pointRef);
        for(int i=0;i<ps.size();i++)
        {
            vector<moeoUCPObjectiveVector> setPopR;
            for(int j=0;j<ps.size();j++)
            {
                if(j!=i)
                {
                    setPopR.insert(setPopR.end(), ps[j].children().begin(), ps[j].children().end());
                }
            }
            ps[i].diversity(hyperVolDif(setPopX,setPopR));
            
        }
        for(int i=0;i<indiceExtern.size();i++)
         ps[indiceExtern[i]].diversity(ps[indiceExtern[i]].diversity()+10000000000000);
        
        
       // cout<<"end diversity"<<endl;                    
      
    }


    /**
     * @warning NOT IMPLEMENTED, DO NOTHING !
     * Updates the diversity values of the whole population _pop by taking the deletion of the objective vector _objVec into account.
     */
    void updateByDeleting(eoPop < SMOEOT > & ps,ObjectiveVector& vec)
    {
        std::cout << "WARNING : updateByDeleting not implemented in moeoCrowdingDiversityAssignment" << std::endl;
    }



  


};

#endif /*MOEO_DECODED_CROWDINGDIVERSITYASSIGNMENT_H_*/
