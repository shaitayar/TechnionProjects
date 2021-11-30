#include "order.h"
#include "product.h"
#include "set.h"
#include <stdbool.h>
#include <stdlib.h>

struct order_t {
    unsigned int id;
    bool shipped;
    Set products;
};

Order createOrder(unsigned int id)
{
    Order new_order=malloc(sizeof(*new_order));
    if(!new_order)
    {
        return NULL;
    }

    new_order->id=id;
    new_order->shipped=false;

    new_order->products=setCreate(productCopy,productFree,productCompareByID);
    if(!new_order->products)
    {
        free(new_order);
        return NULL;
    }
    return new_order;
}

Order copyOrder(Order source)
{
    if(!source)
    {
        return NULL;
    }

    Order copy=malloc(sizeof(*copy));
    if(!copy)
    {
        return NULL;
    }

    copy->id=source->id;
    copy->shipped=source->shipped;

    copy->products=setCopy(source->products);
    if(!copy->products)
    {
        free(copy);
        return NULL;
    }
    return copy;
}

void freeOrder(Order to_delete)
{
    setDestroy(to_delete->products);
    free(to_delete);
}

int compareOrders(Order first, Order second)
{
    if(!first || !second)
    {
        return -1;
    }
    return (first->id)-(second->id);
}

OrderResult changeProductAmountInOrder(Order order, const unsigned int productId, const double amount)
{
    if(!order)
    {
        return ORDER_NULL_ARGUMENT;
    }
    
    for(Product iterator=setGetFirst(order->products) ; iterator ; iterator = setGetNext(order->products))
    {
        if(getProductID(iterator)==productId)
        {
            ProductResult result=addProductAmount(iterator,amount);
            if(result==PRODUCT_NULL_ARGUMENT)
            {
                return ORDER_NULL_ARGUMENT;
            }
            else if(result==PRODUCT_INSUFFICIENT_AMOUNT)
            {
                return ORDER_INSUFFICIENT_AMOUNT;
            }
            return ORDER_SUCCESS;
        }
    }

    return ORDER_PRODUCT_NOT_EXIST;
}