#include <time.h>
class Order
{
private:
	int m_id;
	long long m_quantity;
	double m_price;
	bool m_isBuyOrder;
	time_t m_timestamp;

public:
	Order(int id, long long quantity, double price, bool isBuyOrder)
		: m_id{ id }, m_quantity{ quantity }, m_price{ price }, m_isBuyOrder(isBuyOrder), m_timestamp{ time(nullptr) } {}

	~Order() = default;

	// getters
	int getId() { return m_id; }
	long long getQuantity() const { return m_quantity; }
	double getPrice() const { return m_price; }
	bool isBuyOrder() { return m_isBuyOrder; }
	time_t getTimestamp() const { return m_timestamp; }

	// setters
	void setQuantity(long long& quantity) { m_quantity = quantity; }
	void setPrice(double& price) { m_price = price; }
	void setTimestamp() { m_timestamp = time(nullptr); }
};