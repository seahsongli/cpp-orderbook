#ifndef ORDERBOOKVISUALIZER_H
#define ORDERBOOKVISUALIZER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Orderbook.h"

class OrderbookVisualizer
{
private:
    sf::Font font;
    sf::Text status;

    void drawOrderDepth(sf::RenderWindow& window, const Orderbook& orderbook);

public:
    OrderbookVisualizer();
    void visualize(sf::RenderWindow& window, const Orderbook& orderbook, const std::string& statusText);
};

#endif // ORDERBOOKVISUALIZER_H
