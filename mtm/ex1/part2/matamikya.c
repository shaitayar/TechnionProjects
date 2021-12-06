#include <stdlib.h>
#include "matamikya.h"
#include "amount_set.h"
#include "product.h"
#include <string.h>
#include <math.h>
#include "order.h"
#include "set.h"
#include "matamikya_print.h"

#define AMOUNT_OFFSET 0.001

struct Matamikya_t {
    unsigned int incomes;
    AmountSet products;
    AmountSet orders;

    unsigned int order_counter;
};

Matamikya matamikyaCreate() {
    Matamikya new_storage = malloc(sizeof(*new_storage));
    if (!new_storage) {
        return NULL;
    }

    new_storage->products = asCreate(productCopy, productFree, productCompareByID);
    if(!new_storage->products) {
        matamikyaDestroy(new_storage);
        return NULL;
    }
    new_storage->orders=asCreate(copyOrder,freeOrder,compareOrders);
    if(!new_storage->orders) {
        matamikyaDestroy(new_storage);
        return NULL;
    }
    new_storage->order_counter=0;
    return new_storage;
}

void matamikyaDestroy(Matamikya matamikya) {
    if (!matamikya){
        return;
    }

    asDestroy(matamikya->orders);
    asDestroy(matamikya->products);
    free(matamikya);
}

static bool checkAmount(const double amount, const MatamikyaAmountType amountType) {

    double sub_amount=amount-floor(amount);

    return (amount<0 ||
       (amountType==MATAMIKYA_INTEGER_AMOUNT&& (sub_amount>AMOUNT_OFFSET && sub_amount<(1-AMOUNT_OFFSET))) ||
       (amountType==MATAMIKYA_HALF_INTEGER_AMOUNT && (sub_amount>(0.5+AMOUNT_OFFSET) && sub_amount<(1-AMOUNT_OFFSET))) ||
       (amountType==MATAMIKYA_HALF_INTEGER_AMOUNT && (sub_amount<(0.5-AMOUNT_OFFSET) && sub_amount>AMOUNT_OFFSET)));
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice) {
    if(!matamikya||!name||!customData||!copyData||!freeData||!prodPrice) {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    if(strlen(name)==0|| name[0]<'0'|| (name[0]>'9'&&name[0]<'A') || (name[0]>'Z'&&name[0]<'a') || name[0]>'z')
    {
        return MATAMIKYA_INVALID_NAME;
    }

    if(checkAmount(amount, amountType))
    {
        return MATAMIKYA_INVALID_AMOUNT;
    }

    Product new_product=productCreate(name,id,customData,amountType,amount,0);
    if(!new_product)
    {
        return MATAMIKYA_OUT_OF_MEMORY;
    }

    AmountSetResult result=asRegister(matamikya->products, new_product);
    productFree(new_product);

    if(result==AS_NULL_ARGUMENT || result==AS_OUT_OF_MEMORY)
    {
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    if(result==AS_ITEM_ALREADY_EXISTS)
    {
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    return MATAMIKYA_SUCCESS;
}


MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount) {
    if(!matamikya)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    for(Product iterator=asGetFirst(matamikya->products) ; iterator ; iterator = asGetNext(matamikya->products))
    {
        if(getProductID(iterator)==id)
        {
            ProductResult result=addProductAmount(iterator,amount); //Can be cchanged to a set operation

            if(result==PRODUCT_NULL_ARGUMENT)
            {
                return MATAMIKYA_NULL_ARGUMENT;
            }
            else if(result==PRODUCT_INSUFFICIENT_AMOUNT)
            {
                return MATAMIKYA_INSUFFICIENT_AMOUNT;
            }
            return MATAMIKYA_SUCCESS;
        }
    }

    return MATAMIKYA_PRODUCT_NOT_EXIST;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id) {
    if(!matamikya)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    for(Product iterator=asGetFirst(matamikya->products);iterator;iterator=asGetNext(matamikya->products))
    {
        if(getProductID(iterator)==id){
            asDelete(matamikya->products,iterator);
            return MATAMIKYA_SUCCESS;
        }
    }
    return MATAMIKYA_PRODUCT_NOT_EXIST;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya){
    if(!matamikya){
        return 0;
    }
    Order new_order = createOrder(matamikya->order_counter);
    if (!new_order){
        return 0;
    }
    return matamikya->order_counter++;
}

static Order searchInOrders(Matamikya matamikya, const unsigned int orderId){
    if(!matamikya)
    {
        return NULL;
    }

    for(Order iterator=asGetFirst(matamikya->orders);iterator;iterator=asGetNext(matamikya->orders))
    {
        if(getOrderID(iterator)==orderId){
            return iterator;
        }
    }
    return NULL;
}

static Product searchInProducts(Matamikya matamikya, const unsigned int productId){
    if(!matamikya)
    {
        return NULL;
    }

    for(Product iterator=asGetFirst(matamikya->products);iterator;iterator=asGetNext(matamikya->products))
    {
        if(getProductID(iterator)==productId){
            return iterator;
        }
    }
    return NULL;
}


MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                              const unsigned int productId, const double amount){
    if(!matamikya){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    OrderResult result = changeProductAmountInOrder(searchInOrders(matamikya, orderId), productId, amount);
    if(result==ORDER_NULL_ARGUMENT)
    {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if(result==ORDER_INSUFFICIENT_AMOUNT)
    {
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;

}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId) {
    if(!matamikya)
    {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    Order order=searchInOrders(matamikya,orderId);
    for(Product iterator=getFirstProductInOrder(order);iterator;iterator=getNextProductInOrder(order)) {
        Product warehouse_product=searchInProducts(matamikya,getProductID(iterator));
        double warehouse_product_amount;
        asGetAmount(matamikya->products,warehouse_product,&warehouse_product_amount);
        if(getProductAmount(iterator)>warehouse_product_amount) {
            return MATAMIKYA_INSUFFICIENT_AMOUNT;
        }
    }

    for(Product iterator=getFirstProductInOrder(order);iterator;iterator=getNextProductInOrder(order)) {
        Product warehouse_product=searchInProducts(matamikya,getProductID(iterator));// if NULL
        double amount_to_decrease=getProductAmount(iterator);
        unsigned int incomes=productGetPrice(productGetData(warehouse_product),amount_to_decrease);
        matamikya->incomes+=incomes;
        productAddIncomes(warehouse_product,incomes);
    }

    freeOrder(order);
    return MATAMIKYA_SUCCESS;

}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId) {
    if(!matamikya) {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    AmountSetResult result=asDelete(matamikya->orders,searchInOrders(matamikya,orderId));
    if(result==AS_NULL_ARGUMENT) {
        return MATAMIKYA_NULL_ARGUMENT;
    } else if(result==AS_ITEM_DOES_NOT_EXIST) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    } else {
        return MATAMIKYA_SUCCESS;
    }
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output) {
    if(!matamikya||!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    fprintf(output,"Inventory Status:\n");
    Product iter=asGetFirst(matamikya->products);
    while(iter) {
        mtmPrintProductDetails(productGetName(iter),productGetID(iter),productGetAmount(iter),productGetPricePerUnit(iter),output);
        iter=asGetNext(matamikya->products);
    }

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output) {
    if(!matamikya||!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    Order found=searchInOrders(matamikya,orderId);
    if(!found) {
        return MATAMIKYA_ORDER_NOT_EXIST;
    }

    mtmPrintOrderHeading(orderId,output);
    
    Product iter=asGetFirst(orderGetProducts(found));
    while(iter) {
        mtmPrintProductDetails(productGetName(iter),productGetID(iter),productGetAmount(iter),productGetPricePerUnit(iter),output);
        iter=asGetNext(orderGetProducts(found));
    }

    mtmPrintOrderSummary(calcPrice(found), output);

    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output) {
    if(!matamikya||!output) {
        return MATAMIKYA_NULL_ARGUMENT;
    }

    fprintf(output,"Best Selling Product:\n");
    
    Product iter=asGetFirst(matamikya->products);
    if(iter==NULL) {
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
    double max_incomes=productGetTotalIncomes(iter);
    
    while(iter) {
        if(max_incomes < productGetTotalIncomes(iter)) {
            max_incomes=productGetTotalIncomes(iter);
        }
        iter=asGetNext(matamikya->products);
    }

    iter=asGetFirst(matamikya->products);
    while(iter) {
        if(max_incomes==productGetTotalIncomes(iter)) {
            mtmPrintIncomeLine(productGetName(iter),productGetID(iter),productGetTotalIncomes(iter),output);
        }
    }

    return MATAMIKYA_SUCCESS;
}