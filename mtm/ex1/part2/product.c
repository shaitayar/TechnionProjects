#include "product.h"
#include <stdlib.h>
#include <string.h>

struct Product_t {
    char* name;
    unsigned int id;
    ProductData data;
    ProductAmountType amount_type;
    double amount;
    unsigned int price;
    unsigned int total_incomes;  
};

Product productCreate(char* name, unsigned int id, ProductData data, ProductAmountType amount_type,
                        double amount, unsigned int price, unsigned int total_incomes)
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
    new_product->price=price;
    new_product->total_incomes=total_incomes;

    return new_product;
}


Product productCopy(Product product)
{
    if(!product)
    {
        return NULL;
    }

    char* new_name=malloc(sizeof(product->name));
    if(!new_name)
    {
        return NULL;
    }
    strcpy(new_name,product->name);
    if(!new_name)
    {
        free(new_name);
        return NULL;
    }

    ProductData new_data=malloc(sizeof(product->data));
    if(!new_data)
    {
        free(new_name);
        return NULL;
    }

    memcpy(new_data,product->data,sizeof(product->data));
    if(!new_data)
    {
        free(new_name);
        free(new_data);
    }

    return productCreate(new_name, product->id, new_data, product->amount_type, product->amount, product->price, product->total_incomes);

}

void productFree(Product product)
{
    if(!product)
    {
        return;
    }
    
    free(product->data);
    free(product->name);
    free(product);
}

int productCompareByName(Product product1, Product product2)
{
    if(!product1 || !product2)
    {
        return -1;
    }
    
    return strcmp(product1->name,product2->name);
}

int productCompareByID(Product product1, Product product2)
{
    if(!product1 || !product2)
    {
        return -1;
    }
    
    return (product1->id)-(product2->id);
}

unsigned int getProductID(Product product)
{
    return product->id;
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