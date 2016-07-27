/*
* <moeoFitnessAssignment.h>
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

#ifndef MOEODECODEDFITNESSASSIGNMENT_H_
#define MOEODECODEDFITNESSASSIGNMENT_H_

#include <eoFunctor.h>
#include <eoPop.h>

/**
 * Functor that sets the fitness values of a whole populations of S and X spaces.
 */
template < class SMOEOT, class XMOEOT >
class moeoDecodedFitnessAssignment : public eoBF < eoPop < SMOEOT > &, eoPop < XMOEOT > &, void >
  {
  protected:
	bool toMinimize; /* decide if the fitness is to be minimized or maximized */
	moeoFitnessAssignment < XMOEOT >& fax;

/*
        template<class T> T worst(T&, bool)
	{
		cout << "WARNING: TYPE NOT RECOGNIZED IN moeoDecodedFitnessAssignment, returning zero as worst value!" << endl;
		return 0;
	}
*/
        int worst(const int&, bool toMinimize)
        {
		return (toMinimize?numeric_limits<int>::max():numeric_limits<int>::min());
        }

        double worst(const double&, bool toMinimize)
        {
		return (toMinimize?numeric_limits<double>::max():numeric_limits<double>::min());
        }

  public:

    moeoDecodedFitnessAssignment(bool _toMinimize, moeoFitnessAssignment < XMOEOT >& _fax) :
	toMinimize(_toMinimize), fax(_fax)
    {
    }

    void operator()(eoPop < SMOEOT > & ps, eoPop < XMOEOT > & px)
    {
	  fax(px);
        
          // initialize fitness values with WORST value
          for(unsigned s=0; s<ps.size(); ++s)
               
          {
              ps[s].fitness(0);
             // ps[s].fitness( worst(ps[s].fitness(), toMinimize) );
          }
       
          for(unsigned x=0; x<px.size(); ++x)
          {
              //int count=0;
             // for(int s=0;s<ps.size();s++)
              //{ 
                  //if(px[x].origin()== ps[s].origin())
                  //{
                     // count++;
                      if(px[x].fitness() > ps[px[x].origin()].fitness())
                  {
                      //if(px[x].getTemps()!=ps[s].getTemps())
                      //    cout<<"********++++++++++!!!!!!!!!!!!!!! INCOHERENCE Correspondance ::: "<<px[x].fitness()<<"   S : "<<ps[s].fitness()<<" OoOoOoO0oooooOOO"<<endl;
                      ps[px[x].origin()].fitness( px[x].fitness() );

                      
                  } 
                  //}
              //}
             // if(count!=1)
                
        
          }
       // px.printOn(cout);
    }

    /**
     * Updates the fitness values of the whole population _pop by taking the deletion of the objective vectors originated by origin solution _idx in _ps. 
     * @param _pop the population
     * @param _objVec the objective vector
     */
    //virtual void updateByDeleting(eoPop < SMOEOT > & _ps, eoPop < XMOEOT > & _px, unsigned & _idx) = 0;


  };

#endif /*MOEODECODEDFITNESSASSIGNMENT_H_*/
