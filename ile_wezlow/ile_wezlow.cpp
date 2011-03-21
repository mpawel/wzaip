#include <stdio.h>
#include<math.h>
#define MAX 260
long double dp[MAX][MAX];


void preprocess() {
    long double comb[MAX][MAX];
    double tmp;
    comb[0][0]=1;
    for(int i=1; i<MAX; i++){
	comb[i][0]=comb[i-1][0]/2;//comb/2^n
	for(int j=1; j<=i; j++)
	    comb[i][j]=(comb[i-1][j]+comb[i-1][j-1])/2;
    }
    for(int i=1;i<MAX;i++)
	dp[i][0]=1;
    for(int i=1; i<MAX; i++)
	for(int j=1; j<MAX; j++){
	    for(int k=0; k<=i; k++)
		dp[i][j]+=comb[i][k]*(dp[i-k][j-1]+dp[k][j-1]);
	    dp[i][j]++;
	}
}

int main(){
    preprocess();
    int test_n, n,l;
    scanf("%d",&test_n);
    for ( int i=0; i<test_n; i++){
	scanf("%d%d",&n,&l);
	printf("%d\n",int(round(dp[n][l])));
    }
    return 0;
}
