#include <optional>
#include <iostream>
#include "Orderbook.h"


// implement whether to separate all the type of orders into distinct queues or all into 1 other than
// fill and kill and fill or kill
// fill or kill => if order cannot fill entirely, kill it
// fill and kill => buy immediately, remaining order is killed
void Orderbook::addOrder(const Order& order)
{
	switch (order.getOrderType())
	{
		case OrderType::Market:
		case OrderType::GoodTillCancel:
		case OrderType::GoodForDay:
			if (order.getSide() == Side::Buy)
			{
				buyOrders.push(order);
			}
			else
			{
				sellOrders.push(order);
			}
			break;

		// FOK & FAK Does not need to be added to the queue since we are handling them immediately.
		case OrderType::FillAndKill:
		case OrderType::FillOrKill:
			break;

	}
	
}

std::optional<Order> Orderbook::getBestBuyOrder() const
{
	if (!buyOrders.empty())
	{
		return buyOrders.top();
	}
	// Return an empty std::optional
	return std::nullopt;
}

std::optional<Order> Orderbook::getBestSellOrder() const
{
	if (!sellOrders.empty())
	{
		return sellOrders.top();
	}
	// Return an empty std::optional
	return std::nullopt;
}


void Orderbook::removeBestBuyOrder()
{
	if (!buyOrders.empty())
	{
		buyOrders.pop();
		std::cout << "Buy order removed!" << std::endl;
	}
	else
	{
		std::cout << "No buy orders to remove!" << std::endl;
	}
}

void Orderbook::removeBestSellOrder()
{
	if (!sellOrders.empty())
	{
		sellOrders.pop();
		std::cout << "Sell order removed!" << std::endl;
	}
	else
	{
		std::cout << "No sell orders to remove!" << std::endl;
	}
}

void Orderbook::removeOrder(int orderId, Side side)
{
	if (side == Side::Buy)
	{
		if (buyOrders.remove(orderId))
		{
			std::cout << "Buy Order Id: " << orderId << " has been removed!";
		}
		else
		{
			std::cout << "Buy Order Id: " << orderId << " cannot be found!";
		}
	}
	else
	{
		if (sellOrders.remove(orderId))
		{
			std::cout << "Sell Order Id: " << orderId << " has been removed!";
		}
		else
		{
			std::cout << "Sell Order Id: " << orderId << " cannot be found!";
		}
	}
}
void Orderbook::printExistingOrders()
{
	if (buyOrders.empty() && sellOrders.empty())
	{
		std::cout << "No buy and sell orders at the moment" << std::endl;
		std::cout << "\n";
	}

	if (!buyOrders.empty())
	{
		std::cout << "The existing buy orders are as of follows:" << std::endl;
		auto buyOrdersCopy = buyOrders;
		while (!buyOrdersCopy.empty())
		{
			Order order = buyOrdersCopy.top();
			std::cout << "Order ID: " << order.getId()
				<< ", Price: " << order.getPrice()
				<< ", Quantity: " << order.getQuantity()
				<< ", Timestamp: ";
			order.displayTimestamp();
			buyOrdersCopy.pop();  // Remove the top element from the copy
		}
		std::cout << "\n";
	}
	else
	{
		std::cout << "No buy orders at the moment" << std::endl;
		std::cout << "\n";
	}

	if (!sellOrders.empty())
	{
		std::cout << "The existing sell orders are as of follows:" << std::endl;
		auto sellOrdersCopy = sellOrders;
		while (!sellOrdersCopy.empty())
		{
			Order order = sellOrdersCopy.top();
			std::cout << "Order ID: " << order.getId()
				<< ", Price: " << order.getPrice()
				<< ", Quantity: " << order.getQuantity()
				<< ", Timestamp: ";
			order.displayTimestamp();
			sellOrdersCopy.pop();  // Remove the top element from the copy
			
		}
		std::cout << "\n";
	}
	else
	{
		std::cout << "No sell orders at the moment" << std::endl;
		std::cout << "\n";
	}
}

std::map <double, long long> Orderbook::getBuyOrderVolume() const
{
	std::map<double, long long> volumeMap;
	auto buyOrdersCopy = buyOrders;
	while (!buyOrdersCopy.empty())
	{
		const Order& order = buyOrdersCopy.top();
		volumeMap[order.getPrice()] += order.getQuantity();
		buyOrdersCopy.pop();
	}
	return volumeMap;
}

std::map <double, long long> Orderbook::getSellOrderVolume() const
{
	std::map<double, long long> volumeMap;
	auto sellOrdersCopy = sellOrders;
	while (!sellOrdersCopy.empty())
	{
		const Order& order = sellOrdersCopy.top();
		volumeMap[order.getPrice()] += order.getQuantity();
		sellOrdersCopy.pop();
	}
	return volumeMap;
}