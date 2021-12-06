#include "product.h"
#include <stdlib.h>
#include <string.h>
#include "amount_set.h"

struct Product_t {
    char* name;
    unsigned int id;
    //price in data
    ProductData data;
    ProductAmountType amount_type;
    double amount;
    double total_incomes;
};

Product productCreate(const char* name, unsigned int id, ProductData data, ProductAmountType amount_type,
                     double amount, unsigned int total_incomes)
{
    if(!name || !data)
    {
        return NULL;
    }

    Product new_product=malloc(sizeof(*new_product));
    if(!new_product)
    {
        return NULL;
    }

    new_product->name=malloc(sizeof(name));
    if(!new_product->name)
    {
        free(new_product);
        return NULL;
    }

    strcpy(new_product->name,name);
    if(!new_product->name)
    {
        free(new_product->name);
        free(new_product);
        return NULL;
    }

    new_product->id=id;
    new_product->data=data;
    new_product->amount_type=amount_type;
    new_product->amount=amount;
    new_product->total_incomes=total_incomes;

    return new_product;
}


ASElement productCopy(ASElement product)
{
    if(!((Product)product))
    {
        return NULL;
    }

    char* new_name=malloc(sizeof(((Product)product)->name));
    if(!new_name)
    {
        return NULL;
    }
    strcpy(new_name,((Product)product)->name);


    ProductData new_data = productCopyData(((Product)product)->data);
    if (!new_data){
        free(new_name);
        return NULL;
    }

    return productCreate(new_name, ((Product)product)->id, new_data, ((Product)product)->amount_type, ((Product)product)->amount, ((Product)product)->total_incomes);

}

void productFree(ASElement product)
{
    if(!((Product)product))
    {
        return;
    }
    //Maybe?
    productFreeData(((Product)product)->data);
    free(((Product)product)->name);
    free(((Product)product));
}

int productCompareByName(Product product1, Product product2)
{
    if(!product1 || !product2)
    {
        return 0;
    }

    return strcmp(product1->name,product2->name);
}

int productCompareByID(ASElement product1, ASElement product2)
{
    if(!((Product)product1) || !((Product)product2))
    {
        return 0;
    }

    return (((Product)product1)->id)-(((Product)product2)->id);
}

unsigned int getProductID(Product product)
{
    if(!product)
    {
        return -1;
    }

    return product->id;
}

double getProductAmount(Product product)
{
    if(!product)
    {
        return -1;
    }

    return product->amount;
}

ProductResult addProductAmount(Product product, const double amount)
{
    if(!product)
    {
        return PRODUCT_NULL_ARGUMENT;
    }

    if(product->amount+amount < 0)
    {
        return PRODUCT_INVALID_AMOUNT;
    }

    product->amount+=amount;
    return PRODUCT_SUCCESS;
}
/********************************************************/
ProductData productCopyData(ProductData data){
    ProductData new_data=malloc(sizeof(*(double*)data));
    if(!new_data)
    {
        return NULL;
    }
    memcpy(new_data,data,sizeof(*(double*)data));
    return new_data;
}

ProductData productGetData(Product product) {
    return product->data;
}

void productAddIncomes(Product product, unsigned int incomes) {
    if(!product) {
        return;
    }
    product->total_incomes+=incomes;
}

void productFreeData(ProductData data){
    if(data==NULL) {
        return;
    }
    free((double*)data);
}

double productGetPrice (ProductData data, double amount){
    /*Add Check if Discount*/
    return basicGetPrice(data,amount);
}

double basicGetPrice(ProductData basePrice, double amount){
        return (*(double*)basePrice) * amount;
}

double productGetAmount(Product product) {
    return product->amount;
}

char* productGetName(Product product) {
    return product->name;
}

unsigned int productGetID(Product product) {
    return product->id;
}

double productGetPricePerUnit(Product product) {
    return *(double*)product->data;
}

double productGetTotalIncomes(Product product) {
    return product->total_incomes;
}