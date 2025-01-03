#include "Orderbook.h"
class OrderMatchingEngine
{
private:
	Orderbook& m_orderbook;

public:
	OrderMatchingEngine(Orderbook& orderbook)
		: m_orderbook(orderbook) {}

	void matchOrders(Orderbook& orderbook, Order& newOrder);
	bool fillOrKillMatched(Order newOrder);
};