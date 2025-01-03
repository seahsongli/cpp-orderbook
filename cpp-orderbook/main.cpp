#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <cmath>
#include <condition_variable>

#include "Order.h"
#include "Orderbook.h"
#include "OrderMatchingEngine.h"
#include "OrderbookVisualizer.h"

std::mutex orderbookMutex; // to ensure thread-safe access since multiple threads need to access orderbook.
std::mutex statusMutex; // to protect the status variable
std::condition_variable orderAddedCV; // to notify matcher thread that new order has been added
bool newOrderAdded = false;
std::string status{};

Order generateRandomOrder(int id, std::mt19937 &mt)
{
	/*std::uniform_int_distribution<int> idDist(1, 100);*/

	std::uniform_int_distribution<long long> quantityDist(100, 1000);
	std::uniform_real_distribution<double> priceDist(100.0, 450.0);
	std::uniform_int_distribution<int> typeDist(0, 5); 
	std::uniform_int_distribution<int> sideDist(0, 1); // 0 = Buy, 1 = Sell
	long long quantity = quantityDist(mt);
	double price = priceDist(mt);
	price = std::round(price * 100.0) / 100.0; // round to 2 decimal places

	OrderType type;
	switch (typeDist(mt))
	{
	case 0:
		type = OrderType::GoodTillCancel;
		break;
	case 1:
		type = OrderType::FillAndKill;
		break;
	case 2:
		type = OrderType::FillOrKill;
		break;
	case 3:
		type = OrderType::GoodForDay;
		break;
	case 4:
		type = OrderType::Market;
		break;
	case 5:
		type = OrderType::Limit;
		break;
	default:
		throw std::runtime_error("Unexpected random value for OrderType");
	}
	Side side = (sideDist(mt) == 0 ? Side::Buy : Side::Sell);
	return Order(id, quantity, price, type, side);
}

int main()
{
	Orderbook orderbook = Orderbook();
	OrderMatchingEngine orderMatchingEngine = OrderMatchingEngine(orderbook);
	OrderbookVisualizer orderbookVisualiser = OrderbookVisualizer();
	Order newOrder{};
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Order Book Depth");

	std::random_device rd; // A non-deterministic random seed (from the hardware)
	std::mt19937 mt(rd());

	// In order to simulate real time orders, we will create a separate thread for generation of orders
	// The main thread will be dealing with rendering the smfl window and rendering.
	std::thread orderGenerator([&orderbook, &mt, &newOrder]()
	{
		std::string lastStatusOrder;
		for (int i = 0; i < 30; i++)
		{

			Order generatedOrder = generateRandomOrder(i + 1, mt);

			{ // we put this around braces so that we dont lock the mutex for any longer it needs to be
				std::lock_guard<std::mutex> lock(orderbookMutex);
				newOrder = generatedOrder;
				orderbook.addOrder(newOrder);
				newOrderAdded = true;

			} // mutex unlocks here
			orderAddedCV.notify_one(); // this is used to notify the matcher thread
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	std::thread orderMatcher([&orderMatchingEngine, &orderbook, &newOrder]()
	{
		std::string lastStatus; // To track the last status message
		std::time_t currentDay = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm* currentDate = std::localtime(&currentDay);
		while(true)
		{
			{
				// we put this around braces so that we dont lock the mutex for any longer it needs to be
				std::unique_lock<std::mutex> lock(orderbookMutex);// .wait() below requires unique lock
				orderAddedCV.wait(lock, [] {return newOrderAdded; }); 
				orderMatchingEngine.matchOrders(orderbook, newOrder);
				newOrderAdded = false;
			} // mutex unlocks here

			std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm* nowDate = std::localtime(&now);
			if (currentDate->tm_year != nowDate->tm_year ||
				currentDate->tm_mon != nowDate->tm_mon ||
				currentDate->tm_mday != nowDate->tm_mday)
			{
				currentDate = nowDate;

				{
					std::lock_guard<std::mutex> lock(orderbookMutex);
					auto buyOrders = orderbook.getBuyOrdersReference();
					std::vector<Order> validBuyOrders; // temp storage that will be used to build back the orders
					size_t orderCount = buyOrders.size();

					// Check GFD Validity for BUY orders
					for (size_t i = 0; i < orderCount; ++i)
					{
						Order bestBuyOrder = buyOrders.top();
						buyOrders.pop();

						// Check if it's a GoodForDay order and if it is valid
						if (bestBuyOrder.getOrderType() == OrderType::GoodForDay && !orderbook.isValidGFDOrder(bestBuyOrder))
						{
							std::cout << "Removing invalid GFD order: " << bestBuyOrder.getId() << std::endl;
						}
						else
						{
							validBuyOrders.push_back(bestBuyOrder);	// If valid, store it in the validOrders vector
						}
					}

					for (const auto& order : validBuyOrders) // Push all valid orders back into the priority queue
					{
						buyOrders.push(order);
					}

					// Check GFD Validity for SELL orders
					auto sellOrders = orderbook.getSellOrdersReference();
					std::vector<Order> validSellOrders; // temp storage that will be used to build back the orders
					orderCount = sellOrders.size();

					for (size_t i = 0; i < orderCount; ++i)
					{
						Order bestSellOrder = buyOrders.top();
						sellOrders.pop();

						// Check if it's a GoodForDay order and if it is valid
						if (bestSellOrder.getOrderType() == OrderType::GoodForDay && !orderbook.isValidGFDOrder(bestSellOrder))
						{
							std::cout << "Removing invalid GFD order: " << bestSellOrder.getId() << std::endl;
						}
						else
						{
							validBuyOrders.push_back(bestSellOrder); // If valid, store it in the validOrders vector
						}
					}

					for (const auto& order : validSellOrders) // Push all valid orders back into the priority queue
					{
						sellOrders.push(order);
					}
				}
				std::this_thread::sleep_for(std::chrono::seconds(10)); // Check for a new day every 10 seconds
			}
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
