//
//  eoVectOp.h
//  
//
//  Created by Sophie Jacquin on 20/10/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//


#ifndef _eoVectOp_h
#define _eoVectOp_h
#include<stdio.h>
#include<eoOp.h>
#include<eoPop.h>

template<class GenotypeT>
class eoVectOp: public eoOp<GenotypeT>
{
    public:
    eoVectOp()
        :eoOp<GenotypeT>( eoOp<GenotypeT>::general ) {};
    virtual vector<GenotypeT> operator()(GenotypeT & _eo){ 
        vector<GenotypeT> empty;
        return empty;}
    virtual std::string className() const { return "eoVectOp"; }
};


#endif
