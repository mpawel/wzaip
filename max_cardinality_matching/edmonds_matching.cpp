#include <stdio.h>
#include <algorithm>
#include<limits.h>
#include <string>
#include<iostream>
#include<sstream>
#include <vector>
#include <list>
#include <map>
#include<set>
#include<math.h>
#include<string.h>
#include <stdint.h>

using namespace std;

#define REP(i,n) for (int i=(i); i<(n); i++)
bool compare (const pair<int,int>& ls, const pair<int,int>& rs){
    return ls.first == rs.first ? ls.second> rs.second:
				  ls.first > rs.first;
}

struct UnionFind {
    map<int, int> parent;
    int * weights;
    UnionFind() {}
    UnionFind(int size) {	weights = new int[size];    }

    int get ( int i) {
	if (parent.find(i)==parent.end()) {
	    parent[i]=i;
	    weights[i]=1;
	    return i;
	}
	vector<int> path;path.push_back(i);
	int root = parent[i];
	while (root !=path.back()) {
	    path.push_back(root);
	    root = parent[root];
	}
	for (uint i=0; i<path.size(); i++) {
	    parent[path[i]]=root;
	}
	return root;
    }
    int operator[] (int i);

    void _union(vector<int> path) {
	vector<int> roots;
	for  (uint i=0;i<path.size(); i++) roots.push_back((*this)[path[i]]);
	int hr, hw = INT_MIN;
	for(uint i = 0; i < roots.size(); ++i) {
	    if(weights[roots[i]] >= hw) {
		hr = roots[i];
		hw = weights[roots[i]];
	    }
	}
	for (uint i=0; i<roots.size();i++) {
	    int r = roots[i];
	    if ( r != hr) {
		weights[hr] += weights[r];
		parent[r] = hr;
	    }
	}
    }
};
inline int UnionFind::operator[]( int i) {
    return this->get(i);
}

struct Edmonds {

    map<int,int>  T; //odd level of tree
    map<int,int> S; //even level of tree
    vector<int> unexplored;
    map <int,pair<int,int> > base;
    vector<int> matching;
    vector<int> *G;
    int size;
    UnionFind parent;

    vector<int> findSide(int v,int w, int a) {
	vector<int> path;path.push_back(parent[v]);
	pair<int,int> b = make_pair(v,w);
	while (path.back() != a) {
	    int tnode = S[path.back()];
	    path.push_back(tnode);
	    base[tnode]=b;
	    unexplored.push_back(tnode);
	    if (T.find(tnode)!=T.end())
	    path.push_back(parent[T[tnode]]);
	    else break;
	}
	return path;
    }

void blossom(int v, int w, int a) {
    vector<int> path1, path2;
    path1 = findSide(v,w,a); path2=findSide(w,v,a);
    parent._union(path1);parent._union(path2);
    S[parent[a]]= S[a];
}

vector<int> alternatingPath(int start, int const &goal=-666) {
    vector<int> path;
    int tnode;
    while(true) {
	while (T.find(start) != T.end()) {
	    pair<int,int> tmp= base[start];
	    int v = tmp.first, w=tmp.second;
	    vector<int> p = alternatingPath(v,start);
	    for (vector<int>::reverse_iterator i=p.rbegin(); i!= p.rend(); i++)
		path.push_back((*i));
	    start=w;
	}
	path.push_back(start);
	if (matching[start] == -1)
	    return path;
	tnode= matching[start];
	path.push_back(tnode);
	if ( tnode == goal)
	    return path;
//	if (T.find(tnode)!=T.end()) //stop if not found
	start = T[tnode];
//	else {
//	    printf("nop");
//	    break; //stop if not found

//	}
    }
    return path;
}


void alternate (int v) {
    vector<int> path = alternatingPath(v);
    for (vector<int>::reverse_iterator i=path.rbegin(); i< path.rend()-1;) {
	int x = i[0], y= i[1];
	matching[x]=y;
	matching[y]=x;
	i+=2;
    }
}
void addMatch(int &v, int &w) {
    alternate(v);
    alternate(w);
    matching[v]=w;
    matching[w]=v;
}
int step(map<int,int> &path,int head) {
    head = parent[head];
    int parent_node ;
//    if (S.find(head) != S.end()) //test
    parent_node = parent[S[head]];
//    else return head; //test
    if( parent_node == head)
	return head;
    path[head] = parent_node;
//    if (T.find(parent_node)!=T.end())
    path[parent_node] = parent[T[parent_node]];
//    else return 0;
    return path[parent_node];
}

int ss(int v, int w) {
    if (parent[v] == parent[w])
	return 0;

    map<int,int> path1, path2;
    int head1=v, head2=w;

    while (true) {
	head1=step(path1,head1);
	head2=step(path2,head2);
	if ( head1==head2) {
	    blossom(v,w,head1);
	    return 0;
	}
//	if (S.find(head1)!=S.end() && S.find(head2)!=S.end() ) {
	if (parent[S[head1]] == head1 && parent[S[head2]] == head2)  {
	      addMatch(v, w);
	      return 1;
	  }
//    }
//	else  {
//	    printf("noop");
//	}
	  if (path2.find(head1)!= path2.end()) {
	      blossom(v, w, head1);
	      return 0;
	  }
	  if( path1.find(head2)!=path1.end()) {
	      blossom(v, w, head2);
	      return 0;
	  }
    }
}



bool augment() {
    parent = UnionFind(this->size);
    T.clear(); //odd level of tree
    S.clear(); //even level of tree
    unexplored.clear();
    base.clear();

    for ( int i=0; i<size; i++) {
	if (matching[i]== -1) {
	    S[i]=i;
	    unexplored.push_back(i);
	}
    }

    uint current =0;
    while (current < unexplored.size() ) {
	int v= unexplored[current];
	current++;
	for (uint i=0; i<G[v].size(); i++) {
	    int w = G[v].at(i);
	      if( S.find(parent[w]) != S.end()) {
		  if (ss(v,w)) {
		      return true;
		  }
	      } else if (T.find(w) ==T.end()) {
		  T[w] = v;
		 int u = matching[w];
		  if( S.find(parent[u])==S.end()) {
		      S[u] = w;
		      unexplored.push_back(u);
		  }
	      }
	}
    }
    return false;
}

vector<int> Edmonds_matching(vector<int> *G, const int size) {
    parent = UnionFind(size);
    matching= vector<int>(size,-1);
//    matching.resize(size,-1);
    this->size=size;
     for ( int i=0; i<size; i++) {
	 for ( uint j=0 ; j<G[i].size(); j++) {
	     if (matching[i] <0 && matching[G[i][j]]<0) {
		 matching[i]=G[i][j];
		 matching[G[i][j]]=i;
	     }
	 }
     }
     this->G=G;
     while(augment()) ;
     return matching;
}
};

typedef std::vector<std::vector<int> > Graph;

int main() {
    vector<int> * G;
    Edmonds env;    stdin=fopen("work_scheduling","r");
    int N, C;
    scanf("%u", &N);

    G = new vector<int>[N];
    for(unsigned int i, j; ~scanf("%u %u", &i, &j);) {
	if(i != j) {
	    i--;j--;
	    G[i].push_back(j);
	    G[j].push_back(i);
	}
    }
    C = 0;
    env.Edmonds_matching(G,N);

    for(unsigned int i = 0; i < N; ++i) {
	C += (env.matching[i] != -1);
    }
    printf("%u\n", C);
    for(uint i = 0; i < N; ++i) {
	if(env.matching[i] != -1 && i < (uint) env.matching[i]) {
	    printf("%d %d\n", i, env.matching[i]);
	}
    }
    return EXIT_SUCCESS;
}

