#include <iostream>
#include "Order.h"
#include "Orderbook.h"
#include "OrderMatchingEngine.h"

int main()
{
	Orderbook orderbook = Orderbook();
	OrderMatchingEngine orderMatchingEngine = OrderMatchingEngine(orderbook);
	// id, quantity, price , typeOfOrder
	Order order1 = Order(1, 1000, 100.00, true); // buy order
	Order order2 = Order(2, 1000, 100.00, false); // sell order

	Order order3 = Order(3, 3000, 200.00, true); // buy order
	Order order4 = Order(4, 2000, 100.00, false); // sell order

	orderbook.addOrder(order1);
	orderbook.addOrder(order2);
	orderbook.addOrder(order3);
	orderbook.addOrder(order4);

	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();


}
