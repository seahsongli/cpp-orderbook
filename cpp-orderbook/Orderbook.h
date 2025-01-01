#pragma once
#include <vector>
#include <queue>
#include <optional>
#include <map>
#include "Order.h"

template <typename T, typename Container = std::vector<T>, typename Comparator = std::less<typename Container::value_type>>
class custom_priority_queue : public std::priority_queue<T, Container , Comparator>
{
public:
	using Base = std::priority_queue<T, Container, Comparator>;
	custom_priority_queue(const Comparator& comp = Comparator(), const Container& cont = Container())
		: Base(comp, cont) {}

	bool remove(const int orderId)
	{
		auto it = std::find_if(this->c.begin(), this->c.end(),
			[orderId](const T& order) { return order.getId() == orderId; });
		if (it == this->c.end())
		{
			return false;
		}
		if (it == this->c.begin())
		{
			this->pop();
		}
		else
		{
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
		}
		return true;
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
		return order1.getPrice() > order2.getPrice();
	}
};

struct BuyOrderComparator
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
	custom_priority_queue<Order, std::vector<Order>, BuyOrderComparator> buyOrders;

	// Priority queue for sell orders (min-heap by price)
	custom_priority_queue<Order, std::vector<Order>, SellOrderComparator> sellOrders;

	std::queue<Order> fakOrders;
	std::queue<Order> fokOrders;
	std::queue<Order> gfdOrders; // Need periodic checking (1 day)
	std::queue<Order> gtcOrders;

public:
	void addOrder(const Order& order);
	custom_priority_queue<Order, std::vector<Order>, SellOrderComparator> getSellOrders() { return sellOrders; };
	custom_priority_queue<Order, std::vector<Order>, BuyOrderComparator> getBuyOrders() { return buyOrders; };
	std::optional<Order> getBestBuyOrder() const;
	std::optional<Order> getBestSellOrder() const;

	void removeBestBuyOrder();
	void removeBestSellOrder();
	
	void removeOrder(int orderId, Side side);
	void printExistingOrders();

	std::map <double, long long> getBuyOrderVolume() const;
	std::map <double, long long> getSellOrderVolume() const;

};