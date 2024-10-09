#include <iostream>
#include "OrderMatchingEngine.h"

void OrderMatchingEngine::matchOrders()
{
	std::optional<Order> bestBuy = m_orderbook.getBestBuyOrder();
	std::optional<Order> bestSell = m_orderbook.getBestSellOrder();
	if (!bestBuy || !bestSell)
	{
		std::cout << "No matching orders available." << std::endl;
		return;
	}
	if (bestBuy->getPrice() >= bestSell->getPrice()) // if we found a matching order
	{
		std::cout << "Order matched!" << std::endl;
		if (bestSell->getQuantity() > bestBuy->getQuantity()) 
		{
			// Excess selling quantity.
			std::cout << "Selling quantity is higher than Buying quantity!" << std::endl;
			long long newSellQuantity = bestSell->getQuantity() - bestBuy->getQuantity();
			bestSell->setQuantity(newSellQuantity);
			m_orderbook.removeBestBuyOrder();
		}
		else if (bestBuy->getQuantity() > bestSell->getQuantity())
		{
			// Excess buying quantity.
			std::cout << "Buying quantity is higher than Selling quantity!" << std::endl;
			long long newBuyQuantity = bestBuy->getQuantity() - bestSell->getQuantity();
			bestBuy->setQuantity(newBuyQuantity);
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
		std::cout << "No matching orders found at current prices." << std::endl;
	}
}