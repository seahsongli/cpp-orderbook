#pragma once
#include <vector>
#include <queue>
#include <optional>
#include "Order.h"

struct BuyOrderComparator
{
	bool operator() (const Order& order1, const Order& order2) const
	{
		if (order1.getPrice() == order2.getPrice())
		{
			// return the one with smaller timestamp, i.e. orders that came in first
			return order1.getRawTimestamp() < order2.getRawTimestamp(); 
		}
		return order1.getPrice() > order2.getPrice();
	}
};

struct SellOrderComparator
{
	bool operator() (const Order& order1, const Order& order2) const
	{
		if (order1.getPrice() == order2.getPrice())
		{
			// return the one with smaller timestamp, i.e. orders that came in first
			return order1.getRawTimestamp() < order2.getRawTimestamp();
		}
		return order1.getPrice() < order2.getPrice();
	}
};

class Orderbook
{
private:
	// Priority queue for buy orders (max-heap by price)
	std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> buyOrders;

	// Priority queue for buy orders (min-heap by price)
	std::priority_queue<Order, std::vector<Order>, SellOrderComparator> sellOrders;
public:
	void addOrder(const Order& order);
	std::optional<Order> getBestBuyOrder() const;
	std::optional<Order> getBestSellOrder() const;

	void removeBestBuyOrder();
	void removeBestSellOrder();
	
	void printExistingOrders();

};