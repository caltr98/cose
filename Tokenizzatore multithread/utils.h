typedef struct _coda{
	int dim;
	char**valori;
	int head;
	int tail;
	int elements;
}coda;
void accoda(coda*q,char*value);
void init(coda*q,int dim);
char* decoda(coda*q);
int codavuota(coda*q);
void deinit(coda *q);
int codapiena(coda *q);
