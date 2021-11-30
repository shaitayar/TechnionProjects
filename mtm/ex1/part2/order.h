

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

Order copyOrder(Order source);

void freeOrder(Order to_delete);

int compareOrders(Order first, Order second);

OrderResult changeProductAmountInOrder(Order order, const unsigned int productId, const double amount);