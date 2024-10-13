#include <iostream>
#include "Order.h"
#include "Orderbook.h"
#include "OrderMatchingEngine.h"
#include <thread>
#include <random>
// Create generateRandomOrder function
// Create simple GUI to show how the orderbook looks like
Order generateRandomOrder(int id, std::mt19937 &mt)
{
	
	/*std::uniform_int_distribution<int> idDist(1, 100);*/

	std::uniform_int_distribution<long long> quantityDist(100, 5000);
	std::uniform_real_distribution<double> priceDist(50.0, 250.0);
	std::uniform_int_distribution<int> typeDist(0, 1); // 0 = Buy, 1 = Sell
	
	long long quantity = quantityDist(mt);
	double price = priceDist(mt);
	OrderType type = (typeDist(mt) == 0 ? OrderType::Buy : OrderType::Sell);
	return Order(id, quantity, price, type);

}
int main()
{
	Orderbook orderbook = Orderbook();
	OrderMatchingEngine orderMatchingEngine = OrderMatchingEngine(orderbook);
	std::random_device rd; // A non-deterministic random seed (from the hardware)
	std::mt19937 mt(rd());
	for (int i = 0; i < 10; i++)
	{
		Order newOrder = generateRandomOrder(i + 1, mt);
		orderbook.addOrder(newOrder);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();
}
