#include <optional>
#include <iostream>
#include "Orderbook.h"


void Orderbook::addOrder(const Order& order)
{
	if (order.isBuyOrder())
	{
		buyOrders.push(order);
	}
	else
	{
		sellOrders.push(order);
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
				<< ", Timestamp: " << order.getTimestamp() << '\n';
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
				<< ", Timestamp: " << order.getTimestamp() << '\n';
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