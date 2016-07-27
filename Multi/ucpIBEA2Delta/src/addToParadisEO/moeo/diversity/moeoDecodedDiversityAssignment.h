/*
* <moeoDecodedDiversityAssignment.h>
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

#ifndef MOEODECODEDDIVERSITYASSIGNMENT_H_
#define MOEODECODEDDIVERSITYASSIGNMENT_H_

#include <eoFunctor.h>
#include <eoPop.h>
#include<diversity/moeoDiversityAssignment.h>

/**
 * Functor that sets the diversity values of a whole populations of S and X spaces.
 */
template < class SMOEOT , class XMOEOT >
class moeoDecodedDiversityAssignment : public eoBF < eoPop < SMOEOT > &, eoPop < XMOEOT > &, void >
{
protected:
    bool toMinimize; /* decide if the diversity is to be minimized or maximized */
    moeoDiversityAssignment < XMOEOT >& dax;

    int worst(const int&, bool toMinimize)
    {
        return (toMinimize?numeric_limits<int>::max():numeric_limits<int>::min());
    }

    double worst(const double&, bool toMinimize)
    {
        return (toMinimize?numeric_limits<double>::max():numeric_limits<double>::min());
    }

public:

    moeoDecodedDiversityAssignment(bool _toMinimize, moeoDiversityAssignment < XMOEOT >& _dax) :
        toMinimize(_toMinimize), dax(_dax)
    {
    }

    /** initialize diversity values with WORST value */
    void initializePopS(eoPop < SMOEOT > & ps)
    {
        for(unsigned s=0; s<ps.size(); ++s)
        {
            ps[s].diversity(0);
           // ps[s].diversity( worst(ps[s].diversity(), toMinimize) );
        }
    }

    inline void updateOrigin(eoPop < SMOEOT > & ps, XMOEOT& mx)
    {
        if(toMinimize)
        {
            if(mx.diversity() < ps[s].diversity())
                ps[s].diversity( mx.diversity() );
        }
        else if(mx.diversity() > ps[s].diversity())
            ps[s].diversity( mx.diversity() );
    }

    void operator()(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px)
    {
        initializePopS(ps);

        // run the diversity algorithm |px| times (WARNING: too inefficient! improve by inheritance.)
        for(unsigned x=0; x<px.size(); ++x)
        {
            unsigned s = px[x].origin();

            eoPop< XMOEOT > px_no_s;
            px_no_s.push_back(px[x]);

            for(unsigned x2=0; x2<px.size(); ++x2)
                if((x != x2) && (px[x2].origin() != s))
                    px_no_s.push_back(px[x]);
            dax(px_no_s);

            updateOrigin(ps, px[x]);
        }
    }


    /**
     * Updates the diversity of the whole population _pop by taking the deletion of the objective vectors originated by origin solution idx_s in _ps.
     * @param _pop the population
     * @param _objVec the objective vector
     */
    virtual void updateByDeleting(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px, unsigned & idx_s)
    {


    }

};

#endif /*MOEODECODEDDIVERSITYASSIGNMENT_H_*/
