#include<stdio.h>
#include<sufTree.hpp>

//Struktura wyznacza najdluzsze podslowo wystepujace odpowiednia liczbe razy
struct STLongestWord{
    //zmienne p i k oznaczaja poczatek i koniec wyznaczonego slowa
    int p,k,n;
    int Find(SufTree<int>::SufV &v,int d) {
//d jest glebokoscia wezla w
	d+=v.k - v.p;
	v.e =v.s;
//zlicz liczbe sufiksow osiagalnych z wezla v
	/*FOREACH( it,v.sons)*/
	    for(  map<char,SufTree<int>::SufV*>::iterator i=v.sons.begin(); i!=v.sons.end(); i++)
		v.e+=Find(*(*i).second,d);
	//jesli liczba sufiksow jest nie mniejsza niz c, a glebokosc biezocego wezla jest wieksza od dlugosci aktualnie znalezionego slowa, to zaktualizuj wynik
	if (v.e >= n && d > k-p) {
	    k=v.k;
	    p= v.k -d;
	}
	return v.e;
    }
    STLongestWord(const string &t, int n) :p(0),k(0),n(n) {
	//skonsfruuj drzewo sufiksowe oraz wyznacz wynik
	SufTree<int> tr(t);
	Find(tr.root,0);
    }
};





int main () {

    int test_n;
    scanf("%d",&test_n);


    for ( int test=0; test<test_n; test++) {
	string s,s2;
	char text[200000];
	int n=0;
	scanf("%d",&n);
	scanf("%s",text);
	s.assign(text);
	STLongestWord lw(s,n);
	s2=s.substr(lw.p,lw.k-lw.p);
	printf("%s\n",s2.c_str());
    }
}
