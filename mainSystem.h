#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "FileManager.h"
#define true 1
#define false 0
#define MaxStocklist 20
#define MaxInfo 10

typedef struct product{
	char* barcode;
	char* name;
	int quantity;
	int price;
	struct product *prev;
	struct product *next;
}Product;

typedef struct product_List{
	Product *header;
	Product *tailer;
	int count;
}ProductList;
	
typedef struct sale_List {
	int total;
	int inMoney;
	int change;
	ProductList *productList;
}SaleList;

void displayPL(ProductList *pl);
void homeView();
void saleListView();
int returnView();
int stockView();
void powerView(int power);
int barcodeInputView();
int addQuantity(int curp, char* barcode);
void addSaleList(int curp, char* curn, char* barcode);
int numOfGoodsView();
void rmOfList();
int inMoneyView();
void calcView();
void custView();
void initSaleList();
void initProductList(ProductList *list);
int isOnSaleList(char *barcode);
void linkProduct(Product *temp, Product *newProduct);
void addProduct(ProductList *list, Product *newProduct);
Product *makeProduct(char* barcode, int count, int price, char *pName);
int checkStockList(char *barcode, int* curp, char* curn);
void createReceipt();
void rmProduct(Product *p);
void *myClock(void *param);
void makeCalendar2(char *calendar);
char *makeCalendar();
void setSalemanage();
void createRefund(char info[][MaxInfo][10],int line);
void addsale(int nday,char* shour,char* smin);
int createTotal();
void setRefundmanage(char info[][MaxInfo][10],int line);
char *makeY();
char* makesnum2(int num);
char *makeM();
char *makeDate(); 
void ScannerSensorInterface(char *b);
int TouchScreenInterface();
int PowerButtonInterface();
void DataServerInterface(char *fn, FILE *f);
void displayUpdate(char *text);
void calcControl();
int NOGControl(int n, int q);
