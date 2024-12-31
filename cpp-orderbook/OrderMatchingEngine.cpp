#include <iostream>
#include "OrderMatchingEngine.h"

void OrderMatchingEngine::matchOrders()
{
	std::optional<Order> bestBuy = m_orderbook.getBestBuyOrder();
	std::optional<Order> bestSell = m_orderbook.getBestSellOrder();
	if (!bestBuy || !bestSell)
	{
		/*std::cout << "No matching orders available." << std::endl;*/
		return;
	}
	if (bestBuy->getPrice() >= bestSell->getPrice()) // if we found a matching order
	{
		std::cout << "Buy Order id "<< bestBuy->getId() << " matched with Sell Order id " << bestSell->getId() << std::endl;
		if (bestSell->getQuantity() > bestBuy->getQuantity()) 
		{
			// Excess selling quantity.
			std::cout << "Selling quantity is higher than Buying quantity!" << std::endl;
			long long newSellQuantity = bestSell->getQuantity() - bestBuy->getQuantity();
			m_orderbook.removeBestSellOrder();
			if (newSellQuantity > 0) {
				bestSell->setQuantity(newSellQuantity);
				m_orderbook.addOrder(*bestSell); // bestSell is a std::optional type, but addOrder is taking in Order type as paramter, therefore we deference it.
			}
			
			m_orderbook.removeBestBuyOrder();
		}
		else if (bestBuy->getQuantity() > bestSell->getQuantity())
		{
			// Excess buying quantity.
			std::cout << "Buying quantity is higher than Selling quantity!" << std::endl;
			long long newBuyQuantity = bestBuy->getQuantity() - bestSell->getQuantity();
			m_orderbook.removeBestBuyOrder();
			if (newBuyQuantity > 0)
			{
				bestBuy->setQuantity(newBuyQuantity);
				m_orderbook.addOrder(*bestBuy);
			}

			m_orderbook.removeBestSellOrder();
		}
		else 
		{
			std::cout << "Full order match!" << std::endl;
			// Both quantities match, remove both orders
			m_orderbook.removeBestBuyOrder();
			m_orderbook.removeBestSellOrder();
		}
	}
	else
	{
		/*std::cout << "No matching orders found at current prices." << std::endl;*/
	}
}

void OrderMatchingEngine::matchOrders(Orderbook &orderbook, Order &newOrder)
{
	OrderType orderType = newOrder.getOrderType();
	Side side = newOrder.getSide();
	if (orderType == OrderType::GoodTillCancel)
	{

	}

	else if (orderType == OrderType::FillAndKill)
	{

	}

	else if (orderType == OrderType::FillOrKill)
	{

	}

	else if (orderType == OrderType::GoodForDay)
	{

	}

	else // If orderType is market order
	{
		if (side == Side::Buy)
		{
			std::optional<Order> bestSell = m_orderbook.getBestSellOrder();
			if (!bestSell)
			{
				std::cout << "No matching orders available." << std::endl;
				return;
			}

			if (newOrder.getPrice() >= bestSell->getPrice()) // if we found a matching order
			{
				std::cout << "Buy Order id " << newOrder.getId() << " matched with Sell Order id " << bestSell->getId() << std::endl;
				if (bestSell->getQuantity() > newOrder.getQuantity())
				{
					// Excess selling quantity.
					std::cout << "Selling quantity is higher than Buying quantity!" << std::endl;
					long long newSellQuantity = bestSell->getQuantity() - newOrder.getQuantity();
					m_orderbook.removeBestSellOrder();
					if (newSellQuantity > 0) {
						bestSell->setQuantity(newSellQuantity);
						m_orderbook.addOrder(*bestSell); // bestSell is a std::optional type, but addOrder is taking in Order type as paramter, therefore we deference it.
					}

					m_orderbook.removeBestBuyOrder(); // this should be removing the current Order FIX
				}
				else if (newOrder.getQuantity() > bestSell->getQuantity())
				{
					// Excess buying quantity.
					std::cout << "Buying quantity is higher than Selling quantity!" << std::endl;
					long long newBuyQuantity = newOrder.getQuantity() - bestSell->getQuantity();
					m_orderbook.removeBestBuyOrder();
					if (newBuyQuantity > 0)
					{
						newOrder.setQuantity(newBuyQuantity);
						m_orderbook.addOrder(newOrder);
					}

					m_orderbook.removeBestSellOrder();
				}
				else
				{
					std::cout << "Full order match!" << std::endl;
					// Both quantities match, remove both orders
					m_orderbook.removeBestBuyOrder();
					m_orderbook.removeBestSellOrder();
				}
		}
	}
}