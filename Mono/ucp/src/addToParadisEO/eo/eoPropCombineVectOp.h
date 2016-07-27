//
//  eoPropCombineVectOp.h
//  
//
//  Created by Sophie Jacquin on 22/10/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#ifndef _eoPropCombineVectOp_h
#define _eoPropCombineVectOp_h
#include <eoObject.h>
#include <eoPrintable.h>
#include <eoFunctor.h>
#include <eoOp.h>
#include<addToParadisEO/eo/eoVectOp.h>
#include <utils/eoRNG.h>
#include<stdio.h>
template <class EOT>
class eoPropCombinedVectOp: public eoVectOp<EOT>
{
public:
    /// Ctor from a "true" operator
    eoPropCombinedVectOp(eoVectOp<EOT> & _first, const double _rate)
    {
        ops.push_back(&_first);
        rates.push_back(_rate);
    }
    
    virtual std::string className() const { return "eoPropCombinedMonOp"; }
    
    virtual void add(eoVectOp<EOT> & _op, const double _rate, bool _verbose=false)
    {
        ops.push_back(&_op);
        rates.push_back(_rate);
        // compute the relative rates in percent - to warn the user!
        if (_verbose)
            printOn(std::cout);
    }
    
    // outputs the operators and percentages
    virtual void printOn(std::ostream & _os)
    {
        double total = 0;
        unsigned i;
        for (i=0; i<ops.size(); i++)
            total += rates[i];
        _os << "In " << className() << "\n" ;
        for (i=0; i<ops.size(); i++)
            _os << ops[i]->className() << " with rate " << 100*rates[i]/total << " %\n";
    }
    
    virtual vector<EOT> operator()(EOT & _indi)
    {
        unsigned what = rng.roulette_wheel(rates); // choose one op
        return (*ops[what])(_indi);		   // apply it
    }
protected:
    std::vector<eoVectOp<EOT>*> ops;
    std::vector<double> rates;
};



#endif
