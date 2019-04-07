#include <stdio.h>
#include <stdlib.h>

//program that implements the bisection alghoritmh (zeros theorem proof) on a sorted array, it returns the first discovered 0 position

int bisezione(int a[],int sx,int dx);
int cmp(const void* x,const void* y){
	int a=*(int*)x;
	int b=*(int*)y;
	if(a>b)
		return 1;
	if(a<b)
		return -1;
	return 0;
}
	
	
int main (){
	int n;
	int *a;
	int i;
	scanf("%d",&n);
	a=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
	}
	qsort(a,n,sizeof(int),cmp);
	for(i=0;i<n;i++){//sorted sequence
		printf("-> %d ",a[i]);
	}
	printf("\n");
	printf("0 position %d \n",bisezione(a,0,n-1));
	return 0;
}

int bisezione (int a[],int sx,int dx){
	if(sx>dx)
		return -1; //no 0 in this sequence
	else if(a[sx]*a[dx]>0)
		return -1; //no 0 in the sequence if both sx and dx are positive or negative
	else {
		int m=(sx+dx)/2;
		if(a[m]==0)
			return m;
		else if(a[m]>0)
			return bisezione(a,sx,m);
		else 
			return bisezione(a,m,dx);
	}
	return -1;
}
		
	
	
