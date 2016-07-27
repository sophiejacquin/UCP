/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutation5MO_H
#define eoUCPMutation5MO_H
#include <addToParadisEO/eo/eoVectOp.h>
#define TAILLE_MAX 300

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
class eoUCPMutation5MO: public eoVectOp<GenotypeT>
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
	int nbUnitsS;
	int nbEtats;

// END   Private data of an eoUCPMutation object
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPMutation5MO (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,eoUCPData * _data,int _longueur,int _nb)
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
	        nbUnitsS= _nb;
		nbEtats=pow(2.,nbUnitsS)+1;
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

	cout<<"deb mut5MO"<<endl;
	  vector<GenotypeT> sol;
	  vector<int> unitsS;
	int nbUnits =(*units).size();
	int h,i,j,k;
	  //choix heure début :
	  int nbHeures =_genotype.getNbHeures();
      cout<<"nbHeures="<<nbHeures<<endl;
	  eoUniformGenerator<int> choixHDeb(1,nbHeures-longueur-1);
	  int hDeb=choixHDeb();
	  int hFin=hDeb+longueur;
      eoUniformGenerator<double> choixAlpha(0,1);
      double alpha=choixAlpha();
	 // cout<<"hDeb : "<<hDeb<<" hFin : "<<hFin<<endl;
	 //choix unités :
	int der=0;
	for(j=0;j<nbUnitsS;j++)
	{
		eoUniformGenerator<int> choixUnit(0,nbUnits );
		der= choixUnit();

		bool deja=false;
		for(k=0;k<j;k++)
		if(unitsS[k]==der)deja=true;
		if (deja) j--;
		else {
			unitsS.push_back(der);
			//cout<<der<<" ";
		}
	}
	//cout<<endl;
	// tableaux du graphe :
	//cout<<"av declaration"<<endl;
      double **** graphe;
      graphe =new double***[longueur+1];
      for(int truc=0;truc<longueur+1;truc++)
      {
          graphe[truc]= new double**[nbEtats];
          for(int machin=0;machin<nbEtats;machin++)
          { 
              graphe[truc][machin]=new double*[4];
              for(int chouette=0;chouette<4;chouette++)
                  graphe[truc][machin][chouette]=new double[TAILLE_MAX]; 
          }
      }
	//////cout<<"debut remplissage"<<endl;



	 //verification à priori :
		 for(j=hDeb;j<hFin+1;j++)
		  		{
		  			for(k=0;k<nbUnitsS;k++)
		  			{

		  				 if(_genotype.getTemps(j,unitsS[k])<0 && _genotype.getTemps(j-1,unitsS[k])>0 )
		  				{
		  					if( _genotype.getTemps(j-1,unitsS[k])<tUp[unitsS[k]]) cout<<"attention pb 1ap :"<<j<<" "<<k<<" tUp "<<tUp[unitsS[k]]<<" hDeb "<<hDeb<<"val pred "<<_genotype.getTemps(j-1,unitsS[k])<<endl;
		  				}

		  				else if(_genotype.getTemps(j,unitsS[k])>0 && _genotype.getTemps(j-1,unitsS[k])<0)
		  				{

		  						if( _genotype.getTemps(j-1,unitsS[k])>tDown[unitsS[k]]) cout<<"attention pb 2ap "<<j<<endl;
		  				}
		  			}
		  		}
     // cout<<"ap verif MO5"<<endl;
	//Remplissage du graphe :
	for(h=0;h<longueur+1;h++)
	{
		//////cout<<h<<endl;
		vector<int> description=_genotype.getTemps()[hDeb+h];
		for(i=0;i<nbEtats-1;i++)
		{
			//////cout<<i<<endl;
			
			//vérifier réalisabilité de l'état:
			bool realisable=true;
			double CS=0;
			//vérifier si non interdit par condition initiale ou finale et construction vecteur descriptif de l'état:
			int puis=1;
			int puisSup=2;

			for(j=0;j<nbUnitsS;j++)
			{
				int unite=unitsS[j];
				int tpsIni=_genotype.getTemps(hDeb-1,unite);
				int tpsFin;
				if(hFin+1<nbHeures)tpsFin=_genotype.getTemps(hFin+1,unite);
				int etat;
				if(i%puisSup>=puis)etat=1;
				else etat=-1;
				puisSup*=2;
				puis*=2;
				if((tpsIni>0&&tpsIni+h<tUp[unite] &&etat==-1)||(tpsIni<0 && tpsIni-h >tDown[unite] && etat==1))
					{realisable=false; //cout <<"realisable faux 1: heure "<< h << " "<<j<<" "<<etat<<endl;
                    }
				else if((hFin+1<nbHeures)&&((tpsFin>0 && tpsFin-(longueur -h +1)>0 && etat<0)||(tpsFin<0 && -tpsFin-(longueur -h +1)>0)&&etat==1))
				{realisable=false;// cout <<"realisable faux2 : heure "<< h << " "<<j<<" "<<etat<<endl;
                }
				else if((hFin+1<nbHeures)&& tpsFin>0 && tpsFin<tUp[unite] && h>longueur+1-tpsFin && etat==-1)
				{realisable=false;// cout <<"realisable faux 3 : heure "<< h << " "<<j<<" "<<etat<<endl;
				}
				else if((hFin+1<nbHeures)&& tpsFin>0 && tpsFin<tUp[unite] && longueur+1-tpsFin-tDown[unite]<h<=longueur+1-tpsFin && etat==1)
								{realisable=false; 
					}
			 else if((hFin+1<nbHeures)&& tpsFin<0 && tpsFin>tColdStart[unite] && h<=longueur+1+tpsFin&& h>longueur+1+tpsFin-tUp[unite] &&etat==-1)
			 {realisable=false; cout <<"realisable faux 4 "<< h << " "<<j<<" "<<etat<<endl;}
				description[unite]=etat;

			}
			//////cout<<"verif accessibilité finie "<<realisable<<endl;
			if(realisable==true)
			{
			//	cout<<"realisable "<<realisable<<endl;
				double penalite=0;
				////cout<<"réalisable"<<endl;
				// vérifier si réalisable au sens des demandes et réserves :
				
					//recherche meilleur pred:
					if(h==0)
					{
                       // cout<<"si"<<endl;
						graphe[0][i][0][1]=0;
                        graphe[0][i][0][0]=1;
						penalite=penalityHour(description,hDeb);
						//cout<<"penalite calculée"<<endl;
						//eval avec CS + evalPred :
						graphe[0][i][2][1]=penalite;
                        graphe[0][i][3][1]=penalite;
						//if(graphPred[0][i]==0){
						int puissance=1;
						int puisSup=2;
                       // cout<<"av for"<<endl;
							for(j=0;j<nbUnitsS;j++)
							{
								int tempsIni=_genotype.getTemps(hDeb-1,unitsS[j]);
								int etat=description[unitsS[j]];
								if(i%puisSup>=puissance)etat=1;
								else etat=-1;
								puissance*=2;
								puisSup*=2;
								if(etat<0 && tempsIni>0 &&  tempsIni< tUp[unitsS[j]])
                                {
                                  graphe[0][i][0][1]=-1;
                                    graphe[0][i][0][0]=0;
                                }
								else if(etat>0 && tempsIni<0 &&tempsIni>tDown[unitsS[j]])
                                {
                                    graphe[0][i][0][1]=-1;
                                    graphe[0][i][0][0]=0;
                                }
								else if(etat>0 && tempsIni<0 &&tempsIni>tColdStart[unitsS[j]])
									graphe[h][i][2][1]+=(*units)[unitsS[j]].get_sHot();
								else if(etat>0 && tempsIni==tColdStart[unitsS[j]])graphe[h][i][2][1]+=(*units)[unitsS[j]].get_sCold();
							}
                      //  cout<<"ap for"<<endl;
						//}
					//////cout<<"recherche meilleur pred h=0 finie"<<endl;

					}
					else //si h>0
					{
                        //cout<<"sinon"<<endl;
                        //cout<<h<<endl;
						graphe[h][i][0][1]=-1;
                        graphe[h][i][0][0]=0;
                        
						penalite=penalityHour(description,h+hDeb);
                        //cout<<"penalite"<<endl;
						for(j=0;j<nbEtats;j++)
						{
                            
							//if(graph[h-1][j][0][0]<1)possible=false;
                            for(int z=1;z<graphe[h-1][j][0][0]+1;z++)
                            {
                                bool possible=true;
						
                                k=0;
                                int CS=0;
                                
                                int puissance=1;
                                int puisSup=2;
                               // cout<<"av while mo5"<<endl;
                                while(possible&& k<nbUnitsS)
                                {
                                   // cout<<"z="<<z<<" k="<<k<<endl;
                                    int etat;
                                    if(j<nbEtats-1 && j%puisSup>=puissance)etat=1;
                                    else if(j<nbEtats-1)etat=-1;
                                    else etat=_genotype.getTemps(hDeb+h,unitsS[k]);
                                   // cout<<"ap getTemps MO5"<<endl;
                                    puissance*=2;
                                    puisSup*=2;
                                    if((etat<0 && description[unitsS[k]]>0)||(etat>0 && description[unitsS[k]]<0))
                                    {
                                        //cout<<"dans si h="<<h<<"j ="<<j<<endl;
                                        int tps=tempsMemeEtat(_genotype,graphe,h-1,j,z,k,hDeb,unitsS[k]);
                                       // cout<<"ap tps mm etat"<<endl;
                                        if(etat>0 && tps<tUp[unitsS[k]])
                                        {
                                            possible=false;
                                        }
                                        //cout<<"ap si mo5"<<endl;
                                        else if(etat<0)
                                        {
                                            if(tps< -tDown[unitsS[k]])
                                            {
                                                possible=false;
                                            }
                                            else if(tps< -tColdStart[unitsS[k]])
                                                CS+=(*units)[unitsS[k]].get_sHot();
                                            else CS+=(*units)[unitsS[k]].get_sCold();
                                           
                                        }
                                        // cout<<"là"<<endl;
									//if(h==4 &&k==0 &&tps==3)cout<<tps<<" "<<possible<<"etat "<<etat<<" description "<<description[unitsS[k]]<<" "<<tUp[unitsS[k]]<<endl;

                                    }
                                    k++;
                                }//fin tq
                               // cout<<"ap while l276 5MO"<<endl;
                            
							if(possible && graphe[h-1][j][0][0]>0 )
							{
                                bool domine=false;
                                bool dominant=false;
                                
                            
                                int zob=1;
                                while(zob<graphe[h][i][0][0]+1 && zob<TAILLE_MAX&& !domine )
                                {
                                    if(CS+penalite+graphe[h-1][j][2][z]>=graphe[h][i][2][zob] &&penalite+graphe[h-1][j][3][z]>=graphe[h][i][3][zob])
                                        domine=true; 
                                    else if(CS+penalite+graphe[h-1][j][2][z]<graphe[h][i][2][zob] &&penalite+graphe[h-1][j][3][z]<graphe[h][i][3][zob])
                                    {
                                        if(!dominant){
                                            graphe[h][i][2][zob]=CS+penalite+graphe[h-1][j][2][z];
                                            graphe[h][i][3][zob]=penalite+graphe[h-1][j][3][z];
                                            graphe[h][i][0][zob]=j;
                                            graphe[h][i][1][zob]=z;
                                            dominant=true;}
                                        else
                                        {
                                            graphe[h][i][2][zob]=graphe[h][i][2][(int)(graphe[h][i][0][0])];
                                            graphe[h][i][3][zob]=graphe[h][i][3][(int)(graphe[h][i][0][0])];
                                            graphe[h][i][0][zob]=graphe[h][i][0][(int)(graphe[h][i][0][0])];
                                            graphe[h][i][1][zob]=graphe[h][i][1][(int)(graphe[h][i][0][0])];
                                            graphe[h][i][0][0]--;	
                                            zob--;							
                                        }
                                        
                                        
                                    }
                                    
                                    zob++;
                                }
                                //cout<<" cas 1 ap while graphe[h][1][0][0]="<<graphe[h][1][0][0]<<" h="<<h<<endl;
                                if(!domine && !dominant && graphe[h][i][0][0]+1<TAILLE_MAX)
                                {
                                    graphe[h][i][2][(int)(graphe[h][i][0][0])+1]=CS+penalite+graphe[h-1][j][2][z];
                                    graphe[h][i][3][(int)(graphe[h][i][0][0])+1]=penalite+graphe[h-1][j][3][z];
                                    graphe[h][i][0][(int)(graphe[h][i][0][0])+1]=j;
                                    graphe[h][i][1][(int)(graphe[h][i][0][0])+1]=z;
                                    graphe[h][i][0][0]+=1;
                                }
							}
                               // else graphe[h][i][0][0]=0;
                            }//fin pour parcours front pred
						}//fin pour



					}
					//////cout<<"autre cas fini"<<endl;
					//calcul de FC:
					if(graphe[h][i][0][0]>0 && penalite==0)
					{
                       // cout<<"penalite "<<penalite<<" hDeb="<<hDeb<<" longueur="<<longueur<<endl;
                        double mu=0;
                        double var=0;
                       /// cout<<"eval:"<<endl;
                        fuelCost(_genotype, h+hDeb,description, alpha, mu,  var);
                      //  cout<<"fin aval"<<endl;
                        for(int z=1;z<graphe[h][i][0][0]+1;z++)
                        {
                            graphe[h][i][2][z]+=mu;
                            graphe[h][i][2][z]+=var;
                        }
						
					}
					//////cout<<"calcul Fuel Cost effectué"<<endl;

				

			}
			else graphe[h][i][0][0]=0;
			//////cout<<"fin iteration grde boucle états"<<endl;

		}
		//////cout<<"graphe rempli sauf dernier état"<<endl;
		//dernier état: ajout possibilité de ratacher à d'autres sommets 
		graphe[h][nbEtats-1][0][1]=nbEtats-1;
        graphe[h][nbEtats-1][1][1]=1;
        graphe[h][nbEtats-1][0][0]=1;
		description=_genotype.getTemps()[hDeb+h];
		double penalite=penalityHour(description,h+hDeb);
		graphe[h][nbEtats-1][2][1]=penalite;
        graphe[h][nbEtats-1][3][1]=penalite;
			//Calcul de CS et numero autre sommet pred possible :
			int puis=1;
			int num=0;
			double CS=0;
			for(k=0;k<nbUnitsS;k++)
			{
				if(_genotype.getTemps(hDeb+h-1,unitsS[k])>0)num=num+puis;
				puis*2;
				if(description[unitsS[k]]==1)
				{

					if(_genotype.getTemps(hDeb+h-1,unitsS[k])<0 && _genotype.getTemps(hDeb+h-1,unitsS[k])>tColdStart[unitsS[k]])
						CS+=(*units)[unitsS[k]].get_sHot();
					else CS+=(*units)[unitsS[k]].get_sCold();

				}
			}
					//Calcul de FC:
			graphe[h][nbEtats-1][2][1]+=CS;
        if(h>0){
            graphe[h][nbEtats-1][2][1]+=graphe[h-1][nbEtats-1][2][1];
            graphe[h][nbEtats-1][3][1]+=graphe[h-1][nbEtats-1][3][1];
        }
			
			//regarder si autre pred est un pred possible :
		
		//Calcul penalites :
		if(penalite==0) {
            double mu=0;
            double var=0;
            fuelCost(_genotype, h+hDeb,description, alpha, mu,  var);
                graphe[h][nbEtats-1][2][1]+=mu;
                graphe[h][nbEtats-1][3][1]+=var;
            
        }
		
		//////cout<<"derbier etat"<<endl;


	}
//cout<<"fin remplisage graph"<<endl;
//fin remplissage graphes
	//retour solution:
	//construction du front pareto:
     // cout<<" hDeb+longueur="<<hDeb+longueur<<" nbHeures-1="<<nbHeures-1<<endl;
      if(hDeb+longueur>=nbHeures-1)//-1?
      {
          //constitution du front pareto :
          vector<vector <int> > front;
          ////cout<<"ici"<<endl;
          for(k=1;k<graphe[longueur][0][0][0]+1&& k<TAILLE_MAX;k++)
          {
              vector <int> element;
              element.push_back(0);
              element.push_back(k);
              front.push_back(element);
          }
          ////cout<<"fin pour1"<<endl;
          for(j=1;j<nbEtats;j++)
          {
              
              if(front.size()==0)
              {
                  ////cout<<"si"<<endl;
                  for(k=1;k<graphe[longueur][j][0][0]+1&& k<TAILLE_MAX;k++)
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
                  for(k=1;k<graphe[longueur][j][0][0]+1&& k<TAILLE_MAX;k++)
                  {
                      bool domine=false;
                      
                      for(int temp=0; temp<front.size() && !domine;temp++)
                      {
                          ////cout<<front.size()<<" temp="<<temp<<endl<<" front[temp][0]="<<front[temp][0]<<" front[temp][1]="<<front[temp][1]<<endl;
                          if(graphe[longueur][j][2][k]>= graphe[longueur][front[temp][0]][2][front[temp][1]] && graphe[longueur][j][3][k]>=graphe[longueur][front[temp][0]][3][front[temp][1]]) 
                              domine=true;
                          else if (graphe[longueur][j][2][k]<= graphe[longueur][front[temp][0]][2][front[temp][1]] && graphe[longueur][j][3][k]<=graphe[longueur][front[temp][0]][3][front[temp][1]]) 
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
          }//fin parcours états
          cout<<"taille front :"<<front.size()<<" hDeb+longueur="<<hDeb+longueur<<" nbHeures-1="<<nbHeures-1<<endl;
	//cout <<"actu def "<<actu<<endl;
	//restitution des solutions sous forme binaire:
          for(int f=0;f<front.size();f++)
          {
             int actu= front[f][0];
              int num =front[f][1];
              GenotypeT gen=_genotype;
              for(j=longueur;j>=0;j--)
              {
                  
                  int puissance=1;int puisSup=2;
                  for(k=0;k<nbUnitsS;k++)
                  {
                      if(actu<nbEtats-1)
                      {
                          int etat;
                          if(actu%puisSup>=puissance)etat=1;
                          else etat=-1;
                          gen.setTemps(hDeb+j,unitsS[k],etat);
					
                          puisSup*=2;puissance*=2;
                          //	cout<<j<<": "<<actu<<" "<<tempsMemeEtat(_genotype,graphPred,j,actu,k,hDeb,unitsS[k])<<etat<<endl;
                      }
                    
                  }
			//cout<<j<<": "<<actu<<" mo5 num:"<<num<<" nb elements : "<<graphe[j][actu][0][0]<<endl;
                  int tempon=actu;
                  actu=graphe[j][actu][0][num];
                 num=graphe[j][tempon][1][num];
                 // cout<<"là "<<num<<endl;
              }
              sol.push_back(gen);
          }
		//cout<<endl;
		//cout<<"rest binaire"<<endl;
		//remise des temps comme il faut :
          
          for(int s=0;s<sol.size();s++)
		for(j=hDeb;j<hFin+1;j++)
		{
			for(k=0;k<nbUnitsS;k++)
			{
				if(sol[s].getTemps(j,unitsS[k])<0 && sol[s].getTemps(j-1,unitsS[k])<0 )
				{
					if(sol[s].getTemps(j-1,unitsS[k])>tColdStart[unitsS[k]])sol[s].setTemps(j,unitsS[k],sol[s].getTemps(j-1,unitsS[k])-1);
					else sol[s].setTemps(j,unitsS[k],tColdStart[unitsS[k]]);
				}
				else if(sol[s].getTemps(j,unitsS[k])<0 && sol[s].getTemps(j-1,unitsS[k])>0 )
				{
					sol[s].setTemps(j,unitsS[k],-1);
					if( sol[s].getTemps(j-1,unitsS[k])<tUp[unitsS[k]]) cout<<"attention pb 1 :"<<j<<" "<<k<<": "<<unitsS[k]<<" tUp "<<tUp[unitsS[k]]<<" hDeb "<<hDeb<<"val pred "<<sol[s].getTemps(j-1,unitsS[k])<<endl;
				}
				else if(sol[s].getTemps(j,unitsS[k])>0 && sol[s].getTemps(j-1,unitsS[k])>0)
				{
						if(sol[s].getTemps(j-1,unitsS[k])<tUp[unitsS[k]])sol[s].setTemps(j,unitsS[k],sol[s].getTemps(j-1,unitsS[k])+1);
						else sol[s].setTemps(j,unitsS[k],tUp[unitsS[k]]);
				}
				else if(sol[s].getTemps(j,unitsS[k])>0 && sol[s].getTemps(j-1,unitsS[k])<0)
				{
						 sol[s].setTemps(j,unitsS[k],1);
						if( sol[s].getTemps(j-1,unitsS[k])>tDown[unitsS[k]]) cout<<"attention pb 2 "<<j<<" "<<unitsS[k]<<" tDown "<<tDown[unitsS[k]]<<" hDeb "<<hDeb<<"val pred "<<sol[s].getTemps(j-1,unitsS[k])<<endl;
				}
			}
		}
      }//fin si atteinte fin période
	//////cout<<"rest"<<endl;
	
	//désalouage :
      cout<<"désalouage: taille "<<sol.size()<<endl;
      for(int truc=0;truc<longueur+1;truc++)
      {
         
          for(int machin=0;machin<nbEtats;machin++)
          { 
              
              for(int chouette=0;chouette<4;chouette++)
                  delete []  graphe[truc][machin][chouette]; 
          }
      }
    return sol;
	
    // END code for mutation of the _genotype object
  }

  int tempsMemeEtat(GenotypeT & _eo,double**** graphe,int h,int i,int num,int u,int hDeb,int unit)
  {
      
	  int tps=1;
	  int etat;
	
	  if(i<nbEtats-1)
	  {
	  if(i%( static_cast<int>(pow(2.,u+1)))>=static_cast<int>(pow(2.,u)))etat=1;
	  else etat=-1;
	  
	  }
	else
	{
		if(_eo.getTemps(h+hDeb,unit)>0)etat=1;
		else etat=-1;
		
	}
     
	  int pred= graphe[h][i][0][num];
	  int etatPred;
	  if(pred<nbEtats-1)
	  {
		  if(pred%(static_cast<int>(pow(2.,u+1)))>=static_cast<int>(pow(2.,u)))etatPred=1;
		  else etatPred=-1;
	  }
	  else{
		  if(_eo.getTemps(h-1+hDeb,unit)>0)etatPred=1;
		  		else etatPred=-1;
	  }
      //cout<<"av whileMO5"<<endl;
       h--;
	  while(etat==etatPred && h>0)
	  {
		  tps++;
          //cout<<"i="<<i<<endl;
          num=graphe[h+1][i][1][num];
          //cout<<"pb ici?"<<endl;
		  i=pred;
          
		  etat=etatPred;

		  pred=graphe[h][i][0][num];
       
		if(pred<nbEtats-1){
		  if(pred%(static_cast<int>(pow(2.,u+1)))>=static_cast<int>(pow(2.,u)))etatPred=1;
		 else etatPred=-1;}
		else
		{
			if(_eo.getTemps(h-1+hDeb,unit)>0)etatPred=1;
			else etatPred=-1;
		}
          h--;

	  }
    //cout<<"ap while tps mm etat mut5MO"<<endl;
	  if(h==0 && etat>0 &&_eo.getTemps(hDeb-1,unit)>0) tps+=_eo.getTemps(hDeb-1,unit);
	else if(h==0 && etat<0 &&_eo.getTemps(hDeb-1,unit)<0) tps-=_eo.getTemps(hDeb-1,unit);
//	else tps++;
	  return tps;
  }
  
  double fuelCost(GenotypeT & _eo,int h,vector<int> description, double alpha, double & fc, double & ec)
   {
 	  double lamb,delt;
       fc=0;
       ec=0;
	////////cout<<"deblambda"<<endl;
 	  lambda_delta(lamb,delt,h,_eo,description,alpha);
	//////cout<<"la"<<endl;
 	  int nbUnits=(*units).size();
 	  double prod[nbUnits];
 	  double a1,a2,a0,b1,b2,b0,pi,diff,sum=0;
	
 	  	//int indice;
 	  	do
 	  	  {
 	  	    sum=0;

 	  	    for(unsigned int i=0;i<nbUnits;i++)
 	  	      {
 	  		//indice = _eo.indiceUnitAtTime(i,h);
 	  		if(description[i]>0)
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
            //  cout<<diff<<endl;
 	  	  }
 	  	while(fabs(diff)>0.00001);
		//cout<<"fin while"<<endl;
 	  	double res=0;
 	  	for(unsigned int i=0;i<nbUnits;i++)
 	   {
 	  		if(description[i]>0)
 	  		{
                eoUCPUnit & unite =(*units)[i];
                a1=unite.get_a1();
	  		    a2=unite.get_a2();
	  			a0=unite.get_a0();
                b1=unite.get_b1();
	  		    b2=unite.get_b2();
	  			b0=unite.get_b0();
	  			double p=prod[i];
                 fc+=(a2*p+a1)*p+a0;
	  			 ec+=(b2*p+b1)*p+b0;
                //res+= alpha* fc+(1-alpha)*ec;
 	  		}
 	   }
 	  	//return res;

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
  double penalityHour(vector<int> description, int h)
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


  	  		if(description[i]>0)
  	  		{
  	  			sumPmax+=Pmax;
  	  			sumPmin+=Pmin;
  	  		}
  	      }

  	      demandReserve= (*load)[h-1]+(*reserve)[h-1];
		//////cout<<sumPmax <<" "<<sumPmin<<endl;
  	     if (sumPmax >=demandReserve && sumPmin<=(*load)[h-1])
  	     {
  	    	 return 0;
  	     }
  	     else return 1000000;

    }
 void lambda_delta(double & lambda,double & delta,unsigned int h,GenotypeT & _eo,vector<int> description,double alpha)
   {
     double res=0.0;

     int Pmax=0;
     int Pmin=0;
     int nbUnitsS;
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

 	if(description[i]>0)
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
