#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdint.h>

using namespace std;
//pierwszy bank kretydowy
//kolorujemy poczawszy od kredytowego


void dfs_visit(vector<int> * G, int n, int visited[], uint64_t& t, int start);
int * dfs (vector<int> * G, int n) {
    int *visited = new int[n];
    for ( int b =0; b<n; b++)visited[b]=0;
    uint64_t t=0;
     //0 -kredytowy 1- drugi
    for ( int i=0; i<n; i++) {
	if ( visited[i] == 0) {
	    dfs_visit(G,n,visited,t, i);
	    }
	}

    return  visited;
}

void dfs_visit(vector<int> * G, int n, int visited[], uint64_t& t, int start) {

    visited[start]= (t%2)+1;
    t++;
    for ( uint32_t i=0; i<G[start].size(); i++) {

	if (visited[G[start][i]] == 0)
	    dfs_visit(G,n,visited,t,G[start][i]);

	}

}

int main () {
    int test_n;
    scanf("%d",&test_n);
    for  ( int i=0; i< test_n; i++) {
	int p;
	int * vert_st;
	int n,m;
	scanf("%d %d %d",&n,&m,&p);
	n++;
	vector<int> * G = new vector<int>[n];
	vert_st = new int[n];
	for ( int j=0; j<m; j++) {
	    int v,u;
	    scanf("%d %d",&u, &v);
	    G[u].push_back(v);
	    G[v].push_back(u);
	    }

	int * bank_type = dfs(G,n);
	int sum =0;
	bool flag=true;
	for ( int k=0; k<n; k++) {
	    if ( bank_type[k] == 0) {
		flag=false;
		break;
		}
	    }

	if ( flag == true) {
	for ( int k =0; k<n; k++) {
	    if (bank_type[k]==2) {
		//vert_st > p
		if (vert_st[k] > p) {
		   sum++;
		    }
		}
	}

	printf("%d\n",sum);
	} else printf ("NIELEGALNE\n");
	delete [] bank_type;
	delete [] G;
	}
	return 0;
}
