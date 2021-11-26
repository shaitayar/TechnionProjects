#include <stdlib.h>
#include "matamikya.h"
#include "set.h"
#include "product.h"

struct Matamikya_t
{
    unsigned int incomes;
    Set products;
    Set orders;
};

Matamikya matamikyaCreate()
{
    Matamikya new_storage=malloc(sizeof(*new_storage));
    if(!new_storage)
    {
        return NULL;
    }

    new_storage->products=setCreate(productCopy, productFree, productCompare);

    
}