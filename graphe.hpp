#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <numeric>
#include <queue>

using namespace std;

class TPGraphe {
private:
	bool orientation;
	int i=0;
	int nb_som;
	int nb_arcs;
	map<string,int> sommet;
	bool negatif ;
	map<pair<int,int>,int> fn_lg;
	map<int, set<int> > pred;
    map<int, set<int> > succ;
    
	
public:
  enum EtatSommet { INCONNU , EN_COURS , FAIT };
  TPGraphe(bool _oriented = true) { 
	  orientation = _oriented; 
	    nb_som=0;
	    nb_arcs=0;
  }
  TPGraphe(bool _oriented, int nb_sommets) {
	  nb_som=0;
	  nb_arcs=0;
	  orientation = _oriented;
	  if (nb_sommets <0)
	  throw out_of_range("nombre de sommets négatifs");
	  for (int i=0;i<nb_sommets;i++){
		  sommet["x" + to_string(i)]=nb_som ;
		  nb_som++;
     }
  }
  class ParcoursData {
   public :
   EtatSommet etat ;
   string pere ;
   unsigned date_deb , date_fin ;
   };
  /*TPGraphe(int nombre_sommets,bool orient ){
	  nombre_sommets = nb_som;
	  orient = orientation;
	  }*/
  bool estOriente() { 
	  return orientation;
  }
  int aPourOrdre() {
	  return nb_som;
  }
  int aPourTaille() {
	  return nb_arcs;
  }
  int aPourIndice(string _sommet) {
	  return sommet.at(_sommet);
  }
  void ajout(string _sommet) {
	   sommet[_sommet]=nb_som;
	   nb_som++;
  }
  void ajout(string s1, string s2, double value = 1.0) {
	  
	  // ajouter les sommets ?
    int i1, i2;
    try {
      i1 = aPourIndice(s1);
    } catch (out_of_range const &e) {
      ajout(s1);
      i1 = aPourIndice(s1);
    }
    try {
      i2 = aPourIndice(s2);
    } catch (out_of_range const &e) {
      ajout(s2);
      i2 = aPourIndice(s2);
    }
    // existe-t-il ?
    bool exists = true;
    try {
      // ci-dessous 'tmp' ne sert à rien d'autre qu'à tester =>
      // le __attribute__((unused)) évite le warning du compilateur
      __attribute__((unused)) int tmp = fn_lg.at((pair<int,int>){i1,i2});
    } catch (out_of_range const &e) {
      exists = false;
    }
    if (exists)
      return;
    // TODO negatif = negatif or (value < 0); 
    if (orientation) {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      nb_arcs+= 1;
    } else { // non orient
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      pred[i1].insert(i2);
      succ[i2].insert(i1);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      fn_lg[(pair<int,int>){i2,i1}] = value;
      nb_arcs += 2;
    }
	  
	  
	  }
  void ajout(int i1, int i2, double value = 1.0) {
	  // ajouter les sommets ?
    if (i1 >= nb_som or i2 >= nb_som or i1 < 0 or i2 <0)
      throw out_of_range("indice incorrect");
    // existe-t-il ?
    bool exists = true;
    try {
      __attribute__((unused)) int tmp = fn_lg.at((pair<int,int>){i1,i2});
    } catch (out_of_range const &e) {
      exists = false;
    }
    if (exists)
      return;
    negatif = negatif or (value < 0); 
    if (orientation ) {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      nb_arcs  += 1;
    } else {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      pred[i1].insert(i2);
      succ[i2].insert(i1);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      fn_lg[(pair<int,int>){i2,i1}] = value;
      nb_arcs += 1;
    }
	  
  }
  float longueur(string s1, string s2) {
	  int i1 = aPourIndice(s1); 
      int i2 = aPourIndice(s2);
       return fn_lg[(pair<int,int>){i1,i2}];
	  
  }
  float longueur(int x0, int x1) {
	  return fn_lg[(pair<int,int>){x0,x1}];
  }
  vector <int> aPourSuccesseurs(string s) {
    vector<int> v;
    int idS = aPourIndice(s);
    for (auto i: succ[idS])
      v.push_back(i);
    return v;
  }
  vector <int> aPourSuccesseurs(int s) {
    vector<int> v;
    for (auto i: succ[s])
      v.push_back(i);
    return v;
  }
  vector <int> aPourPredecesseurs(string s) {
    vector<int> v;
    int idS= aPourIndice(s);
    for (auto i: pred[idS])
      v.push_back(i);
    return v;
  }
  vector <int> aPourPredecesseurs(int s) {
    vector<int> v;
    for (auto i: pred[s])
      v.push_back(i);
    return v;
  }
  vector<pair<int,int>> aPourArcs() {
    vector<pair<int,int>> arcs;
    for (auto a: fn_lg) {
      arcs.push_back(a.first);
    }
    return arcs;
  }
  string aPourLabel(int indice){
    for(auto e: sommet) {
		if (e.second == indice)
			return e.first;
  }
	throw out_of_range(" indice inexistant ");	
  }
  
  vector<ParcoursData>  ParcoursProfondeur(){
	vector<ParcoursData> d;
	// init structure
	for(int i=0;i<aPourOrdre();i++){
		d.push_back(ParcoursData());
		d[i].etat=INCONNU;
		d[i].pere="";
	}
	// parcours des CFC
	int temps=0;
	for (auto s:sommet) {
		if(d[s.second].etat==INCONNU){
			VisiterParcoursProfondeur(d, s.second, temps);	
		}
	}
	// renvoie résultat
	return d;
   }
   vector<ParcoursData>  ParcoursLargeur(){
	   vector<ParcoursData> d;
	  //initialisation structure
	  for(int i=0;i<aPourOrdre();i++){
		d.push_back(ParcoursData());
		d[i].etat=INCONNU;
		d[i].pere="";
      }
      //parcours des CFC
      int temps=0;
      for (auto s:sommet) {
		if(d[s.second].etat==INCONNU){
			VisiterParcoursLargeur(d, s.second, temps);	
		}
	  }
      // renvoie résultat
	  return d;
    }
   
  void VisiterParcoursProfondeur(vector<ParcoursData>& p, int v, int &temps){
	temps += 1;
	p.at(v).date_deb=temps;
	p.at(v).etat=EN_COURS;
	for (auto w:succ[v]){
		if (p.at(w).etat==INCONNU){
			p.at(w).pere=v;
			VisiterParcoursProfondeur(p,w,temps);
		}
	}
	p.at(v).etat=FAIT;
	temps += 1;
	p.at(v).date_fin=temps;
  }
  
 void VisiterParcoursLargeur(vector<ParcoursData> p, int s, int &temps){
	temps += 1;
	p.at(s).etat=EN_COURS;
	queue < int > F ;
	F.push(s);
	 while(not F.empty()){
	   int courant=F.front();
	   temps += 1;
	   p.at(courant).date_fin=temps+1;
	   p.at(courant).etat=FAIT;
	   for (auto e:succ[courant]){
		   if(p.at(e).etat==INCONNU){
			  p.at(e).etat=EN_COURS;
			  p.at(e).pere=courant;
			  F.push(e);
		   }
	   }
	  }
  }
  
  
  //Partie 3  Composantes Fortement Connexes – Mise en niveaux
  // recherche de CFC par Tarjan
  class ParcoursTarjan{
	  int index, dateMin, cfc;
	  bool estDansPile;  
  };
  
  
  
	 
	
	 
	  
 
  
  
  	
};

#endif
