#include <SFML/Graphics.hpp>
#include <map>
#include <algorithm> // For std::max
#include "Orderbook.h"

class OrderbookVisualizer
{
private:
    sf::Font font;

    void drawOrderDepth(sf::RenderWindow& window, const Orderbook& orderbook) {
        const auto& buyVolumes = orderbook.getBuyOrderVolume();
        const auto& sellVolumes = orderbook.getSellOrderVolume();

        float windowHeight = window.getSize().y;
        float windowWidth = window.getSize().x;
        float maxBarWidth = windowWidth / 2 - 100; // Leave space for both buy and sell on each side
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

        // Draw Buy Orders (Bids) - on the left
        float buyYOffset = 50.0f; // Start from the top of the drawing area
        for (const auto& [price, quantity] : combinedBuyVolumes) {
            float normalizedWidth = (quantity / overallMaxQuantity) * maxBarWidth; // Normalize width based on quantity

            // Create rectangle for the buy order
            sf::RectangleShape buyRect(sf::Vector2f(normalizedWidth, maxBarHeight));
            buyRect.setFillColor(sf::Color(0, 255, 0, 128)); // Semi-transparent green
            buyRect.setPosition(50.0f, buyYOffset); // Position on the left
            window.draw(buyRect);

            // Draw price and quantity text
            sf::Text priceText, quantityText;
            priceText.setFont(font);
            priceText.setString(std::to_string(price));
            priceText.setCharacterSize(15);
            priceText.setFillColor(sf::Color::Black);
            priceText.setPosition(50.0f + normalizedWidth + 10, buyYOffset + maxBarHeight / 2 - 10); // Position beside the bar

            quantityText.setFont(font);
            quantityText.setString(std::to_string(quantity));
            quantityText.setCharacterSize(15);
            quantityText.setFillColor(sf::Color::Black);
            quantityText.setPosition(50.0f + normalizedWidth + 10, buyYOffset + maxBarHeight / 2 + 10); // Position beside the bar

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

            // Draw price and quantity text
            sf::Text priceText, quantityText;
            priceText.setFont(font);
            priceText.setString(std::to_string(price));
            priceText.setCharacterSize(15);
            priceText.setFillColor(sf::Color::Black);
            priceText.setPosition(windowWidth - 50.0f - normalizedWidth - 10 - 50, sellYOffset + maxBarHeight / 2 - 10); // Position beside the bar

            quantityText.setFont(font);
            quantityText.setString(std::to_string(quantity));
            quantityText.setCharacterSize(15);
            quantityText.setFillColor(sf::Color::Black);
            quantityText.setPosition(windowWidth - 50.0f - normalizedWidth - 10 - 50, sellYOffset + maxBarHeight / 2 + 10); // Position beside the bar

            window.draw(priceText);
            window.draw(quantityText);

            // Update yOffset for sell orders
            sellYOffset += maxBarHeight + 5; // Move down for the next order
        }
    }

public:
    void visualize(sf::RenderWindow& window, const Orderbook& orderbook)
    {
        // Window created in main to prevent constantly generating a new window everytime we call visualize
        window.clear(sf::Color::Black);
        drawOrderDepth(window, orderbook);
        window.display();
    } 
};
