

typedef enum ProductAmountType_t {
    PRODUCT_INTEGER_AMOUNT,
    PRODUCT_HALF_INTEGER_AMOUNT,
    PRODUCT_ANY_AMOUNT,
} ProductAmountType;

typedef struct Product_t *Product;

typedef void *ProductData;

Product productCreate(char* name, unsigned int id, ProductData data, ProductAmountType amount_type,
                        unsigned int amount, unsigned int price, unsigned int total_incomes);

Product productCopy(Product product);

void productFree(Product product);

int productCompare(Product product1, Product product2);