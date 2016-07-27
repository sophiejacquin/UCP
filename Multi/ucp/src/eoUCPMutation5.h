/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoUCPMutation5_H
#define eoUCPMutation5_H


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
class eoUCPMutation5: public eoMonOp<GenotypeT>
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
  eoUCPMutation5 (vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS,eoUCPData * _data,int _longueur,int _nb)
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
  bool operator()(GenotypeT & _genotype)
  {

	cout<<"deb mut5"<<endl;
	  bool isModified(true);
	  vector<int> unitsS;
	int nbUnits =(*units).size();
	int h,i,j,k;
	  //choix heure début :
	  int nbHeures =_genotype.getNbHeures();
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
	double graphEval[longueur+1][nbEtats];
	int**  graphPred = new int*[longueur+1];
	for (j=0;j<longueur+1;j++) graphPred[j]=new int[nbEtats];
	//////cout<<"debut remplissage"<<endl;



	 //verification à priori :
		/* for(j=hDeb;j<hFin+1;j++)
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
		  		}*/
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
								{realisable=false; //cout <<"realisable faux 3 : heure "<< h << " "<<j<<" "<<etat<<endl;
					}
			 else if((hFin+1<nbHeures)&& tpsFin<0 && tpsFin>tColdStart[unite] && h<=longueur+1+tpsFin&& h>longueur+1+tpsFin-tUp[unite] &&etat==-1)
			 {realisable=false; cout <<"realisable faux 4 "<< h << " "<<j<<" "<<etat<<endl;}
				description[unite]=etat;

			}
			//////cout<<"verif accessibilité finie "<<realisable<<endl;
			if(realisable==true)
			{
				
				double penalite=0;
				////cout<<"réalisable"<<endl;
				// vérifier si réalisable au sens des demandes et réserves :
				
					//recherche meilleur pred:
					if(h==0)
					{
						graphPred[0][i]=0;
						penalite=penalityHour(description,hDeb);
						
						//eval avec CS + evalPred :
						graphEval[0][i]=penalite;
						//if(graphPred[0][i]==0){
						int puissance=1;
						int puisSup=2;
							for(j=0;j<nbUnitsS;j++)
							{
								int tempsIni=_genotype.getTemps(hDeb-1,unitsS[j]);
								int etat=description[unitsS[j]];
								if(i%puisSup>=puissance)etat=1;
								else etat=-1;
								puissance*=2;
								puisSup*=2;
								if(etat<0 && tempsIni>0 &&  tempsIni< tUp[unitsS[j]])
									graphPred[0][i]=-1;
								else if(etat>0 && tempsIni<0 &&tempsIni>tDown[unitsS[j]])
									graphPred[0][i]=-1;
								else if(etat>0 && tempsIni<0 &&tempsIni>tColdStart[unitsS[j]])
									graphEval[h][i]+=(*units)[unitsS[j]].get_sHot();
								else if(etat>0 && tempsIni==tColdStart[unitsS[j]]) graphEval[h][i]+=(*units)[unitsS[j]].get_sCold();
							}
						//}
					

					}
					else //si h>0
					{
						graphPred[h][i]=-1;
						penalite=penalityHour(description,h+hDeb);
						for(j=0;j<nbEtats;j++)
						{
							bool possible=true;
							if(graphPred[h-1][j]<0)possible=false;
						
							k=0;
							int CS=0;
							//>0){ possible=false;}
							//else ////cout<<"là c'est bon"<<endl;
							int puissance=1;
							int puisSup=2;
							while(possible&& k<nbUnitsS)
							{
								int etat;
								if(j<nbEtats-1 && j%puisSup>=puissance)etat=1;
								else if(j<nbEtats-1)etat=-1;
								else etat=_genotype.getTemps(hDeb+h,unitsS[k]);
								puissance*=2;
								puisSup*=2;
								//if(h==13&& k==2)cout<<"etat "<<etat<<" description "<<description[unitsS[k]]<<endl;
								if((etat<0 && description[unitsS[k]]>0)||(etat>0 && description[unitsS[k]]<0))
								{
									//calcul temps mm état :
								

									int tps=tempsMemeEtat(_genotype,graphPred,h-1,j,k,hDeb,unitsS[k]);

									if(etat>0 && tps<tUp[unitsS[k]])
									{
										possible=false;
									}
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
									//if(h==4 &&k==0 &&tps==3)cout<<tps<<" "<<possible<<"etat "<<etat<<" description "<<description[unitsS[k]]<<" "<<tUp[unitsS[k]]<<endl;

								}
								k++;
							}//fin tq
							if(possible)
							{
								//cout<<possible<<" "<<j<<" "<<graphPred[h][i]<<" "<<graphEval[h][i]<<" "<<graphEval[h-1][j]<<" CS "<<CS<<" penalite "<<penalite<<endl;
								//regarde si meilleur predesseur :
								if(graphPred[h][i]==-1|| graphEval[h][i]>graphEval[h-1][j]+CS+penalite)
								{
									//cout<<"là "<<h<<endl;
									graphPred[h][i]=j;
									graphEval[h][i]=graphEval[h-1][j]+CS+penalite;
								}
							}
						}//fin pour



					}
					//////cout<<"autre cas fini"<<endl;
					//calcul de FC:
					if(graphPred[h][i]>-1&&penalite==0)
					{
						graphEval[h][i]+=fuelCost(_genotype,h+hDeb,description,alpha);
					}
					//////cout<<"calcul Fuel Cost effectué"<<endl;

				

			}
			else graphPred[h][i]=-1;
			//////cout<<"fin iteration grde boucle états"<<endl;

		}
		//////cout<<"graphe rempli sauf dernier état"<<endl;
		//dernier état: ajout possibilité de ratacher à d'autres sommets 
		graphPred[h][nbEtats-1]=nbEtats-1;
		description=_genotype.getTemps()[hDeb+h];
		double penalite=penalityHour(description,h+hDeb);
		
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
			graphEval[h][nbEtats-1]=CS;
			if(h>0)graphEval[h][nbEtats-1]+=graphEval[h-1][nbEtats-1];
			graphEval[h][nbEtats-1]+=penalite;
			//regarder si autre pred est un pred possible :
			/*if(h>0)
			{
				bool acceptable = true;
				int k=0;
				while(k<nbUnitsS && acceptable)
				{
					int tps=tempsMemeEtat(_genotype,graphPred,h-1,num,k,hDeb,unitsS[k]);
					bool pos=false;
					if(description[unitsS[k]]==tUp[unitsS[k]] && tps>=tUp[unitsS[k]]-1)
					{
										pos=true;
					}
					else if(description[unitsS[k]]==1  && tps>=-tDown[unitsS[k]])
					{
						pos=true;
					}
					else if(description[unitsS[k]]==-1 && tps>=tUp[unitsS[k]])
					{
								pos=true;
					}
					else if(description[unitsS[k]]==tColdStart[unitsS[k]] && tps>= -tColdStart[unitsS[k]]-1)
					{
						pos=true;
					}

					else if(description[unitsS[k]]>0 && tps== description[unitsS[k]]-1)
					{
						pos=true;
					}
					else if(description[unitsS[k]]<0 && tps== -description[unitsS[k]]-1)
					{
						pos=true;
					}
					acceptable= acceptable&& pos;
					k++;
				}
				if(acceptable && graphEval[h-1][num]+CS+penalite<graphEval[h][nbEtats-1])
				{
					graphPred[h][nbEtats-1]=num;
					graphEval[h][nbEtats-1]=graphEval[h-1][num]+CS+penalite;
				}
			}*/
		//////cout<<"pred de base mis"<<endl;
		//chercher si il existe un autre pred meilleur :
		/*int nbPred=0;
		for(j=0;j<nbEtats-1;j++)
		{
			
			bool possible=true;
			if(h==0||graphPred[h-1][j]<0)possible=false;
			else nbPred++;		
			k=0;
			int CS=0;
			
			int puissance=1;
			int puisSup=2;
			while(possible&& k<nbUnitsS)
			{
				int etat;
				if( j%puisSup>=puissance)etat=1;
				else etat=-1;
				puisSup*=2;puissance*=2;
				if(etat* description[unitsS[k]]<0)
				{
									//calcul temps mm état :
					int tps=tempsMemeEtat(_genotype,graphPred,h-1,j,k,hDeb,unitsS[k]);
					if(etat<0 && tps<tUp[unitsS[k]])
					{
						possible=false;
					}
					else if(etat>0)
					{
						if(tps< -tDown[unitsS[k]])
						{
							possible=false;
						}
						else if(tps< tColdStart[unitsS[k]])
						CS+=(*units)[unitsS[k]].get_sHot();
						else CS+=(*units)[unitsS[k]].get_sCold();
					}

				}
				k++;
			}//fin tq

			if(possible)
			{
				//////cout<<possible<<endl;
								//regarde si meilleur predesseur :
				if(graphPred[h][nbEtats-1]==-1|| graphEval[h][nbEtats-1]>graphEval[h-1][j]+CS+penalite)
				{
					graphPred[h][nbEtats-1]=j;
					graphEval[h][nbEtats-1]=graphEval[h-1][j]+CS+penalite;
				}
			}
		}//fin pour*/
		//cout<<" h "<<h<<" nbPred "<<nbPred<<endl;
		//////cout<<"fin pour"<<endl;

		//Calcul penalites :
		if(penalite==0) graphEval[h][nbEtats-1]+=fuelCost(_genotype,h+hDeb,description,alpha);
		
		//////cout<<"derbier etat"<<endl;


	}
//////cout<<"fin remplisage graph"<<endl;
//fin remplissage graphes
	//retour solution:
	//recherche meilleur dernier etat valide:
	int actu=nbEtats-1;
	for(j=0;j<nbEtats-1;j++)
	{
		if(graphPred[longueur-1][j]>-1)
		{
			//cout<<graphEval[longueur-1][j]<<endl;
			if(graphEval[longueur-1][j]<graphEval[longueur-1][actu]) actu=j;
		}
	}
	//cout <<"actu def "<<actu<<endl;
	//restitution des solutions sous forme binaire:
	
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
					_genotype.setTemps(hDeb+j,unitsS[k],etat);
					
					puisSup*=2;puissance*=2;
				//	cout<<j<<": "<<actu<<" "<<tempsMemeEtat(_genotype,graphPred,j,actu,k,hDeb,unitsS[k])<<etat<<endl;
				}
				//else cout<< j<<": "<<actu<<" "<<_genotype

			}
			//cout<<j<<": "<<actu<<endl;
			actu=graphPred[j][actu];

			_genotype.setModif(hDeb+j,true);
		}
		//cout<<endl;
		//////cout<<"rest binaire"<<endl;
		//remise des temps comme il faut :
		for(j=hDeb;j<hFin+1;j++)
		{
			for(k=0;k<nbUnitsS;k++)
			{
				if(_genotype.getTemps(j,unitsS[k])<0 && _genotype.getTemps(j-1,unitsS[k])<0 )
				{
					if(_genotype.getTemps(j-1,unitsS[k])>tColdStart[unitsS[k]])_genotype.setTemps(j,unitsS[k],_genotype.getTemps(j-1,unitsS[k])-1);
					else _genotype.setTemps(j,unitsS[k],tColdStart[unitsS[k]]);
				}
				else if(_genotype.getTemps(j,unitsS[k])<0 && _genotype.getTemps(j-1,unitsS[k])>0 )
				{
					_genotype.setTemps(j,unitsS[k],-1);
					if( _genotype.getTemps(j-1,unitsS[k])<tUp[unitsS[k]]) cout<<"attention pb 1 :"<<j<<" "<<k<<": "<<unitsS[k]<<" tUp "<<tUp[unitsS[k]]<<" hDeb "<<hDeb<<"val pred "<<_genotype.getTemps(j-1,unitsS[k])<<endl;
				}
				else if(_genotype.getTemps(j,unitsS[k])>0 && _genotype.getTemps(j-1,unitsS[k])>0)
				{
						if(_genotype.getTemps(j-1,unitsS[k])<tUp[unitsS[k]])_genotype.setTemps(j,unitsS[k],_genotype.getTemps(j-1,unitsS[k])+1);
						else _genotype.setTemps(j,unitsS[k],tUp[unitsS[k]]);
				}
				else if(_genotype.getTemps(j,unitsS[k])>0 && _genotype.getTemps(j-1,unitsS[k])<0)
				{
						 _genotype.setTemps(j,unitsS[k],1);
						if( _genotype.getTemps(j-1,unitsS[k])>tDown[unitsS[k]]) cout<<"attention pb 2 "<<j<<" "<<unitsS[k]<<" tDown "<<tDown[unitsS[k]]<<" hDeb "<<hDeb<<"val pred "<<_genotype.getTemps(j-1,unitsS[k])<<endl;
				}
			}
		}
	//////cout<<"rest"<<endl;
	
	//désalouage :
	//////cout<<"deb desalouage"<<endl;
	for (j=0;j<longueur+1;j++)
 		delete[] graphPred[j];
	//////cout<<"ici"<<endl;
	  delete[] graphPred;
	cout<<"desaloué"<<endl;
    return isModified;
	
    // END code for mutation of the _genotype object
  }

  int tempsMemeEtat(GenotypeT & _eo,int** graphPred,int h,int i,int u,int hDeb,int unit)
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
	  int pred= graphPred[h][i];
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
	  while(etat==etatPred && h>0)
	  {
		  tps++;
		  i=pred;
		  etat=etatPred;
		  h--;
		  pred=graphPred[h][i];
		if(pred<nbEtats-1){
		  if(pred%(static_cast<int>(pow(2.,u+1)))>=static_cast<int>(pow(2.,u)))etatPred=1;
		 else etatPred=-1;}
		else
		{
			if(_eo.getTemps(h-1+hDeb,unit)>0)etatPred=1;
			else etatPred=-1;
		}

	  }
	  if(h==0 && etat>0 &&_eo.getTemps(hDeb-1,unit)>0) tps+=_eo.getTemps(hDeb-1,unit);
	else if(h==0 && etat<0 &&_eo.getTemps(hDeb-1,unit)<0) tps-=_eo.getTemps(hDeb-1,unit);
//	else tps++;
	  return tps;
  }
  
  double fuelCost(GenotypeT & _eo,int h,vector<int> description, double alpha)
   {
 	  double lamb,delt;
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
 	  	  }
 	  	while(fabs(diff)>0.00001);
		////////cout<<"fin while"<<endl;
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
