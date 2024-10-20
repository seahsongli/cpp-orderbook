#include "OrderbookVisualizer.h"
#include <map>
#include <algorithm> // For std::max
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>

OrderbookVisualizer::OrderbookVisualizer() {
    if (!font.loadFromFile("../../../arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    status.setFont(font);
    status.setCharacterSize(15);
    status.setFillColor(sf::Color::White);
    status.setPosition(100, 100);
}

void OrderbookVisualizer::drawOrderDepth(sf::RenderWindow& window, const Orderbook& orderbook) {
    std::ostringstream oss; // to format the string to 2dp
    std::string formatted_price;
    std::string formatted_quantity;

    const auto& buyVolumes = orderbook.getBuyOrderVolume();
    const auto& sellVolumes = orderbook.getSellOrderVolume();

    float windowHeight = window.getSize().y;
    float windowWidth = window.getSize().x;
    float maxBarWidth = windowWidth / 2 - 150; // Leave space for both buy and sell on each side
    float barHeightConstraint = 40.0;

    // Combine buy volumes
    std::map<double, double> combinedBuyVolumes;
    for (const auto& [price, quantity] : buyVolumes) {
        combinedBuyVolumes[price] += quantity;
    }

    // Combine sell volumes
    std::map<double, double> combinedSellVolumes;
    for (const auto& [price, quantity] : sellVolumes) {
        combinedSellVolumes[price] += quantity;
    }

    // Calculate the maximum quantity for normalization
    double maxBuyQuantity = 0.0;
    for (const auto& [price, quantity] : combinedBuyVolumes) {
        if (quantity > maxBuyQuantity) {
            maxBuyQuantity = quantity;
        }
    }

    double maxSellQuantity = 0.0;
    for (const auto& [price, quantity] : combinedSellVolumes) {
        if (quantity > maxSellQuantity) {
            maxSellQuantity = quantity;
        }
    }

    double overallMaxQuantity = std::max(maxBuyQuantity, maxSellQuantity);
    float maxBarHeight = std::min((windowHeight - 100) / std::max(static_cast<int>(combinedBuyVolumes.size()), static_cast<int>(combinedSellVolumes.size())), barHeightConstraint);
    float buyYOffset = 50.0f; // Start from the top of the drawing area

    // Draw Buy Orders (Bids) - on the left
    for (const auto& [price, quantity] : combinedBuyVolumes) {
        float normalizedWidth = (quantity / overallMaxQuantity) * maxBarWidth; // Normalize width based on quantity

        // Create rectangle for the buy order
        sf::RectangleShape buyRect(sf::Vector2f(normalizedWidth, maxBarHeight));
        buyRect.setFillColor(sf::Color(0, 255, 0, 128)); // Semi-transparent green
        buyRect.setPosition(50.0f, buyYOffset); // Position on the left
        window.draw(buyRect);

        // Draw price and quantity text
        sf::Text priceText;
        sf::Text quantityText;

        oss << std::fixed << std::setprecision(2) << price;
        formatted_price = oss.str();

        priceText.setFont(font);
        priceText.setString("$" + formatted_price);
        priceText.setCharacterSize(15);
        priceText.setFillColor(sf::Color::White);
        priceText.setPosition(50.0f + normalizedWidth + 10, buyYOffset + maxBarHeight / 2 - 20); // Position beside the bar

        oss.str("");    // Clear the string stream
        oss.clear();    // Clear any error flags

        oss << std::fixed << std::setprecision(0) << quantity;
        formatted_quantity = oss.str();

        quantityText.setFont(font);
        quantityText.setString(formatted_quantity);
        quantityText.setCharacterSize(15);
        quantityText.setFillColor(sf::Color::White);
        quantityText.setPosition(50.0f + normalizedWidth + 10, buyYOffset + maxBarHeight / 2 + 5); // Position beside the bar

        oss.str("");    // Clear the string stream
        oss.clear();    // Clear any error flags

        window.draw(priceText);
        window.draw(quantityText);

        // Update yOffset for buy orders
        buyYOffset += maxBarHeight + 5; // Move down for the next order
    }

    // Draw Sell Orders (Asks) - on the right
    float sellYOffset = 50.0f; // Start from the top of the drawing area
    for (const auto& [price, quantity] : combinedSellVolumes) {
        float normalizedWidth = (quantity / overallMaxQuantity) * maxBarWidth; // Normalize width based on quantity

        // Create rectangle for the sell order
        sf::RectangleShape sellRect(sf::Vector2f(normalizedWidth, maxBarHeight));
        sellRect.setFillColor(sf::Color(255, 0, 0, 128)); // Semi-transparent red
        sellRect.setPosition(windowWidth - 50.0f - normalizedWidth, sellYOffset); // Position on the right
        window.draw(sellRect);

        // Format price to 2dp
        oss << std::fixed << std::setprecision(2) << price;
        formatted_price = oss.str();

        // Draw price and quantity text
        sf::Text priceText, quantityText;
        priceText.setFont(font);
        priceText.setString("$" + formatted_price);
        priceText.setCharacterSize(15);
        priceText.setFillColor(sf::Color::White);
        priceText.setPosition(windowWidth - 50.0f - normalizedWidth - 10 - 50, sellYOffset + maxBarHeight / 2 - 20); // Position beside the bar

        oss.str("");    // Clear the string stream
        oss.clear();    // Clear any error flags

        // Format quantity to 2dp
        oss << std::fixed << std::setprecision(0) << quantity;
        formatted_quantity = oss.str();

        quantityText.setFont(font);
        quantityText.setString(formatted_quantity);
        quantityText.setCharacterSize(15);
        quantityText.setFillColor(sf::Color::White);
        quantityText.setPosition(windowWidth - 50.0f - normalizedWidth - 10 - 50, sellYOffset + maxBarHeight / 2 + 5); // Position beside the bar

        oss.str("");    // Clear the string stream
        oss.clear();    // Clear any error flags

        window.draw(priceText);
        window.draw(quantityText);

        // Update yOffset for sell orders
        sellYOffset += maxBarHeight + 5; // Move down for the next order
    }
}

void OrderbookVisualizer::visualize(sf::RenderWindow& window, const Orderbook& orderbook, const std::string& statusText) {
    double windowHeight = window.getSize().y;
    float windowWidth = window.getSize().x;
    status.setPosition(windowWidth / 2 - 10, windowHeight * 0.9);
    status.setString(statusText);

    window.clear(sf::Color::Black);

    drawOrderDepth(window, orderbook);

    window.draw(status);
    window.display();
}
