#ifndef MATAMIKYA_PRODUCT_H
#define MATAMIKYA_PRODUCT_H



typedef enum ProductAmountType_t {
    PRODUCT_INTEGER_AMOUNT,
    PRODUCT_HALF_INTEGER_AMOUNT,
    PRODUCT_ANY_AMOUNT,
} ProductAmountType;

typedef enum ProductResult_t {
    PRODUCT_SUCCESS = 0,
    PRODUCT_NULL_ARGUMENT,
    PRODUCT_OUT_OF_MEMORY,
    PRODUCT_INVALID_NAME,
    PRODUCT_INVALID_AMOUNT,
    PRODUCT_NOT_EXIST,
    PRODUCT_INSUFFICIENT_AMOUNT,
} ProductResult;

typedef struct Product_t *Product;

typedef void *ProductData;

Product productCreate(char* name, unsigned int id, ProductData data, ProductAmountType amount_type,
                      double amount, unsigned int total_incomes);

Product productCopy(Product product);

void productFree(Product product);

int productCompareByName(Product product1, Product product2);

int productCompareByID(Product product1, Product product2);

unsigned int getProductID(Product product);

double getProductAmount(Product product);

ProductResult addProductAmount(Product product, const double amount);

/*************************************************/

ProductData productCopyData(ProductData data);

ProductData productGetData(Product product);

void productAddIncomes(Product product, unsigned int incomes);

void productFreeData(ProductData data);

double productGetPrice (ProductData data, double Amount);

/*Get Price- Discounts*/
double basicGetPrice(ProductData basePrice, double amount);


#endif //MATAMIKYA_PRODUCT_H