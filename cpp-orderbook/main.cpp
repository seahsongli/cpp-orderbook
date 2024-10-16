#include <iostream>
#include <thread>
#include <random>

#include "Order.h"
#include "Orderbook.h"
#include "OrderMatchingEngine.h"
#include "OrderbookVisualizer.h"


// Create simple GUI to show how the orderbook looks like
// Fix CmakeLists.txt to include sfml libraries.
// test out the visualization, include hot reload / real time updates too.
Order generateRandomOrder(int id, std::mt19937 &mt)
{
	
	/*std::uniform_int_distribution<int> idDist(1, 100);*/

	std::uniform_int_distribution<long long> quantityDist(100, 1000);
	std::uniform_real_distribution<double> priceDist(100.0, 450.0);
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
	OrderbookVisualizer orderbookVisualiser = OrderbookVisualizer();

	std::random_device rd; // A non-deterministic random seed (from the hardware)
	std::mt19937 mt(rd());

	// In order to simulate real time orders, we will create a separate thread for generation of orders
	// The main thread will be dealing with rendering the smfl window and rendering.
	std::thread orderGenerator([&orderbook, &mt]()
	{
		for (int i = 0; i < 20; i++)
		{
			Order newOrder = generateRandomOrder(i + 1, mt);
			orderbook.addOrder(newOrder);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});
	
	sf::RenderWindow window(sf::VideoMode(1200, 1200), "Order Book Depth");
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}
		orderbookVisualiser.visualize(window, orderbook);
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // To adjust refresh rate of visualisation
	}

	
	orderGenerator.join(); // Main thread will wait for orderGeneratorThread to complete before continuing.
	/*orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();

	orderMatchingEngine.matchOrders();
	orderbook.printExistingOrders();*/
	
}
