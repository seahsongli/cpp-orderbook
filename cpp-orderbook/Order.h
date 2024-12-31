#pragma once
#include <chrono>
#include <ctime>
#include <iostream>

#include "OrderType.h"
#include "Side.h."

class Order
{
private:
	int m_id;
	long long m_quantity;
	double m_price;
	OrderType m_orderType;
	Side m_side;
	std::chrono::time_point<std::chrono::system_clock> m_timestamp; // milliseconds


public:
	Order(int id, long long quantity, double price, OrderType orderType, Side side)
		: m_id{ id }, m_quantity{ quantity }, m_price{ price }, m_orderType{orderType}, m_side{side},
		m_timestamp{ std::chrono::system_clock::now() } {}

	Order() = default; // default constructor
	~Order() = default;

	// Getters
	int getId() { return m_id; }
	long long getQuantity() const { return m_quantity; }
	double getPrice() const { return m_price; }
	OrderType getOrderType() const { return m_orderType; }
	Side getSide() const { return m_side; }
	std::chrono::time_point<std::chrono::system_clock> getTimestamp() const { return m_timestamp; }

	// Setters
	void setQuantity(long long& quantity) { m_quantity = quantity; }
	void setPrice(double& price) { m_price = price; }

	// Utility Functions
	std::time_t getRawTimestamp() const;
	void displayTimestamp() const;
};