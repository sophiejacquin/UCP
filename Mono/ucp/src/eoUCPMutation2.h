/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutation2_H
#define eoUCPMutation2_H


#include <eoOp.h>
#include <iostream>
using namespace std;
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPMutation2: public eoMonOp<GenotypeT>
{
private:
// START Private data of an eoUCPMutation object
  //  varType anyVariable;	
std::vector<eoUCPUnit> * units; /*!< Pointer to the vector that contains the units*/
  std::vector<double> * load; /*!< Pointer to the vector that contains the hourly demand*/
	std::vector<double> * reserve;
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;
	int longueur;

// END   Private data of an eoUCPMutation object
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPMutation2 (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,eoUCPData * _data,int _longueur)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
		if(_data!=NULL){
     		 units=_data->get_units();
      		load=_data->get_load();
		reserve=_data->get_reserve();
		}
		longueur=_longueur;
	}
  eoUCPMutation2 ()
  	{
  		
  		
  		longueur=0;
  	}
  	
    // END   Code of Ctor of an eoUCPEvalFunc object
  

  /// The class name. Used to display statistics
  string className() const { return "eoUCPMutation"; }

  /**
   * modifies the parent
   * @param _genotype The parent genotype (will be modified)
   */
  bool operator()(GenotypeT & _genotype)
  {
	//cout<<"deb mut2"<<endl;
	  bool isModified(true);
	  //choix heure début :
	  int nbHeures =_genotype.getNbHeures();
	  eoUniformGenerator<int> choixHDeb(0,nbHeures-longueur+1);
      eoUniformGenerator<double> choixLambda(0,1);
      double alpha=choixLambda();
	  int hDeb=choixHDeb();
	  int l,j;
	  //choix Unité:
	  eoUniformGenerator<int> choixUnit(0,_genotype.getNbUnits());
	  int i=choixUnit();

	  //recalcul de hDeb:
	  vector<vector<int> > temps=_genotype.getTemps();
	  while(hDeb<nbHeures &&((temps[hDeb][i]<0 &&temps[hDeb][i]>tDown[i])||(temps[hDeb][i]>0 &&temps[hDeb][i]<tUp[i])))
	  {
		  hDeb++;
	  }
	//cout<<"apres 1er while "<<i<<" "<<hDeb<<endl;
	  if(hDeb<nbHeures)
	  {
		  if(hDeb+longueur>nbHeures)l=nbHeures-hDeb;
		else l=longueur;
		double total=0;
//		for(int c=hDeb;c<hDeb+l;c++)total+=_genotype.getEval(c);

		  double graphe[l][tUp[i]-tColdStart[i]][2];
		  //Etat ini:
		  for(j=0;j<tUp[i]-tColdStart[i];j++)
		  {
			  graphe[0][j][1]=0;
			  graphe[0][j][0]=-1;
		  }
		  if( temps[hDeb][i]>0)
		  {
			  graphe[0][temps[hDeb][i]-1][0]=0;
			//if(i==2) cout<<"TEMPSDEB "<<temps[hDeb][i]-1<<endl;
		  }
		  else graphe[0][tUp[i]-1-temps[hDeb][i]][0]=0;
		//cout<<"etat ini fin"<<endl;
		  //Les autres états :
		  for(int h=1;h<l;h++)
		  {
			  //cas classiques:
			  for(int j=1;j<tUp[i]-tColdStart[i];j++)
			  {
				  if(graphe[h-1][j-1][0]>-1 ){
					  graphe[h][j][1]=eval(j,j-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][j-1][1];
					 // if(_genotype.getModif(hDeb+l-1)==true ||graphe[h][j][1]<=total)
					 graphe[h][j][0]=j-1;
				  }
				  else {
					  graphe[h][j][0]=-1;

				  }

			  }
			  //autres cas :
			  //cas :1 :
			  graphe[h][0][0]=-1;
			  for(int j=tUp[i]-1-tDown[i];j<tUp[i]-tColdStart[i];j++)
			  {
				  if(graphe[h-1][j][0]>-1)
				  {
					  double e=eval(0,j,h+hDeb,i,_genotype,alpha)+graphe[h-1][j][1];
					  if(e<graphe[h][0][1]||graphe[h][0][0]==-1)
					  {
						  graphe[h][0][1]=e;
						 // if(_genotype.getModif(hDeb+l-1)==true ||e<=total)
						 graphe[h][0][0]=j;
					  }
				  }
			  }
			  //cas tUp:
			  if(graphe[h-1][tUp[i]-1][0]>-1)
			  {
				  double e=eval(tUp[i]-1,tUp[i]-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][tUp[i]-1][1];
				  if(e<graphe[h][tUp[i]-1][1]||graphe[h][tUp[i]-1][0]<0)
				  {
					  graphe[h][tUp[i]-1][1]=e;
					  graphe[h][tUp[i]-1][0]=tUp[i]-1;
				  }
			  }
			  //cas tCS :
			  if(graphe[h-1][tUp[i]-tColdStart[i]-1][0]>-1)
			  {
				  double e = eval(tUp[i]-tColdStart[i]-1,tUp[i]-tColdStart[i]-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][tUp[i]-tColdStart[i]-1][1];
				  if(e<graphe[h][tUp[i]-tColdStart[i]-1][1]||graphe[h][tUp[i]-tColdStart[i]-1][0]<0)
				  {
					  graphe[h][tUp[i]-tColdStart[i]-1][1]=e;
					  graphe[h][tUp[i]-tColdStart[i]-1][0]=tUp[i]-tColdStart[i]-1;
				  }
			  }

		  }
		  //Restitution des résultats : (on pourra reduire la taille du graphe)
		  //cas hDeb+l=nbHeures:
		//cout<<"restitution "<<hDeb+l<<endl;
		  if(hDeb+l>nbHeures)
		  {
			//cout<<"ici"<<endl;
			  int s=0;
			  for(j=1;j<tUp[i]-tColdStart[i];j++)
			  {
				  if ((graphe[l-1][j][0]<0)||(graphe[l-1][j][1]<graphe[l-1][s][1] &&graphe[l-1][j][0]>0))
					  s=j;
			  }
			//cout<<s;
			  for(j=hDeb+l;j>hDeb;j--)
			 {
			 				  int t;
			 				  int pred=graphe[j-hDeb-1][s][0];
							  double eval;
			 				  if (j>hDeb+1) eval=graphe[j-hDeb-1][s][1]-graphe[j-hDeb-2][pred][1];
			 				  else eval=_genotype.getEval(hDeb);
			 				  if(s<tUp[i])t=s+1;
			 				  else t=tUp[i]-s-1;
								//if(t==0) cout<<"MERCREI"<<endl;
			 				  _genotype.setTemps(j-1,i,t);
			 				  _genotype.setEval(j-1,eval);
							  _genotype.setModif(j-1,true);
			 				  s=pred;
			 }

		  }
		  //Sinon:
		  else{
			  int tempsFin=temps[hDeb+l-1][i];
			  int s;
			  if(tempsFin>0)
			  {
				  	   s=tempsFin-1;

			  }
			  else{
				  	  s=tUp[i]-tempsFin -1;
			  }
				//cout<<s<<" "<<hDeb<<" "<<l<<endl;
			  for(j=hDeb+l;j>hDeb;j--)
			  {
				  int t;
				  int pred=graphe[j-hDeb-1][s][0];
				
				   double eval;
				  if (j>hDeb+1) eval=graphe[j-hDeb-1][s][1]-graphe[j-hDeb-2][pred][1];
				  else eval=_genotype.getEval(hDeb);
					
				  if(s<tUp[i])t=s+1;
				  else t=tUp[i]-s-1;
				
	
				  _genotype.setTemps(j-1,i,t);
				  _genotype.setEval(j-1,eval);
				_genotype.setModif(j-1,true);
				  s=pred;
				
			  }
		  }

	  }
	

    return isModified;
	
    // END code for mutation of the _genotype object
  }
  bool operator()(GenotypeT & _genotype,int num)
   {
 	//cout<<"deb mut2"<<endl;
 	  bool isModified(true);
 	  //choix heure début :
 	  int nbHeures =_genotype.getNbHeures();
 	  eoUniformGenerator<int> choixHDeb(0,nbHeures-longueur+1);
       eoUniformGenerator<double> choixAlpha(0,1);
       double alpha=choixAlpha();
 	  int hDeb=choixHDeb();
 	  int l,j;
 	  //choix Unité:
 	  
 	  int i=num;
 	//cout<<"i = "<<i<<endl;
 	  //recalcul de hDeb:
 	  vector<vector<int> > temps=_genotype.getTemps();
 	  while(hDeb<nbHeures &&((temps[hDeb][i]<0 &&temps[hDeb][i]>tDown[i])||(temps[hDeb][i]>0 &&temps[hDeb][i]<tUp[i])))
 	  {
 		  hDeb++;
 	  }
 	//cout<<"apres 1er while "<<i<<" "<<hDeb<<endl;
 	  if(hDeb<nbHeures)
 	  {
 		  if(hDeb+longueur>nbHeures)l=nbHeures-hDeb;
 		else l=longueur;
 		double total=0;
 //		for(int c=hDeb;c<hDeb+l;c++)total+=_genotype.getEval(c);

 		  double graphe[l][tUp[i]-tColdStart[i]][2];
 		  //Etat ini:
 		  for(j=0;j<tUp[i]-tColdStart[i];j++)
 		  {
 			  graphe[0][j][1]=0;
 			  graphe[0][j][0]=-1;
 		  }
 		  if( temps[hDeb][i]>0)
 		  {
 			  graphe[0][temps[hDeb][i]-1][0]=0;
 			//if(i==2) cout<<"TEMPSDEB "<<temps[hDeb][i]-1<<endl;
 		  }
 		  else graphe[0][tUp[i]-1-temps[hDeb][i]][0]=0;
 		//cout<<"etat ini fin"<<endl;
 		  //Les autres états :
 		  for(int h=1;h<l;h++)
 		  {
 			  //cas classiques:
 			  for(int j=1;j<tUp[i]-tColdStart[i];j++)
 			  {
 				  if(graphe[h-1][j-1][0]>-1 ){
 					  graphe[h][j][1]=eval(j,j-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][j-1][1];
 					 // if(_genotype.getModif(hDeb+l-1)==true ||graphe[h][j][1]<=total)
 					 graphe[h][j][0]=j-1;
 				  }
 				  else {
 					  graphe[h][j][0]=-1;

 				  }

 			  }
 			  //autres cas :
 			  //cas :1 :
 			  graphe[h][0][0]=-1;
 			  for(int j=tUp[i]-1-tDown[i];j<tUp[i]-tColdStart[i];j++)
 			  {
 				  if(graphe[h-1][j][0]>-1)
 				  {
 					  double e=eval(0,j,h+hDeb,i,_genotype,alpha)+graphe[h-1][j][1];
 					  if(e<graphe[h][0][1]||graphe[h][0][0]==-1)
 					  {
 						  graphe[h][0][1]=e;
 						 // if(_genotype.getModif(hDeb+l-1)==true ||e<=total)
 						 graphe[h][0][0]=j;
 					  }
 				  }
 			  }
 			  //cas tUp:
 			  if(graphe[h-1][tUp[i]-1][0]>-1)
 			  {
 				  double e=eval(tUp[i]-1,tUp[i]-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][tUp[i]-1][1];
 				  if(e<graphe[h][tUp[i]-1][1]||graphe[h][tUp[i]-1][0]<0)
 				  {
 					  graphe[h][tUp[i]-1][1]=e;
 					  graphe[h][tUp[i]-1][0]=tUp[i]-1;
 				  }
 			  }
 			  //cas tCS :
 			  if(graphe[h-1][tUp[i]-tColdStart[i]-1][0]>-1)
 			  {
 				  double e = eval(tUp[i]-tColdStart[i]-1,tUp[i]-tColdStart[i]-1,h+hDeb,i,_genotype,alpha)+graphe[h-1][tUp[i]-tColdStart[i]-1][1];
 				  if(e<graphe[h][tUp[i]-tColdStart[i]-1][1]||graphe[h][tUp[i]-tColdStart[i]-1][0]<0)
 				  {
 					  graphe[h][tUp[i]-tColdStart[i]-1][1]=e;
 					  graphe[h][tUp[i]-tColdStart[i]-1][0]=tUp[i]-tColdStart[i]-1;
 				  }
 			  }

 		  }
 		  //Restitution des résultats : (on pourra reduire la taille du graphe)
 		  //cas hDeb+l=nbHeures:
 		//cout<<"restitution "<<hDeb+l<<endl;
 		  if(hDeb+l>nbHeures)
 		  {
 			//cout<<"ici"<<endl;
 			  int s=0;
 			  for(j=1;j<tUp[i]-tColdStart[i];j++)
 			  {
 				  if ((graphe[l-1][j][0]<0)||(graphe[l-1][j][1]<graphe[l-1][s][1] &&graphe[l-1][j][0]>0))
 					  s=j;
 			  }
 			//cout<<s;
 			  for(j=hDeb+l;j>hDeb;j--)
 			 {
 			 				  int t;
 			 				  int pred=graphe[j-hDeb-1][s][0];
 							  double eval;
 			 				  if (j>hDeb+1) eval=graphe[j-hDeb-1][s][1]-graphe[j-hDeb-2][pred][1];
 			 				  else eval=_genotype.getEval(hDeb);
 			 				  if(s<tUp[i])t=s+1;
 			 				  else t=tUp[i]-s-1;
 								//if(t==0) cout<<"MERCREI"<<endl;
 			 				  _genotype.setTemps(j-1,i,t);
 			 				  _genotype.setEval(j-1,eval);
 							  _genotype.setModif(j-1,true);
 			 				  s=pred;
 			 }

 		  }
 		  //Sinon:
 		  else{
 			  int tempsFin=temps[hDeb+l-1][i];
 			  int s;
 			  if(tempsFin>0)
 			  {
 				  	   s=tempsFin-1;

 			  }
 			  else{
 				  	  s=tUp[i]-tempsFin -1;
 			  }
 				//cout<<s<<" "<<hDeb<<" "<<l<<endl;
 			  for(j=hDeb+l;j>hDeb;j--)
 			  {
 				  int t;
 				  int pred=graphe[j-hDeb-1][s][0];
 				//cout<<pred<<endl;
 				   double eval;
 				  if (j>hDeb+1) eval=graphe[j-hDeb-1][s][1]-graphe[j-hDeb-2][pred][1];
 				  else eval=_genotype.getEval(hDeb);
 					//cout<<"ici"<<endl;
 				  if(s<tUp[i])t=s+1;
 				  else t=tUp[i]-s-1;
 				//cout<<"là "<<j-1<<endl;
 	//if(t==0) cout<<"MERCREDI"<<endl;
 	//if(j==hDeb+1 && i==2 && t!=-5) cout<<"erreur "<<s<<" "<<t<<endl;
 				  _genotype.setTemps(j-1,i,t);
 				  _genotype.setEval(j-1,eval);
 				_genotype.setModif(j-1,true);
 				  s=pred;
 				
 			  }
 		  }

 	  }
 	

     return isModified;
 	
     // END code for mutation of the _genotype object
   }
  double eval(int actu,int pred,int h,int i,GenotypeT & _genotype,double alpha)
  {
	//cout<<"debut eval"<<endl;
	  double eval=0;
	  double fC, SC, penalites;
	  int tps;
	  int tpsPred;
	  if(actu<tUp[i]) tps=actu+1;
	  else tps=tUp[i]-actu-1;
	  if(pred<tUp[i]) tpsPred=pred+1;
	 	  else tpsPred=tUp[i]-pred-1;
	 
		//cout<<fC<<endl;;
	  SC=StartUpCostHour(_genotype,h,i,tps,tpsPred);
	//cout<<SC<<endl;
	  penalites=penalityHour(_genotype,h,i,tps);
	fC=0;
	if(penalites==0)  fC=fuelCost(_genotype,h,i,tps,alpha);
	//cout<<penalites<<endl;
	//cout<<"fin eval"<<endl;
	return penalites+fC+SC;



  }
  
  double fuelCost(GenotypeT & _eo,int h,int usine,int tps,double alpha)
   {
 	  double lamb,delt;
	//cout<<"deblambda"<<endl;
 	  lambda_delta(lamb,delt,h,_eo,usine,tps,alpha);
	//cout<<"la"<<endl;
 	  int nbUnits=(*units).size();
 	  double prod[nbUnits];
 	  double a1,a2,a0,b0,b1,b2,pi,diff,sum=0;
	
 	  	//int indice;
 	  	do
 	  	  {
 	  	    sum=0;

 	  	    for(unsigned int i=0;i<nbUnits;i++)
 	  	      {
 	  		//indice = _eo.indiceUnitAtTime(i,h);
 	  		if((_eo.getTemps(h,i)>0&& i!=usine)||(i==usine &&tps>0))
 	  		  {
 	  		    eoUCPUnit & unite =(*units)[i];
 	  		    a1=unite.get_a1();
 	  		    a2=unite.get_a2();
                  b1 = unite.get_b1();
                  b2 = unite.get_b2();
                  pi=(lamb - (alpha * a1 + (1 - alpha) * b1)) / (2*a2*alpha + (1-alpha) * 2 * b2) ;

 	  		    if(pi<unite.get_Pmin())
 	  		      pi=unite.get_Pmin();

 	  		    else if(pi>unite.get_Pmax())
 	  		      pi=unite.get_Pmax();

 	  		    prod[i]=pi;
 	  		    sum += pi;
 	  		  }
 	  	      }

 	  	    diff=(*load)[h-1]-sum;

 	  	    if(diff<0)
 	  	      lamb=lamb-delt;
 	  	    else
 	  	      lamb=lamb+delt;

 	  	    delt=delt*0.5;
 	  	  }
 	  	while(fabs(diff)>0.00001);
		//cout<<"fin while"<<endl;
 	  	double res=0;
 	  	for(unsigned int i=0;i<nbUnits;i++)
 	   {
 	  		if((_eo.getTemps(h,i)>0&& i!=usine)||(i==usine &&tps>0))
 	  		{
                eoUCPUnit & unite =(*units)[i];
                a1=unite.get_a1();
	  		    a2=unite.get_a2();
	  			a0=unite.get_a0();
                b1=unite.get_b1();
	  		    b2=unite.get_b2();
	  			b0=unite.get_b0();
	  			double p=prod[i];
                double fc=(a2*p+a1)*p+a0;
	  			double ec=(b2*p+b1)*p+b0; 	
                 res+= alpha* fc+(1-alpha)*ec;
            }
 	   }
 	  	return res;

   }
  double StartUpCostHour(GenotypeT & _eo, int h,int usine,int tps, int tpsPred)
   {
 	  int nbUnits=(*units).size();
 	  unsigned i;
 	  double cost=0;
 	  for(i=0;i<nbUnits;i++)
 	  {
 		  if((i!=usine && _eo.getTemps(h,i)==1)||(i==usine &&tps==1))
 		  {
 			  if((i!=usine &&_eo.getTemps(h-1,i)>-(*units)[i].get_tColdStart())||(i==usine && tpsPred>-(*units)[i].get_tColdStart()))
 			  {
 				  cost=cost+(*units)[i].get_sHot();
 			  }
 			  else cost=cost+(*units)[i].get_sCold();
 		  }
 	  }
 	  return cost;
   }
  double penalityHour(GenotypeT & _eo,int h,int unit,int tps)
    {
  	  //Test existence :
  	  //retour :

  	      double Pmax,Pmin,sumPmin=0, sumPmax=0,demandReserve=0;
  	      int nbUnits=(*units).size();
  	      for(unsigned i=0;i<nbUnits;i++)
  	        {
  	  	eoUCPUnit & unite = (*units)[i];
  	  	Pmax = unite.get_Pmax();
  	  	Pmin = unite.get_Pmin();


  	  		if((_eo.getTemps(h,i)>0&& i!=unit)||(i==unit &&tps>0))
  	  		{
  	  			sumPmax+=Pmax;
  	  			sumPmin+=Pmin;
  	  		}
  	        }

  	      demandReserve= (*load)[h-1]+(*reserve)[h-1];
  	     if (sumPmax >=demandReserve && sumPmin<=demandReserve)
  	     {
  	    	 return 0;
  	     }
  	     else return 1000000;

    }
 void lambda_delta(double & lambda,double & delta,unsigned int h,GenotypeT & _eo,int usine,int tps,double alpha)
   {
     double res=0.0;

     int Pmax=0;
     int Pmin=0;

     int nbUnits=(*units).size();
  //   std::vector<bool> * mbits = _eo.get_p_mbits();

     double a1=0;
     double a2=0;
       double b1=0;
       double b2=0;

     double lmin=numeric_limits<double>::max();
     double lmax=0;

     double lminTmp=0;
     double lmaxTmp=0;

     for(unsigned  i=0;i<nbUnits;i++)
       {
 	eoUCPUnit & unite = (*units)[i];

 	if((_eo.getTemps(h,i)>0&& i!=usine)||(i==usine &&tps>0))
 	  {
 	    Pmax = unite.get_Pmax();
 	    Pmin = unite.get_Pmin();

 	    a1 = unite.get_a1();
 	    a2 = unite.get_a2();
          b1 = unite.get_b1();
          b2 = unite.get_b2();

          lminTmp = alpha * (a1+(2*a2*Pmin)) + (1-alpha) * (b1+(2*b2*Pmin));
          lmaxTmp = alpha * (a1+(2*a2*Pmax)) + (1-alpha) * (b1+(2*b2*Pmax));
 	    if(lminTmp<lmin)
 	      lmin=lminTmp;
 	    if(lmaxTmp>lmax)
 	      lmax=lmaxTmp;
 	  }
       }
     lambda=(lmax+lmin)/2;
     delta= (lmax-lmin)/2;
   }

	
};

#endif
