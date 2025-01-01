#include <iostream>
#include "OrderMatchingEngine.h"

bool OrderMatchingEngine::fillOrKillMatched(Order newOrder)
{
	Side side = newOrder.getSide();

	if (side == Side::Buy)
	{
		custom_priority_queue<Order, std::vector<Order>, SellOrderComparator> sellOrdersCopied = m_orderbook.getSellOrdersCopy();
		while (newOrder.getQuantity() > 0)
		{
			if (sellOrdersCopied.empty())
			{
				std::cout << "No Sell Orders available!. Killing order now......." << std::endl;
				return false;
			}

			const Order& bestSell = sellOrdersCopied.top();

			if (newOrder.getPrice() >= bestSell.getPrice()) // if we found a matching order
			{
				std::cout << "Buy Order id " << newOrder.getId() << " matched with Sell Order id " << bestSell.getId() << std::endl;
				if (bestSell.getQuantity() >= newOrder.getQuantity())
				{
					return true;
				}
				else // partial order filled, we need to check the next best sellOrder
				{
					std::cout << "Selling quantity is lower than Buying quantity!" << std::endl;
					long long newBuyQuantity = newOrder.getQuantity() - bestSell.getQuantity();
					sellOrdersCopied.pop();
					newOrder.setQuantity(newBuyQuantity);
				}
			}
			else // not enough matching orders, so we should kill the entire order
			{
				std::cout << "Not enough orders to satisfy Fill Or Kill order." << std::endl;
				return false;
			}
		}
		return true;
	}
	else // sell side
	{
		custom_priority_queue<Order, std::vector<Order>, BuyOrderComparator> buyOrdersCopied = m_orderbook.getBuyOrdersCopy();
		while (newOrder.getQuantity() > 0)
		{
			if (buyOrdersCopied.empty())
			{
				std::cout << "No Buy Orders available!. Killing order now......." << std::endl;
				return false;
			}

			const Order& bestBuy = buyOrdersCopied.top();

			if (newOrder.getPrice() <= bestBuy.getPrice()) // if we found a matching order
			{
				std::cout << "Sell Order id " << newOrder.getId() << " matched with Buy Order id " << bestBuy.getId() << std::endl;
				if (bestBuy.getQuantity() >= newOrder.getQuantity())
				{
					return true;
				}
				else // partial order filled, we need to check the next best buyOrder
				{
					std::cout << "Selling quantity is lower than Buying quantity!" << std::endl;
					long long newSellQuantity = newOrder.getQuantity() - bestBuy.getQuantity();
					buyOrdersCopied.pop();
					newOrder.setQuantity(newSellQuantity);
				}
			}
			else // not enough matching orders, so we should kill the entire order
			{
				std::cout << "Not enough orders to satisfy Fill Or Kill order." << std::endl;
				return false;
			}
		}
		return true;
	}
}

void OrderMatchingEngine::matchOrders(Orderbook &orderbook, Order &newOrder)
{
	OrderType orderType = newOrder.getOrderType();
	Side side = newOrder.getSide();

	if (orderType == OrderType::FillAndKill) // match whatever you can and kill the rest of the order
	{
		if (side == Side::Buy)
		{
			while (newOrder.getQuantity() > 0)
			{
				std::optional<Order> bestSell = m_orderbook.getBestSellOrder();
				if (!bestSell)
				{
					std::cout << "No sell orders available. Killing order now......." << std::endl;
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
						std::cout << "Fill And Kill Order completed!" << std::endl;
						return;
					}
					else if (newOrder.getQuantity() > bestSell->getQuantity())
					{
						// Excess buying quantity.
						std::cout << "Buying quantity is higher than Selling quantity!" << std::endl;
						long long newBuyQuantity = newOrder.getQuantity() - bestSell->getQuantity();
						newOrder.setQuantity(newBuyQuantity);
						m_orderbook.removeBestSellOrder();
					}
					else
					{
						std::cout << "Full order match!" << std::endl;
						// Both quantities match, remove both orders
						m_orderbook.removeBestSellOrder();
						std::cout << "Fill And Kill Order completed!" << std::endl; 
						return;
					}
				}
				else
				{
					std::cout << "Fill And Kill Order not fully matched. Remaining quantity: " << newOrder.getQuantity() << std::endl;
					return;
				}

			}
		}
		else // sell order
		{
			while (newOrder.getQuantity() > 0)
			{
				std::optional<Order> bestBuy = m_orderbook.getBestBuyOrder();
				if (!bestBuy)
				{
					std::cout << "No Buy orders available. Killing order now......." << std::endl;
					return;
				}
				if (newOrder.getPrice() <= bestBuy->getPrice()) // if we found a matching order
				{
					std::cout << "Sell Order id " << newOrder.getId() << " matched with Buy Order id " << bestBuy->getId() << std::endl;
					if (bestBuy->getQuantity() > newOrder.getQuantity())
					{
						// Excess selling quantity.
						std::cout << "Selling quantity is higher than Buying quantity!" << std::endl;
						long long newBuyQuantity = bestBuy->getQuantity() - newOrder.getQuantity();
						m_orderbook.removeBestBuyOrder();
						bestBuy->setQuantity(newBuyQuantity);
						m_orderbook.addOrder(*bestBuy); // bestSell is a std::optional type, but addOrder is taking in Order type as paramter, therefore we deference it.
						std::cout << "Fill And Kill Order completed!";
						return;
					}
					else if (newOrder.getQuantity() > bestBuy->getQuantity())
					{
						// Excess selling quantity.
						std::cout << "Buying quantity is higher than Selling quantity!" << std::endl;
						long long newSellQuantity = newOrder.getQuantity() - bestBuy->getQuantity();
						newOrder.setQuantity(newSellQuantity);
						m_orderbook.removeBestBuyOrder();
					}
					else
					{
						std::cout << "Full order match!" << std::endl;
						// Both quantities match, remove both orders
						m_orderbook.removeBestBuyOrder();
						std::cout << "Fill And Kill Order completed!" << std::endl;
						return;
					}
				}
				else
				{
					std::cout << "Fill And Kill Order not fully matched. Remaining quantity: " << newOrder.getQuantity() << std::endl;
					return;
				}
		}

		}
		
	}

	else if (orderType == OrderType::FillOrKill) // if cannot fully match then kill order
	{
		if (side == Side::Buy)
		{
			if (fillOrKillMatched(newOrder))
			{
				while (newOrder.getQuantity() > 0)
				{
					std::optional<Order> bestSell = m_orderbook.getBestSellOrder();
					if (!bestSell)
					{
						std::cout << "No Sell orders available. Killing order now......." << std::endl;
						return;
					}
					long long newOrderQuantity = newOrder.getQuantity() - bestSell->getQuantity();
					if (newOrderQuantity >= 0)
					{
						m_orderbook.removeBestSellOrder();
					}
					else // current best sell quantity larger than newOrder
					{
						m_orderbook.removeBestSellOrder();
						long long newBestSellQuantity = -newOrderQuantity;
						bestSell->setQuantity(newBestSellQuantity);
						m_orderbook.addOrder(*bestSell);
					}
				}
			}
			else
			{
				std::cout << "Fill or kill order not satisfied! Killing order now......" << std::endl;
			}
		}
		else // sell side
		{
			if (fillOrKillMatched(newOrder))
			{
				while (newOrder.getQuantity() > 0)
				{
					std::optional<Order> bestBuy = m_orderbook.getBestBuyOrder();
					if (!bestBuy)
					{
						std::cout << "No Buy orders available. Killing order now......." << std::endl;
						return;
					}
					long long newOrderQuantity = newOrder.getQuantity() - bestBuy->getQuantity();
					if (newOrderQuantity >= 0)
					{
						m_orderbook.removeBestBuyOrder();
					}
					else // current best sell quantity larger than newOrder
					{
						m_orderbook.removeBestBuyOrder();
						long long newBestBuyQuantity = -newOrderQuantity;
						bestBuy->setQuantity(newBestBuyQuantity);
						m_orderbook.addOrder(*bestBuy);
					}
				}
			}
			else
			{
				std::cout << "Fill or kill order not satisfied! Killing order now......" << std::endl;
			}
		}

	}

	else // If orderType is market order, GTC or GFD
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

					m_orderbook.removeOrder(newOrder.getId(), side); // remove newOrder since it is scarce
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
					m_orderbook.removeOrder(newOrder.getId(), side);
					m_orderbook.removeBestSellOrder();
				}
			}

			else // If it is a sell Order
			{
				std::optional<Order> bestBuy = m_orderbook.getBestBuyOrder();
				if (!bestBuy)
				{
					std::cout << "No matching orders available." << std::endl;
					return;
				}
				if (newOrder.getPrice() <= bestBuy->getPrice()) // if we found a matching order
				{
					std::cout << "Sell Order id " << newOrder.getId() << " matched with Buy Order id " << bestBuy->getId() << std::endl;
					if (bestBuy->getQuantity() > newOrder.getQuantity())
					{
						// Excess selling quantity.
						std::cout << "Selling quantity is higher than Buying quantity!" << std::endl;
						long long newBuyQuantity = bestBuy->getQuantity() - newOrder.getQuantity();
						m_orderbook.removeBestBuyOrder();
						if (newBuyQuantity > 0) {
							bestBuy->setQuantity(newBuyQuantity);
							m_orderbook.addOrder(*bestBuy); // bestSell is a std::optional type, but addOrder is taking in Order type as paramter, therefore we deference it.
						}

						m_orderbook.removeBestBuyOrder(); // remove new Order
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
	}
}