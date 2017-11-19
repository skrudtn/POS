#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 100

typedef struct stock{
	char *name;
	char *barcode;
	int price;
	int quantity;
	struct stock *prev;
	struct stock *next;
}Stock;

typedef struct stockList {
	int count;
	Stock *header;
	Stock *tailer;
}StockList;

StockList *getStockList(char* fileName,int day);
void initStockList(StockList *list);
void linkStock(Stock *temp, Stock *newProduct);
void addStock(StockList *list, Stock *newProduct);
Stock *makeStock(char *name, char *bar, int quantity, int price);
void syncStockFile(StockList *st, int nday);
char *getMyClockFile();
void setMyClockFile(char *myClock);
void makeStockList(StockList *list,int nday);
char* makesnum3(int num);
void fixStockFile(StockList *tmp, StockList *sl, char* fileName);
