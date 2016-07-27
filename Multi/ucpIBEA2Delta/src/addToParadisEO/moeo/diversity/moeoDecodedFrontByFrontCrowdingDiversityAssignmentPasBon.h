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

#ifndef MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT2_H_
#define MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT2_H_

#include <addToParadisEO/moeo/diversity/moeoDecodedCrowdingDiversityAssignment.h>
#include <comparator/moeoFitnessThenDiversityComparator.h>
#include <comparator/moeoPtrComparator.h>
#include<diversity/moeoFrontByFrontCrowdingDiversityAssignment.h>
#include<float.h>

/**
 * Diversity assignment sheme based on crowding proposed in:
 * K. Deb, A. Pratap, S. Agarwal, T. Meyarivan, "A Fast and Elitist Multi-Objective Genetic Algorithm: NSGA-II", IEEE Transactions on Evolutionary Computation, vol. 6, no. 2 (2002).
 * Tis strategy assigns diversity values FRONT BY FRONT. It is, for instance, used in NSGA-II.
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedFrontByFrontCrowdingDiversityAssignment 
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


    moeoFrontByFrontCrowdingDiversityAssignment<XMOEOT> diversityX;
    /**
     * Sets the distance values
     * @param _pop the population
     */
public:
    void operator()(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px)
    {
        //cout<<"on vient bien là"<<endl;
        diversityX(px);
            // set huge value for elements in 'ps'
            this->initializePopS(ps);
            
            // set crowding distances for 'px'
            
            
            // apply diversity values to 'ps'
            for(unsigned x=0; x<px.size(); ++x)
                if(px[x].fitness()== ps[px[x].origin()].fitness())
                    this->updateOrigin(ps, px[x]);
        
    }
    void initializePopS(eoPop < SMOEOT > & ps)
    {
        for(unsigned s=0; s<ps.size(); ++s)
            ps[s].diversity(0.0 );
    }
    
    /** update original 'ps' element with value from 'px' element */
    inline void updateOrigin(eoPop < SMOEOT > & ps, XMOEOT& mx)
    {
        //cout<<"diversité de mx"<<mx.diversity();
        if(mx.diversity() >ps[mx.origin()].diversity())
            ps[mx.origin()].diversity( mx.diversity() );
    }
    

    


   



  


};

#endif /*MOEO_DECODED_FRONTBYFRONTCROWDINGDIVERSITYASSIGNMENT_H_*/
