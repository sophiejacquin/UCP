/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutationMO_H
#define eoUCPMutationMO_H


#include <addToParadisEO/eo/eoVectOp.h>
#include <iostream>
#define TAILLE_MAX 300
using namespace std;
/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoUCPMutationMO: public eoVectOp<GenotypeT>
{
private:
// START Private data of an eoUCPMutation object
  //  varType anyVariable;	
/*!< Pointer to the vector that contains the units*/
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
    eoUCPMutationMO  (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,eoUCPData * _data,int _longueur)
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
  eoUCPMutationMO ()
  	{
  		
  		
  		longueur=0;
  	}
  	
    // END   Code of Ctor of an eoUCPEvalFunc object
  

  /// The class name. Used to display statistics
  string className() const { return "eoUCPMutationMO"; }

  /**
   * modifies the parent
   * @param _genotype The parent genotype (will be modified)
   */
  vector<GenotypeT> operator()(GenotypeT & _genotype,int i=-1) 
  {
	//cout<<"deb mut2MO"<<endl;
      vector<GenotypeT> mutants;
	  eoUniformGenerator<int> choixLambda (0,_genotype.getN()+1);
      double alpha=(double)choixLambda()/(double)_genotype.getN();
	  //choix heure début :
	  int nbHeures =_genotype.getNbHeures();
	  eoUniformGenerator<int> choixHDeb(0,nbHeures-longueur+1);
	  int hDeb=choixHDeb();
	////cout<<"hDeb="<<hDeb<<endl;
	  int l,j,k,h;
	  //choix Unité:
	  eoUniformGenerator<int> choixUnit(0,_genotype.getNbUnits());
	  if(i==-1) i=choixUnit();
	  vector<vector<int> > temps=_genotype.getTemps();
	  while(hDeb<nbHeures &&((temps[hDeb][i]<0 &&temps[hDeb][i]>tDown[i])||(temps[hDeb][i]>0 &&temps[hDeb][i]<tUp[i])))
	  {
		  hDeb++;
	  }
	  if(hDeb<nbHeures)
	  {
		  if(hDeb+longueur>nbHeures)l=nbHeures-hDeb;
		else l=longueur;
		double total=0;
		  double graphe[l][tUp[i]-tColdStart[i]][4][TAILLE_MAX];
		for(h=0;h<l;h++)
		{
			for(j=0;j<TAILLE_MAX;j++)
			for(k=0;k<tUp[i]-tColdStart[i];k++)
			{ graphe[h][k][1][j]=-2;
				graphe[h][k][0][0]=0;
			}	
		}
		  //Etat ini:
		  for(j=0;j<tUp[i]-tColdStart[i];j++)
		  {
			  graphe[0][j][1][0]=0;
			  graphe[0][j][0][0]=0;
			  graphe[0][j][2][0]=0;
			graphe[0][j][3][0]=0;
		  }
		  if( temps[hDeb][i]>0)
		  {
			  graphe[0][temps[hDeb][i]-1][0][0]=1;
			graphe[0][temps[hDeb][i]-1][0][1]=0;
			graphe[0][temps[hDeb][i]-1][1][0]=1;
			graphe[0][temps[hDeb][i]-1][1][1]=1;
			graphe[0][temps[hDeb][i]-1][2][0]=1;
			graphe[0][temps[hDeb][i]-1][2][1]=0;
			graphe[0][temps[hDeb][i]-1][3][1]=0;
			//if(i==2) //cout<<"TEMPSDEB "<<temps[hDeb][i]-1<<endl;
		  }
		  else{ 
			graphe[0][tUp[i]-1-temps[hDeb][i]][0][0]=1;
			graphe[0][tUp[i]-1-temps[hDeb][i]][0][1]=0;
			graphe[0][tUp[i]-1-temps[hDeb][i]][1][0]=1;
			graphe[0][tUp[i]-1-temps[hDeb][i]][1][1]=0;
			graphe[0][tUp[i]-1-temps[hDeb][i]][2][0]=1;
			graphe[0][tUp[i]-1-temps[hDeb][i]][2][1]=0;
			graphe[0][tUp[i]-1-temps[hDeb][i]][3][1]=0;
		}
		//cout<<"etat ini fin"<<endl;
		  //Les autres états :
		  for( h=1;h<l;h++)
		  {
				////cout<<"cas classique i="<<i<<endl;
			  //cas classiques:
			  for(int j=1;j<tUp[i]-tColdStart[i];j++)
			  {
				graphe[h][j][0][0]=0;
				  if(graphe[h-1][j-1][0][0]>0 ){
					double mu=0;
					double var=0;
					////cout<<"av eval"<<endl;
					eval(j,j-1,h+hDeb,i,_genotype,alpha,mu,var);
					////cout<<"ap eval"<<endl;
					for(k=1;k<graphe[h-1][j-1][0][0]+1 && k<TAILLE_MAX;k++)
					{
						////cout<<"k="<<k<<" h="<<h<<" j="<<j<<endl;

					 	 graphe[h][j][2][k]=mu+graphe[h-1][j-1][2][k];
						graphe[h][j][3][k]=var+graphe[h-1][j-1][3][k];
					 // if(_genotype.getModif(hDeb+l-1)==true ||graphe[h][j][1]<=total)
					 	graphe[h][j][0][k]=j-1;
						graphe[h][j][1][k]=k;
						 graphe[h][j][0][0]=graphe[h][j][0][0]+1;
						
					
					}

				  }
				  /*else {
					  graphe[h][j][0][0]=0;

				  }*/
			//cout<<"cas classique graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;

			  }
			  //autres cas :
//cout<<"cas1"<<endl; 
			  //cas :1 :
			  graphe[h][0][0][0]=0;
			  for(int j=tUp[i]-1-tDown[i];j<tUp[i]-tColdStart[i];j++)
			  {
				//cout<<" cas 1 deb graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;
				  if(graphe[h-1][j][0][0]>0)
				  {
					double mu=0;
					double var=0;
					eval(0,j,h+hDeb,i,_genotype,alpha,mu ,var);
					for(k=1;k<graphe[h-1][j][0][0]+1&& k<TAILLE_MAX;k++)
					{
						bool domine=false;
						bool dominant=false;
						int zob=1;
					  while(zob<graphe[h][0][0][0]+1 && zob<TAILLE_MAX&& !domine )
					  {
						if(mu+graphe[h-1][j][2][k]>=graphe[h][0][2][zob] &&var+graphe[h-1][j][3][k]>=graphe[h][0][3][zob])
						domine=true; 
						else if(mu+graphe[h-1][j][2][k]<graphe[h][0][2][zob] &&var+graphe[h-1][j][3][k]<graphe[h][0][3][zob])
						{
							if(!dominant){
							graphe[h][0][2][zob]=mu+graphe[h-1][j][2][k];
							graphe[h][0][3][zob]=var+graphe[h-1][j][3][k];
							graphe[h][0][0][zob]=j;
							graphe[h][0][1][zob]=k;
							dominant=true;}
							else
							{
							graphe[h][0][2][zob]=graphe[h][0][2][(int)(graphe[h][0][0][0])];
							graphe[h][0][3][zob]=graphe[h][0][3][(int)(graphe[h][0][0][0])];
							graphe[h][0][0][zob]=graphe[h][0][0][(int)(graphe[h][0][0][0])];
							graphe[h][0][1][zob]=graphe[h][0][1][(int)(graphe[h][0][0][0])];
							graphe[h][0][0][0]--;	
							zob--;							
							}
							
							
						}

						zob++;
					  }
					//cout<<" cas 1 ap while graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;
					  if(!domine && !dominant && graphe[h][0][0][0]+1<TAILLE_MAX)
					  {
							graphe[h][0][2][(int)(graphe[h][0][0][0])+1]=mu+graphe[h-1][j][2][k];
							graphe[h][0][3][(int)(graphe[h][0][0][0])+1]=var+graphe[h-1][j][3][k];
							graphe[h][0][0][(int)(graphe[h][0][0][0])+1]=j;
							graphe[h][0][1][(int)(graphe[h][0][0][0])+1]=k;
							graphe[h][0][0][0]+=1;
					  }
					}
				  }
				//cout<<" cas 1 graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;
			  }
//cout<<"castUp"<<endl;
			  //cas tUp:
			  if(graphe[h-1][tUp[i]-1][0][0]>0)
			  {
				  double mu=0;
				  double var=0;
				  eval(tUp[i]-1,tUp[i]-1,h+hDeb,i,_genotype,alpha,mu,var);

				  for(k=1;k<graphe[h-1][tUp[i]-1][0][0]+1&& k<TAILLE_MAX;k++)
					{
						bool domine=false;
						bool dominant=false;
						int zob=1;
					  while(zob<graphe[h][tUp[i]-1][0][0]+1 && zob<TAILLE_MAX&& !domine )
					  {
						if(mu+graphe[h-1][tUp[i]-1][2][k]>=graphe[h][tUp[i]-1][2][zob] &&var+graphe[h-1][tUp[i]-1][3][k]>=graphe[h][tUp[i]-1][3][zob])
						domine=true; 
						else if(mu+graphe[h-1][tUp[i]-1][2][k]<graphe[h][tUp[i]-1][2][zob] &&var+graphe[h-1][tUp[i]-1][3][k]<graphe[h][tUp[i]-1][3][zob])
						{
							if(!dominant){
							graphe[h][tUp[i]-1][2][zob]=mu+graphe[h-1][tUp[i]-1][2][k];
							graphe[h][tUp[i]-1][3][zob]=var+graphe[h-1][tUp[i]-1][3][k];
							graphe[h][tUp[i]-1][0][zob]=tUp[i]-1;
							graphe[h][tUp[i]-1][1][zob]=k;
							dominant=true;}
							else
							{
							graphe[h][tUp[i]-1][2][zob]=graphe[h][tUp[i]-1][2][(int)(graphe[h][tUp[i]-1][0][0])];
							graphe[h][tUp[i]-1][3][zob]=graphe[h][tUp[i]-1][3][(int)(graphe[h][tUp[i]-1][0][0])];
							graphe[h][tUp[i]-1][0][zob]=graphe[h][tUp[i]-1][0][(int)(graphe[h][tUp[i]-1][0][0])];
							graphe[h][tUp[i]-1][1][zob]=graphe[h][tUp[i]-1][1][(int)(graphe[h][tUp[i]-1][0][0])];
							graphe[h][tUp[i]-1][0][0]--;
							zob--;								
							}
							
							
						}

						zob++;
					  }
					  if(!domine && !dominant && graphe[h][tUp[i]-1][0][0]+1<TAILLE_MAX)
					  {
							graphe[h][tUp[i]-1][2][(int)(graphe[h][tUp[i]-1][0][0])+1]=mu+graphe[h-1][tUp[i]-1][2][k];
							graphe[h][tUp[i]-1][3][(int)(graphe[h][tUp[i]-1][0][0])+1]=var+graphe[h-1][tUp[i]-1][3][k];
							graphe[h][tUp[i]-1][0][(int)(graphe[h][tUp[i]-1][0][0])+1]=tUp[i]-1;
							graphe[h][tUp[i]-1][1][(int)(graphe[h][tUp[i]-1][0][0])+1]=k;
							graphe[h][tUp[i]-1][0][0]+=1;
					  }
					}
				//cout<<"graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;
			  }
			
//cout<<"casTCS"<<endl;
			  //cas tCS :
			  if(graphe[h-1][tUp[i]-tColdStart[i]-1][0][0]>0)
			  {								 
				  double mu=0;
				  double var=0;
				  eval(tUp[i]-tColdStart[i]-1,tUp[i]-tColdStart[i]-1,h+hDeb,i,_genotype,alpha,mu,var);
				
				  for(k=1;k<graphe[h-1][tUp[i]-tColdStart[i]-1][0][0]+1&& k<TAILLE_MAX;k++)
					{
						bool domine=false;
						bool dominant=false;
						int zob=1;
					  while(zob<graphe[h][tUp[i]-tColdStart[i]-1][0][0]+1&& zob<TAILLE_MAX && !domine)
					  {
						if(mu+graphe[h-1][tUp[i]-tColdStart[i]-1][2][k]>=graphe[h][tUp[i]-tColdStart[i]-1][2][zob] &&var+graphe[h-1][tUp[i]-tColdStart[i]-1][3][k]>=graphe[h][tUp[i]-tColdStart[i]-1][3][zob])
						domine=true; 
						else if(mu+graphe[h-1][tUp[i]-tColdStart[i]-1][2][k]<graphe[h][tUp[i]-tColdStart[i]-1][2][zob] &&var+graphe[h-1][tUp[i]-tColdStart[i]-1][3][k]<graphe[h][tUp[i]-tColdStart[i]-1][3][zob])
						{
							if(!dominant){
							graphe[h][tUp[i]-tColdStart[i]-1][2][zob]=mu+graphe[h-1][tUp[i]-tColdStart[i]-1][2][k];
							graphe[h][tUp[i]-tColdStart[i]-1][3][zob]=var+graphe[h-1][tUp[i]-tColdStart[i]-1][3][k];
							graphe[h][tUp[i]-tColdStart[i]-1][0][zob]=tUp[i]-tColdStart[i]-1;
							graphe[h][tUp[i]-tColdStart[i]-1][1][zob]=k;
							dominant=true;}
							else
							{
							graphe[h][tUp[i]-tColdStart[i]-1][2][zob]=graphe[h][tUp[i]-tColdStart[i]-1][2][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])];
							graphe[h][tUp[i]-tColdStart[i]-1][3][zob]=graphe[h][tUp[i]-tColdStart[i]-1][3][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])];
							graphe[h][tUp[i]-tColdStart[i]-1][0][zob]=graphe[h][tUp[i]-tColdStart[i]-1][0][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])];
							graphe[h][tUp[i]-tColdStart[i]-1][1][zob]=graphe[h][tUp[i]-tColdStart[i]-1][1][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])];
							graphe[h][tUp[i]-tColdStart[i]-1][0][0]--; zob--;								
							}
							
						}

						zob++;
					  }
					  if(!domine && !dominant && graphe[h][tUp[i]-tColdStart[i]-1][0][0]+1<TAILLE_MAX)
					  {
							graphe[h][tUp[i]-tColdStart[i]-1][2][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])+1]=mu+graphe[h-1][tUp[i]-tColdStart[i]-1][2][k];
							graphe[h][tUp[i]-tColdStart[i]-1][3][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])+1]=var+graphe[h-1][tUp[i]-tColdStart[i]-1][3][k];
							graphe[h][tUp[i]-tColdStart[i]-1][0][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])+1]=tUp[i]-tColdStart[i]-1;
							graphe[h][tUp[i]-tColdStart[i]-1][1][(int)(graphe[h][tUp[i]-tColdStart[i]-1][0][0])+1]=k;
							graphe[h][tUp[i]-tColdStart[i]-1][0][0]+=1;
					  }
					}
			  }

		  }
		
		  //Restitution des résultats : (on pourra reduire la taille du graphe)
		  //cas hDeb+l=nbHeures:
		////cout<<"restitution "<<hDeb+l<<" nbHeures "<<nbHeures<<endl;
		  if(hDeb+l>nbHeures-1)//-1?
		  {
			//constitution du front pareto :
			vector<vector <int> > front;
       // cout<<"ici MO"<<endl;
			  for(k=1;k<graphe[l-1][0][0][0]+1&& k<TAILLE_MAX;k++)
			  {
				vector <int> element;
				element.push_back(0);
				element.push_back(k);
				front.push_back(element);
			  }
				////cout<<"fin pour1"<<endl;
			  for(j=1;j<tUp[i]-tColdStart[i];j++)
			  {

				if(front.size()==0)
				{
					////cout<<"si"<<endl;
					  for(k=1;k<graphe[l-1][j][0][0]+1&& k<TAILLE_MAX;k++)
			  		{
						vector <int> element;
						element.push_back(j);
						element.push_back(k);
						front.push_back(element);
			 	 	 }
					////cout<<"fin pour si"<<endl;
				}
				else{
					////cout<<"sinon"<<endl;
					 for(k=1;k<graphe[l-1][j][0][0]+1&& k<TAILLE_MAX;k++)
					{
						bool domine=false;
						
						for(int temp=0; temp<front.size() && !domine;temp++)
						{
							////cout<<front.size()<<" temp="<<temp<<endl<<" front[temp][0]="<<front[temp][0]<<" front[temp][1]="<<front[temp][1]<<endl;
							if(graphe[l-1][j][2][k]>= graphe[l-1][front[temp][0]][2][front[temp][1]] && graphe[l-1][j][3][k]>=graphe[l-1][front[temp][0]][3][front[temp][1]]) 
								domine=true;
							else if (graphe[l-1][j][2][k]<= graphe[l-1][front[temp][0]][2][front[temp][1]] && graphe[l-1][j][3][k]<=graphe[l-1][front[temp][0]][3][front[temp][1]]) 
							{
								//suppression élement dominé du front :
								front.erase(front.begin()+ temp);
								temp--;

							}
						}
						////cout<<"fin pour interne"<<endl;
						if(!domine)
						{
							vector<int> el;
							el.push_back(j);
							el.push_back(k);
							front.push_back(el);
						}
						
		
					}
					////cout<<"fin pour sinon"<<endl;
				}
			  }
			//cout<<"fin pour 2"<<endl;
			////cout<<s;
			//choix aléatoire d'une solution :
			//eoUniformGenerator <int> choixSol(0,front.size());
			//int s=choixSol();
              //cout<<"taille front :"<<front.size()<<endl;
              for(int s=0;s<front.size();s++)
              {
                  GenotypeT mutant= _genotype;
			int etat=front[s][0];
			int num=front[s][1];
			//cout<<"front constitué et sol choisie"<<endl;
			  for(j=hDeb+l;j>hDeb;j--)
			 {
							//cout<<"ici"<<endl;
			 				  int t;
			 				  int Etatpred=graphe[j-hDeb-1][etat][0][num];
							  int numPred=graphe[j-hDeb-1][etat][1][num];
                        
			 				  if(etat<tUp[i])t=etat+1;
			 				  else t=tUp[i]-etat-1;
			 				 mutant.setTemps(j-1,i,t);
			 				  etat=Etatpred;
							  //cout<<"j="<<j<<"etat="<<etat<<endl;
							  num=numPred;
			 }
                  mutant.invalidate();
                  mutant.setModifTrue();
                  mutants.push_back(mutant);

		  }
          }
		  //Sinon:reprendre de là
		  else{
			//cout<<"sinon"<<endl;
			  int tempsFin=temps[hDeb+l-1][i];
			  int etat;
			  if(tempsFin>0)
			  {
				  	   etat=tempsFin-1;

			  }
			  else{
				  	  etat=tUp[i]-tempsFin -1;
			  }
              //cout<<"taille :"<<graphe[l-1][etat][0][0]+1<<endl;
              for(int num=1; num<graphe[l-1][etat][0][0]+1;num++)
            {
				////cout<<s<<" "<<hDeb<<" "<<l<<endl;
                GenotypeT mutant= _genotype;
			  for(j=hDeb+l;j>hDeb;j--)
			 {
			 	 int t;
			 	int Etatpred=graphe[j-hDeb-1][etat][num][0];
				int numPred=graphe[j-hDeb-1][etat][num][1];
			 	  if(etat<tUp[i])t=etat+1;
			 	if(etat<tUp[i])t=etat+1;
			 	else t=tUp[i]-etat-1;
                 mutant.setTemps(j-1,i,t);
			 	etat=Etatpred;
				num=numPred;
			 }
                mutant.invalidate();
                mutant.setModifTrue();
                mutants.push_back(mutant);
            }
		  }

	  }
      _genotype.setModifTrue();
      //cout<<"finMut2MO"<<endl;
      return mutants;
    // END code for mutation of the _genotype object
  }
  
   void eval(int actu,int pred,int h,int i,GenotypeT & _genotype,double alpha, double & prod, double & em)
    {
        //cout<<"debut eval"<<endl;
        double eval=0;
        double  penalites,SC;
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
        
        if(penalites==0)  fuelCost(_genotype,h,i,tps,alpha,prod,em);
        prod+=SC+penalites;
        em+=penalites;
        //cout<<penalites<<endl;
        //cout<<"fin eval"<<endl;
        
        
        
        
    }
    
    double fuelCost(GenotypeT & _eo,int h,int usine,int tps,double alpha, double & ec, double & em)
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
               
                ec+=fc;
                em+=ec;
            }
        }
 	  	
        
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
