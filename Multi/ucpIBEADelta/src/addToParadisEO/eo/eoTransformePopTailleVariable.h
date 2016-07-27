//
//  eoTransformePopTailleVariable.h
//  
//
//  Created by Sophie Jacquin on 20/10/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#ifndef _eoTransformePopTailleVariable_h
#define _eoTransformePopTailleVariable_h
#include <eoInvalidateOps.h>
#include <eoPop.h>
#include <addToParadisEO/eo/eoVectOp.h>
///////////////////////////////////////////////////////////////////////////////
// class eoSGATransform
///////////////////////////////////////////////////////////////////////////////
#include <vector>          // std::vector
#include <utils/eoRNG.h>
#include <eoTransform.h>

/** eoSGATransform: transforms a population using genetic operators.
 * It does it exactly as class eoSGA, i.e. only accepts 
 *    quadratic crossover and unary mutation
 * It is here mainly for tutorial reasons
 */
template<class EOT> class eoTransformePopTailleVariable : public eoTransform<EOT>
{
public:
    
    /// Default constructor.
    eoTransformePopTailleVariable(eoQuadOp<EOT>& _cross, double _cProba, 
                   eoMonOp<EOT>& _mutate, double _mProba, eoVectOp<EOT> & _vectMut, double _vecMutProba)
    : cross(_cross),
    crossoverProba(_cProba),
    mutate(_mutate), 
    mutationProba(_mProba),
    vectMut(_vectMut),
    mutVecProba(_vecMutProba){}
    eoTransformePopTailleVariable (eoQuadOp<EOT>& _cross, double _cProba, 
                                   eoMonOp<EOT>& _mutate, double _mProba)
    : cross(_cross),
    crossoverProba(_cProba),
    mutate(_mutate), 
    mutationProba(_mProba),
    vectMut(),
    mutVecProba(0.0)
    
    {
        
    }
    eoTransformePopTailleVariable()
    : 
    crossoverProba(0),
    mutationProba(0),
    vectMut(),
    mutVecProba(0){}
   
    
    /**
     * Transforms a population.
     * @param pop The population to be transformed.
     */
    void operator()(eoPop<EOT>& _pop) 
    {
        
        unsigned i;
        //cout<<"av for "<<_pop.size()<<endl;
        for (i=0; i<_pop.size()/2; i++) 
        {
            //cout<<"i="<<i<<endl;
            if ( rng.flip(crossoverProba) ) 
            {
               // cout<<"ici"<<endl;
                // this crossover generates 2 offspring from two parents
                cross(_pop[2*i], _pop[2*i+1]);
            }
        }
        //cout<<"fin pour1"<<endl;
        for (i=0; i < _pop.size(); i++) 
        {
            if (rng.flip(mutationProba) ) 
            {
                mutate(_pop[i]);
            }
            
        }
        int size=_pop.size();
        i=0;
        //cout<<"av while"<<endl;
        while ( i < size) 
        {
            if (rng.flip(mutVecProba) ) 
            {
                vector<EOT> popEnfant(vectMut(_pop[i]));
                //cout<<"là"<<endl;
                //_pop.erase(_pop.begin()+i);
               
                //size=size-1; 
                //_pop.insert(_pop.end(),popEnfant.begin(),popEnfant.end());
                for(int j=0;j<popEnfant.size();j++)
                {
                   
                    _pop.push_back(popEnfant[j]);
                }
                
            }
             i++;
            
        }
       // cout<<"pop size fin transform:"<<_pop.size();
        
    };
    
private:
    eoInvalidateQuadOp<EOT> cross;
    double crossoverProba;
    eoInvalidateMonOp<EOT> mutate;
    double mutationProba;
    eoVectOp<EOT> & vectMut;
    double mutVecProba;
};



#endif
