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

#ifndef MOEO_DECODED_CROWDINGDIVERSITYASSIGNMENT_H_
#define MOEO_DECODED_CROWDINGDIVERSITYASSIGNMENT_H_

#include <eoPop.h>
#include <comparator/moeoOneObjectiveComparator.h>
//#include <addToParadisEO/moeo/diversity/moeoDecodedDiversityAssignment.h>  // TODO: create abstract class for this

/**
 * Diversity assignment sheme based on crowding proposed in:
 * K. Deb, A. Pratap, S. Agarwal, T. Meyarivan, "A Fast and Elitist Multi-Objective Genetic Algorithm: NSGA-II", IEEE Transactions on Evolutionary Computation, vol. 6, no. 2 (2002).
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedCrowdingDiversityAssignment : public eoBF < eoPop < SMOEOT > &, eoPop < XMOEOT > &, void >//public moeoDecodedDiversityAssignment < SMOEOT, XMOEOT > // TODO: use abstract class for this
{
public:

    /** the objective vector type of the solutions */
    typedef typename XMOEOT::ObjectiveVector ObjectiveVector;


    /**
     * Returns a big value (regarded as infinite)
     */
    double inf() const
    {
        return std::numeric_limits<double>::max();
    }


    /**
     * Returns a very small value that can be used to avoid extreme cases (where the min bound == the max bound)
     */
    double tiny() const
    {
        return 1e-6;
    }

    /** initialize diversity values with WORST value */
    void initializePopS(eoPop < SMOEOT > & ps)
    {
        for(unsigned s=0; s<ps.size(); ++s)
            ps[s].diversity(0.0);
    }

    /** update original 'ps' element with value from 'px' element */
    inline void updateOrigin(eoPop < SMOEOT > & ps, XMOEOT& mx)
    {
        for(int s=0;s<ps.size();s++)
        if(ps[s].origin()==mx.origin() && mx.diversity() > ps[s].diversity())
            ps[s].diversity( mx.diversity() );
    }


    /**
     * Computes diversity values for every solution contained in the population _pop
     * @param _pop the population
     */
    void operator()(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px)
    {
        if (px.size() <= 2)
        {
            for (unsigned int x=0; x<px.size(); ++x)
            {
                px[x].diversity(inf());
                updateOrigin(ps, px[x]);
            }
        }
        else
        {
            // set huge value for elements in 'ps'
            initializePopS(ps);

            // set crowding distances for 'px'
            setDistances(px);

            // apply diversity values to 'ps'
            for(unsigned x=0; x<px.size(); ++x)
                updateOrigin(ps, px[x]);
        }
    }


    /**
     * @warning NOT IMPLEMENTED, DO NOTHING !
     * Updates the diversity values of the whole population _pop by taking the deletion of the objective vector _objVec into account.
     */
    void updateByDeleting(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px, unsigned & idx_s)
    {
        std::cout << "WARNING : updateByDeleting not implemented in moeoCrowdingDiversityAssignment" << std::endl;
    }


protected:

    /**
     * Sets the distance values
     * @param _pop the population
     */
    virtual void setDistances (eoPop < XMOEOT > & px)
    {
        double min, max, distance;
        unsigned int nObjectives = XMOEOT::ObjectiveVector::nObjectives();
        // set diversity to 0
        for (unsigned int i=0; i<px.size(); i++)
        {
            px[i].diversity(0.0);
        }
        // for each objective
        for (unsigned int obj=0; obj<nObjectives; obj++)
        {
            // comparator
            moeoOneObjectiveComparator < XMOEOT > objComp(obj);
            // sort
            std::sort(px.begin(), px.end(), objComp);
            // min & max
            min = px[0].objectiveVector()[obj];
            max = px[px.size()-1].objectiveVector()[obj];
            // set the diversity value to infiny for min and max
            px[0].diversity(inf());
            px[px.size()-1].diversity(inf());
            for (unsigned int i=1; i<px.size()-1; i++)
            {
                int ip1 = nextDiffOrigin(px, i);
                int im1 = prevDiffOrigin(px, i);
                if((im1==-1) || (ip1==-1))
                {
                    px[i].diversity(inf());
                    continue;
                }

                if(px[i].diversity() == inf())
                    continue;

                if(px[ip1].objectiveVector()[obj] < px[im1].objectiveVector()[obj])
                {
                    cout << "ERROR: CHECK moeoDecodedFrontByFrontCrowdingDiversityAssignment because previous element should be bigger than next after sorting algorithm" << endl;
                    exit(1);
                }

                distance = (px[ip1].objectiveVector()[obj] - px[im1].objectiveVector()[obj]) / (max-min);
                px[i].diversity(px[i].diversity() + distance);
            }
        }
    }


    /** returns index of next element in 'px' after 'idx' with different origin in population 'ps'; returns -1 if not found */
    inline int nextDiffOrigin(eoPop < XMOEOT > & px, unsigned idx)
    {
        for(unsigned i=idx+1; i<px.size(); ++i)
            if(px[idx].origin() != px[i].origin() )
                return i;
        return -1;
    }

    /** returns index of previous element in 'px' before 'idx' with different origin in population 'ps'; returns -1 if not found */
    inline int prevDiffOrigin(eoPop < XMOEOT > & px, unsigned idx)
    {
        for(int i=((int)idx)-1; i>=0; --i)
            if(px[idx].origin() != px[i].origin())
                return i;
        return -1;
    }

};

#endif /*MOEO_DECODED_CROWDINGDIVERSITYASSIGNMENT_H_*/
