#include <stdio.h>
#include <stdlib.h>

void maxheapify(int a[],int sx,int hpsize);
void buildmaxheap(int a[],int sx,int hpsize);
void heapsort(int a[],int n);
int parent(int i){
	return (i/2);
}
int left(int i){
	return ((i*2)+1);
}
int right (int i){
	return ((i*2)+2);
}
void swap(int *p,int *q){
	int tmp;
	tmp=*p;
	*p=*q;
	*q=tmp;
}

int main () {
	int n;
	int *a;
	int i;
	scanf("%d",&n);
	a=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	heapsort(a,n);
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;
}


void maxheapify(int a[],int sx,int hpsize){
	int largest;
	int l=left(sx);
	int r=right(sx);
	if(l<=hpsize && a[sx]<a[l])
		largest=l;
	else largest=sx;
	if(r<=hpsize && a[largest]<a[r])
		largest=r;
	if(largest!=sx){
		swap(&a[sx],&a[largest]);
		maxheapify(a,largest,hpsize);
	}
}
	
	
	
	
void buildmaxheap(int a[],int sx,int hpsize){
	int i;
	for(i=hpsize/2;i>=0;i--){
		maxheapify(a,i,hpsize);
	}
}
	
void heapsort(int a[],int n){
	int heapsize=n-1;
	int i;
	buildmaxheap(a,0,heapsize);
	/*swap(&a[0],&a[heapsize]);
	heapsize--;
	maxheapify(a,0,heapsize);
	swap(&a[0],&a[heapsize]);
	heapsize--;	
	maxheapify(a,0,heapsize);
	*/
	for(i=n-1;i>=1;i--){
		swap(&a[0],&a[heapsize]);	
		heapsize--;
		maxheapify(a,0,heapsize);
	}
}
		
		
