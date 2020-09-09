#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

#define OPEN(fd,path,flags)\
	if((fd=fopen(path,flags))==NULL){\
		perror("aprendo file");\
		exit(-1);\
	}
typedef struct _strlis{
	char*stringa;
	struct _strlis*next;
}strlis;
static coda buffer1;
static coda buffer2;
static int finelavori1;
static int finelavori2;
static pthread_mutex_t mtx1=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mtx2=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t fullbuf1=PTHREAD_COND_INITIALIZER;
static pthread_cond_t emptybuf1=PTHREAD_COND_INITIALIZER;
static pthread_cond_t fullbuf2=PTHREAD_COND_INITIALIZER;
static pthread_cond_t emptybuf2=PTHREAD_COND_INITIALIZER;
static void *reader(void*args);
static void *tokener(void*args);
static void *countandprint(void*args);
void sending1(char*buffer,int len);
void reading(FILE *fd);
void toking(char*line);
void universalsending(char*buffer,int len,coda *bufferk,pthread_cond_t *fullbufk,pthread_cond_t *emptybufk,pthread_mutex_t *mtxk,int i);
void inseriscistringhe(strlis **lis,char*string);
void readlist(strlis**l);
int main(int argc,char*argv[]){
	pthread_t tidReader;
	pthread_t tidTokener;
	pthread_t tidCountPrint;
	//char *name="procedimentocreazionelibreriastatica";
	int err;
	if(argc!=2){
		printf("numero argssbagliato\n");
		exit (-1);
	}
	init(&buffer1,5);
	init(&buffer2,4);
	if((err=pthread_create(&tidReader,NULL,&reader,argv[argc-1]))!=0){
		errno=err;
		perror("creando lettore");
	}
	if((err=pthread_create(&tidTokener,NULL,&tokener,NULL))!=0){
		errno=err;
		perror("creando tokenizzatore");
	}
	if((err=pthread_create(&tidCountPrint,NULL,&countandprint,NULL))!=0){
		errno=err;
		perror("creando contatorestampante");
	}
	pthread_join(tidReader,NULL);
	pthread_join(tidTokener,NULL);
	pthread_join(tidCountPrint,NULL);
	return 0;
}
static void *reader(void*args){
	////printf("arrivo a creare thread\n");
	//fflush(stdout);
	char *filename=(char*)args;
	FILE *fd;
	OPEN(fd,filename,"r");
	reading(fd);

	fflush(stdout);
	return (void*)0;
}

void reading(FILE *fd){
	char *buf=malloc(1024*sizeof(char));
	//char *oldtest;
	int len;
	while(fgets(buf,1024,fd)!=NULL){
		len=strlen(buf);
		buf[len-1]='\0';
		universalsending(buf,len,&buffer1,&fullbuf1,&emptybuf1,&mtx1,0);
		//sending1(buf,len);
		//oldtest=malloc(len*sizeof(char));
		//strcpy(oldtest,buffer);
	}
	pthread_mutex_lock(&mtx1);
	finelavori1=1;
	pthread_mutex_unlock(&mtx1);
	//sending1(NULL,-1);
	return;
}
void sending1(char*buffer,int len){
	////printf("arrivo a sending,input %s\n",buffer);
	//fflush(stdout);
	char *data=NULL;
	if(len!=-1){
		data=malloc(len*sizeof(char));
		strcpy(data,buffer);
	}
	pthread_mutex_lock(&mtx1);
	while(codapiena(&buffer1)){
		pthread_cond_wait(&fullbuf1,&mtx1);
	}
	accoda(&buffer1,data);
	pthread_cond_signal(&emptybuf1);
	pthread_mutex_unlock(&mtx1);
}

static void *tokener(void*args){
	char *data;
	pthread_mutex_lock(&mtx1);
	int ok=1;
	while(ok/*!codavuota(&buffer1) || !finelavori1*/){
		pthread_mutex_unlock(&mtx1);
		pthread_mutex_lock(&mtx1);
		while(codavuota(&buffer1)){
			pthread_cond_wait(&emptybuf1,&mtx1);
		}
		data=decoda(&buffer1);//dobbiamo evitare wait while holding dopo decoda lascio il lock adesso
		
		pthread_cond_signal(&fullbuf1);
	
		ok=(!codavuota(&buffer1) || !finelavori1);
		
		pthread_mutex_unlock(&mtx1);
		toking(data);
		//pthread_mutex_lock(&mtx1);
	}
	//pthread_mutex_unlock(&mtx1);
	pthread_mutex_lock(&mtx2);
	finelavori2=1;
	pthread_mutex_unlock(&mtx2);
	return (void*)0;
}
void toking(char*line){
	char *pt;
	char *word;
	word=strtok_r(line," ",&pt);
	while(word!=NULL){
		//printf("come lo mando:%s\n",word);
		universalsending(word,strlen(word),&buffer2,&fullbuf2,&emptybuf2,&mtx2,2);
		////printf("data mandato");
		word=strtok_r(NULL," ",&pt);
	}
	//printf("\n");
	//printf("toking ti saluta via\n");
	return;
}

static void *countandprint(void*args){
	strlis *lis;
	char *data;
	pthread_mutex_lock(&mtx2);
	int ok=1;
	while(ok/*!codavuota(&buffer2)||!finelavori2*/){
		pthread_mutex_unlock(&mtx2);
		pthread_mutex_lock(&mtx2);
		while(codavuota(&buffer2)){
			fflush(stdout);
			pthread_cond_wait(&emptybuf2,&mtx2);
		}
		data=decoda(&buffer2);//dobbiamo evitare wait while holding dopo decoda lascio il lock adesso
		
		pthread_cond_signal(&fullbuf2);
	
		ok=(!codavuota(&buffer2)||!finelavori2);
	
		pthread_mutex_unlock(&mtx2);
		//printf("%s\n",data);
		inseriscistringhe(&lis,data);

	}
	//pthread_mutex_unlock(&mtx2);
	printf("finelavori\n");
	printf("adesso stampo la lista di parole uniche\n");
	readlist(&lis);			
	fflush(stdout);
	return (void*)0;
}
void inseriscistringhe(strlis **lis,char*string){
	strlis *new;
	strlis *corr;
	strlis *prec;
	if(*lis==NULL){
		new=malloc(sizeof(strlis));
		new->stringa=string;
		new->next=NULL;
		*lis=new;
		return;
	}
	corr=*lis;
	while(corr!=NULL){	
		if(strcmp(corr->stringa,string)==0){
			free(string);//pulizia di stringhe inutili
			return;
		}
		prec=corr;
		corr=corr->next;
	}
	new=malloc(sizeof(strlis));
	new->stringa=string;
	new->next=NULL;
	prec->next=new;
}
void readlist(strlis **lis){
	strlis *l=*lis;
	if(l==NULL)
		return;
	printf("%s\n",l->stringa);
	readlist(&(l->next));
	free(l->stringa);//pulizia di stringhe inutili
	free(l);
}	


void universalsending(char*buffer,int len,coda *bufferk,pthread_cond_t *fullbufk,pthread_cond_t *emptybufk,pthread_mutex_t *mtxk,int i){
	
	char *data=NULL;
	if(len!=-1){
		data=malloc(len*sizeof(char));
		strcpy(data,buffer);
	}
	pthread_mutex_lock(mtxk);

	while(codapiena(bufferk)){

		fflush(stdout);
		pthread_cond_wait(fullbufk,mtxk);
	}

	accoda(bufferk,data);

	pthread_cond_signal(emptybufk);

	pthread_mutex_unlock(mtxk);
	
	fflush(stdout);
}

