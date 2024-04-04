
#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include<algorithm>
#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<string>
#include<utility>
#include<limits>

using namespace std;

class TPGraphe {
  struct Arc {
    int s1, s2;
    double value;
  };
  struct AdjList {
    vector<int> idx;
    vector<int> sommets;
  };
  bool oriented;
  bool negatif; // vrai ssi arc de longueur négative
  int ordre=0;
  int taille=0;
  map<string,int> sommets;
  map<int, set<int> > pred;
  map<int, set<int> > succ;
  map<pair<int,int>, int> fn_lg;

public:
  ////////// PARTIE 1 //////////
  TPGraphe(bool _oriented = true) :
    oriented(_oriented), negatif(false)
  {};
  TPGraphe(bool _oriented, int nb_sommets) :
    oriented(_oriented), negatif(false)
  {
    if (nb_sommets < 0)
      throw out_of_range("nombre de sommets négatifs");
    string base = "x";
    for (int i = 0; i < nb_sommets; i++) {
      ajout(base+to_string(i));
    }
  };
  bool estOriente() {
    return oriented;
  }
  int aPourOrdre() {
    return ordre;
  }
  int aPourTaille() {
    return taille;
  }
  int aPourIndice(string _sommet) {
    return sommets.at(_sommet);
  }
  void ajout(string _sommet) {
    sommets[_sommet] = ordre;
    ordre += 1;
  }
  void ajout(string s1, string s2, int value = 1) {
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
    negatif = negatif or (value < 0); 
    if (oriented) {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      taille += 1;
    } else { // non orient
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      pred[i1].insert(i2);
      succ[i2].insert(i1);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      fn_lg[(pair<int,int>){i2,i1}] = value;
      taille += 2;
    }
  }
  void ajout(int i1, int i2, int value = 1) {
    // ajouter les sommets ?
    if (i1 >= ordre or i2 >= ordre or i1 < 0 or i2 <0)
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
    if (oriented) {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      taille += 1;
    } else {
      // ajout successeur/predecesseur
      pred[i2].insert(i1);
      succ[i1].insert(i2);
      pred[i1].insert(i2);
      succ[i2].insert(i1);
      // memorise longueur arc
      fn_lg[(pair<int,int>){i1,i2}] = value;
      fn_lg[(pair<int,int>){i2,i1}] = value;
      taille += 1;
    }
  }
  int longueur(string s1, string s2) {
    int i1 = aPourIndice(s1); 
    int i2 = aPourIndice(s2);
   return fn_lg[(pair<int,int>){i1,i2}];
  }
  int longueur(int x0, int x1) {
    return fn_lg[(pair<int,int>){x0,x1}];
  }

  ////////// PARTIE 2 //////////
  vector<int> aPourSuccesseurs(string s) {
    vector<int> v;
    int idS = aPourIndice(s);
    for (auto i: succ[idS])
      v.push_back(i);
    return v;
  }
  vector<int> aPourSuccesseurs(int s) {
    vector<int> v;
    for (auto i: succ[s])
      v.push_back(i);
    return v;
  }
  vector<int> aPourPredecesseurs(string s) {
    vector<int> v;
    int idS = aPourIndice(s);
    for (auto i: pred[idS])
      v.push_back(i);
    return v;
  }
  vector<int> aPourPredecesseurs(int s) {
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

  }; // TPGraphe

#endif
