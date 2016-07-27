// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// moeoSteadyHyperVolumeContinue
// (c) GeNeura Team, 1999, Marc Schoenauer, 2000
/* 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
 */
//-----------------------------------------------------------------------------

#ifndef _eoSteadyHyperVolumeContinue_h
#define _eoSteadyHyperVolumeContinue_h

#include <eoContinue.h>

/** 
    A continuator:  does a minimum number of generations, then
    stops whenever a given number of generations takes place without improvement 
*/
template< class EOT>
class moeoSteadyHyperVolumeContinue: public eoContinue<EOT>
{
public:
    typedef typename EOT::ObjectiveVector ObjectiveVector;

  /// Ctor for setting a
  moeoSteadyHyperVolumeContinue( unsigned long _minGens, unsigned long _steadyGens,  moeoHyperVolumeMetric<ObjectiveVector> _hyperVol)
    : repMinGenerations( _minGens ), repSteadyGenerations( _steadyGens),hyperVol(_hyperVol),
      steadyState(false), thisGenerationPlaceHolder(0),
      thisGeneration(thisGenerationPlaceHolder){};
	


  /** Returns false when a certain number of generations is
   * reached withtout improvement */
  virtual bool operator() ( const eoPop<EOT>& _vEO ) {
    thisGeneration++;
       vector<ObjectiveVector> res;
      for(int i=0;i<_vEO.size();i++)
          res.push_back(_vEO[i].objectiveVector());
      double currentHyperVol =hyperVol(res);
    

    if (steadyState) {	   // already after MinGenenerations
      if (currentHyperVol > bestSoFar) {
	bestSoFar = currentHyperVol;
	lastImprovement = thisGeneration;
      } else {
	if (thisGeneration - lastImprovement > repSteadyGenerations) {
	  std::cout << "STOP in moeoSteadyHyperVolumeContinue: Done " << repSteadyGenerations 
	       << " generations without improvement\n apres "<<thisGeneration<<" generations"<<endl;
	  return false;
	}
      } 
    } else {		   // not yet in steady state
      if (thisGeneration > repMinGenerations) { // go to steady state
	steadyState = true;
	bestSoFar = currentHyperVol;
	lastImprovement = thisGeneration;
	std::cout << "moeoSteadyHyperVolumeContinue: Done the minimum number of generations\n";
      }
    }
    return true;
  }

  /** Sets the parameters (minimum nb of gen. + steady nb of gen.) 
      and sets the current generation to 0 (the begin)*/
  virtual void totalGenerations( unsigned long _mg, unsigned long _sg ) { 
    repMinGenerations = _mg; 
    repSteadyGenerations = _sg;
    reset();
  };

  /// Resets the state after it's been reached 
  virtual void reset () {
    steadyState=false;
    thisGeneration = 0;
  }

  /** accessors*/
  virtual unsigned long minGenerations( ) 
  {  return repMinGenerations; 	};
  virtual unsigned long steadyGenerations( ) 
  {  return repSteadyGenerations; 	};
    
  virtual std::string className(void) const { return "moeoSteadyHyperVolumeContinue"; }
private:
    moeoHyperVolumeMetric<ObjectiveVector> hyperVol;
  unsigned long repMinGenerations;
  unsigned long  repSteadyGenerations;
  bool steadyState;
  unsigned long thisGenerationPlaceHolder;
  unsigned long& thisGeneration;
  unsigned int lastImprovement;
  double bestSoFar;
};

#endif

