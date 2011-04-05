#include  <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;



int mex ( vector<int> heap) {

    sort(heap.begin(), heap.end());

//    for ( int i=0; i<heap.size(); i++)
//	printf("%d",heap[i]);

    int prev=0;
    for ( int i=0; i<heap.size(); i++) {
//	printf("%d %d\n",heap[i],prev+1);
	int test = heap[i]-prev+1;
	if ( (heap[i] - (prev+1)) > 0) {
	    return prev+1;
	}
	prev=heap[i];
    }
    return 0;
}

int nimber_addition ( int a, int b) {
    vector<int> abprim;
    for ( int i=0; i<a; i++) abprim.push_back(i+b);
    for ( int i=0; i<b; i++) abprim.push_back(i+a);

    return mex(abprim);
}

int nimadd ( vector<int> h) {
    int sum=h[0];
    for ( int i=1;h.size();i++)
	sum=nimber_addition(sum,h[i]);
}

void solve() {


    int test_n;
    scanf("%d",&test_n);
    for( int t=0; t<test_n; t++) {
	vector<int> h;
	int n;
	for ( int ni=0; ni<n; ni++) {
	    vector<int> hh;
	    int dn=1;
	    char d,dprev;
	    scanf("%c",&dprev);

	    while ( scanf("%c",&d) > 0) {
		if (d == dprev ) {
		    dn++;
		} else {
		    hh.push_back(dn);
		    dn=1;
		}
		dprev=d;
	    }
	    hh.push_back(dn);
	    h.push_back(nimadd(hh));
	}
	printf("%d ",h.back());
    }

}

int main () {

    stdin = fopen("test","r");
//    int t[] = {0,1,3,4,5,6,7};
//    vector<int> test;test.assign(t,t+7);
//    printf("\n%d\n",mex(test));

//    vector<int> hh;
//    int dn=1;
//    char d,dprev;
//    scanf("%c",&dprev);

//    while ( scanf("%c",&d) > 0) {
//	if (d == dprev ) {
//	    dn++;
//	} else {
//	    hh.push_back(dn);
//	    dn=1;
//	}
//	dprev=d;
//    }
//    hh.push_back(dn);
//    for ( int i=0; i<hh.size(); i++)
//	printf("%d ",hh[i]);


solve();
    return 0;
}
