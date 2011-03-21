#include <stdio.h>
#include <map>
#include <algorithm>
#include <vector>
#include <limits.h>
#include <set>

using namespace std;

struct Edge {
    int to, from;
    int weight;
    Edge() : to(0),from(0), weight(0) {}
    Edge (int f, int t, int w) :to(t),from(f), weight(w) {}

    bool operator ==( const Edge& rhs) const{
	return (this->to == rhs.to && this->weight == rhs.weight && this->from == rhs.from);
    }
//    bool operator<(const Edge &rhs) const{
//	return (this->weight < rhs.weight);
//    }
};

struct Edmonds {
    set<int> cycle;
    bool *visited;
    vector< vector<Edge> > Edmond(int root, vector< vector<Edge> > reverseG) {
	visited = new bool[reverseG.size()];
	for ( int i=0; i<reverseG.size(); i++) visited[i]=false;

	//remove all edges entering to root
	if (reverseG[root].empty() == false)
	    reverseG[root].clear();
	// for each node, select the edge entering it with smallest weight
	vector< vector<Edge> > outEdges;outEdges.resize(reverseG.size());
	for (int i=0; i<reverseG.size(); i++) {
	    Edge emin;
	    if (reverseG[i].empty()==false) {
		emin = reverseG[i][0];
		for ( int j=0; j<reverseG[i].size(); j++) {
		    if (reverseG[i][j].weight < emin.weight) {
			emin = reverseG[i][j];
		    }
		}
	    } else continue;
	    outEdges[emin.to].push_back(Edge(emin.to,emin.from,emin.weight));
	}
	//detect cycles
	vector<set<int> > cycles;
	cycle.clear();
	getCycle(root, outEdges);
	cycles.push_back(cycle);
	for ( int i=0; i<outEdges.size(); i++) {
		if (visited[i]==false) {
		    cycle.clear();
		    getCycle(i,outEdges);
		    cycles.push_back(cycle);
	    }
	}
	// for each cycle formed, modify the path to merge it into another part of the graph
	vector< vector<Edge> > outEdgesReverse; outEdgesReverse.resize(outEdges.size());
	for ( int i=0; i<outEdges.size(); i++) {
	    for ( int j=0; j<outEdges[i].size(); j++) {
		Edge e=outEdges[i][j];
		outEdgesReverse[e.to].push_back(Edge(e.to,e.from,e.weight));
	    }
	}
	for ( int i=0; i<cycles.size(); i++) {
	    if (cycles[i].find(root)==cycles[i].end())
		mergeCycles(cycles[i],reverseG,outEdges,outEdgesReverse);
	}
	return outEdges;
    }



void mergeCycles(set<int> &cyc, vector< vector<Edge> > &reverseG,vector< vector<Edge> > &outEdges, vector< vector<Edge> > &outEdgesReverse) {

    vector<Edge> cycleAllinEdge;
    Edge minInternal;minInternal.from=-666;
    for ( set<int>::iterator i=cyc.begin(); i!=cyc.end(); i++) {
	int n=(*i);
	for ( int j=0; j<reverseG[n].size(); j++) {
	    if (cyc.find(reverseG[n][j].to)!=cyc.end()) {
		if (minInternal.from== -666 || minInternal.weight > reverseG[n][j].weight) {
		    minInternal=reverseG[n][j];
		    continue;
		}
	    } else {
		cycleAllinEdge.push_back(reverseG[n][j]);
	    }
	}
    }
    // find the incoming edge with minimum modified cost
    Edge minExternal;minExternal.weight=-666;
    int minModifiedWeight = INT_MAX;
    for (int i=0; i<cycleAllinEdge.size(); i++) {
	Edge e= cycleAllinEdge[i];
	int w = e.weight - (outEdgesReverse.at(e.from).at(0).weight - minInternal.weight);
	if (minExternal.weight==-666 || minModifiedWeight > w) {
	    minExternal = e;
	    minModifiedWeight=w;
	}
    }
    // add the incoming edge and remove the inner-circuit incoming edge
    Edge removing;
    if (outEdgesReverse.at(minExternal.from).empty()==true) return;
    removing= outEdgesReverse.at(minExternal.from).at(0);
    outEdgesReverse.at(minExternal.from).clear();
    outEdgesReverse.at(minExternal.to).push_back(Edge(minExternal.to,minExternal.from, minExternal.weight));
    for (vector<Edge>::iterator i=outEdges[removing.to].begin(); i!=outEdges[removing.to].end();  i++ ) {

	if ( (*i).to == removing.from) {
	    outEdges[removing.to].erase(i);
	    break;
	}
    }
    outEdges[minExternal.to].push_back(Edge(minExternal.to,minExternal.from, minExternal.weight));
}



    void getCycle(int n, vector<vector<Edge> > outEdges){
	visited[n] = true;
	cycle.insert(n);
	if(outEdges[n].empty() == true) return;
	for ( int i=0; i<outEdges[n].size(); i++)
		if(visited[outEdges[n][i].to]==false){
		    getCycle(outEdges[n][i].to, outEdges);
		}
	}
};



int main() {
   int test_n; //ilosc testos
   scanf("%d",&test_n);
   for ( int t=1; t<=test_n; t++) {
       int v, e;//ilosc wezlow, krawedzi
       scanf("%d %d",&v, &e);
       vector< vector<Edge> > r_graph;r_graph.resize(v);

       for ( int i=0; i<e; i++) {
	   int from, to, weight; //wezel, poloczenie, waga
	   scanf("%d %d %d",&from, &to, &weight);
	   r_graph[to].push_back(Edge(to,from,weight));
       }

       Edmonds ed;
       vector< vector<Edge> > result = ed.Edmond(0,r_graph);
       int sum=0;
       for ( int i=0; i<result.size(); i++)
	   for ( int j=0; j<result[i].size(); j++)
	       sum+=result[i][j].weight;
       if ( sum == INT_MAX)printf("Case #%d: Possums!\n",t);
       else printf("Case #%d: %d\n",t,sum);
   }
}
