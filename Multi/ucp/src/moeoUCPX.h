/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for creating a new representation in EO
================================================

Mandatory:
- a default constructor (constructor without any argument)
- the I/O functions (readFrom and printOn)

However, if you are using dynamic memory, there are 2 places
to allocate it: the default constructor (if possible?), or, more in
the EO spirit, the eoInit object, that you will need to write anyway
(template file init.tmpl).

But remember that a COPY CONSTRUCTOR will be used in many places in EO,
so make sure that the default copy constructor works, or, even better,
do write your own if in doubt.
And of course write the corresponding destructor!

*/

#ifndef _moeoUCPX_h
#define _moeoUCPX_h
#include "eoUCPUnit.h"
#include "eoUCPData.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include<float.h>

#include <addToParadisEO/moeo/core/Origin.h>
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen

 * Note that you MUST derive your structure from EO<fitT>
 * but you MAY use some other already prepared class in the hierarchy
 * like eoVector for instance, if you handle a vector of something....

 * If you create a structure from scratch,
 * the only thing you need to provide are
 *        a default constructor
 *        IO routines printOn and readFrom
 *
 * Note that operator<< and operator>> are defined at EO level
 * using these routines
 */
class moeoUCPX: public moeoRealVector < moeoUCPObjectiveVector >, public Origin{
public:
  /** Ctor: you MUST provide a default ctor.
   * though such individuals will generally be processed
   * by some eoInit object
   */
  moeoUCPX()
  {
    // START Code of default Ctor of an moeoUCPX object
	nbHeures=0;
	nbUnits=0;
      pointNadir.push_back(1);
      pointNadir.push_back(1);
      pointIdeal.push_back(0);
      pointIdeal.push_back(0);
    // END   Code of default Ctor of an moeoUCPX object
  }




  virtual ~moeoUCPX()
  {
    // START Code of Destructor of an eoEASEAGenome object
    // END   Code of Destructor of an eoEASEAGenome object
  }

  virtual string className() const { return "moeoUCPX"; }

  /** printing... */
    void printOn(ostream& os) const
    {
        //if(this->fitness()<4 &&1<this->fitness())
       //   if( this->diversity()==DBL_MAX)
        os<<this->objectiveVector()[0]<<"   "<<this->objectiveVector()[1]<<"   "<<this->origin()<<endl;
	
	/** HINTS
	 * in EO we systematically write the sizes of things before the things
	 * so readFrom is easier to code (see below)
	 */

    // END   Code of default output
    }

  /** reading...
   * of course, your readFrom must be able to read what printOn writes!!!
   */
    void readFrom(istream& is)
      {
	// of course you should read the fitness first!
	
    // START Code of input

	/** HINTS
	 * remember the eoUCP object will come from the default ctor
	 * this is why having the sizes written out is useful
	 */

    // END   Code of input
      }
	void adPeriode(vector<int> etat)
	{
		temps.push_back(etat);
		modif.push_back(true);
		eval.push_back(0);
		nbHeures++;
	}
	int getNbHeures()
	{
		return nbHeures;
	}
	int getNbUnits()
	{
		return nbUnits;
	}
	void setNbUnits(int _nbUnits)
	{
		nbUnits=_nbUnits;
	}
    void setNbHeures(int _nbUnits)
	{
		nbHeures=_nbUnits;
	}
	const vector<bool>& getModif()
	{
		return modif;
	}
	bool getModif(int i)
	{
		return modif[i];
	}
	void setModif(int i,bool b)
	{
		modif[i]=b;
	}
	double getEval(int i)
	{
		return eval[i];
	}
	void setEval(int i,double e)
	{eval[i]=e;}
	int getTemps(int i,int j)
	{
		return temps[i][j];
	}
	const vector<vector<int> > & getTemps()
	{
		return temps;
	}
	void setTemps(int i, int j, int t)
	{
		temps[i][j]=t;
	}
    void setTemps(vector<vector<int> > _temps)
    {
        temps=_temps;
    }
    double getAlpha() {
        return alpha;
        
    }
    void setAlpha(double _alpha) {
        alpha=_alpha;
    }
    void setNorme(vector<double> _pointNadir, vector<double> _pointIdeal)
    {
        pointNadir=_pointNadir;
        pointIdeal=_pointIdeal;
    }
    vector<double> getPointIdeal()
    {
        return pointIdeal;
    }
    vector<double> getPointNadir()
    {
        return pointNadir;
    }
private:			   // put all data here
    // START Private data of an eoUCP object
	int nbHeures;
	int nbUnits;
	vector<bool> modif;
	vector<double> eval;
	vector<vector<int> > temps;
    vector<double> pointNadir;
    vector<double> pointIdeal;
    double alpha;
    // END   Private data of an eoUCP object
};

#endif
