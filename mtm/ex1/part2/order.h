#ifndef MATAMIKYA_ORDER_H
#define MATAMIKYA_ORDER_H

#include "set.h"
#include "product.h"
#include "amount_set.h"

typedef enum OrderResult_t {
    ORDER_SUCCESS = 0,
    ORDER_NULL_ARGUMENT,
    ORDER_OUT_OF_MEMORY,
    ORDER_INVALID_NAME,
    ORDER_INVALID_AMOUNT,
    ORDER_PRODUCT_ALREADY_EXIST,
    ORDER_PRODUCT_NOT_EXIST,
    ORDER_NOT_EXIST,
    ORDER_INSUFFICIENT_AMOUNT,
} OrderResult;

typedef struct order_t *Order;

Order createOrder(unsigned int id);

ASElement copyOrder(ASElement source);

void freeOrder(ASElement to_delete);

int compareOrders(ASElement first, ASElement second);

OrderResult changeProductAmountInOrder(Order order, const unsigned int productId, const double amount);

unsigned int getOrderID(Order order);

Product getFirstProductInOrder(Order order);

Product getNextProductInOrder(Order order);

double calcPrice(Order order);

AmountSet orderGetProducts(Order order);

#endif //MATAMIKYA_ORDER_H