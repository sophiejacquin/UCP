#ifndef MOEO_UCP_DECODER_H_
#define MOEO_UCP_DECODER_H_

#include <core/MOEO.h>
#include <eoFunctor.h>
#include <moeoUCPEvalFunc.h>
#include <addToParadisEO/moeo/core/moeoDecoder.h>
#include "eoUCPData.h"

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
        
           evalS.setData(data);
      }
   void operator () (MOEOT & eo, eoPop<MOEOTX>& popX)
   {
       // REMEMBER: type MOEOTX must inherit from class Origin
       // decode _eo and return a population of type MOEOTX
      // cout<<"deb decoder"<<endl;
      
       int nbHours=eo.getNbHeures();
       int nbUnits=eo.getNbUnits();
       int n=eo.getN();
       evalS(eo);
       moeoUCPObjectiveVector eoObj=eo.objectiveVector();
       MOEOTX moeoX0;
       moeoX0.setNbUnits(nbUnits);
       moeoX0.setNbHeures(nbHours);
       moeoX0.setAlpha(0);
       moeoX0.setTemps(eo.getTemps());
       moeoUCPObjectiveVector objVec0;
       objVec0[0]=0;
       objVec0[1]=0;
       moeoX0.objectiveVector(objVec0);
       if(eoObj[1]==0)
           eval(moeoX0);
       moeoUCPObjectiveVector moeoX0Obj=moeoX0.objectiveVector();
       // cout<<" obj0 1 ucpdecoder :"<<eoObj[0]<<endl;
       objVec0[0]=moeoX0Obj[0]+eoObj[0];
       objVec0[1]=moeoX0Obj[1]+eoObj[1];
       moeoX0.objectiveVector(objVec0);
       popX.push_back(moeoX0);
       MOEOTX moeoX1;
       moeoX1.setNbUnits(nbUnits);
       moeoX1.setNbHeures(nbHours);
       moeoX1.setAlpha(1);
        moeoX1.setTemps(eo.getTemps());
       moeoUCPObjectiveVector objVec1;
       objVec1[0]=0;
       objVec1[1]=0;
        moeoX1.objectiveVector(objVec1);
      // cout<<"apres def objVec1"<<endl;
       // moeoX1.setTemps(eo.getTemps());
       if(eoObj[1]==0)
           eval(moeoX1);
      // cout<<"apres eval"<<endl;
       moeoUCPObjectiveVector moeoX1Obj=moeoX1.objectiveVector();
       //cout<<"moeoX1Obj"<<endl;
       //cout<<eoObj[0]<<endl;
       objVec1[0]=moeoX1Obj[0]+eoObj[0];
       objVec1[1]=moeoX1Obj[1]+eoObj[1];
       moeoX1.objectiveVector(objVec1);
       popX.push_back(moeoX1);
       eo.setPointIdeal(moeoX1Obj[0],moeoX0Obj[1]);
       eo.setPointNadir(moeoX0Obj[0],moeoX1Obj[1]);
       //cout<<"n="<<n<<endl;
       for(unsigned i=1;i<n;++i)
       {
           MOEOTX moeoX;
           moeoX.setNbUnits(nbUnits);
           moeoX.setNbHeures(nbHours);
           moeoX.setAlpha((double)i/(double)n);
           moeoX.setTemps(eo.getTemps());
           moeoX.setNorme(eo.getPointNadir(),eo.getPointIdeal());
           moeoUCPObjectiveVector objVec;
           objVec[0]=0;
           objVec[1]=0;
           moeoX.objectiveVector(objVec);
           if(eoObj[1]==0)
               eval(moeoX);
           moeoUCPObjectiveVector moeoXObj=moeoX.objectiveVector();
          // cout<<eoObj[0]<<endl;
           objVec[0]=moeoXObj[0]+eoObj[0];
           objVec[1]=moeoXObj[1]+eoObj[1];
           moeoX.objectiveVector(objVec);
           popX.push_back(moeoX);
           
       }
      // cout<<"fin decder"<<endl;
   }
      private :
      moeoUCPEvalFunc<MOEOTX> eval;
      moeoUCPEvalFuncS<MOEOT> evalS;

  };

#endif
