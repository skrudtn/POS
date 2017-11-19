#include "FileManager.h"

int getCharArr(char *name);
void makeStockList(StockList *list, int nday){
	Stock *newst;
	char fn[100];
	char sday[2];
	
	// name , barcode, quantity, price
	newst = makeStock("orange","001",100,100);
	addStock(list, newst);
	newst = makeStock("beer","010",100,310);
	addStock(list, newst);
	newst = makeStock("wine","100",100,200);
	addStock(list, newst);
	newst = makeStock("bread","101",100,130);
	addStock(list, newst);
	newst = makeStock("cheese","110",100,110);
	addStock(list, newst);
	newst = makeStock("pokachip","111",100,210);
	addStock(list, newst);
	newst = makeStock("ramyun","011",100,220);
	addStock(list, newst);
	newst = makeStock("gun","010",100,15000);
	addStock(list, newst);
	
   syncStockFile(list,nday);
	
}

/*
void DataServerInterface(char *fn, FILE *f){
	f = fopen(fn, "r");
}*/
StockList *getStockList(char *fileName,int nday){
	char *temp;
	char *name;
	char *bar;
	int num;
	int price;
	FILE *fd;
	StockList *stockList;
	stockList = (StockList*)malloc(sizeof(StockList));
	initStockList(stockList);
	fd = fopen(fileName, "r");
	//DataServerInterface(fileName, fd);
	if (fd== NULL) {
		makeStockList(stockList,nday);
		
		//printf("file not opened!\n");
	}else{
		while(!feof(fd)){
			name = malloc(sizeof(char*));
			bar = malloc(sizeof(char*));
			fscanf(fd, "%s %d %s %d", name, &price, bar, &num);
			name[strlen(name)] = '\0';
			Stock *newStock = makeStock(name, bar, num, price);
			addStock(stockList, newStock);
		}
		fclose(fd);
		return stockList;
	}
	return stockList;
}

char* makesnum3(int num){
	char *tt2 = malloc(sizeof(char) * 2);
	char temp[4];
	char tt[4];
	strcpy(temp,"0");
	
	sprintf(tt,"%d",num); 
	if(num < 10){
		strcat(temp,tt);	
	}else{
		strcpy(temp,tt);	
	}
	
    strcpy(tt2,temp);
    return tt2;
}

void syncStockFile(StockList *st, int nday) {
	char fn[100];
	char* sday = makesnum3(nday);
	strcpy(fn,"");
	strcat(fn,"stock/201711");
	strcat(fn,sday);
	strcat(fn,"_product.txt");
	
//	printf("%s",fn);
	FILE *f;
	f = fopen(fn, "wt");
	if (f == NULL) {
		printf(" file not opened\n");
	}else{
	
		Stock *temp = st->header->next;
		while (temp != st->tailer->prev) {
			//printf("%s %d %s %d\r\n", temp->name, temp->price, temp->barcode, temp->quantity);
			fprintf(f, "%s %d %s %d\r\n", temp->name, temp->price, temp->barcode, temp->quantity);
			temp = temp->next;
		}
		fprintf(f, "%s %d %s %d", temp->name, temp->price, temp->barcode, temp->quantity);
	}
	fclose(f);
}
void fixStockFile(StockList *tmp, StockList *sl, char* fileName) {
	Stock *temp = tmp->header->next;
	
	while (temp != tmp->tailer) {
		
		Stock *slt = sl->header->next;
		while (slt != sl->tailer) {
		
			if(strncmp(temp->name,slt->name,4)==0){
			
				slt->quantity += temp->quantity;
				break;	
			}
			slt = slt->next;
		}
		
		temp = temp->next;
	}

	FILE *f;
	f = fopen(fileName, "wt");
	if (f == NULL) {
		printf(" file not opened\n");
	}
	temp = sl->header->next;
	while (temp != sl->tailer->prev) {
		//printf("%s %d %s %d\r\n", temp->name, temp->price, temp->barcode, temp->quantity);
		fprintf(f, "%s %d %s %d\r\n", temp->name, temp->price, temp->barcode, temp->quantity);
		temp = temp->next;
	}
	fprintf(f, "%s %d %s %d", temp->name, temp->price, temp->barcode, temp->quantity);
	fclose(f);
}
char *getMyClockFile() {
	FILE *f;
	char *myClock = (char*)malloc(sizeof(20));
	f = fopen("time/time.txt", "r");
	if (f == NULL) {
		printf(" file not opened\n");
	}else{
		fscanf(f, "%s", myClock);
	}

	//printf("load time.. %s\n", myClock);
	fclose(f);
	return myClock;
}

void setMyClockFile(char *myClock) {
	FILE *f;
	f = fopen("time/time.txt", "w");
	if (f == NULL) {
		printf(" file not opened\n");
	}else{
		fprintf(f, "%s", myClock);
	}
	
	fclose(f);
}

void initStockList(StockList *list) {
		
	list->count = 0;
	list->header = makeStock(0,0,0,0);
	list->tailer = makeStock(0,0,0,0);
	list->header->prev = NULL;
	list->header->next = list->tailer;
	list->tailer->prev = list->header;
	list->tailer->next = NULL;

}

void linkStock(Stock *temp, Stock *newR) {
	newR->prev = temp->prev;
	newR->next = temp;
	temp->prev->next = newR;
	temp->prev = newR;
}
void addStock(StockList *list, Stock *newStock) {
	Stock *temp = list->header->next;
	while (temp != list->tailer) {
		temp = temp->next;
	}
	linkStock(temp, newStock);
	list->count++;
}
Stock *makeStock(char *name, char *bar, int quantity, int price){
	Stock *s = (Stock*)malloc(sizeof(Stock));
	//s->next = NULL;
	//s->prev = NULL;
	
	s->barcode = bar;
	s->name = name;
	s->price = price;
	s->quantity = quantity;
	return s;
}
