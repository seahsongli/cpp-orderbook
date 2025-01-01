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
				m_buyOrders.push(order);
			}
			else
			{
				m_sellOrders.push(order);
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
	if (!m_buyOrders.empty())
	{
		return m_buyOrders.top();
	}
	// Return an empty std::optional
	return std::nullopt;
}

std::optional<Order> Orderbook::getBestSellOrder() const
{
	if (!m_sellOrders.empty())
	{
		return m_sellOrders.top();
	}
	// Return an empty std::optional
	return std::nullopt;
}


void Orderbook::removeBestBuyOrder()
{
	if (!m_buyOrders.empty())
	{
		m_buyOrders.pop();
		std::cout << "Buy order removed!" << std::endl;
	}
	else
	{
		std::cout << "No buy orders to remove!" << std::endl;
	}
}

void Orderbook::removeBestSellOrder()
{
	if (!m_sellOrders.empty())
	{
		m_sellOrders.pop();
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
		if (m_buyOrders.remove(orderId))
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
		if (m_sellOrders.remove(orderId))
		{
			std::cout << "Sell Order Id: " << orderId << " has been removed!";
		}
		else
		{
			std::cout << "Sell Order Id: " << orderId << " cannot be found!";
		}
	}
}

bool Orderbook::isValidGFDOrder(const Order& order)
{
	auto orderTime = order.getTimestamp();

	auto now = std::chrono::system_clock::now();

	std::time_t orderTimeT = std::chrono::system_clock::to_time_t(orderTime);
	std::time_t nowT = std::chrono::system_clock::to_time_t(now);

	std::tm orderDate = *std::localtime(&orderTimeT);
	std::tm currentDate = *std::localtime(&nowT);

	if (orderDate.tm_year == currentDate.tm_year &&
		orderDate.tm_mon == currentDate.tm_mon &&
		orderDate.tm_mday == currentDate.tm_mday)
	{
		return true; // Order is still valid
	}

	return false; // Order is no longer valid
}

void Orderbook::printExistingOrders()
{
	if (m_buyOrders.empty() && m_sellOrders.empty())
	{
		std::cout << "No buy and sell orders at the moment" << std::endl;
		std::cout << "\n";
	}

	if (!m_buyOrders.empty())
	{
		std::cout << "The existing buy orders are as of follows:" << std::endl;
		auto buyOrdersCopy = m_buyOrders;
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

	if (!m_sellOrders.empty())
	{
		std::cout << "The existing sell orders are as of follows:" << std::endl;
		auto sellOrdersCopy = m_sellOrders;
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
	auto buyOrdersCopy = m_buyOrders;
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
	auto sellOrdersCopy = m_sellOrders;
	while (!sellOrdersCopy.empty())
	{
		const Order& order = sellOrdersCopy.top();
		volumeMap[order.getPrice()] += order.getQuantity();
		sellOrdersCopy.pop();
	}
	return volumeMap;
}