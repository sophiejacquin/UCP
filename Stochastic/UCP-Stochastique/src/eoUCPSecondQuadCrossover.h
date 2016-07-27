/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for simple quadratic crossover operators
=================================================

Quadratic crossover operators modify the both genotypes
*/

#ifndef eoUCPSecondQuadCrossover_H
#define eoUCPSecondQuadCrossover_H

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
class eoUCPSecondQuadCrossover: public eoQuadOp<GenotypeT>
{
public:
  /**
   * Ctor - no requirement
   */
// START eventually add or modify the anyVariable argument
  eoUCPSecondQuadCrossover(vector<int> & _tUp, vector<int> & _tDown, vector<int> & _tCS)
	{
		tUp= _tUp;
		tDown= _tDown;
		tColdStart=_tCS;
		//if(_data!=NULL){
     		 //units=_data->get_units();
      		//load=_data->get_load();
		//}
	}

  /// The class name. Used to display statistics
  string className() const { return "eoUCPSecondQuadCrossover"; }

  /**
   * eoQuad crossover - _genotype1 and _genotype2 are the (future)
   *       offspring, i.e. _copies_ of the parents, to be modified
   * @param _genotype1 The first parent
   * @param _genotype2 The second parent
   */
  bool operator()(GenotypeT& _genotype1, GenotypeT & _genotype2)
  {
cout<<"deb cross2"<<endl;
     bool oneAtLeastIsModified =true;
     int nbHeures=_genotype1.getNbHeures();
     int nbUnits=_genotype1.getNbUnits();
     int i,t,j;
     vector< vector<int> > temps1=_genotype1.getTemps();
     vector < vector<int> > temps2=_genotype2.getTemps();
     // Choix de tStart et tend.
     eoUniformGenerator<int> choixTS(0,nbHeures-3);
     int TS=choixTS();
     eoUniformGenerator<int> choixTE(TS+1,nbHeures-1);
	int TE=choixTE();
//	cout <<"TE "<<TE<<" TS "<<TS<<endl;
     //echange des parties intermediaires :
     for(i=0;i<nbUnits;i++)
     {
    	 //traitement TS:
    	 if(temps1[TS][i]!=temps2[TS][i])
    	 {
    		 _genotype1.setTemps(TS,i,temps2[TS][i]);
    		 _genotype1.setModif(TS,true);
    		 _genotype2.setTemps(TS,i,temps1[TS][i]);
    		  _genotype2.setModif(TS,true);

    	 }
	////cout<<_genotype1.getTemps(TS,i)<<endl;
    	 //traitement des autres heures:
    	 for(t=TS+1;t<TE+1;t++)
    	 {
		
    		 _genotype1.setTemps(t,i,temps2[t][i]);
		
    		 _genotype2.setTemps(t,i,temps1[t][i]);
		
    	 }
	

     }
	if(TE+1<nbHeures)
	{
		_genotype1.setModif(TE+1,true);
		_genotype2.setModif(TE+1,true);
	}
	 for(t=TS+1;t<TE+1;t++)
    	 {
		double eval1=_genotype1.getEval1(t);
		double eval2=_genotype1.getEval2(t);
		bool modif1=_genotype1.getModif(t);
    		
		_genotype1.setEval1(t,_genotype2.getEval1(t));
    		_genotype1.setEval2(t,_genotype2.getEval2(t));
		_genotype2.setEval1(t,eval1);
		_genotype2.setEval2(t,eval2);
		_genotype1.setModif(t,_genotype2.getModif(t));
		_genotype2.setModif(t,modif1);
    	 }
	

	//_genotype1.setModif(TE,true);
	//	_genotype2.setModif(TE,true);

     //transition 1 1->2:
     for(i=0;i<nbUnits;i++)
     {
    	 if(temps2[TS][i]!=temps1[TS][i])
    	 {
    		 //cas1 positif non extremal
    		 if(temps2[TS][i]>0 && temps2[TS][i]<tUp[i])
    		 {
			//cout<<"cas1"<<endl;
    			 //Ramene à 1:
    			 t=TS;
    			 j=temps2[TS][i]-1;
    			 while(j>0&&t-1>0)
    			 {
    				 t--;
    				 _genotype1.setTemps(t,i,j);
				_genotype1.setModif(t,true);
    				 j--;

    			 }
    			 if(t>0)
    			 {
    			 //lie à un tDown:
    			 //recherche tDown:
    			 int tL=t;//Temps[tL][i]=1 ou tL=1
    			 int tD=t-1;
    			 while(tD>0 && temps1[tD][i]>tDown[i] ) tD--;
    			 //lien :

    			 //s'il y a un tDow
    			 if(temps1[tD][i]<=tDown[i])
    			 {
    			 for(j=tD+1;j<tL;j++)
    			 {
    				 if(_genotype1.getTemps(j-1,i)>tColdStart[i])
    				 {
    					 _genotype1.setTemps(j,i,_genotype1.getTemps(j-1,i)-1);
    				 }
    				 else  _genotype1.setTemps(j,i,tColdStart[i]);
    				 _genotype1.setModif(j,true);
    			 }
				_genotype1.setModif(tD,true);
    			 }
    			 //sinon
    			 else{
    				 j=tDown[i];
    				 t=tL-1;
    				 while(j<0 && t>0)
    				 {
    					 _genotype1.setTemps(t,i,j);
    					  _genotype1.setModif(t,true);
    					 j++;
    					 t--;

    				 }
    			 }
    			 }
//		cout<<"fin1"<<endl;

    		 }
    		 //cas 2:
    		 else if(temps2[TS][i]<0 && temps2[TS][i]>tColdStart[i])
    		 {
			//cout<<"cas2"<<endl; 
    			 //incrementation jusque -1
    			 t=TS;
			//cout<<_genotype1.getTemps(t,i)<<endl;
    			 while(t>0 &&_genotype1.getTemps(t,i)<-1)
    			 {
				
    				 _genotype1.setTemps(t-1,i,_genotype1.getTemps(t,i)+1);
				_genotype1.setModif(t-1,true);
    				 t--;
    			 }
				 int tD=t-1;
		         if(tD>0&&tD-tUp[i]>=0 && temps1[tD-tUp[i]][i]<=tDown[i])
   			 {
    				 int cont=0;
    				 int min=tD-tUp[i];
    				 if (min<0) min=0;
    				 for(j=tD;j>min;j--)
    				 {
    					 _genotype1.setTemps(j,i,tUp[i]-cont);
    					 _genotype1.setModif(j,true);
    					 cont++;
    				 }
    			 }
			else{ if (tD>0&&tD-tUp[i]<0 && temps1[0][i]!=tUp[i])
			{
				if(temps1[0][i]<0) {cout<<"ATTENTION Negatif!"<<endl; cout<<temps1[-1][i];}
                                for(j=1;j<tD+1;j++)//TS???
                                {
                                        if(_genotype1.getTemps(j-1,i)<tUp[i])_genotype1.setTemps(j,i,_genotype1.getTemps(j-1,i)+1);
                                        else _genotype1.setTemps(j,i,tUp[i]);
                                        _genotype1.setModif(j,true);
                                }
                        }

    			 else
    			 {
    				 t=tD;
    				 while(t>0 && _genotype1.getTemps(t,i)!=tUp[i])
    				 {
    					 _genotype1.setTemps(t,i,tUp[i]);
    					_genotype1.setModif(t,true);
    					 t--;

    				 }
    			 }}
//			cout<<"fin2"<<endl;

    		 }
    		 //cas3 :
    		 else if(temps2[TS][i]==tUp[i])
    		 {
				//cout<<"cas3"<<endl;
    			 if(TS-tUp[i]>=0 &&temps1[TS-tUp[i]][i]<=tDown[i])
    			 {
    				 int min=TS-tUp[i];
    				 if (min<0) min=0;
    				 //on decremente
    				 int cont=1;
    				 for(j=TS-1;j>min;j--)
    				 {
    					 _genotype1.setTemps(j,i,tUp[i]-cont);
    					_genotype1.setModif(j,true);
    					 cont++;
    				 }
    			 }
			else if(TS-tUp[i]<0 && temps1[0][i]!=tUp[i])
			{
				for(j=1;j<TS;j++)
				{
					if(_genotype1.getTemps(j-1,i)<tUp[i])_genotype1.setTemps(j,i,_genotype1.getTemps(j-1,i)+1);
					else _genotype1.setTemps(j,i,tUp[i]);
					 _genotype1.setModif(j,true);
				}
			}
    			 else
    			 {
    				 t=TS-1;
    				 while(t>0&&_genotype1.getTemps(t,i)!=tUp[i])
    				 {
    					 _genotype1.setTemps(t,i,tUp[i]);
    					_genotype1.setModif(t,true);
    					 t--;
    				 }
				/*if(_genotype1.getTemps(t,i)!=tUp[i])//our debugger:
				{
					cout<<" tDown "<<tDown[i]<<" tUp "<<tUp[i]<<" i "<<i<<endl;
					for(t=0;t<TS+1;t++) {
					cout<<_genotype1.getTemps(t,i)<<" ";
					}cout<<endl;
					for(t=0;t<TS+1;t++) {
                                        cout<<temps1[t][i]<<" ";
                                        }
					cout<<endl;
					for(t=0;t<TS+1;t++) {
                                        cout<<temps2[t][i]<<" ";
                                        }
                                        cout<<endl;
					cout<<temps1[-1][i];
				}*/
    			 }
//			cout<<"fcas3"<<endl;
    		 }
    		 //cas4 :
    		 else
    		 {
			//cout<<"cas4 "<<temps2[TS][i]<<endl;
    			 if(TS+tColdStart[i]>=0 &&temps1[TS+tColdStart[i]][i]==tUp[i])
    			 {
//				cout<<"ici"<<endl;
    				 int min =TS+tColdStart[i];
    				 if (min<0) min=0;
    				 int cont=1;
    				 for(t=TS-1;t>min;t--)
    				 {
    					 _genotype1.setTemps(t,i,tColdStart[i]+cont);
					_genotype1.setModif(t,true);
    							 cont++;
    				 }
    			 }
			else if (TS+tColdStart[i]<0 &&temps1[0][i]!=tColdStart[i])
			{
				int ori=_genotype1.getTemps(0,i);
				for(j=1;j<TS;j++)
				{
					if(ori-j>tColdStart[i]) _genotype1.setTemps(j,i,ori-j);
					else _genotype1.setTemps(j,i,tColdStart[i]);
				 	 _genotype1.setModif(j,true);

				}
			}
    			 else
    			 {
//				cout<<"là"<<endl;
    				 //on cherche le premier tDown valide :
    				 bool trouve=false;
    				 int tD=TS-1;
    				 while(trouve==false && tD>=0)
    				 {
    					 if(temps1[tD][i]<=tDown[i]&&-(tColdStart[i]-temps1[tD][i])<TS-tD )
    						trouve=true;
    					 else tD--;
    				 }
    				 // on decremente :
//				cout<<"apres while "<<tD<<" "<<i<<endl;
				/*if(tD<0)
			{cout<<"ici cas 4 "<<tDown[i]<<endl;for(j=0;j<=TS;j++)cout<<_genotype1.getTemps(j,i)<<" ";cout<<endl;}*/
    				 for(j=tD+1;j<TS;j++)
    				 {
    					 if(_genotype1.getTemps(j-1,i)>tColdStart[i])_genotype1.setTemps(j,i,_genotype1.getTemps(j-1,i)-1);
    					 else _genotype1.setTemps(j,i,tColdStart[i]);
    					 _genotype1.setModif(j,true);
    				 }
    			 }
//				cout<<"fincas4"<<endl;
    		 }
    	 }
     }

     //transition 1 2->1:
     for(i=0;i<nbUnits;i++)
          {
         	 if(temps2[TS][i]!=temps1[TS][i])
         	 {
         		 //cas1
         		 if(temps1[TS][i]>0 && temps1[TS][i]<tUp[i])
         		 {
//				cout<<"deb1"<<endl;
         			 //Ramene à 1:
         			 t=TS;
         			 j=temps1[TS][i]-1;
         			 while(j>0&&t-1>0)
         			 {
         				 t--;
         				 _genotype2.setTemps(t,i,j);
					_genotype2.setModif(t,true);
         				 j--;

         			 }
         			 if(t>0)
         			 {
         			 //lie à un tDown:
         			 //recherche tDown:
         			 int tL=t;
         			 int tD=t-1;
         			 while(tD>0 && temps2[tD][i]>tDown[i]) tD--;
         			 //lien :

         			 //s'il y a un tDow
         			 if(temps2[tD][i]<=tDown[i])
         			 {
         			 for(j=tD+1;j<tL;j++)
         			 {
         				 if(_genotype2.getTemps(j-1,i)>tColdStart[i])
         				 {
         					 _genotype2.setTemps(j,i,_genotype2.getTemps(j-1,i)-1);
         				 }
         				 else  _genotype2.setTemps(j,i,tColdStart[i]);
         				 _genotype2.setModif(j,true);
         			 }
         			 }
         			 //sinon là
         			 else{
         				 j=tDown[i];
         				 t=tL-1;
         				 while(j<0 && t>0)
         				 {
         					 _genotype2.setTemps(t,i,j);
         					  _genotype2.setModif(t,true);
         					 j++;
         					 t--;

         				 }
         			 }
         			 }
//				cout<<"fin1"<<endl;


         		 }
         		 //cas 2:
         		 else if(temps1[TS][i]<0 && temps1[TS][i]>tColdStart[i])
         		 {
//			cout<<"deb2"<<endl;
         			 //incrementation
         			 t=TS;
         			 while(t>0 && _genotype2.getTemps(t,i)<-1)
         			 {
         				 _genotype2.setTemps(t-1,i,_genotype2.getTemps(t,i)+1);
					_genotype2.setModif(t-1,true);
         				 t--;
         			 }
         			 int tD=t-1;
         			 if(tD>0&&tD-tUp[i]>=0 && temps2[tD-tUp[i]][i]<=tDown[i])
         			 {
//					cout<<"la1"<<endl;
         				 int cont=0;
         				 int min=tD-tUp[i];
         				 if (min<0) min=0;
         				 for(j=tD;j>min;j--)
         				 {
         					 _genotype2.setTemps(j,i,tUp[i]-cont);
         					_genotype2.setModif(j,true);
         					 cont++;
         				 }
         			 }
				else if (tD>0&&tD-tUp[i]<0 && temps2[0][i]!=tUp[i]) 
				{
//				cout<<"la2"<<endl;
					for(j=1;j<tD+1;j++)
                                         {
                                                 if(_genotype2.getTemps(j-1,i)<tUp[i])_genotype2.setTemps(j,i,_genotype2.getTemps(j-1,i)+1);
                                                 else _genotype2.setTemps(j,i,tUp[i]);
                                                _genotype2.setModif(j,true);
                                         }

				}
         			 else
         			 {
//				cout<<"la3"<<endl;
         				 t=tD;
         				 while(t>0&&_genotype2.getTemps(t,i)!=tUp[i])
         				 {
         					 _genotype2.setTemps(t,i,tUp[i]);
         					_genotype2.setModif(t,true);
         					 t--;

         				 }
         			 }
//				cout<<"fin2"<<endl;

         		 }
         		 //cas3 :
         		 else if(temps1[TS][i]==tUp[i])
         		 {
//					cout<<"deb3"<<endl;
         			 if(TS-tUp[i]>=0&&temps2[TS-tUp[i]][i]<=tDown[i])
         			 {
         				 int min=TS-tUp[i];
         				 if (min<0) min=0;
         				 //on decremente
         				 int cont=1;
         				 for(j=TS-1;j>min;j--)
         				 {
         					 _genotype2.setTemps(j,i,tUp[i]-cont);
         					_genotype2.setModif(j,true);
         					 cont++;
         				 }
         			 }
				 else if(TS-tUp[i]<0 && temps2[0][i]!=tUp[i])
	               		 {
                               		 for(j=1;j<TS;j++)
                               		 {
                                       		 if(_genotype2.getTemps(j-1,i)<tUp[i])_genotype2.setTemps(j,i,_genotype2.getTemps(j-1,i)+1);
                                       		 else _genotype2.setTemps(j,i,tUp[i]);
                                 	        _genotype2.setModif(j,true);
                               		 }
                       		 }
			
         			 else
         			 {
         				 t=TS-1;
         				 while(t>0&&_genotype2.getTemps(t,i)!=tUp[i])
         				 {
         					 _genotype2.setTemps(t,i,tUp[i]);
         					_genotype2.setModif(t,true);
         					 t--;
         				 }
					/* if(_genotype2.getTemps(t,i)!=tUp[i])//our debugger:
                                {
                                        for(t=0;t<TS+1;t++) {
                                        cout<<_genotype2.getTemps(t,i)<<" ";
                                        }cout<<endl;cout<<temps1[-1][i];

                                }*/

        			 }
			//	cout<<"fin3"<<endl;
         		 }
         		 //cas4 :
         		 else
         		 {
			//	cout<<"deb4  "<<tColdStart[i]<<" "<<temps1[TS][i]<<endl;
         			 if(TS+tColdStart[i]>=0&&temps2[TS+tColdStart[i]][i]==tUp[i])
         			 {
//					cout<<"là"<<endl;
         				 int min =TS+tColdStart[i];
         				 if (min<0) min=0;
         				 int cont=1;
         				 for(t=TS-1;t>min;t--)
         				 {
         					 _genotype2.setTemps(t,i,tColdStart[i]+cont);
						_genotype2.setModif(t,true);
         							 cont++;
         				 }
         			 }
				 else if (TS+tColdStart[i]<0 &&temps2[0][i]!=tColdStart[i])
                        {
//				cout<<"ici"<<endl;
                                int ori=_genotype2.getTemps(0,i);
                                for(j=1;j<TS;j++)
                                {
                                        if(ori-j>tColdStart[i]) _genotype2.setTemps(j,i,ori-j);
                                        else _genotype2.setTemps(j,i,tColdStart[i]);
                                      _genotype2.setModif(j,true);

                                }
                        }

         			 else
         			 {
//					cout<<"ici mm"<<endl;
         				 //on cherche le premier tDown valide :
         				 bool trouve=false;
         				 int tD=TS-1;
         				 while(tD>-1&&trouve==false)
         				 {
         					 if(temps2[tD][i]<=tDown[i]&&-(tColdStart[i]-temps2[tD][i])<TS-tD )
         						trouve=true;
         					 else tD--;
         				 }
//					cout<<"tD "<<tD<<endl;
					if(tD<0)
		                        {cout<<"ici "<<tDown[i]<<" "<<tColdStart[i]<<endl;for(j=0;j<=TS;j++)cout<<_genotype2.getTemps(j,i)<<" ";cout<<endl;}

         				 // on decremente :
         				 for(j=tD+1;j<TS;j++)
         				 {
         					 if(_genotype2.getTemps(j-1,i)>tColdStart[i])_genotype2.setTemps(j,i,_genotype2.getTemps(j-1,i)-1);
         					 else _genotype2.setTemps(j,i,tColdStart[i]);
         					_genotype2.setModif(j,true);
         				 }
         			 }
//				cout<<"fin4"<<endl;
         		 }
         	 }
          }
     // transition TE à 1
     for(i=0;i<nbUnits;i++)
     {
    	 if(temps2[TE][i]!=temps1[TE][i])
    	 {
    		 //cas 1 val positive :
    		 if(temps2[TE][i]>0)
    		 {
			//cout<<"deb+"<<endl;
    			 int cont=1;
    			 //incrementer jusque tUp :
    			 bool pos=true;
			j=temps2[TE][i]+1;
    			 while(j<=tUp[i])
    			 {
    				 if(TE+cont<nbHeures){
    				 _genotype1.setTemps(TE+cont,i,j);
				
    				  _genotype1.setModif(TE+cont,true);}
    				 else pos=false;
    				 cont++;j++;

    			 }
    			 if(pos){
    			 int tD=TE+cont; //premiere periode apres tUp
    			 if(tD-tDown[i]-1<nbHeures && temps1[tD-tDown[i]-1][i]<0)
    			 {
    				 //on décrémente jusque tDown :
    				 for(j=0;j<-tDown[i];j++)
    				 {
    					 _genotype1.setTemps(tD+j,i,-j-1);
    					_genotype1.setModif(tD+j,true);
    				 }
    				 //on suit la tendance :
    				 cont=tDown[i]-1;
				if(cont<tColdStart[i]) cont=tColdStart[i];
    				 t=tD-tDown[i];
    				 while(t<nbHeures &&temps1[t][i]!=1&&temps1[t][i]!=cont)
    				 {
    					 _genotype1.setTemps(t,i,cont);
					_genotype1.setModif(t,true);
    					 if(cont>tColdStart[i])cont--;
    					 t++;

    				 }if(t<nbHeures) _genotype1.setModif(t,true);


    			 }
    			 else
    			 {
    				 int cont=1;
    				 //incrementer jusque tUp :
				j=temps2[TE][i]+1;
    				 while(j<=tUp[i]&&TE+cont<nbHeures)
    				 {
    					 _genotype1.setTemps(TE+cont,i,j);
    					 _genotype1.setModif(TE+cont,true);
    					 cont++;j++;
    				 }
    				 t=TE+cont;
    				 while(t<nbHeures && temps1[t][i]<tUp[i])
    				 {
    					 _genotype1.setTemps(t,i,tUp[i]);
    					_genotype1.setModif(t,true);
    					 t++;
    				 }if(t<nbHeures) _genotype1.setModif(t,true);
    			 }//cout<<"fin+"<<endl;
			}

    		 }
    		 //cas val négative :
    		 else
    		 {
			//cout<<"deb -"<<endl;
    			 //on decremente jusque tDown :
    			 int tD=TE;
    			 while(tD+1<nbHeures && _genotype1.getTemps(tD,i)>tDown[i])
    			 {
    				 tD++;
    				 _genotype1.setTemps(tD,i,_genotype1.getTemps(tD-1,i)-1);
    				 _genotype1.setModif(tD,true);
    			 }
    			 if(tD+tUp[i]<nbHeures && temps1[tD+tUp[i]][i]==tUp[i])
    			 {
    				 //on incrémente jusque tUp:
    				 for(j=1;j<tUp[i]+1;j++)
    				 {
    					 _genotype1.setTemps(tD+j,i,j);
    					_genotype1.setModif(tD+j,true);
    				 }if(tD+tUp[i]+1<nbHeures)_genotype1.setModif(tD+tUp[i]+1,true);
    			 }
    			 else
    			 {
    				 int cont=temps2[TE][i];
				if(cont>tDown[i]) cont=tDown[i];
    				 t=tD+1;
    				 while(t<nbHeures && (temps1[t][i]!=tColdStart[i]|| cont-1>tColdStart[i]) && temps1[t][i]!=1)
    				 {
						if(cont>tColdStart[i])cont--;
    					 _genotype1.setTemps(t,i,cont);
    					_genotype1.setModif(t,true);
    					 
    					 t++;
    				 }if(t<nbHeures) _genotype1.setModif(t,true);
    			 }
			//cout<<"fin val -"<<endl;
    		 }
    	 }
     }
     //transition de TE à 2 :
     for(i=0;i<nbUnits;i++)
         {
        	 if(temps2[TE][i]!=temps1[TE][i])
        	 {
        		 //cas 1 val positive :
        		 if(temps1[TE][i]>0)
        		 {
				//cout<<"debut casval pos"<<endl;
        			 int cont=1;
        			 //incrementer jusque tUp si possible:
        			 bool pos =true;
        			 for(j=temps1[TE][i]+1;j<=tUp[i];j++)
        			 {
        				 if(TE+cont<nbHeures)
        				 {
        				 _genotype2.setTemps(TE+cont,i,j);

        				  _genotype2.setModif(TE+cont,true);
        				 }
        				 else pos=false;
        				 cont++;


        			 }
        			 if(pos)
        			 {
        			 int tD=TE+cont; //premiere periode apres tUp
        			 if(tD-tDown[i]-1<nbHeures && temps2[tD-tDown[i]-1][i]<0)
        			 {
        				 //on décrémente jusque tDown  :
        				 for(j=0;j<-tDown[i];j++)
        				 {
        					 _genotype2.setTemps(tD+j,i,-j-1);
        					_genotype2.setModif(tD+j,true);
        				 }
        				 //on suit la tendance :
        				 cont=tDown[i]-1;
					if(cont<tColdStart[i]) cont=tColdStart[i];
        				 t=tD-tDown[i];
        				 while(t<nbHeures&&temps2[t][i]!=1&&temps2[t][i]!=cont)
        				 {
        					 _genotype2.setTemps(t,i,cont);
						 _genotype2.setModif(t,true);
        					 if(cont>tColdStart[i])cont--;
        					 t++;

        				 }if(t<nbHeures)_genotype2.setModif(t,true);


        			 }
        			 else
        			 {

        				 int t=tD;
        				 while(t<nbHeures && temps2[t][i]<tUp[i])
        				 {
        					 _genotype2.setTemps(t,i,tUp[i]);
        					 _genotype2.setModif(t,true);
        					 t++;
        				 }if(t<nbHeures)_genotype2.setModif(t,true);
        			 }
        			 }
			//	cout<<"fin cas val pos"<<endl;

        		 }
        		 //cas val négative :
        		 else
        		 {
				//cout<<"debut cas val -"<<endl;
        			 //on decremente jusque tDown :
        			 int tD=TE;
        			 while(tD+1<nbHeures && _genotype2.getTemps(tD,i)>tDown[i])
        			 {
        				 tD++;
        				 _genotype2.setTemps(tD,i,_genotype2.getTemps(tD-1,i)-1);
        				 _genotype2.setModif(tD,true);
        			 }if(tD+1<nbHeures)_genotype2.setModif(tD+1,true);
        			 if(tD+tUp[i]<nbHeures && temps2[tD+tUp[i]][i]==tUp[i])
        			 {
        				 //on incrémente jusque tUp:
        				 for(j=1;j<tUp[i]+1;j++)
        				 {
        					 _genotype2.setTemps(tD+j,i,j);
        					_genotype2.setModif(tD+j,true);
        				 }if(tD+tUp[i]+1)_genotype2.setModif(tD+tUp[i]+1,true);
        			 }
        			 else
        			 {
        				int cont=temps1[TE][i];
					if(cont>tDown[i]) cont=tDown[i];
        				 t=tD+1;
        				 while(t<nbHeures &&(temps2[t][i]!=tColdStart[i]|| cont-1>tColdStart[i])&& temps2[t][i]!=1)
        				 {
						if(cont>tColdStart[i])cont--;
        					 _genotype2.setTemps(t,i,cont);
        					_genotype2.setModif(t,true);
        					 
        					 t++;
        				 }if(t<nbHeures)_genotype2.setModif(t,true);
        			 }
			//	cout<<"fin cas val -"<<endl;
        		 }
			/*for(int h=1;h<nbHeures;h++)
			{
				int pred=_genotype1.getTemps(h-1,i);
				int actu=_genotype1.getTemps(h,i);
				if(pred>0)
				{
					if(!((pred<tUp[i] && actu==pred+1)||(pred==tUp[i] &&(actu==-1 || actu==tUp[i]))))
					{cout<<"ERREURE Croisement "<<i<<" "<<h<<" "<<TE<<" "<<TS<<endl; _genotype1.printOn(cout);cout<<_genotype1.getTemps(-1,i);}
				
				}
				else
				{
					if(!((pred>tColdStart[i]&&actu==pred-1)||(pred<=tDown[i] && actu==1)||(pred==tColdStart[i] && actu==tColdStart[i])))
					{cout<<"ERREURE Croisement"<<endl; _genotype1.printOn(cout); cout<<_genotype1.getTemps(-1,i);}
				}
			}*/
        	 }
         }
	cout<<"fin croisement"<<endl;
    return oneAtLeastIsModified;
    // END code for crossover of _genotype1 and _genotype2 objects
  }

private:
// START Private data of an eoUCPSecondQuadCrossover object
  //  varType anyVariable;
		// std::vector<eoUCPUnit> * units; /*!< Pointer to the vector that contains the units*/
  //std::vector<double> * load; /*!< Pointer to the vector that contains the hourly demand*/
	vector<int> tUp;
	vector<int > tDown;
	vector<int> tColdStart;

// END   Private data of an eoUCPSecondQuadCrossover object
};

#endif
