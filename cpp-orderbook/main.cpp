#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <cmath>

#include "Order.h"
#include "Orderbook.h"
#include "OrderMatchingEngine.h"
#include "OrderbookVisualizer.h"

std::mutex orderbookMutex; // to ensure thread-safe access since multiple threads need to access orderbook.
std::mutex statusMutex; // to protect the status variable
std::string status{};

Order generateRandomOrder(int id, std::mt19937 &mt)
{
	/*std::uniform_int_distribution<int> idDist(1, 100);*/

	std::uniform_int_distribution<long long> quantityDist(100, 1000);
	std::uniform_real_distribution<double> priceDist(100.0, 450.0);
	std::uniform_int_distribution<int> typeDist(0, 1); // 0 = Buy, 1 = Sell
	
	long long quantity = quantityDist(mt);
	double price = priceDist(mt);
	price = std::round(price * 100.0) / 100.0; // round to 2 decimal places

	OrderType type = (typeDist(mt) == 0 ? OrderType::Buy : OrderType::Sell);
	return Order(id, quantity, price, type);
}

int main()
{
	Orderbook orderbook = Orderbook();
	OrderMatchingEngine orderMatchingEngine = OrderMatchingEngine(orderbook);
	OrderbookVisualizer orderbookVisualiser = OrderbookVisualizer();
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Order Book Depth");

	std::random_device rd; // A non-deterministic random seed (from the hardware)
	std::mt19937 mt(rd());

	// In order to simulate real time orders, we will create a separate thread for generation of orders
	// The main thread will be dealing with rendering the smfl window and rendering.
	std::thread orderGenerator([&orderbook, &mt]()
	{
		std::string lastStatusOrder;
		for (int i = 0; i < 30; i++)
		{

			Order newOrder = generateRandomOrder(i + 1, mt);

			{ // we put this around braces so that we dont lock the mutex for any longer it needs to be
				std::lock_guard<std::mutex> lock(orderbookMutex);
				orderbook.addOrder(newOrder);

			} // mutex unlocks here

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	std::thread orderMatcher([&orderMatchingEngine]()
	{
		std::string lastStatus; // To track the last status message

		while(true)
		{
			{
				// we put this around braces so that we dont lock the mutex for any longer it needs to be
				std::lock_guard<std::mutex> lock(orderbookMutex);
				orderMatchingEngine.matchOrders();
				
			} // mutex unlocks here

			
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
			
	});
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

		{
			std::lock_guard<std::mutex> lock(statusMutex);
			orderbookVisualiser.visualize(window, orderbook, status);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // To adjust refresh rate of visualisation
	}
	
	orderGenerator.join(); // Main thread will wait for orderGeneratorThread to complete before continuing.
	orderMatcher.detach(); // .detach() will ensure that the thread will run independently from the main thread.
}
