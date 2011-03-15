#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <string.h>



struct kmp {
    //pref zaczyna sie od 1
    int pref[1005];
    const char *bad;
    int n;
    int q;

    void cperf(const char *P, int n) {
	bad = P;
	this->n=n;
	pref[1]=0;
	for (int32_t k=0,q=2; q<=this->n;q++) {
	    while (k && P[k+1]!=P[q]) {
		k=pref[k];
		}
	    if(P[k+1]==P[q])
		k=k+1;
	    pref[q]=k;
	    }
	this->q=0;
	}

    void reset() {
	this->q =0;
	}

    bool push (char c) {
	while(q && bad[q+1]!=c ) {
	    q=bad[q];
	    }
	if(bad[q+1]==c)
	    q=q+1;
	if (q==n) {
	    //znaleziono pewna kombinacje
	    q=pref[q];
	    return true;
	    }
	return false;
	}



};

int compare (const void * a, const void * b)
{
  return strcmp( (*(kmp*)a).bad , (*(kmp*)b).bad );
}


kmp kmps[16];
int kmp_ok;
int main () {

    int fail_n;
    scanf("%d\n",&fail_n);

    for ( int i=0; i<fail_n; i++) {
	char *p = new char[10005];
	scanf("%s\n",p+1);
	p[0]=0;
	int n=strlen(p+1);
	kmps[i].cperf(p,n);
	}

    qsort(kmps,fail_n,sizeof(kmp),compare);
    char c;
    c=getchar();

    for ( ; c!=EOF;) {
	kmp_ok =0;
	for ( int i=0; i<fail_n; i++) {
	    if (kmps[i].push(c)) {
		kmp_ok+=1<<i;
		}
	    }
	    if (kmp_ok) {
		for ( int i=0; i<fail_n; i++)
		    kmps[i].reset();
		putchar('*');
		}
	    else putchar(c);

	c=getchar();
	}


    return 0;
}


