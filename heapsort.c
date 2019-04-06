#include <stdio.h>
#include <stdlib.h>

typedef struct harray{
	int *array;
	int arraysize;
	int heapsize;
}	heaparray;

void maxheapify(heaparray *a,int i);
void buildmaxheap(heaparray *a);
void heapsort(heaparray *a);
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
	heaparray a;
	int i;
	scanf("%d",&(a.arraysize));
	a.heapsize=(a.arraysize) - 1; 
	a.array=(int*)malloc((a.arraysize)*sizeof(int));
	for(i=0;i<a.arraysize;i++)
		scanf("%d",&a.array[i]);
	heapsort(&a);
	for(i=0;i<a.arraysize;i++)
		printf("%d ",a.array[i]);
	printf("\n");
	return 0;
}


void maxheapify(heaparray *a,int i){
	int largest;
	int l=left(i);
	int r=right(i);
	if(l<=a->heapsize && a->array[i]<a->array[l])
		largest=l;
	else largest=i;
	if(r<=a->heapsize && a->array[largest]<a->array[r])
		largest=r;
	if(largest!=i){
		swap(&a->array[i],&a->array[largest]);
		maxheapify(a,largest);
	}
}
	
	
	
	
void buildmaxheap(heaparray *a){
	int i;
	for(i=a->heapsize/2;i>=0;i--){
		maxheapify(a,i);
	}
}
	
void heapsort(heaparray *a){
	int i;
	buildmaxheap(a);
	/*swap(&a[0],&a[heapsize]);
	heapsize--;
	maxheapify(a,0,heapsize);
	swap(&a[0],&a[heapsize]);
	heapsize--;	
	maxheapify(a,0,heapsize);
	*/
	for(i=a->heapsize;i>=1;i--){
		swap(&(a->array[0]),&(a->array[a->heapsize]));	
		a->heapsize=a->heapsize - 1;
		maxheapify(a,0);
	}
}
		
		
