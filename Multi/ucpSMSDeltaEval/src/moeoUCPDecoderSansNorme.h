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
       //cout<<"deb dec"<<endl;
       // REMEMBER: type MOEOTX must inherit from class Origin
       // decode _eo and return a population of type MOEOTX
       
        
       int nbHours=eo.getNbHeures();
       int nbUnits=eo.getNbUnits();
       int n=eo.getN();
       evalS(eo);
       moeoUCPObjectiveVector eoObj=eo.objectiveVector();
       //cout<<"n="<<n<<endl;
       for(unsigned i=0;i<n+1;++i)
       {
           MOEOTX moeoX;
           moeoX.setNbUnits(nbUnits);
           moeoX.setNbHeures(nbHours);
           moeoX.setAlpha((double)i/(double)n);
           moeoX.setTemps(eo.getTemps());
            moeoUCPObjectiveVector objVec;
           objVec[0]=0;
           objVec[1]=0;
           moeoX.objectiveVector(objVec);
           if(eoObj[1]==0)
               eval(moeoX);
            moeoUCPObjectiveVector moeoXObj=moeoX.objectiveVector();
           objVec[0]=moeoXObj[0]+eoObj[0];
          objVec[1]=moeoXObj[1]+eoObj[1];
           moeoX.objectiveVector(objVec);
           popX.push_back(moeoX);
	  // const ObjectiveVector& obj = moeoX.objectiveVector();
	  // eo.addDecodedChild(obj);
           
       }
       // cout<<"fin dec"<<endl;
   }
      private :
      moeoUCPEvalFunc<MOEOTX> eval;
      moeoUCPEvalFuncS<MOEOT> evalS;

  };

#endif
