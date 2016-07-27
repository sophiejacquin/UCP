#ifndef MOEO_UCP_DECODER_H_
#define MOEO_UCP_DECODER_H_

#include <core/MOEO.h>
#include <eoFunctor.h>
#include <moeoUCPEvalFunc.h>
#include <addToParadisEO/moeo/core/moeoDecoder.h>
#include "eoUCPData.h"
#include "moeoUCPCorrection.h"
#include"moeoUCPEvalFuncS.h"
/**
 * Base class for a decoder that converts an EO in population of MOEO.
 */
template < class MOEOT, class MOEOTX >
class moeoUCPDecoder : public moeoDecoder<MOEOT, MOEOTX>
  {
  public:

    /** The type of objective vector */
    typedef typename MOEOT::ObjectiveVector ObjectiveVector;

  /*!
   * \brief Operator () which decodes an EOT into a population of MOEOT
   *
   * \param _eo : an EO solution to be decoded
   */
      moeoUCPDecoder(eoUCPData* data)
      {
          eval.setData(data);
          correcteur.setData(data);
           evalS.setData(data);
      }
   void operator () (MOEOT & eo, eoPop<MOEOTX>& popX)
   {
       // REMEMBER: type MOEOTX must inherit from class Origin
       // decode _eo and return a population of type MOEOTX
       
       correcteur(eo);
       int nbHours=eo.get_nbHours();
       int nbUnits=eo.get_nbUnits();
       int n=eo.getN();
       evalS(eo);
       moeoUCPObjectiveVector eoObj=eo.objectiveVector();
       MOEOTX moeoX0;
       moeoX0.set(nbUnits,nbHours);
       moeoX0.setAlpha(0);
       moeoX0.set_mbits(eo.get_mbits());
       moeoUCPObjectiveVector objVec0;
       objVec0[0]=0;
       objVec0[1]=0;
       moeoX0.objectiveVector(objVec0);
       if(eo.get_penalty_demand()==0)
           eval(moeoX0);
       moeoUCPObjectiveVector moeoX0Obj=moeoX0.objectiveVector();
       // cout<<eoObj[0]<<endl;
       objVec0[0]=moeoX0Obj[0]+eoObj[0];
       objVec0[1]=moeoX0Obj[1]+eoObj[1];
       moeoX0.objectiveVector(objVec0);
       popX.push_back(moeoX0);
       MOEOTX moeoX1;
       moeoX1.set(nbUnits,nbHours);
       moeoX1.setAlpha(1);
       moeoX1.set_mbits(eo.get_mbits());
       moeoUCPObjectiveVector objVec1;
       objVec1[0]=0;
       objVec1[1]=0;
       moeoX1.objectiveVector(objVec1);
       if(eo.get_penalty_demand()==0)
           eval(moeoX1);
       moeoUCPObjectiveVector moeoX1Obj=moeoX0.objectiveVector();
       // cout<<eoObj[0]<<endl;
       objVec1[0]=moeoX1Obj[0]+eoObj[0];
       objVec1[1]=moeoX1Obj[1]+eoObj[1];
       moeoX1.objectiveVector(objVec1);
       popX.push_back(moeoX1);
       eo.setPointIdeal(moeoX1Obj[0],moeoX0Obj[1]);
       eo.setPointNadir(moeoX1Obj[1],moeoX0Obj[0]);
       //cout<<"n="<<n<<endl;
       for(unsigned i=1;i<n;++i)
       {
           MOEOTX moeoX;
           moeoX.set(nbUnits,nbHours);
           moeoX.setAlpha((double)i/(double)n);
           moeoX.set_mbits(eo.get_mbits());
           moeoX.setNorme(eo.getPointNadir(),eo.getPointIdeal());
           moeoUCPObjectiveVector objVec;
           objVec[0]=0;
           objVec[1]=0;
           moeoX.objectiveVector(objVec);
           if(eo.get_penalty_demand()==0)
               eval(moeoX);
           moeoUCPObjectiveVector moeoXObj=moeoX.objectiveVector();
          // cout<<eoObj[0]<<endl;
           objVec[0]=moeoXObj[0]+eoObj[0];
           objVec[1]=moeoXObj[1]+eoObj[1];
           moeoX.objectiveVector(objVec);
           popX.push_back(moeoX);
           
       }
   }
      private :
      moeoUCPEvalFunc<MOEOTX> eval;
      moeoUCPCorrection<MOEOT> correcteur;
      moeoUCPEvalFuncS<MOEOT> evalS;

  };

#endif
