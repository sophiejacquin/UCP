#ifndef UCPObjectiveVectorTraits_h
#define UCPObjectiveVectorTraits_h

#include <iostream>
using namespace std;
#include <stdio.h>
#include <moeo>

/*!
 * \file UCPObjectiveVectorTraits.h
 * \brief UCP Objective vector trait
 * \author Henri LARGET
 */


/*!
 * \class UCPObjectiveVectorTraits
 * \brief Class that represents an individual, solution for the UCP
 */
class UCPObjectiveVectorTraits : public moeoObjectiveVectorTraits
{
  public:
    static bool minimizing (int i) { return true; }
    static bool maximizing (int i) { return false; }
    static unsigned int nObjectives () { return 2; }
};
typedef moeoRealObjectiveVector<UCPObjectiveVectorTraits> UCPObjectiveVector;


#endif