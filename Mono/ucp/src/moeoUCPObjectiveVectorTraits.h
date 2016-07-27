#include <iostream>
using namespace std;
#include <stdio.h>
#include <moeo>

// the moeoObjectiveVectorTraits : minimizing 2 objectives
class moeoUCPObjectiveVectorTraits : public moeoObjectiveVectorTraits {
    public:
    static bool minimizing (int i)
    {
        return true;
    }
    static bool maximizing (int i)
    {
        return false;
    }
    static unsigned int nObjectives ()
    {
        return 2;
    }
};

// objective vector of real values
typedef moeoRealObjectiveVector < moeoUCPObjectiveVectorTraits > moeoUCPObjectiveVector;

/*class VectUCPObjectiveVector : public moeoVector < moeoUCPObjectiveVector, double > {

};*/



//typedef moeoVector < VectUCPObjectiveVector, bool > moeoUCPObjectiveMatrix;

