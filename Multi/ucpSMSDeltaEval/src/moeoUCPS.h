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

#ifndef _moeoUCPS_h
#define _moeoUCPS_h
#include "eoUCPUnit.h"
#include "eoUCPData.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <addToParadisEO/moeo/core/Origin.h>
#include <addToParadisEO/moeo/core/AddDecodedChildren.h>
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

class moeoUCPS: public moeoRealVector < moeoUCPObjectiveVector >, public Origin, public AddDecodedChildren< moeoUCPObjectiveVector > 
{
public:
  /** Ctor: you MUST provide a default ctor.
   * though such individuals will generally be processed
   * by some eoInit object
   */
  moeoUCPS()
  {
    // START Code of default Ctor of an moeoUCPS object
	nbHeures=0;
	nbUnits=0;
      pointIdeal.push_back(0);
      pointIdeal.push_back(0);
      pointNadir.push_back(1);
      pointNadir.push_back(1);
    // END   Code of default Ctor of an moeoUCPS object
  }
    



  virtual ~moeoUCPS()
  {
    // START Code of Destructor of an eoEASEAGenome object
    // END   Code of Destructor of an eoEASEAGenome object
  }

  virtual string className() const { return "moeoUCPS"; }

  /** printing... */
    void printOn(ostream& os) const
    {
        os<<"fitness:"<<this->fitness()<<"  diversity:    "<<this->diversity()<<"   "<< "origine : "<<this->origin()<<endl;
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
    void IniEval()
    {
        for(int i=0;i<n+1;i++)
        {
            vector<double> e1,e2;
            for(int t=0;t<nbHeures;t++)
            {
                e1.push_back(0);e2.push_back(0);
            }
            eval1.push_back(e1);
            eval2.push_back(e2);
        }
        
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
    void setModifFalse()
    {
        for(int i=0;i<nbHeures;i++)
            modif[i]=false;
    }
    void setModifTrue()
    {
        for(int i=0;i<nbHeures;i++)
            modif[i]=true;
    }
	/*double getEval(int i)
	{
		return eval[i];
	}*/
    vector<double> getEval1(int i)
	{
		return eval1[i];
	}
    vector<double> getEval2(int i)
	{
		return eval2[i];
	}
    double getEval1(int i, int j)
    {
        return eval1[i][j];
    }
    double getEval2(int i, int j)
    {
        return eval2[i][j];
    }
	/*void setEval(int i,double e)//asupprimer
	{
        eval[i]=e;
    }*/
    void setEvals(int i,vector<double> e,vector<double> e2)
	{
        eval1[i]=e;
        eval2[i]=e2;
    }

    void setEval1(int i,vector<double> e)
	{
        eval1[i]=e;
    }
    void setEval2(int i,vector<double> e)
	{
        eval2[i]=e;
    }
    void setEval1(int i,int j, double e)
    {
        eval1[i][j]=e;
    }
    void setEval2(int i,int j, double e)
    {
        eval2[i][j]=e;
    }
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
    void setN(int _n)
    {
        n=_n;
    }
    int getN()
    {
        return n;
    }
    vector<double> getPointIdeal()
    {
        return pointIdeal;
    }
    vector<double> getPointNadir()
    {
        return pointNadir;
    }
    void setPointNadir( double a, double b)
    {
        pointNadir[0]=a;
        pointNadir[1]=b;
    }
    void setPointIdeal( double a, double b)
    {
        pointIdeal[0]=a;
        pointIdeal[1]=b;
    }
private:			   // put all data here
    // START Private data of an eoUCP object
	int nbHeures;
	int nbUnits;
    vector< vector<double> > eval1;
    vector< vector<double> > eval2;
	vector<bool> modif;
	vector<double> eval;
	vector<vector<int> > temps;
    int n;
    vector<double> pointIdeal;
    vector<double> pointNadir;
    // END   Private data of an eoUCP object
};

#endif
