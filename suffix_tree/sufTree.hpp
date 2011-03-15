#ifndef SUFTREE_HPP
#define SUFTREE_HPP

#include <string>
#include <map>
#include<set>

#define FORD(x,b,e) for ( int x=b;x>=(e);--x)
#define VAR(v,n) __typeof(n) v= (n)
#define FOREACH (i,c) for ( VAR(i,(c).begin()); i!=(c).end() ;++i)

using namespace std;

template<typename _T> struct SufTree {
    struct SufV {
	map<char,SufV *> sons;
	int p, k;
	bool s; //is sufix?
	_T e;
	SufV * par; //parent

	SufV ( int p, int k, SufV * par, bool s) : p(p), k(k), par(par), s(s) {}
	~SufV() {
	    for( typename map<char,SufV*>::iterator i=sons.begin(); i!=sons.end(); i++)
//	    FOREACH(i,sons)
		delete (*i).second;
	}

    };
//reprezentuje tablice lnk i test
    struct VIP {
	SufV *p;
	char l;
	VIP(SufV *p, char l) : p(p), l(l) {}
	bool operator<(const VIP &a) const {
	    return (a.p > p) || (a.p == p && a.l > l);
	}
    };

SufV root;
string text;
SufTree(const string &t) : root(0,0,0,0), text(t) {
    map<VIP,SufV*> lnk;
    set<VIP> test;
    int len = t.length();
//Utworz pierwszy wezel drzewa reprezentujacego ostatnia litere słowa
    SufV *v=root.sons[t[len-1]] = new SufV(len-1,len,&root,1);
    lnk[VIP(&root,t[len-1])]=v;
    test.insert(VIP(&root,t[len-1]));
//dla kolejnych sufiksow slowa(od naktroszych do najdlozszych)...
    FORD(i,len-2,0) {
	char a=t[i];
//jesli z korzenia nie wychodzi krawedz dla litery slowa na pozycji i..
	if (!root.sons[a]) {
//nastepuje aktualizacja tablicy test dla wezlow na sciezce od wezla reprezentujacego popredni sufiks do korzenia
	    SufV * it = v;
	    while (it) {
		test.insert(VIP(it,a));
		it=it->par;
	    }
	    it=v;
//dodawanie nowego syna dla korzenia
	    lnk[VIP(it,a)]=v=root.sons[t[i]]= new SufV(i,len,&root,1);
	} else {
//Przy wykorzystaniu tablicy test oraz lnk nastepuje wyznaczenie krawedzi drzewa, ktora trzeba podzielic w celu dodania kolejnego sufiksu
	    char lw;
	    SufV *head, *head2, *x, *x1;
	    int lw2=0, gl=0;
	    for (x=v; x!= &root && test.find(VIP(x,a))  == test.end(); x=x->par) lw2+=x->k-x->p;
	    for (x1=x; x1 && !lnk[VIP(x1,a)]; x1=x1->par) {
		gl +=x1->k -x1->p;
		lw = t[x1->p];
	    }
	    if (x1) gl--;
//Nastepuje podzial krawedzi drzewa sufiksowego
	    SufV * head1=x1 ? lnk[VIP(x1,a)] : &root;
	    if (x == x1) head=head1;
	    else {
		head2 = (!x1)?root.sons[a] : head1->sons[lw];
		head = head1->sons[t[head2->p]]= new SufV(head2->p,head2->p+1+gl,head1,0);
		head->sons[t[head->k]] = head2;
		head2->p= head->k;
		head2->par = head;
		for (VAR(it,test.lower_bound(VIP(head2,0))); it->p == head2;) test.insert(VIP(head,(it++)->l));
	    }
//Aktualizacja zawartosci talic test oraz lnk
	    for (SufV * it = v; it != x1; it=it->par) test.insert(VIP(it,a));
	    lnk[VIP(x,a)] = head;
	    SufV *v1 = v;
	    v=head->sons[t[len-lw2]] = new SufV(len-lw2,len,head,1);
	    lnk[VIP(v1,a)] = v;
	    }
	}
    }


};
#endif // SUFTREE_HPP
