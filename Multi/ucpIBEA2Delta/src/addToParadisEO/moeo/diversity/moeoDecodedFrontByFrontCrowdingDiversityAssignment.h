/*
* <moeoDecodedFrontByFrontCrowdingDiversityAssignment.h>
* Copyright (C) DOLPHIN Project-Team, INRIA Futurs, 2006-2009
* (C) OPAC Team, LIFL, 2002-2009
*
* Igor Machado, Sophie Jacquin, Arnaud Liefooghe, Waldo Cancino
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

//A EXAMINER AVEC ATTENTION---------------

#ifndef MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT2_H_
#define MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT2_H_

#include <addToParadisEO/moeo/diversity/moeoDecodedCrowdingDiversityAssignment.h>
#include <comparator/moeoFitnessThenDiversityComparator.h>
#include <comparator/moeoPtrComparator.h>
#include<diversity/moeoFrontByFrontCrowdingDiversityAssignment.h>


/**
 * Diversity assignment sheme based on crowding proposed in:
 * K. Deb, A. Pratap, S. Agarwal, T. Meyarivan, "A Fast and Elitist Multi-Objective Genetic Algorithm: NSGA-II", IEEE Transactions on Evolutionary Computation, vol. 6, no. 2 (2002).
 * Tis strategy assigns diversity values FRONT BY FRONT. It is, for instance, used in NSGA-II.
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedFrontByFrontCrowdingDiversityAssignment : public moeoDecodedCrowdingDiversityAssignment < SMOEOT , XMOEOT >
{
public:

    /** the objective vector type of the solutions */
    typedef typename XMOEOT::ObjectiveVector ObjectiveVector;


    /**
     * @warning NOT IMPLEMENTED, DO NOTHING !
     * Updates the diversity values of the whole population _pop by taking the deletion of the objective vector _objVec into account.
     */
    void updateByDeleting(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px, unsigned & idx_s)
    {
        std::cout << "WARNING : updateByDeleting not implemented in moeoFrontByFrontCrowdingDistanceDiversityAssignment" << std::endl;
    }

private:

    using moeoDecodedCrowdingDiversityAssignment < SMOEOT , XMOEOT >::inf;
    using moeoDecodedCrowdingDiversityAssignment < SMOEOT , XMOEOT >::tiny;
    //using moeoDecodedCrowdingDiversityAssignment < SMOEOT , XMOEOT >::nextDiffOrigin;
   // using moeoDecodedCrowdingDiversityAssignment < SMOEOT , XMOEOT >::prevDiffOrigin;

    /**
     * Sets the distance values
     * @param _pop the population
     */
public:
    void operator()(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px)
    {
       // cout<<"dans cet op"<<endl;
        if (px.size() <= 2)
        {
            for (unsigned int x=0; x<px.size(); ++x)
            {
                px[x].diversity(0.0);
                this->updateOrigin(ps, px[x]);
            }
        }
        else
        {
            // set huge value for elements in 'ps'
            this->initializePopS(ps);
            
            // set crowding distances for 'px'
            setDistances(px);
            //à enlever plus tard :
            for(unsigned s=0;s<ps.size();s++)
            {
                for(unsigned  x=0; x<px.size(); ++x)
                {
                   if(px[x].origin()==ps[s].origin() && px[x].fitness()<ps[s].fitness())
                   {
                      
                       px[x].diversity(0.0);
                   }
                }
            }
            //fin à enlever
            
            //px.printOn(cout);
            // apply diversity values to 'ps'
            for(unsigned x=0; x<px.size(); ++x)
                //if(px[x].fitness()== ps[px[x].origin()].fitness())// ca c'est faux
                    this->updateOrigin(ps, px[x]);
        }
    }
    
    inline void updateOrigin(eoPop < SMOEOT > & ps, XMOEOT& mx)
    {
        //cout<<"diversité de mx"<<mx.diversity();
        if(mx.diversity() >ps[mx.origin()].diversity())
            ps[mx.origin()].diversity( mx.diversity() );
    }

    
    
 

    void setDistances (eoPop <XMOEOT> & px)
    {
       // cout<<"set dist"<<endl;
        
        //moeoFrontByFrontCrowdingDiversityAssignment<XMOEOT> diversityX;
        //diversityX(px);
        
        unsigned int a,b;
        double min, max, distance;
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        // set diversity to 0 for every individual
        for (unsigned int i=0; i<px.size(); i++)
        {
            px[i].diversity(0.0);
        }
        // sort the whole pop according to fitness values
        moeoFitnessThenDiversityComparator < XMOEOT > fitnessComparator;
        std::vector<XMOEOT *> sortedptrpop;
        sortedptrpop.resize(px.size());
        // due to intensive sort operations for this diversity assignment,
        // it is more efficient to perform sorts using only pointers to the
        // population members in order to avoid copy of individuals
        for(unsigned int i=0; i< px.size(); i++) sortedptrpop[i] = & (px[i]);
        //sort the pointers to population members
        moeoPtrComparator<XMOEOT> cmp2( fitnessComparator);
        std::sort(sortedptrpop.begin(), sortedptrpop.end(), cmp2);
        // compute the crowding distance values for every individual "front" by "front" (front : from a to b)
        a = 0;	        			// the front starts at a
        while (a < px.size())
        {
            b = lastIndex(sortedptrpop,a);	// the front ends at b
            //b = lastIndex(_pop,a);	// the front ends at b
            // if there is less than 2 individuals in the front...
            if ((b-a) < 2)
            {
                for (unsigned int i=a; i<=b; i++)
                {
                    sortedptrpop[i]->diversity(inf());
                    //_pop[i].diversity(inf());
                }
            }
            // else...
            else
            {
                // for each objective
                for (unsigned int obj=0; obj<nObjectives; obj++)
                {
                    // sort in the descending order using the values of the objective 'obj'
                    moeoOneObjectiveComparator < XMOEOT > objComp(obj);
                    moeoPtrComparator<XMOEOT> cmp2( objComp );
                    std::sort(sortedptrpop.begin()+a, sortedptrpop.begin()+b+1, cmp2);
                    /*cout<<a<<" "<<b<<endl;
                    for (unsigned int i=a; i<=b; i++)
                    {
                        cout<<(sortedptrpop[i])->objectiveVector()[obj]<<"  ";
                    }*/
                    // min & max
                    min = (sortedptrpop[a])->objectiveVector()[obj];
                    max = (sortedptrpop[b])->objectiveVector()[obj];

                    // avoid extreme case
                    if (min == max)
                    {
                        //cout<<"ICIIIII"<<endl;
                        min -= tiny();
                        max += tiny();
                    }
                    //cout<<"min="<<min<<"max="<<max<<endl;
                    // set the diversity value to infiny for min and max
                    sortedptrpop[a]->diversity(inf());
                    sortedptrpop[b]->diversity(inf());
                    // set the diversity values for the other individuals
                    for (unsigned int i=a+1; i<b; i++)
                    {
                        int ip1 = nextDiffOrigin(sortedptrpop, i);
                        int im1 = prevDiffOrigin(sortedptrpop, i);
                        if((im1==-1) || (ip1==-1))
                        {
                            sortedptrpop[i]->diversity(inf());
                            continue;
                        }

                        if(sortedptrpop[i]->diversity() == inf())
                            continue;


                        distance = ( sortedptrpop[im1]->objectiveVector()[obj] - sortedptrpop[ip1]->objectiveVector()[obj] ) / (max-min);
                        
                        
                        if(sortedptrpop[im1]->objectiveVector()[obj] < sortedptrpop[ip1]->objectiveVector()[obj])
                        {
                            cout << "ERROR: CHECK moeoDecodedFrontByFrontCrowdingDiversityAssignment because previous element should be bigger than next after sorting algorithm" << endl;
                            cout << "distance is negative: " << distance << endl;
                            cout << "max: " << max << endl;
                            cout << "min: " << min << endl;
                            cout<<"diff:"<<max-min<<endl;
                            cout<<"im1="<<im1<<"  "<<sortedptrpop[im1]->objectiveVector()[obj]<<" fit "<<sortedptrpop[im1]->fitness()<< " ip1="<<ip1<<"  "<<sortedptrpop[ip1]->objectiveVector()[obj]<<" fit "<<sortedptrpop[ip1]->fitness()<<" fit "<<sortedptrpop[i]->fitness()<<endl;
                            exit(1);
                        }
                        

                        sortedptrpop[i]->diversity(sortedptrpop[i]->diversity() + distance);
                    }
                }
            }
            // go to the next front
            a = b+1;
        }
    }
    
    inline int prevDiffOrigin (std::vector<XMOEOT *> & px, unsigned idx)
    {
        for(unsigned i=idx+1; i<px.size(); ++i)
            if(px[idx]->origin() != px[i]->origin() && px[idx]->fitness() == px[i]->fitness()  )
                return i;
        return -1;
    }
    
    /** returns index of previous element in 'px' before 'idx' with different origin in population 'ps'; returns -1 if not found */
    inline int nextDiffOrigin(std::vector<XMOEOT *> & px, unsigned idx)
    {
        for(int i=((int)idx)-1; i>=0; --i)
            if(px[idx]->origin() != px[i]->origin()&& px[idx]->fitness() == px[i]->fitness())
                return i;
        return -1;
    }



    /**
     * Returns the index of the last individual having the same fitness value than _pop[_start]
     * @param _pop the vector of pointers to population individuals
     * @param _start the index to start from
     */

    unsigned int lastIndex (std::vector<XMOEOT *> & _pop, unsigned int _start)
    {
        unsigned int i=_start;
        while ( (i<_pop.size()-1) && (_pop[i]->fitness()==_pop[i+1]->fitness()) )
        {
            i++;
        }
        return i;
    }


};

#endif /*MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT_H_*/
