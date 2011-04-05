#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdint.h>

using namespace std;


void dfs_visit (vector<int32_t>const graf[],int8_t colors[],int32_t v, int8_t c) {
    colors[v]=c;
    for (uint32_t i=0; i<graf[v].size(); i++) {
	uint32_t u =graf[v].at(i);
	if (colors[u] == 0) {
	    dfs_visit (graf,colors,u,(c==1?2:1));
	}
    }
}


int main () {
    int test_n;
    scanf("%d",&test_n);
    for  ( int t=0; t< test_n;t++) {
	int p;
	int n,m;
	scanf("%d %d %d",&n,&m,&p);
	vector<int> *G = new vector<int>[n];
	int8_t visited[n];
	memset(visited,0,n);
	for ( int j=0; j<m; j++) {
	    int v,u;
	    scanf("%d %d",&u, &v);
		    v--; u--;
	    G[u].push_back(v);
	    G[v].push_back(u);
	}

	dfs_visit(G,visited,0, 1);
	int sum =0;
	bool flag=true;

	for ( int_fast16_t i=0; i<n; i++) {
	    int_fast16_t diff=0;
	    for ( int_fast16_t j=0; j<G[i].size(); j++) {
		if (visited[i] == visited[G[i][j]])
	    diff++;
	    }
	    if (diff > 0) flag=false;
	}


	if ( flag == true) {
	for ( int_fast16_t k =0; k<n; k++) {
	    if (visited[k]==2) {

		if (G[k].size() > p) {
		   sum++;
		    }
		}
	}

	printf("%d\n",sum);
	} else printf ("NIELEGALNE\n");
	delete []G;
	}
	return 0;
}
