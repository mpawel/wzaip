#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iterator>
#include <string.h>
#include <vector>

using namespace std;

struct Edge;
struct Vertex {
  map<char,Edge> edges; //krawedzie do synow
  Vertex *f; //suffix link
  int numer; //numer sufiksu (0 to cale slowo, -1 oznacza ze wierzcholek nie jest lisciem)
};
struct Edge{
  int l,r; //x[l]..x[r] to fragment textu reprezentujacy krawedz
  Vertex *v;
};

Vertex *suf_tree;
const char *text;
int leaf; //liczba utworzonych lisci

inline void Canonize(Edge &ed, const char *x){

    if (ed.l <= ed.r){
	Edge e = ed.v->edges[x[ed.l]];
	while (e.r - e.l <= ed.r - ed.l){
	    ed.l += e.r - e.l + 1;
	    ed.v = e.v;
	    if (ed.l <= ed.r) e = ed.v->edges[x[ed.l]];
	}
    }
}

inline bool Test_and_split(Vertex* &w, const Edge &ed){
    w = ed.v;

    if (ed.l <= ed.r){
	char c = text[ed.l];
	Edge e = ed.v->edges[c];
	if (text[ed.r + 1] == text[e.l + ed.r - ed.l + 1]) return true;
	w = new Vertex; w->numer = -1;
	ed.v->edges[c].r = e.l + ed.r - ed.l;
	ed.v->edges[c].v = w;
	e.l += ed.r - ed.l + 1;
	w -> edges[text[e.l]] = e;
	return false;
    }
    return ed.v->edges.find(text[ed.l]) != ed.v->edges.end();
}

void Update(Edge &ed, int n){
    Vertex *oldr = suf_tree, *w;

    while (!Test_and_split(w, ed)){
	Edge e;
	e.v = new Vertex; e.l = ed.r + 1; e.r = n - 1;
	e.v->numer = leaf++;
	w->edges[text[ed.r + 1]] = e;
	if (oldr != suf_tree) oldr->f = w;
	oldr = w;
	ed.v = ed.v->f;
	Canonize(ed, text);
    }
    if (oldr != suf_tree) oldr->f = ed.v;
}

Vertex* Create_suffix_tree(const char *x, int n){
    Vertex *top; //pinezka
    Edge e;
    top = new Vertex; suf_tree = new Vertex; text = x;
    top->numer = suf_tree->numer = -1;
    e.v = suf_tree; leaf = 0;

    for (int i=0; i<n; ++i) {
	e.r = -i; e.l = -i; top->edges[x[i]] = e;
}
    suf_tree->f = top;
    e.l = 0; e.v = suf_tree;
    for(int i=0;i<n;++i) {
	e.r = i - 1; Update(e, n);
	e.r++; Canonize(e, x);
    }
    return suf_tree;
}

void Delete(Vertex *w){
    for (map<char,Edge>::iterator i=w->edges.begin(); i!=w->edges.end(); i++)
	Delete((*i).second.v);
    delete w;
}

// Funkcja sprawdza, czy slowo s jest podslowem slowa x, ktorego drzewem sufiksowym
// jest r.
bool Find(const char *s, Vertex *r, const char *x){
  int i=0, n=strlen(s);
  while (i<n){
    if (r->edges.find(s[i]) == r->edges.end()) return false;
    Edge e = r->edges[s[i]];
    for (int j = e.l; j <= e.r; ++j){
     if (i == n) return true;
     if (s[i++] != x[j]) return false;
    }
    r = e.v;
  }
  return true;
}


// Funkcja main z kodem testujacym i prezentujacym dzialanie
int main(){
  string x; //napis dla ktorego budujemy drzewo sufiksowe
  int n;
  x = "abcabcaabbcc~";
  n = x.size();
  Vertex *r = Create_suffix_tree(x.c_str(), n);

  printf("%d %d %d\n", Find("abcaa", r, x.c_str()), Find("abcd", r, x.c_str()), Find("abcabcaabbcc", r, x.c_str()));


  Delete(r);
  return 0;
}

