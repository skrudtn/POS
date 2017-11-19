#include "tests.h"
#include "FileManager.c"
#include "mainSystem.c"

Stock* findBarcode(StockList* stockList, char* barcode) {
	Stock* temp = stockList->header->next;
	
	while (temp!= stockList->tailer) {
		if (strcmp(temp->barcode, barcode)==0) {
			return temp;
		}
		temp = temp->next;

	}
	return NULL;
}
START_TESTS()

START_TEST("checkStock Test")
	StockList *stockList = getStockList("stock/20170927_product.txt", 27);
	int a = 500;
	char *name;
	ASSERT(checkStockList(stockList, "001", &a, name));
	ASSERT(checkStockList(stockList, "010", &a, name));
	ASSERT(checkStockList(stockList, "010", &a, name));
	ASSERT_NOT_EQUALS(checkStockList(stockList, "011", &a, name),1);
END_TEST()

START_TEST("IsOnSaleList Test")
	SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
	initSaleList(saleList);
	ProductList *pl = saleList->productList;
	Product *newProduct;

	newProduct = makeProduct("001", 1,100, "fruit");
	addProduct(pl, newProduct);
	
	ASSERT_EQUALS(isOnSaleList(saleList, "001"),1);
	ASSERT_NOT_EQUALS(isOnSaleList(saleList, "002"), 1);
END_TEST()

START_TEST("AddQuantiy Test")
	SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
	initSaleList(saleList);
	ProductList *pl = saleList->productList;
	Product *newProduct;
	
	int price = 100;
	
	newProduct = makeProduct("001", 2,price, "fruit");
	addProduct(pl, newProduct);
	newProduct = makeProduct("010", 1, price, "beer");
	addProduct(pl, newProduct);


	ASSERT_EQUALS(addQuantity(saleList, price, "001"), 3);
	ASSERT_EQUALS(addQuantity(saleList, price, "010"), 2);
END_TEST()

START_TEST("AddList Test")
	SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
	initSaleList(saleList);
	ProductList *pl = saleList->productList;
	Product *newProduct;
	
	int price = 100;
	char* name = "fruit";
	char* barcode = "001";
	newProduct = makeProduct(barcode, 2,price, name);
	addProduct(pl, newProduct);
	addSaleList(saleList, price, name, barcode);
	
	
	ASSERT_EQUALS_TEXT(barcode, pl->tailer->prev->barcode);
	ASSERT_NOT_EQUALS_TEXT("", pl->tailer->prev->barcode);
END_TEST()

START_TEST("sale Test")
	SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
	initSaleList(saleList);
	ProductList *pl = saleList->productList;
	Product *newProduct;

	newProduct = makeProduct("001", 1,100, "fruit");
	addProduct(pl, newProduct);
	newProduct = makeProduct("010", 1,150, "beer");
	addProduct(pl, newProduct);
	
	StockList *stockList = getStockList("stock/20170927_product.txt", 27);
	StockList *beforeList = getStockList("stock/20170927_product.txt", 27);
	
	
	calcControl(saleList, stockList);
	
	int expect = findBarcode(beforeList, "001")->quantity;
	int expect2 = findBarcode(beforeList, "010")->quantity;
	
	ASSERT_EQUALS(findBarcode(stockList, "001")->quantity, expect-1);
	ASSERT_EQUALS(findBarcode(stockList, "010")->quantity, expect2-1);
END_TEST()

START_TEST("refund Test")

StockList* refundList = (StockList*)malloc(sizeof(StockList));
initStockList(refundList);


Stock *newStock = makeStock("fruit", "", 3, 0); // fruit 3
addStock(refundList, newStock);
newStock = makeStock("beer", "", 2, 0);
addStock(refundList, newStock);


StockList *stockList = getStockList("stock/20170927_product.txt", 27);
StockList *beforeList = getStockList("stock/20170927_product.txt", 27);



fixStockFile(refundList, stockList, "stock/20170927_product.txt");

int expect = findBarcode(beforeList, "001")->quantity;
int expect2 = findBarcode(beforeList, "010")->quantity;


ASSERT_EQUALS(findBarcode(stockList, "001")->quantity, expect+3); //fruit 3
ASSERT_EQUALS(findBarcode(stockList, "010")->quantity, expect2+2);
END_TEST()

START_TEST("NOG Test")

StockList* stockList = (StockList*)malloc(sizeof(StockList));
initStockList(stockList);


Stock *newStock = makeStock("fruit", "", 20, 0); // fruit 3
addStock(stockList, newStock);


SaleList *saleList = (SaleList*)malloc(sizeof(SaleList));
initSaleList(saleList);
ProductList *pl = saleList->productList;
Product *newProduct;

newProduct = makeProduct("001", 1, 100, "fruit");
addProduct(pl, newProduct);


ASSERT_EQUALS(NOGControl(1,2, saleList,stockList), 3); //fruit current 1    +2 stock 100
ASSERT_NOT_EQUALS(NOGControl(1, 1000, saleList, stockList), 1001);
ASSERT_NOT_EQUALS(NOGControl(1, -500, saleList, stockList), -499);
END_TEST()


END_TESTS()
