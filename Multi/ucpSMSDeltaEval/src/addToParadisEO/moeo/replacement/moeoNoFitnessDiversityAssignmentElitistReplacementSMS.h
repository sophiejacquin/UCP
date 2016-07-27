/*
* <moeoNoFitnessDiversityAssignmentElitistReplacement.h>
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

#ifndef MOEONOFITNESSDIVERSITYASSIGNMENTELITISTREPLACEMENTSMS_H_
#define MOEONOFITNESSDIVERSITYASSIGNMENTELITISTREPLACEMENTSMS_H_

#include <comparator/moeoComparator.h>
#include <comparator/moeoFitnessThenDiversityComparator.h>
#include <comparator/moeoFitnessComparator.h>
#include <diversity/moeoDiversityAssignment.h>
#include <diversity/moeoDummyDiversityAssignment.h>
#include <fitness/moeoFitnessAssignment.h>
#include <replacement/moeoReplacement.h>
#include <addToParadisEO/moeo/diversity/moeoDecodedSMSDiversityAssignment.h>

/**
 * Elitist replacement strategy that consists in keeping the N best individuals (not applying fitness or diversity assignment).
 */
template < class MOEOT, class XMOEO > class moeoNoFitnessDiversityAssignmentElitistReplacementSMS: public moeoReplacement < MOEOT >
  {
  public:

    /**
     * Full constructor.
     * @param _comparator the comparator (used to compare 2 individuals)
     */
    moeoNoFitnessDiversityAssignmentElitistReplacementSMS (moeoComparator < MOEOT > & _comparator) :
      comparator (_comparator),comparator2(defaultComparator2)
    {}


    /**
     * Constructor without comparator. A moeoFitThenDivComparator is used as default.
     */
    moeoNoFitnessDiversityAssignmentElitistReplacementSMS () :
        comparator (defaultComparator),comparator2(defaultComparator2)
    {}


    /**
     * Replaces the first population by adding the individuals of the second one, sorting with a moeoComparator and resizing the whole population obtained.
        * @param _parents the population composed of the parents (the population you want to replace)
        * @param _offspring the offspring population
     */
    void operator () (eoPop < MOEOT > &_parents, eoPop < MOEOT > &_offspring)
    {
        cout<<"deb replacement"<<endl;
         moeoDecodedSMSDiversityAssignment<MOEOT,XMOEO> diversity;
      
      unsigned int sz = _parents.size ();
        eoPop<MOEOT> unionPop;
        unionPop.reserve (_parents.size () + _offspring.size ());
        std::copy (_parents.begin (), _parents.end (), back_inserter (unionPop));
         std::copy (_offspring.begin (), _offspring.end (), back_inserter (unionPop));
        std::sort(unionPop.begin(), unionPop.end(), comparator2);
        _parents.clear();
        cout<<"union trié et parent effacé"<<endl;
        if(unionPop[sz-1].fitness()!=unionPop[sz].fitness())
        {
           // cout<<"si"<<endl;
            _parents.insert(_parents.end(),unionPop.begin(), unionPop.begin()+sz);//verifier si c'est bien +sz
        }
        else{
           // cout<<"sinon"<<endl;
            int indDebFront=sz-1;
            while(indDebFront-1>=0 && unionPop[indDebFront-1].fitness()== unionPop[sz].fitness())
                indDebFront --;
            eoPop<MOEOT> rang;
            //cout<<"av insert indDebFront"<<indDebFront<<endl;
             _parents.insert(_parents.end(),unionPop.begin(), unionPop.begin()+indDebFront);//encore une fois vérifier bornes
            int i = indDebFront;
            //cout<<"av while"<<endl;
            while(i<unionPop.size()&&unionPop[i].fitness()==unionPop[indDebFront].fitness())
            {
                rang.push_back(unionPop[i]);
                i++;
            }
            //cout<<"fin while"<<endl;
            int placeRestantes= sz-_parents.size();
            while(placeRestantes>0)
            {
                diversity(rang);
                std::sort(rang.begin(), rang.end(), comparator);
                //rang.printOn(cout);
                _parents.push_back(rang[0]);
                rang.erase(rang.begin());
                placeRestantes--;
            }
            cout<<"end replacement"<<endl;
            cout<<"taille parents :"<<_parents.size()<<endl;
        }
        
      // merges offspring and parents into a global population
      /*_parents.reserve (_parents.size () + _offspring.size ());
      std::copy (_offspring.begin (), _offspring.end (), back_inserter (_parents));

      // fitness and diversity assignment already done!
       
      // sorts the whole population according to the comparator
      std::sort(_parents.begin(), _parents.end(), comparator);
       
       // reverse(_parents.begin(), _parents.end());
      // finally, resize this global population
        // _parents.printOn(cout);
      _parents.resize (sz);*/
       
      // and clear the offspring population
       // cout<<"apres replacement :"<<endl;
        //_parents.printOn(cout);
      _offspring.clear ();
    }


  protected:

    /** a fitness then diversity comparator can be used as default */
    moeoFitnessThenDiversityComparator < MOEOT > defaultComparator;
      moeoFitnessComparator< MOEOT> defaultComparator2;
    /** this object is used to compare solutions in order to sort the population */
    class Cmp
      {
      public:
        /**
         * Ctor.
         * @param _comp the comparator
         */
        Cmp(moeoComparator < MOEOT > & _comp) : comp(_comp)
        {}
        /**
         * Returns true if _moeo1 is greater than _moeo2 according to the comparator
         * _moeo1 the first individual
         * _moeo2 the first individual
         */
        bool operator()(const MOEOT & _moeo1, const MOEOT & _moeo2)
        {
          return comp(_moeo2,_moeo1);
        }
      private:
        /** the comparator */
        moeoComparator < MOEOT > & comp;
      }
    comparator, comparator2;
      
     

  };

#endif /*MOEONOFITNESSDIVERSITYASSIGNMENTELITISTREPLACEMENT_H_ */
