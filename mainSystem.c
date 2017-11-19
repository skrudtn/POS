#include "mainSystem.h"

int year = 2017;
int mon = 11;
int hour = 0;
int day = 15;
int min;
int daychange = 0;
int Ismain = 0;
char scoment[100];
char ccoment[100];
StockList *st;
SaleList *sl;
int main() {
	//printf("ff");
	char *clock = getMyClockFile();
	char yyyy[5];
	char mm[3];
	char dd[3];
	char hh[3];
	char min2[3];
	char cc[12];
	yyyy[4] = '\0';
	mm[2] = '\0';
	dd[2] = '\0';
	hh[2] = '\0';
	min2[2] = '\0';
	
	strcpy(cc,clock);
	strncpy(yyyy,cc,4);
	strncpy(mm,cc+4,2);
	strncpy(dd,cc+6,2);
	strncpy(hh,cc+8,2);
	strncpy(min2,cc+10,2);
	
	
	year = atoi(yyyy);
	mon = atoi(mm);
	day = atoi(dd);
	hour = atoi(hh);
	min = atoi(min2);
	
	
	powerView(0);
	return 0;
}
void refreshStockList(int nday){
	char *calendar = makeCalendar();
	char *fileName = malloc(sizeof(char) * 40);
	strcpy(fileName, "stock/");
	strcat(fileName, "201711");
	strcat(fileName,makesnum2(nday+1));
	strcat(fileName, "_product.txt");
	//printf("%s",fileName);
	StockList *stockList = getStockList(fileName, nday+1);
	
	Stock *temp = stockList->header->next;
	while (temp != stockList->tailer) {
		temp->quantity = 100;
		temp = temp->next;
	}
	st = stockList;
	//syncStockFile(stockList,nday+1);
}
void powerView(int power) {
	int *min = (int*)malloc(sizeof(int));
	*min = 10;
	pthread_t p;
	pthread_create(&p, NULL, myClock, min);

	char *calendar = makeCalendar();
	char *fileName = malloc(sizeof(char) * 40);
	strcpy(fileName, "stock/");
	strcat(fileName, calendar);
	strcat(fileName, "_product.txt");
	//printf("%s",fileName);
	st = getStockList(fileName, day);

	sl = (SaleList*)malloc(sizeof(SaleList));
	initSaleList();

	//printf("%d", power);
	while (power != 1) {
		system("clear");
		printf("1. power on  ");
		power = PowerButtonInterface();
	}
	homeView();
}
int PowerButtonInterface() {
	int power2;
	scanf("%d", &power2);
	if (power2 != 1) {
		return 0;
	}
	return 1;
}
int TouchScreenInterface() {
	int num;
	scanf("%d", &num);
	if (num < 1 || num > 9) {
		return -1;
	}
	return num;
}
void PrintTime(){
	int a;
	printf("%d-%d-%d-%d-%d\n",2017,11,day,hour,min);	
	printf("Input anything\n");
	scanf("%d",&a);
	
}
void homeView() {

	//checkStockList(stockList, "001");
	int n = 0;
	int ret;
	do {
		system("clear");
		saleListView();
		custView();
		Ismain = 1;
		
		if (daychange == 1) {
			daychange = 0;
			puts("Next Day, refresh stock!\n");
			refreshStockList(day-1);
		
		}
		
		printf("1.InBarcode 2:NumOfGoods 3:RemoveList 4:Sell 5:Refund 6:Stock 7:Time 8:Power Off \n");
		//printf("%d %d\n",hour, min);
		n = TouchScreenInterface();
		Ismain = 0;
		switch (n) {
		case 1: ret = barcodeInputView(); break;
		case 2: ret = numOfGoodsView(); break;
		case 3:  rmOfList(); break;
		case 4: ret = inMoneyView(); break;
		case 5: ret = returnView(); break;
		case 6: ret = stockView(); break;
		case 7: PrintTime(); break;
		case 8: break;
		default: break;
		}
	} while (n != 8);
	powerView(0);


}

void saleListView() {
	int index = 1;
	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	printf("\n");
	printf("  ------------------------------------------------------------------------------\n");
	printf("      Casher View \n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" number\t  |name\t\t  |price\t  |quantity\t  |totalprice\t  |\n");
	while (temp != pl->tailer) {
		//temp->temp->price*temp->quantity
		printf(" %-5d", index);
		printf("\t   %-10s", temp->name);
		printf("\t   %-10d", temp->price);
		printf("\t   %-10d", temp->quantity);
		printf("\t   %-10d\n", temp->price*temp->quantity);
		temp = temp->next;
		index++;
	}
	printf("%s\n",scoment);
	strcpy(scoment,"");
	printf(" -------------------------------------------------------------------------------\n\n");
	pl->count = index - 1;
}
void custView() {
	printf("\n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" Customer View \n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" Total Buy : %d\t    \n", sl->total);
	printf(" Cash : %d\t    \n", sl->inMoney);
	printf(" Change : %d\t    \n", sl->change);
	printf("%s\n",ccoment);
	strcpy(ccoment,"");
	printf("  ------------------------------------------------------------------------------\n");
}

void setChange() {
	sl->change = sl->inMoney - sl->total;
}
void ScannerSensorInterface(char *b) {
	scanf("%s", b);
	b[strlen(b)] = '\0';

}
int barcodeInputView() {
	char barcode[10];
	int curprice = 0;
	char name[10];
	int t2;

	printf("insert barcode : \n");

	ScannerSensorInterface(barcode);

	if (checkStockList(barcode, &curprice, name) == true) {
		if (isOnSaleList( barcode) == false) {

			addSaleList(curprice, name, barcode);

		}
		else {
			t2 = addQuantity(curprice, barcode);
		}
		return 1;
	}
	else {
		strcpy(scoment, "There is no item of that barcode");
		strcpy(ccoment, "There is no item of that barcode");
		return -1;
	}
}

int checkStockList(char *barcode, int* curp, char* curn) {
	Stock *temp = st->header->next;
	while (temp != st->tailer) {
		if (strncmp(temp->barcode, barcode, 3) == 0) {
			*curp = temp->price;
			strcpy(curn, temp->name);
			return true;
		}
		temp = temp->next;
	}
	return false;
}

int isOnSaleList(char *barcode) {
	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	while (temp != pl->tailer) {
		if (strncmp(temp->barcode, barcode, 3) == 0) {
		
			sl->total = sl->total + temp->price;
			return true;
		}
		temp = temp->next;
	}
	//saleList???†ë‹¤.
	return false;
}
int addQuantity(int curp, char* barcode) {


	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	while (temp != pl->tailer) {
		if (strcmp(temp->barcode, barcode) == 0) {
			temp->quantity++;
			sl->total = sl->total + curp;
			break;
		}
		temp = temp->next;
	}

	while (temp != pl->tailer) {
		if (strcmp(temp->barcode, barcode) == 0) {
			return temp->quantity;
		}
		temp = temp->next;
	}
	return -1;
}
void addSaleList(int curp, char* curn, char* barcode) {
	char* newname = (char*)malloc(sizeof(char) * 10);
	char* newbar = (char*)malloc(sizeof(char) * 3);
	int* newprice = (int*)malloc(sizeof(int));
	strcpy(newname, curn);
	strcpy(newbar, barcode);
	newprice[0] = curp;
	ProductList *pl = sl->productList;
	Product *newProduct;

	newProduct = makeProduct(newbar, 1, newprice[0], newname);
	sl->total = sl->total + newprice[0];

	addProduct(pl, newProduct);
}
/*
Product* findProductOnList(Product* p, int n){
int i;
for (i = 0; i < n; i++) {
p = p->next;
}
return p;
}*/
int NOGControl(int n, int q) {
	int back;
	int stockq;
	int i;

	Product *p = sl->productList->header;
	for (i = 0; i < n; i++) {
		p = p->next;
	}


	Stock *s = st->header->next;
	while (s != st->tailer) {
		if (!(strcmp(p->barcode, s->barcode))) {
			stockq = s->quantity;
			break;
		}
		s = s->next;
	}

	if ((p->quantity + q) > stockq) {
		
		char ts[100];
		
		strcat(ts,"Not enough remains : ");
		strcat(ts,p->name);
		strcpy(ccoment,ts);
		
		return -1;
	}
	else if (p->quantity + q <= 0) {
		sl->total = sl->total - (p->price * p->quantity);
		p = p->next;
		rmProduct(p->prev);
		return 0;
	}
	else {
		p->quantity = p->quantity + q;
		sl->total = sl->total + (q * p->price);
		return p->quantity;
	}
}
int numOfGoodsView() {
	int n = 0;
	int q = -100;
	int back = 0;
	int i;
	printf("Input line number on Sale List, '0' is exit\n");
	while (n<1 || n>sl->productList->count) {

		printf("No: ");
		scanf("%d", &n);
		getchar();
		
		if(n==0) {
			return 0;
		}
	}


	

	printf("how many?\n");
	scanf("%d", &q);

	return NOGControl(n,q);
	



	//homeView(saleList, stockList);
}

void rmOfList() {
	int n = 0;
	int i;
	
	printf("Input line number you want to remove.\n");
	scanf("%d", &n);
	
	if(n<1 || n>sl->productList->count){
		strcpy(ccoment, "There is no exist that line");
		strcpy(scoment, "There is no exist that line");
	}else{
		Product *p = sl->productList->header;
		for (i = 0; i < n; i++) {
			p = p->next;
		}
		sl->total = sl->total - (p->price * p->quantity);
		p = p->next;
		rmProduct(p->prev);
	}
	

	
}

int inMoneyView() {
	int im = 0;
	int t = 0;
	char ch;
	
	
	t = sl->total;
	
	printf("give me money more than %d won\n",t);
	scanf("%d", &im);
	getchar();
	if (im >= t) {
		if (sl->productList->header->next == sl->productList->tailer) {
		initSaleList();
			return 1;
		}
		sl->inMoney = im;
		sl->change = im - t;
		calcView();
		return 1;
	}
	else {
		printf("less than total. .... \n");
		ch = getchar();
		return -1;
	}
	//homeView(saleList, stockList);
}
void calcControl() {
	ProductList *pList = sl->productList;
	Product *pTemp = pList->header->next;
	Stock *sTemp = st->header->next;

	while (sTemp != st->tailer && pTemp != pList->tailer) {
		if (strcmp(sTemp->barcode, pTemp->barcode) == 0) {
			//printf("[]%d %d",sTemp->quantity,pTemp->quantity);
			sTemp->quantity = sTemp->quantity - pTemp->quantity;
			pTemp = pTemp->next;
			rmProduct(pTemp->prev);
			sTemp = st->header->next;
		}
		sTemp = sTemp->next;
	}
	sTemp = st->header->next;

	syncStockFile(st, day);
	//Àç°í¼­¹ö »óÇ°°³¼ö ¾÷µ¥ÀÌÆ®  -> ÆÇ¸ÅµµÁß ³¯Â¥ ¹Ù²î¾ùÀ»¶§? 
	initSaleList();
}
void calcView() {
	system("clear");
	int n;
	
	saleListView();
	custView();
	createReceipt();
	setSalemanage();
	//¿µ¼öÁõÃâ·Â 
	
	calcControl();






	


	//	printf("%d - %d - %d", day, hour, min);
	//	homeView(saleList, stockList);
}
void setSalemanage() {
	FILE *f;
	ProductList *pList = sl->productList;
	char today[100] = "saleinfo/";
	char *calender = makeCalendar();
	char wr[100];
	char *sday = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);

	strcat(today, calender);
	strcat(today, "_sale_management.txt");

	//makesnum(sday,day);
	//	makesnum(shour,hour);
	//	makesnum(smin,min);


	int t = 0;
	f = fopen(today, "a+");
	if (f == NULL) {
		printf("file not opened!\n");
	}
	strcpy(wr, "");
	strcat(wr, "¿µ¼öÁõ ¹øÈ£ : 2017.11."); strcat(wr, sday); strcat(wr, ".");
	strcat(wr, shour); strcat(wr, "."); strcat(wr, smin); strcat(wr, "\r\n");
	fprintf(f, wr);
	fprintf(f, "ÆÇ¸Å»óÇ°\r\n");
	Product *temp = pList->header->next;
	while (temp != pList->tailer) {
		t += temp->price*temp->quantity;
		fprintf(f, "%s,\t%d,\t%d,\t%d\r\n", temp->name, temp->price, temp->quantity, temp->price*temp->quantity);
		temp = temp->next;
	}


	sl->total = t;
	fprintf(f, "ÃÑ¾×  %d\r\n", sl->total);
	fclose(f);
}

void createReceipt() {
	FILE *f;
	ProductList *pList = sl->productList;
	char today[100] = "sale/sale_";
	char wr[100];
	char *date = makeDate();

	char *sday = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);


	strcat(today, date);
	strcat(today, ".txt");


	//makesnum(sday,day);
	//	makesnum(shour, hour);
	//	makesnum(smin,min);

	int t = 0;
	f = fopen(today, "wt");
	if (f == NULL) {
		printf("file not opened!\n");
	}


	strcpy(wr, "");
	strcat(wr, "¿µ¼öÁõ ¹øÈ£ : 2017.11."); strcat(wr, sday); strcat(wr, ".");
	strcat(wr, shour); strcat(wr, "."); strcat(wr, smin); strcat(wr, ".\r\n");
	fprintf(f, wr);
	strcpy(wr, ""); strcat(wr, "³¯Â¥ : 2017.11."); strcat(wr, sday); strcat(wr, ".\r\n");
	fprintf(f, wr);
	strcpy(wr, "");
	fprintf(f, "ÆÇ¸Å»óÇ°\r\n");
	Product *temp = pList->header->next;
	while (temp != pList->tailer) {
		t += temp->price*temp->quantity;
		fprintf(f, "%s, %d, %d, %d\r\n", temp->name, temp->price, temp->quantity, temp->price*temp->quantity);
		temp = temp->next;
	}


	sl->total = t;
	fprintf(f, "ÃÑ¾×  %d", sl->total);
	fclose(f);
}
void setRefundmanage(char info[][MaxInfo][10], int line) {
	FILE *f;

	char today[100] = "refundinfo/";
	char wr[100];
	makeCalendar2(wr);
	int i;
	strcat(today, wr);
	strcat(today, "_refund_management.txt");

	f = fopen(today, "a+");
	if (f == NULL) {

		printf("file not opened!\n");
	}
	else {
		strcpy(wr, "");
		strcat(wr, "¿µ¼öÁõ ¹øÈ£ : ");
		strcat(wr, "2017");
		strcat(wr, ".");
		strcat(wr, "11");
		strcat(wr, ".");
		strcat(wr, info[0][4]);
		strcat(wr, ".");
		strcat(wr, info[0][5]);
		strcat(wr, ".");
		strcat(wr, info[0][6]);
		strcat(wr, ".\r\n");
		fprintf(f, wr);
		fprintf(f, "È¯ºÒ»óÇ°\r\n");

		for (i = 3; i<line - 1; i++) {
			fprintf(f, "%s, %s, %s, %s", info[i][0], info[i][2], info[i][1], info[i][3]);
		}
		fprintf(f, "È¯ºÒ¾× : %s\r\n", info[line - 1][1]);

		//printf("ffff");
	}
	fclose(f);
}
void createRefund(char info[][MaxInfo][10], int line) {

	FILE *f;

	char today[100] = "refund/refund_";
	
	int i;
	char wr[100];
	strcat(today,"201711");
	strcat(today,makesnum2(day));
	strcat(today,makesnum2(hour));
	strcat(today,makesnum2(min));
	
	strcat(today, ".txt");


	f = fopen(today, "wt");
	if (f == NULL) {
		printf("file not opened!\n");
		printf("....\n");
		scanf("%d", &i);

	}
	else {
		strcpy(wr, "");
		strcat(wr, "¿µ¼öÁõ ¹øÈ£ : ");
		strcat(wr, "2017");
		strcat(wr, ".");
		strcat(wr, "11");
		strcat(wr, ".");
		strcat(wr, info[0][4]);
		strcat(wr, ".");
		strcat(wr, info[0][5]);
		strcat(wr, ".");
		strcat(wr, info[0][6]);
		strcat(wr, ".\r\n");
		fprintf(f, wr);

		strcpy(wr, "");
		strcat(wr, "³¯Â¥ : ");
		strcat(wr, "2017");
		strcat(wr, ".");
		strcat(wr, "11");
		strcat(wr, ".");
		strcat(wr, info[0][4]);
		strcat(wr, ".\r\n");
		fprintf(f, wr);
		strcpy(wr, "");
		fprintf(f, "È¯ºÒ»óÇ°\r\n");

		for (i = 3; i<line - 1; i++) {
			fprintf(f, "%s, %s, %s, %s", info[i][0], info[i][2], info[i][1], info[i][3]);
		}
		fprintf(f, "È¯ºÒ ±Ý¾× : %s", info[line - 1][1]);
	}

	fclose(f);
	setRefundmanage(info, line);
}
int returnView() {
	char filename[30] = "sale/sale_";
	char input[20];
	char info[MaxStocklist][MaxInfo][10];
	char *Y = makeY();
	char *M = makeM();
	int innum;
	int line = 0;
	int i, j;

	printf("Input receipt numbers. (YYYYMMDDhhmm)\n");
	//strcpy(input,"201711270046");
	scanf("%s", input);
	strcat(filename, input);
	strcat(filename, ".txt");

	FILE *fd;
	fd = fopen(filename, "r");
	if (fd == NULL) {
		strcpy(scoment,"You can't refund by this receipt.");
		strcpy(ccoment,"You can't refund by this receipt.");
	
	}
	else {
		char *pStr;
		char strTemp[50];
		char *ptr = NULL;
		char tok[] = " .:,";
		while (!feof(fd)) {
			ptr = NULL;
			pStr = fgets(strTemp, sizeof(strTemp), fd);
			i = 0;
			ptr = strtok(pStr, tok);
			while (ptr != NULL)
			{
				strcpy(info[line][i], ptr);
				i++;
				ptr = strtok(NULL, tok);
			}
			line++;
		}
		fclose(fd);
		
		printf("Do you really want to refund? yes=1 no=-1?\n");
		scanf("%d", &innum);
		StockList *tempList;
		if (innum == 1) {
			char fn[100];
			strcpy(fn, "");
			strcat(fn, "stock/");
			strcat(fn, Y);
			strcat(fn, M);
			strcat(fn, info[0][4]);
			strcat(fn, "_product.txt");
	
	
			char saledate[100] = "";
			strcat(saledate, info[0][2]);
			strcat(saledate, "³â ");
			strcat(saledate, info[0][3]);
			strcat(saledate, "¿ù ");
			strcat(saledate, info[0][4]);
			strcat(saledate, "ÀÏ ");
			strcat(saledate, info[0][5]);
			strcat(saledate, "½Ã ");
			strcat(saledate, info[0][6]);
			strcat(saledate, "ºÐ ");
	
			printf("Sell date : %s\n", saledate);
			printf("Refund money : %s\n", info[line - 1][1]);
	
			for (i = 3; i<line - 1; i++) {
				printf("%s | Quantity : %s / Unit price : %s / Price %s\n", info[i][0], info[i][2], info[i][1], info[i][3]);
			}
			createRefund(info, line);
	
	
			tempList = (StockList*)malloc(sizeof(StockList));
			initStockList(tempList);
	
	
			for (i = 3; i<line - 1; i++) {
				Stock *newStock = makeStock(info[i][0], "", atoi(info[i][2]), 0);
				addStock(tempList, newStock);
			}
			StockList *stockList2 = getStockList(fn, day);
			fixStockFile(tempList, stockList2, fn);
			return 1;
			//	homeView(saleList, stockList);
		}
		else {
			return -1;
			//	homeView(saleList, stockList);
		}
	}
	fclose(fd);
}

int createTotal() {

	FILE *fd;
	FILE *to;
	int i, j;
	int temptotal = 0;

	char info[200][20][10];
	char *sday = makesnum2(day);
	char fn[100];
	ProductList *pl;
	Product *temp;


	strcpy(fn, "");
	strcat(fn, "settle/settle_");
	strcat(fn, makeY());
	strcat(fn, makeM());
	strcat(fn, sday);
	strcat(fn, ".txt");


	to = fopen(fn, "a+");
	if (to == NULL) {
		printf("file not opened! - settle \n");
	}
	else {
		char fn2[100];
		int line = 0;
		int newline = 0;
		char *pStr;
		char strTemp[50];

		fprintf(to, "³¯Â¥ : %s.%s.%s\r\n", makeY(), makeM(), sday);

		fprintf(to, "ÆÇ¸Å»óÇ°\r\n");
		strcpy(fn2, "saleinfo/");
		strcat(fn2, makeY());
		strcat(fn2, makeM());
		strcat(fn2, sday);
		strcat(fn2, "_sale_management.txt");

		fd = fopen(fn2, "r");
		if (fd == NULL) {
			printf("file not opened! - sale_management\n");
		}
		else {


			pl = (ProductList*)malloc(sizeof(ProductList));
			initProductList(pl);

			while (!feof(fd)) {
				char *ptr = NULL;
				char tok[] = " .,:";
				pStr = fgets(strTemp, sizeof(strTemp), fd);

				i = 0;
				ptr = strtok(pStr, tok);
				while (ptr != NULL)
				{
					strcpy(info[line][i], ptr);
					i++;
					ptr = strtok(NULL, tok);
				}


				if (strcmp(info[line][0], "¿µ¼öÁõ") == 0) {
					newline = line;
				}
				else {
					if (line>newline + 1) {
						if (strcmp(info[line][0], "ÃÑ¾×") == 0) {
							temptotal += atoi(info[line][1]);
						}
						else {
							Product *temp = pl->header->next;
							int flag = 0;
							while (temp != pl->tailer) {
								if (strcmp(temp->name, info[line][0]) == 0) {
									temp->quantity += atoi(info[line][2]);
									flag = 1;
									break;
								}
								temp = temp->next;
							}
							if (flag == 0) {
								Product *newProduct;
								newProduct = makeProduct("", atoi(info[line][2]), atoi(info[line][1]), info[line][0]);
								addProduct(pl, newProduct);
							}

						}
					}
				}
				line++;
			}


			temp = pl->header->next;

			while (temp != pl->tailer->prev) {
				fprintf(to, "%s, %d, %d, %d\r\n", temp->name, temp->quantity, temp->price, temp->quantity*temp->price);
				temp = temp->next;
			}

			fprintf(to, "ÆÇ¸Å±Ý¾× : %d\r\n", temptotal);
		}
		////////////////////////
		//È¯ºÒ ÃÑ¾× /////////////
		///////////////////////
		fprintf(to, "È¯ºÒ»óÇ°\r\n");
		strcpy(fn2, "refundinfo/");
		strcat(fn2, makeY());
		strcat(fn2, makeM());
		strcat(fn2, sday);
		strcat(fn2, "_refund_management.txt");

		fd = fopen(fn2, "r");
		if (fd == NULL) {
			printf("file not opened! - refund_management \n");
		}
		else {

			pl = (ProductList*)malloc(sizeof(ProductList));
			initProductList(pl);

			while (!feof(fd)) {
				char *ptr = NULL;
				char tok[] = " .,:";
				pStr = fgets(strTemp, sizeof(strTemp), fd);

				i = 0;
				ptr = strtok(pStr, tok);
				while (ptr != NULL)
				{
					strcpy(info[line][i], ptr);
					i++;
					ptr = strtok(NULL, tok);
				}


				if (strcmp(info[line][0], "¿µ¼öÁõ") == 0) {
					newline = line;
				}
				else {
					if (line>newline + 1) {
						if (strcmp(info[line][0], "È¯ºÒ¾×") == 0) {
							temptotal += atoi(info[line][1]);
						}
						else {
							Product *temp = pl->header->next;
							int flag = 0;
							while (temp != pl->tailer) {
								if (strcmp(temp->name, info[line][0]) == 0) {
									temp->quantity += atoi(info[line][1]);
									flag = 1;
									break;
								}
								temp = temp->next;
							}
							if (flag == 0) {
								Product *newProduct;
								newProduct = makeProduct("", atoi(info[line][1]), atoi(info[line][2]), info[line][0]);
								addProduct(pl, newProduct);
							}

						}
					}
				}
				line++;
			}


			temp = pl->header->next;

			while (temp != pl->tailer) {
				fprintf(to, "%s, %d, %d, %d\r\n", temp->name, temp->quantity, temp->price, temp->quantity*temp->price);
				temp = temp->next;
			}

			fprintf(to, "È¯ºÒ±Ý¾× : %d\r\n", temptotal);

		}
		fclose(fd);
	}
	fclose(to);

	return 1;
}

int stockView() {
	FILE *fp;
	int n;
	Stock *temp = st->header->next;
	char *calender = makeCalendar();
	char fn[100];
	char *sday = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);

	//makesnum(sday,day);
	//	makesnum(shour, hour);
	//	makesnum(smin,min);

	printf("Check stock data\n");
	while (temp != st->tailer) {
		printf("%s, %d, %d\n", temp->name, temp->price, temp->quantity);
		temp = temp->next;
	}

	temp = st->header->next;
	strcpy(fn, "std/stock_");
	strcat(fn, calender);
	strcat(fn, ".txt");
	fp = fopen(fn, "wt");
	if (fn == NULL) {
		printf("file not opened!\n");
		
	}
	else {
		fprintf(fp, "³¯Â¥ : %s.%s.%s.%s.%s\r\n", makeY(), makeM(), sday, shour, smin);
		//	printf("?í’ˆëª?tê°€ê²?të°”ì½”??t?˜ëŸ‰\n");
		while (temp != st->tailer) {
			fprintf(fp, "%s, %d, %d\r\n", temp->name, temp->price, temp->quantity);
			temp = temp->next;
		}
	}
	fclose(fp);
	
	printf(".... \n");
	scanf("%d", &n);
	
	return 1;
	//	homeView(saleList, stockList);
}

void initSaleList() {
	ProductList *pl = (ProductList*)malloc(sizeof(ProductList));
	initProductList(pl);
	sl->total = 0;
	sl->inMoney = 0;
	sl->change = 0;
	sl->productList = pl;
}

void initProductList(ProductList *list) {
	list->header = makeProduct(0, 0, 0, 0);
	list->tailer = makeProduct(0, 0, 0, 0);
	list->count = 0;

	list->header->prev = NULL;
	list->header->next = list->tailer;
	list->tailer->prev = list->header;
	list->tailer->next = NULL;
}

void addProduct(ProductList *list, Product *newProduct) {
	Product *temp = list->header->next;

	while (temp != list->tailer) {
		temp = temp->next;
	}
	linkProduct(temp, newProduct);

}

void rmProduct(Product *p) {
	p->prev->next = p->next;
	p->next->prev = p->prev;

	free(p);
}

void linkProduct(Product *temp, Product *newProduct) {
	newProduct->prev = temp->prev;
	newProduct->next = temp;
	temp->prev->next = newProduct;
	temp->prev = newProduct;
}

Product *makeProduct(char *barcode, int quantity, int price, char *name) {
	Product *p = (Product*)malloc(sizeof(Product));
	p->barcode = barcode;
	p->quantity = quantity;
	p->name = name;
	p->price = price;
	//p->next = NULL;
	//p->prev = NULL;
}
void *myClock(void *param) {
	time_t timer;
	struct tm *t;

	timer = time(NULL);
	t = localtime(&timer);
	min = *((int*)param);

	int sec = t->tm_sec;

	for (;;) {
		timer = time(NULL);
		t = localtime(&timer);
		//printf("tm_sec : %d\n", t->tm_sec);
		//printf("day %d - hour %d - min %d - sec %d - tm_sec %d\n", day, hour, min, sec, t->tm_sec);
		//printf("sec : %d\n", sec);
		if (sec + 1 == t->tm_sec) {
			char clock[12];
			strcpy(clock,"201711");
			strcat(clock,makesnum2(day));
			strcat(clock,makesnum2(hour));
			strcat(clock,makesnum2(min));
			
			setMyClockFile(clock);
			min += 6;
			//printf("day %d - hour %d - min %d - sec %d - tm_sec %d\n", day, hour, min, sec, t->tm_sec);
			sec = t->tm_sec;
			if (sec == 59) {
				
				sec = -1;
			}
		}
		if (min >= 60) {
			min = 0;
			hour++;

		}
		if (hour >= 24) {
			hour = 0;
			createTotal();
			if(Ismain == 1){
				puts("Next Day, refresh stock!\n");
				
				refreshStockList(day);
			}else{
				daychange= 1;
			}
			day++;
		}
		//printf("%d : %d : %d : %d", day, hour, min, sec);
	}
}
char *makeY() {
	//char yyyy[4];
	char *calendar = malloc(sizeof(char) * 4);

	//sprintf(yyyy,"%d",year); 
	strcpy(calendar, "2017");
	//calendar[strlen(calendar)] = '\0';

	return calendar;
}
char *makeM() {
	char *mm = makesnum2(mon);

	char *calendar = malloc(sizeof(char) * 2);

	strcpy(calendar, mm);
	//	makesnum(month,mon);

	//	strcat(calendar,month);

	//	calendar[strlen(calendar)] = '\0';

	return calendar;
}
char* makesnum2(int num) {
	char *tt2 = malloc(sizeof(char) * 2);
	char temp[4];
	char tt[4];
	strcpy(temp, "0");

	sprintf(tt, "%d", num);
	if (num < 10) {
		strcat(temp, tt);
	}
	else {
		strcpy(temp, tt);
	}

	strcpy(tt2, temp);
	return tt2;
}

void makeCalendar2(char *calendar) {
	char yyyy[4];
	char *month;
	char *dd;
	//char *calendar = malloc(sizeof(char) * 20);

	//strcpy(calendar, "");

	//sprintf(yyyy,"%d",year); 

	month = makesnum2(mon);

	dd = makesnum2(day);
	//printf("[%s]",month);
	strcpy(calendar, "2017");

	strcat(calendar, month);
	strcat(calendar, dd);
	//calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	//return calendar;
}
char* makeCalendar() {
	char yyyy[4];
	char *month = makesnum2(mon);
	char *dd = makesnum2(day);
	char *calendar = malloc(sizeof(char) * 8);


	strcpy(calendar, "2017");
	strcat(calendar, month);
	strcat(calendar, dd);
	//calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	return calendar;
}
char *makeDate() {
	char yyyy[4];
	char *month = makesnum2(mon);
	char *dd = makesnum2(day);
	char *hh = makesnum2(hour);
	char *mm = makesnum2(min);
	char *calendar = malloc(sizeof(char) * 12);

	strcpy(calendar, "");
	//sprintf(yyyy,"%d",year); 


	strcpy(calendar, "2017");
	strcat(calendar, month);
	strcat(calendar, dd);
	strcat(calendar, hh);
	strcat(calendar, mm);
	//	calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	return calendar;
}
