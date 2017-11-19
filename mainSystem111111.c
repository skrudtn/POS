#include "mainSystem.h"

int year= 2017;
int mon = 9;
int hour = 0;
int day = 27;
int min;
int daychange=0;
/*
int main(){
	//printf("ff");
	getMyClockFile();
	powerView(0);
	return 0;
}
*/
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
	StockList *stockList = getStockList(fileName,day);
	
	SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
	initSaleList(saleList);
	
	//printf("%d", power);
	while (power != 1) {
		system("clear");
		printf("1. power on  ");
		power = PowerButtonInterface();
	}
	homeView(saleList,stockList);
}
int PowerButtonInterface(){
	int power2;
	scanf("%d", &power2);
	if(power2 != 1){
		return 0;
	}
	return 1;
}
int TouchScreenInterface(){
	int num;
	scanf("%d", &num);
	if(num < 1 || num > 7){
		return -1;
	}
	return num;
}
void homeView(SaleList *saleList, StockList *stockList){
	
	//checkStockList(stockList, "001");
	int n = 0;
	int ret;
	do{
		system("clear");
		if(daychange==1){
			daychange=0;
			createTotal(saleList, stockList);
		}
		saleListView(saleList);
		custView(saleList);
		
		printf("1.InBarcode 2:NumOfGoods 3:Sell 4:Refund 5:Stock 6:Power Off \n");
		//printf("%d %d\n",hour, min);
		n = TouchScreenInterface();
		
		switch(n){
			case 1: ret = barcodeInputView(saleList, stockList); break;
			case 2: ret =  numOfGoodsView(saleList, stockList); break;
			case 3: ret =  inMoneyView(saleList, stockList); break;
			case 4: ret =  returnView(saleList, stockList); break;
			case 5: ret =  stockView(saleList, stockList); break;
			case 6: break;
			case 7: ret =  createTotal(saleList, stockList);break;
			default: break;
		}
		printf("%s",saleList->productList->header->next->name);
	}while(n!=6);
	powerView(0); 
	
	
}

void saleListView(SaleList *sl) {
	int index = 1;
	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	printf("\n");
	printf("  ------------------------------------------------------------------------------\n");
	printf("      Casher View \n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" number\t  |name\t  |price\t  |quantity\t  |totalprice\t  |\n");
	while (temp != pl->tailer) {
		//temp->temp->price*temp->quantity
		printf(" %d", index);
		printf("\t  %s", temp->name);
		printf("\t  %d", temp->price);
		printf("\t\t  %d", temp->quantity);
		printf("\t\t  %d\n", temp->price*temp->quantity);
		temp = temp->next;
		index++;
	}
	printf(" -------------------------------------------------------------------------------\n\n");
	pl->count = index-1;
}
void custView(SaleList *sl) {
	printf("\n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" Customer View \n");
	printf("  ------------------------------------------------------------------------------\n");
	printf(" Total Buy : %d\t    \n", sl->total);
	printf(" Cash : %d\t    \n", sl->inMoney);
	printf(" Change : %d\t    \n", sl->change);
	printf("  ------------------------------------------------------------------------------\n");
}

void setChange(SaleList *list){
	list->change = list->inMoney - list->total;
}
void ScannerSensorInterface(char *b){
	scanf("%s",b);
	b[strlen(b)] = '\0';
	
}
int barcodeInputView(SaleList *saleList, StockList *stockList){
	char barcode[10];
	int curprice=0;
	char name[10];
	char* t1;
	int t2;
	
	printf("¹ÙÄÚµå¹øÈ£ÀÔ·Â\n");
	
	ScannerSensorInterface(barcode);

	if(checkStockList(stockList, barcode,&curprice,name) == true) {
		if (isOnSaleList(saleList, barcode) == false) {
			t1 = addSaleList(saleList, curprice, name, barcode);
		
		}else{
			t2 = addQuantity(saleList, curprice, barcode);
		}
		return 1;
	}else{
		return -1;
	}
}

int checkStockList(StockList *sl, char *barcode, int* curp, char* curn) {
	Stock *temp = sl->header->next;
	printf("%d!!!11111\n", temp->price);
	while (temp != sl->tailer) {
		if (strncmp(temp->barcode,barcode,3)==0){
			printf("%s\n", temp->barcode);
			printf("%s\n", temp->name);
			printf("%d\n", temp->price);
			 *curp = temp->price;
			printf("%d????????\n", temp->price);
			 //printf("%d", temp->price);

			printf("%s\n", curn);
			printf("%s\n", temp->name);
			printf("%d\n", strlen(temp->name));

			printf("%d\n", strlen(curn));
			//strncpy(curn,temp->name,strlen(temp->name));
			 printf("%s\n", temp->name);
			 printf("%s\n", curn);
			 printf("%d!!!11111\n", temp->price);
			 printf("return checkStockList\n");
			 return true;
		}
		temp = temp->next;
	}
	return false;
}

int isOnSaleList(SaleList *sl, char *barcode){
	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	while(temp != pl->tailer){
		if(strncmp(temp->barcode, barcode,3)==0){
		
			sl->total = sl->total + temp->price;
			return true;
		}
		temp = temp->next;
	}
	//saleList???†ë‹¤.
	return false;
}
int addQuantity(SaleList *sl, int curp, char* barcode){
	ProductList *pl = sl->productList;
	Product *temp = pl->header->next;
	while(temp != pl->tailer){
		if(strcmp(temp->barcode, barcode)==0){
			temp->quantity++;
			sl->total = sl->total + curp;
			break;
		}
		temp = temp->next;
	}
	
	while(temp != pl->tailer){
		if(strcmp(temp->barcode, barcode)==0){
			return temp->quantity;
		}
		temp = temp->next;
	}
	return -1;
}
char* addSaleList(SaleList *saleList, int curp, char* curn, char* barcode){
	char* newname  = (char*)malloc(sizeof(char)*10);
	strcpy(newname, curn);
	ProductList *pl = saleList->productList;
	Product *newProduct;
	
	newProduct = makeProduct(barcode, 1, curp, newname);
	saleList->total = saleList->total + curp;
			
	addProduct(pl, newProduct);
	
	return pl->tailer->prev->barcode;
	//displayPL(pl);
}
/*
Product* findProductOnList(Product* p, int n){
	int i;
	for (i = 0; i < n; i++) {
		p = p->next;
	}
	return p;
}*/
int numOfGoodsView(SaleList *saleList, StockList *stockList) {
	int n=0;
	int q = -100;
	int back = 0;
	int i;
	int stockq;
	while (n<1 || n>saleList->productList->count ){
		 
		printf("No:");
		scanf("%d", &n);
		getchar();
	}
	
	
	Product *p = saleList->productList->header;
	for (i = 0; i < n; i++) {
		p = p->next;
	}
	
	
	Stock *s = stockList->header->next;
	while (s != stockList->tailer) {
		if (!(strcmp(p->barcode, s->barcode))){ 
			stockq = s->quantity;
			break;
		}
		s = s->next; 
	}

	printf("how many?\n");
	scanf("%d", &q);
	
	if ((p->quantity + q) > stockq) {
		printf("%s ê°œìˆ˜ê°€ ë¶€ì¡±í•©?ˆë‹¤.\n", p->name);
		back = 0;
		while (back != 1) {
			printf("1. ?¤ë¡œ ");
			scanf("%d", &back);
		}
		return -1;
	}
	else if (p->quantity + q <= 0) {
		saleList->total = saleList->total - (p->price * p->quantity);
		p = p->next;
		rmProduct(p->prev);
		return 0;
	}
	else {
		p->quantity = p->quantity + q;
		saleList->total = saleList->total + (q * p->price);
		return p->quantity;
	}
	


	//homeView(saleList, stockList);
}

void rmOfList(SaleList *saleList,StockList *stockList){
	int n=0;
	int i;
	while (n<1 || n>saleList->productList->count) {
		printf("?? œ??ë²ˆí˜¸ ?…ë ¥\n");
		scanf("%d", &n);
	}

	Product *p = saleList->productList->header;
	for (i = 0; i < n; i++) {
		p = p->next;
	}
	saleList->total = saleList->total - (p->price * p->quantity);
	p = p->next;
	rmProduct(p->prev);
	homeView(saleList, stockList);
}

int inMoneyView(SaleList *saleList, StockList *stockList) {
	int im=0;
	int t=0;
	t = saleList->total;


	printf("ë°›ì????…ë ¥");
	scanf("%d", &im);
	if(im >= t){
		saleList->inMoney = im;
		saleList->change = im-t;
		calcView(saleList, stockList);
		return 1;
	}else{
		printf("¹ÞÀº µ·ÀÌ Àû½À´Ï´Ù.\n"); 
		return -1;
	}		
	//homeView(saleList, stockList);
}

void calcView(SaleList *saleList, StockList *stockList){
	system("clear");
	int n;
	if (saleList->productList->header->next == saleList->productList->tailer) {
		homeView(saleList, stockList);
	}
	saleListView(saleList);
	custView(saleList);
	createReceipt(saleList);
	setSalemanage(saleList);
	//¿µ¼öÁõÃâ·Â 
	ProductList *pList = saleList->productList;
	Product *pTemp = pList->header->next;
	Stock *sTemp = stockList->header->next;
	
	while(sTemp != stockList->tailer && pTemp != pList->tailer){
		if (strcmp(sTemp->barcode, pTemp->barcode) == 0) {
		//printf("[]%d %d",sTemp->quantity,pTemp->quantity);
			sTemp->quantity = sTemp->quantity - pTemp->quantity;
			pTemp = pTemp->next;
			rmProduct(pTemp->prev);
			sTemp = stockList->header->next;
		}
		sTemp = sTemp->next;
	}
	sTemp = stockList->header->next;
	

	n = 0;
	while (n != 1) {
		printf("1. ?¤ë¡œ ");
		scanf("%d", &n);
	}


	
	
	
	syncStockFile(stockList, day);
	//Àç°í¼­¹ö »óÇ°°³¼ö ¾÷µ¥ÀÌÆ®  -> ÆÇ¸ÅµµÁß ³¯Â¥ ¹Ù²î¾ùÀ»¶§? 
	initSaleList(saleList);
	
	
//	printf("%d - %d - %d", day, hour, min);
//	homeView(saleList, stockList);
}
void setSalemanage(SaleList *saleList){
	FILE *f;
	ProductList *pList = saleList->productList;
	char today[100] = "saleinfo/";
	char *calender = makeCalendar();
	char wr[100];
	char *sday  = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);
	
	strcat(today,calender);
	strcat(today,"_sale_management.txt");
	
	//makesnum(sday,day);
//	makesnum(shour,hour);
//	makesnum(smin,min);
	
	
	int t = 0;
	f = fopen(today, "a+");
	if (f == NULL) {
		printf("file not opened!\n");
	}
	strcpy(wr,""); 
	strcat(wr,"¿µ¼öÁõ ¹øÈ£ : 2017.09.");strcat(wr,sday);strcat(wr,".");
	strcat(wr,shour);strcat(wr,".");strcat(wr,smin);strcat(wr,"\r\n");
	fprintf(f, wr);
	fprintf(f, "ÆÇ¸Å»óÇ°\r\n");
	Product *temp = pList->header->next;
	while (temp != pList->tailer) {
		t += temp->price*temp->quantity;
		fprintf(f, "%s,\t%d,\t%d,\t%d\r\n", temp->name, temp->price, temp->quantity, temp->price*temp->quantity);
		temp = temp->next;
	}

	
	saleList->total = t;
	fprintf(f, "ÃÑ¾×  %d\r\n", saleList->total);
	fclose(f);
}

void createReceipt(SaleList *saleList) {
	FILE *f;
	ProductList *pList = saleList->productList;
	char today[100] = "sale/sale_";
	char wr[100];
	char *date = makeDate();
	
	char *sday  = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);
	

	strcat(today,date);
	strcat(today,".txt");
	
	
	//makesnum(sday,day);
//	makesnum(shour, hour);
//	makesnum(smin,min);
	
	int t = 0;
	f = fopen(today, "wt");
	if (f == NULL) {
		printf("file not opened!\n");
	}
	
	
	strcpy(wr,""); 
	strcat(wr,"¿µ¼öÁõ ¹øÈ£ : 2017.09.");strcat(wr,sday);strcat(wr,".");
	strcat(wr,shour);strcat(wr,".");strcat(wr,smin);strcat(wr,".\r\n");
	fprintf(f, wr);
	strcpy(wr,""); strcat(wr,"³¯Â¥ : 2017.09.");strcat(wr,sday);strcat(wr,".\r\n");
	fprintf(f, wr);
	strcpy(wr,"");
	fprintf(f, "ÆÇ¸Å»óÇ°\r\n");
	Product *temp = pList->header->next;
	while (temp != pList->tailer) {
		t += temp->price*temp->quantity;
		fprintf(f, "%s, %d, %d, %d\r\n", temp->name, temp->price, temp->quantity, temp->price*temp->quantity);
		temp = temp->next;
	}

	
	saleList->total = t;
	fprintf(f, "ÃÑ¾×  %d", saleList->total);
	fclose(f);
}
void setRefundmanage(char info[][MaxInfo][10],int line){
	FILE *f;
		
	char today[100] ="refundinfo/";
	char wr[100];
	char *Y = makeY();
	char *M = makeM();
	int i;
	strcat(today,Y);
	strcat(today,M);
	strcat(today,info[0][4]);
	strcat(today,"_refund_management.txt");
	
	f = fopen(today, "a+");
	if (f == NULL) {
		
		printf("file not opened!\n");
	}else{
		strcpy(wr,""); 
		strcat(wr,"¿µ¼öÁõ ¹øÈ£ : ");
		strcat(wr,Y);
		strcat(wr,".");
		strcat(wr,M);
		strcat(wr,".");
		strcat(wr,info[0][4]);
		strcat(wr,".");
		strcat(wr,info[0][5]);
		strcat(wr,".");
		strcat(wr,info[0][6]);
		strcat(wr,".\r\n");
		fprintf(f, wr);
		fprintf(f, "È¯ºÒ»óÇ°\r\n");
		
		for(i=3;i<line-1;i++){ 
			fprintf(f, "%s, %s, %s, %s", info[i][0],info[i][2],info[i][1],info[i][3]);
		}
		fprintf(f, "È¯ºÒ¾× : %s\r\n", info[line-1][1]);
		
		//printf("ffff");
	}	
	fclose(f);
}
void createRefund(char info[][MaxInfo][10],int line){

	FILE *f;
		
	char today[100] ="refund/refund_";
	char wr[100];
	char *Y = makeY();
	char *M = makeM();
	int i;
	
	strcat(today,Y);
	strcat(today,M);
	strcat(today,info[0][4]);
	strcat(today,info[0][5]);
	strcat(today,info[0][6]);
	strcat(today,".txt");
	
	
	f = fopen(today, "wt");
	if (f == NULL) {
		printf("file not opened!\n");
	}else{
		strcpy(wr,""); 
		strcat(wr,"¿µ¼öÁõ ¹øÈ£ : ");
		strcat(wr,Y);
		strcat(wr,".");
		strcat(wr,M);
		strcat(wr,".");
		strcat(wr,info[0][4]);
		strcat(wr,".");
		strcat(wr,info[0][5]);
		strcat(wr,".");
		strcat(wr,info[0][6]);
		strcat(wr,".\r\n");
		fprintf(f, wr);
		
		strcpy(wr,"");
		strcat(wr,"³¯Â¥ : ");
		strcat(wr,Y);
		strcat(wr,".");
		strcat(wr,M);
		strcat(wr,".");
		strcat(wr,info[0][4]);
		strcat(wr,".\r\n");
		fprintf(f, wr);
		strcpy(wr,"");
		fprintf(f, "È¯ºÒ»óÇ°\r\n");
		
		for(i=3;i<line-1;i++){ 
			fprintf(f, "%s, %s, %s, %s", info[i][0],info[i][2],info[i][1],info[i][3]);
		}
		fprintf(f, "È¯ºÒ ±Ý¾× : %s", info[line-1][1]);
	}
	
	fclose(f);
	setRefundmanage(info,line);
}
int returnView(SaleList *saleList, StockList *stockList){
	char filename[30] = "sale/sale_";
	char input[20];
	char info[MaxStocklist][MaxInfo][10];
	char *Y = makeY();
	char *M = makeM();
	int innum;
	int line=0;
	int i,j;
	
	printf("¿µ¼öÁõ ¹øÈ£¸¦ ÀÔ·ÂÇÏ¼¼¿ä. (YYYYMMDDhhmm)\n");
	//strcpy(input,"201709270046");
	scanf("%s",input);
	strcat(filename,input);
	strcat(filename,".txt");
	
	FILE *fd;
	fd = fopen(filename, "r");
	if (fd== NULL) {
		printf("file not opened!\n");
	}else{
		char *pStr;
		char strTemp[50];
		char *ptr =NULL;
		char tok[]=" .:,";
		while(!feof(fd)){
			ptr=NULL;
			pStr = fgets( strTemp, sizeof(strTemp), fd );
			i=0;
			ptr=strtok(pStr, tok);
			while (ptr != NULL)            
			{
			   	strcpy(info[line][i], ptr);     
			    i++;             
			    ptr = strtok(NULL, tok);
			}
			line++;
		}
		fclose(fd);
	}
	printf("Á¤¸» È¯ºÒÇÏ½Ã°Ú½À´Ï±î yes=1 no=-1?\n");
	scanf("%d",&innum);
	StockList *tempList;
	if(innum==1){
		char fn[100];
		strcpy(fn,"");
		strcat(fn,"stock/");
		strcat(fn,Y);
		strcat(fn,M);
		strcat(fn,info[0][4]);
		strcat(fn,"_product.txt");
	
	
		char saledate[100]="";
		strcat(saledate,info[0][2]);
		strcat(saledate,"³â "); 
		strcat(saledate,info[0][3]);
		strcat(saledate,"¿ù ");
		strcat(saledate,info[0][4]);
		strcat(saledate,"ÀÏ "); 
		strcat(saledate,info[0][5]);
		strcat(saledate,"½Ã "); 
		strcat(saledate,info[0][6]);
		strcat(saledate,"ºÐ "); 
		
		printf("ÆÇ¸Å ³¯Â¥ : %s\n",saledate); 
		printf("È¯ºÒ ±Ý¾× : %s\n",info[line-1][1]);
		
		for(i=3;i<line-1;i++){
			printf("%s | ¼ö·® : %s / ´Ü°¡ : %s / ÆÇ¸Å ±Ý¾× %s\n",info[i][0],info[i][2],info[i][1],info[i][3]);
		}
		createRefund(info,line);
		
		
		tempList = (StockList*)malloc(sizeof(StockList));
		initStockList(tempList);
		
		
		for(i=3;i<line-1;i++){
			Stock *newStock = makeStock(info[i][0], "", atoi(info[i][2]), 0);
			addStock(tempList, newStock);	
		}
		StockList *stockList2 = getStockList(fn,day);
		fixStockFile(tempList, stockList2,fn);
		return 1;
	//	homeView(saleList, stockList);
	}else{
		return -1;
	//	homeView(saleList, stockList);
	}
}

int createTotal(SaleList *saleList, StockList *stockList){
		
	FILE *fd;
	FILE *to;
	int i,j;
	int temptotal=0;
	
	char info[200][20][10];
	char *sday= makesnum2(day);
	char fn[100];
	ProductList *pl;
	Product *temp;
	
	
	strcpy(fn,"");
	strcat(fn,"settle/settle_");
	strcat(fn,makeY());
	strcat(fn,makeM());
	strcat(fn,sday);
	strcat(fn,".txt");
		
		
	to= fopen(fn,"a+");
	if (to== NULL) {
		printf("file not opened!\n");
	}else{
		char fn2[100];
		int line=0;
		int newline=0;
		char *pStr;
		char strTemp[50];
		
		fprintf(to,"³¯Â¥ : %s.%s.%s\r\n",makeY(),makeM(),sday);
		
		fprintf(to,"ÆÇ¸Å»óÇ°\r\n");
		strcpy(fn2,"saleinfo/");
		strcat(fn2,makeY());
		strcat(fn2,makeM());
		strcat(fn2,sday);
		strcat(fn2,"_sale_management.txt");
		
		fd = fopen(fn2, "r");
		if (fd== NULL) {
			printf("file not opened!\n");
		}else{
			
			
			pl = (ProductList*)malloc(sizeof(ProductList));
			initProductList(pl);
			
			while(!feof(fd)){
				char *ptr =NULL;
				char tok[]=" .,:";
				pStr = fgets( strTemp, sizeof(strTemp), fd );
			
				i=0;
				ptr=strtok(pStr, tok);
				while (ptr != NULL)            
				{
				   	strcpy(info[line][i], ptr);     
				    i++;             
				    ptr = strtok(NULL, tok);
				}
				
				
				if(strcmp(info[line][0],"¿µ¼öÁõ")==0){
					newline = line;
				}else{
					if(line>newline+1){
						if(strcmp(info[line][0],"ÃÑ¾×")==0){
							temptotal += atoi(info[line][1]);
						}else{
							Product *temp = pl->header->next; 
							int flag=0;
							while (temp != pl->tailer) {
								if(strcmp(temp->name,info[line][0])==0){
									temp->quantity += atoi(info[line][2]);
									flag=1;
									break;
								}
								temp = temp->next;
							}
							if(flag==0){
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
				fprintf(to,"%s, %d, %d, %d\r\n",temp->name,temp->quantity,temp->price,temp->quantity*temp->price);
				temp = temp->next;
			}
			
			fprintf(to,"ÆÇ¸Å±Ý¾× : %d\r\n",temptotal);
		}
			////////////////////////
			//È¯ºÒ ÃÑ¾× /////////////
			///////////////////////
			 fprintf(to,"È¯ºÒ»óÇ°\r\n");
			strcpy(fn2,"refundinfo/");
			strcat(fn2,makeY());
			strcat(fn2,makeM());
			strcat(fn2,sday);
			strcat(fn2,"_refund_management.txt");
			
			fd = fopen(fn2, "r");
		if (fd== NULL) {
			printf("file not opened!\n");
		}else{
			
			pl = (ProductList*)malloc(sizeof(ProductList));
			initProductList(pl);
			
			while(!feof(fd)){
				char *ptr =NULL;
				char tok[]=" .,:";
				pStr = fgets( strTemp, sizeof(strTemp), fd );
			
				i=0;
				ptr=strtok(pStr, tok);
				while (ptr != NULL)            
				{
				   	strcpy(info[line][i], ptr);     
				    i++;             
				    ptr = strtok(NULL, tok);
				}
				
				
				if(strcmp(info[line][0],"¿µ¼öÁõ")==0){
					newline = line;
				}else{
					if(line>newline+1){
						if(strcmp(info[line][0],"È¯ºÒ¾×")==0){
							temptotal += atoi(info[line][1]);
						}else{
							Product *temp = pl->header->next; 
							int flag=0;
							while (temp != pl->tailer) {
								if(strcmp(temp->name,info[line][0])==0){
									temp->quantity += atoi(info[line][1]);
									flag=1;
									break;
								}
								temp = temp->next;
							}
							if(flag==0){
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
						
			while (temp != pl->tailer->prev) {
				fprintf(to,"%s, %d, %d, %d\r\n",temp->name,temp->quantity,temp->price,temp->quantity*temp->price);
				temp = temp->next;
			}
			
			fprintf(to,"È¯ºÒ±Ý¾× : %d\r\n",temptotal);
			
		}
		fclose(fd);
	}
	fclose(to);
	
	return 1;
}

int stockView(SaleList *saleList, StockList *stockList){
	FILE *fp;
	int n;
	Stock *temp = stockList->header->next;
	char *calender = makeCalendar();
	char fn[100];
	char *sday  = makesnum2(day);
	char *shour = makesnum2(hour);
	char *smin = makesnum2(min);
	
	//makesnum(sday,day);
//	makesnum(shour, hour);
//	makesnum(smin,min);
	
	printf("Àç°í È®ÀÎ\n");
	while (temp != stockList->tailer) {
		printf("%s, %d, %d\n", temp->name, temp->price, temp->quantity);
		temp = temp->next;
	}
	
	temp = stockList->header->next;
	strcpy(fn,"std/stock_");
	strcat(fn,calender);
	strcat(fn,".txt");
	fp = fopen(fn, "wt");
	if (fn == NULL) {
		printf("file not opened!\n");
	}else{
		fprintf(fp,"³¯Â¥ : %s.%s.%s.%s.%s\r\n",makeY(),makeM(),sday,shour,smin); 
	//	printf("?í’ˆëª?tê°€ê²?të°”ì½”??t?˜ëŸ‰\n");
		while (temp != stockList->tailer) {
			fprintf(fp,"%s, %d, %d\r\n", temp->name, temp->price, temp->quantity);
			temp = temp->next;
		}
	}
	fclose(fp);
	n = 0;
	while (n != 1) {
		printf("1. ?¤ë¡œ ");
		scanf("%d", &n);
	}
	return 1;
//	homeView(saleList, stockList);
}

void initSaleList(SaleList *sl) {
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
void displayPL(ProductList *pl) {
	Product *temp = pl->header->next;

	while (temp != pl->tailer) {
		printf("%s??n", temp->name);

		temp = temp->next;
	}
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
			daychange=1;
			day++;
		}
		//printf("%d : %d : %d : %d", day, hour, min, sec);
	}
}
char *makeY() {
	//char yyyy[4];
	char *calendar = malloc(sizeof(char) * 4);

	//sprintf(yyyy,"%d",year); 
	strcpy(calendar,"2017");
	//calendar[strlen(calendar)] = '\0';

	return calendar;
}
char *makeM() {
	char *mm= makesnum2(mon);

	char *calendar = malloc(sizeof(char) * 2);

	strcpy(calendar, mm);
//	makesnum(month,mon);
	
//	strcat(calendar,month);
	
//	calendar[strlen(calendar)] = '\0';

	return calendar;
}
char* makesnum2(int num){
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
	strcpy(calendar,"2017");
	
	strcat(calendar,month);
	strcat(calendar, dd);
	//calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	//return calendar;
}
char* makeCalendar() {
	char yyyy[4];
	char *month= makesnum2(mon);
	char *dd= makesnum2(day);
	char *calendar = malloc(sizeof(char) * 8);


	strcpy(calendar,"2017");
	strcat(calendar,month);
	strcat(calendar, dd);
	//calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	return calendar;
}
char *makeDate() {
	char yyyy[4];
	char *month= makesnum2(mon);
	char *dd= makesnum2(day);
	char *hh= makesnum2(hour);
	char *mm= makesnum2(min);
	char *calendar = malloc(sizeof(char) * 12);

	strcpy(calendar, "");
	//sprintf(yyyy,"%d",year); 
	
	
	strcpy(calendar,"2017");
	strcat(calendar,month);
	strcat(calendar, dd);
	strcat(calendar, hh);
	strcat(calendar, mm);
//	calendar[strlen(calendar)] = '\0';

	//printf("%s\n", calendar);
	return calendar;
}

