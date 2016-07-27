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
#include <addToParadisEO/eo/eoVectOp.h>
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
class eoUCPMutation2: public eoVectOp<GenotypeT>
{
private:
// START Private data of an eoUCPMutation object
  //  varType anyVariable;	
/*!< Pointer to the vector that contains the units*/
 vector<eoUCPData> scenarios;
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;
	int nbScenarios;
	int longueur;

// END   Private data of an eoUCPMutation object
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPMutation2 (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,vector<eoUCPData> _scenarios,int _longueur)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
		scenarios=_scenarios;
		longueur=_longueur;
		nbScenarios=scenarios.size();
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
  vector<GenotypeT> operator()(GenotypeT & _genotype)
  {
	cout<<"deb mut2"<<endl;
	
	 // bool isModified(true);
	vector<GenotypeT> sol;
	  //choix heure début :
	  int nbHeures =_genotype.getNbHeures();
	  eoUniformGenerator<int> choixHDeb(0,nbHeures-longueur+1);
	  int hDeb=choixHDeb();
	////cout<<"hDeb="<<hDeb<<endl;
	  int l,j,k,h;
	  //choix Unité:
	  eoUniformGenerator<int> choixUnit(0,_genotype.getNbUnits());
	  int i=choixUnit();
	//cout<<"unit="<<i<<endl;
	////cout<<"i = "<<i<<endl;
	  //recalcul de hDeb:
	  vector<vector<int> > temps=_genotype.getTemps();
	  while(hDeb<nbHeures &&((temps[hDeb][i]<0 &&temps[hDeb][i]>tDown[i])||(temps[hDeb][i]>0 &&temps[hDeb][i]<tUp[i])))
	  {
		  hDeb++;
	  }
	cout<<"hdeb ap while="<<hDeb<<endl;
	////cout<<"apres 1er while "<<i<<" "<<hDeb<<endl;
	  if(hDeb<nbHeures)
	  {
		  if(hDeb+longueur>nbHeures)l=nbHeures-hDeb;
		else l=longueur;
		double total=0;
//		for(int c=hDeb;c<hDeb+l;c++)total+=_genotype.getEval(c);

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
		cout<<"etat ini fin"<<endl;
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
					eval(j,j-1,h+hDeb,i,_genotype,mu,var);
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
			cout<<"cas classique graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;

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
					eval(0,j,h+hDeb,i,_genotype,mu ,var);
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
cout<<"castUp"<<endl;
			  //cas tUp:
			  if(graphe[h-1][tUp[i]-1][0][0]>0)
			  {
				  double mu=0;
				  double var=0;
				  eval(tUp[i]-1,tUp[i]-1,h+hDeb,i,_genotype,mu,var);

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
			cout<<"casTCS"<<endl;
			  //cas tCS :
			  if(graphe[h-1][tUp[i]-tColdStart[i]-1][0][0]>0)
			  {								 
				  double mu=0;
				  double var=0;
				  eval(tUp[i]-tColdStart[i]-1,tUp[i]-tColdStart[i]-1,h+hDeb,i,_genotype,mu,var);
				
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
		cout<<"restitution "<<hDeb+l<<" nbHeures "<<nbHeures<<endl;
		  if(hDeb+l>=nbHeures-1)//-1?
		  {
			//constitution du front pareto :
			vector<vector <int> > front;
			////cout<<"ici"<<endl;
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
			cout<<"fin pour 2"<<endl;
			////cout<<s;
			//choix aléatoire d'une solution :
			eoUniformGenerator <int> choixSol(0,front.size());
			//int s=choixSol();
			for(int s=0;s<front.size();s++)
			{
				GenotypeT gen=_genotype;
			int etat=front[s][0];
			int num=front[s][1];
			cout<<"front constitué et sol choisie"<<endl;
			  for(j=hDeb+l;j>hDeb;j--)
			 {
							//cout<<"ici"<<endl;
			 				  int t;
			 				  int Etatpred=graphe[j-hDeb-1][etat][0][num];
							  int numPred=graphe[j-hDeb-1][etat][1][num];
							 // if(j>2)cout<<"num="<<num<<" numPred="<<numPred<<" etat="<<etat<<" Etatpred="<<Etatpred<<" unite="<<i<<" tUp="<<tUp[i]<<" tCS="<<tColdStart[i]<<" nb etats pred="<<graphe[j-hDeb-2][Etatpred][0][0]<<endl;
							  double eval1;
			 				  if (j>hDeb+1) eval1=graphe[j-hDeb-1][etat][2][num]-graphe[j-hDeb-2][Etatpred][2][numPred];
			 				  else eval1=gen.getEval1(hDeb);
							  double eval2;
			 				  if (j>hDeb+1) eval2=graphe[j-hDeb-1][etat][3][num]-graphe[j-hDeb-2][Etatpred][3][numPred];
			 				  else eval2=gen.getEval2(hDeb);
			 				  if(etat<tUp[i])t=etat+1;
			 				  else t=tUp[i]-etat-1;
								//if(t==0) //cout<<"MERCREI"<<endl;
			 				  gen.setTemps(j-1,i,t);
			 				  gen.setEval1(j-1,eval1);
			 				  gen.setEval2(j-1,eval2);
							  gen.setModif(j-1,true);
			 				  etat=Etatpred;
							  //cout<<"j="<<j<<"etat="<<etat<<endl;
							  num=numPred;
							  //cout<<"num="<<num<<endl;
			 }
			sol.push_back(gen);
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
			//eoUniformGenerator <int> choixNum(1,graphe[l-1][etat][0][0]+1);
			//int num=choixNum();
				////cout<<s<<" "<<hDeb<<" "<<l<<endl;
			for(int num=1;num<graphe[l-1][etat][0][0]+1;num++)
			{
				GenotypeT gen=_genotype;
			  for(j=hDeb+l;j>hDeb;j--)
			 {
			 	 int t;
			 	int Etatpred=graphe[j-hDeb-1][etat][num][0];
				int numPred=graphe[j-hDeb-1][etat][num][1];
				double eval1;
			 	if (j>hDeb+1) eval1=graphe[j-hDeb-1][etat][num][2]-graphe[j-hDeb-2][Etatpred][numPred][2];
			        else eval1=_genotype.getEval1(hDeb);
				double eval2;
			 	if (j>hDeb+1) eval2=graphe[j-hDeb-1][etat][num][3]-graphe[j-hDeb-2][Etatpred][numPred][3];
			 	else eval2=_genotype.getEval2(hDeb);
			 	  if(etat<tUp[i])t=etat+1;
			 	if(etat<tUp[i])t=etat+1;
			 	else t=tUp[i]-etat-1;
								//if(t==0) //cout<<"MERCREI"<<endl;
			 	gen.setTemps(j-1,i,t);
			        gen.setEval1(j-1,eval1);
			 	gen.setEval2(j-1,eval2);
				gen.setModif(j-1,true);
			 	etat=Etatpred;
				num=numPred;
			 }
			sol.push_back(gen);
			}
		  }

	  }
	
cout<<"fin mut2"<<endl;
   // return isModified;
	return sol;
    // END code for mutation of the _genotype object
  }
  
  void eval(int actu,int pred,int h,int i,GenotypeT & _genotype, double & mu, double & var)
  {
	////cout<<"debut eval"<<endl;
	int s;
	mu=0;
	vector<double> eval;
	for(s=0;s<nbScenarios; s++)
	{	
	  
	  double fC, penalites;
	double SC=0;
	  int tps;
	  int tpsPred;
	  if(actu<tUp[i]) tps=actu+1;
	  else tps=tUp[i]-actu-1;
	  if(pred<tUp[i]) tpsPred=pred+1;
	 	  else tpsPred=tUp[i]-pred-1;
	 
		////cout<<"calcul SC h="<<h<<"i="<<i<<" tps="<<tps<<" tpsPred="<<tpsPred<<" s="<<s<<endl;;
	 if(h>0) SC=StartUpCostHour(_genotype,h,i,tps,tpsPred,s);
	
	////cout<<"calcul penalite"<<endl;
	  penalites=penalityHour(_genotype,h,i,tps,s);
	fC=0;
	////cout<<"calcul fC"<<endl;
	if(penalites==0)  fC=fuelCost(_genotype,h,i,tps,s);
	////cout<<penalites<<endl;
	////cout<<"fin eval"<<endl;
	
	 eval.push_back( penalites+fC+SC);
	mu+=eval[s];
	}
	mu=mu/nbScenarios;
	var=0;
	for(s=0;s<nbScenarios; s++)
	{
		var+=(eval[s]-mu)*(eval[s]-mu);	
	}
	var=var/nbScenarios;

  }
  
  double fuelCost(GenotypeT & _eo,int h,int usine,int tps,int s)
   {
 	  double lamb,delt;
	////cout<<"deblambda"<<endl;
 	  lambda_delta(lamb,delt,h,_eo,usine,tps,s);
	////cout<<"la"<<endl;
	vector<eoUCPUnit>  * units= scenarios[s].get_units();
	std::vector<double>  *load= scenarios[s].get_load();
	std::vector<double>  * reserve= scenarios[s].get_reserve() ; 
 	  int nbUnits=(*units).size();
 	  double prod[nbUnits];
 	  double a1,a2,a0,pi,diff,sum=0;
	
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


 	  		    pi=0.5*(lamb-a1)/(a2);

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
		////cout<<"fin while"<<endl;
 	  	double res=0;
 	  	for(unsigned int i=0;i<nbUnits;i++)
 	   {
 	  		if((_eo.getTemps(h,i)>0&& i!=usine)||(i==usine &&tps>0))
 	  		{
 	  			 eoUCPUnit & unite =(*units)[i];
 	  		     a1=unite.get_a1();
 	  		    a2=unite.get_a2();
 	  			a0=unite.get_a0();
 	  			double p=prod[i];
 	  			double fc=a0+(a2*p+a1)*p;
 	  			res += fc;
 	  		}
 	   }
 	  	return res;

   }
  double StartUpCostHour(GenotypeT & _eo, int h,int usine,int tps, int tpsPred,int s)
   {
	vector<eoUCPUnit>  * units= scenarios[s].get_units();
	////cout<<"l544"<<endl;
 	  int nbUnits=(*units).size();
	////cout<<"nbUnits="<<nbUnits<<endl;
 	  unsigned i;
 	  double cost=0;
 	  for(i=0;i<nbUnits;i++)
 	  {

		////cout<<"av if h="<<h<<" i="<<i<<endl;//cout<<_eo.getTemps(h,i)<<endl;
 		  if((i!=usine && _eo.getTemps(h,i)==1)||(i==usine &&tps==1))
 		  {
				////cout<<"dans if"<<endl;
 			  if((i!=usine &&_eo.getTemps(h-1,i)>-(*units)[i].get_tColdStart())||(i==usine && tpsPred>-(*units)[i].get_tColdStart()))
 			  {
 				  cost=cost+(*units)[i].get_sCold();
 			  }
 			  else cost=cost+(*units)[i].get_sHot();
 		  }
 	  }
		////cout<<"fin SC"<<endl;
 	  return cost;
   }
  double penalityHour(GenotypeT & _eo,int h,int unit,int tps,int s)
    {
  	  //Test existence :
  	  //retour :
	vector<eoUCPUnit>  * units= scenarios[s].get_units();
	std::vector<double>  *load= scenarios[s].get_load();
	std::vector<double>  * reserve= scenarios[s].get_reserve();  
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
 void lambda_delta(double & lambda,double & delta,unsigned int h,GenotypeT & _eo,int usine,int tps,int s)
   {
	vector<eoUCPUnit>  * units= scenarios[s].get_units();

     double res=0.0;

     int Pmax=0;
     int Pmin=0;

     int nbUnits=(*units).size();
  //   std::vector<bool> * mbits = _eo.get_p_mbits();

     double a1=0;
     double a2=0;

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

 	    lminTmp = a1+(2*a2*Pmin);
 	    lmaxTmp = a1+(2*a2*Pmax);
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
