#include <iostream>
#include "Order.h"
using namespace std;

int main()
{
	Order order1 = Order(1, 1000, 100.00, true);
	std::cout << order1.getQuantity() << " " << order1.getPrice() << " " << order1.isBuyOrder() << " " << order1.getTimestamp();
}
