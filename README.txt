Link to project: https://github.com/seahsongli/cpp-orderbook
# C++ Orderbook

This project is an implementation of an **Orderbook** system in C++, featuring multithreaded execution, basic order matching, and a simple GUI for real-time visualization. The system efficiently matches buy and sell orders while allowing live updates of the orderbook state.

<img src="./recording_cpp_orderbook.gif" alt="Order Book Demo" width="800" height= "300" />

## Features

- **Multithreading**: 
  - One thread generates random orders (both buy and sell).
  - Another thread handles order matching.
  - The main thread is responsible for rendering the GUI.
- **Order Matching**: 
  - Basic price-time priority order matching.
  - Partial order fulfillment is handled: when a buy or sell order's quantity exceeds the available counterpart.
  - Real-time order removal and insertion into the orderbook.
- **Simple GUI**: A graphical interface built with SFML displays orderbook depth and shows matched orders in real-time.
- **Data Structures**: The system uses priority queues to manage buy and sell orders efficiently, maintaining price-time priority.
  
## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Order Matching Logic](#order-matching-logic)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

Ensure you have the following installed on your system:

- **C++17 or later**: Required for modern C++ features.
- **CMake**: For building the project.
- **SFML**: Used for the GUI. Install it as follows:

    **Linux:**
    ```bash
    sudo apt-get install libsfml-dev
    ```

    **macOS (with Homebrew):**
    ```bash
    brew install sfml
    ```

    **Windows**: Download and install from the official [SFML website](https://www.sfml-dev.org/).

### Build Instructions

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/orderbook.git
    cd orderbook
    ```

2. Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```bash
    cmake ..
    ```

4. Build the project:
    ```bash
    make
    ```

5. Run the executable:
    ```bash
    ./orderbook
    ```

## Usage

Once the application is built and running, the GUI will display live updates of the orderbook with the following features.

- **Random Buy/Sell Orders**: The system automatically generates random buy and sell orders in a separate thread.
- **View Orderbook Depth**: The GUI visualizes the current depth of the orderbook (bid/ask volumes).
- **Order Matching**: A separate thread continuously matches buy and sell orders according to price and time priority.

### Example Workflow:

1. **Launching the GUI**: Upon running the program, a window with a visual representation of the orderbook is displayed.
2. **Order Generation**: Orders are generated every second in a separate thread and added to the orderbook.
3. **Order Matching**: Another thread continuously checks for matching orders and executes them when conditions are met.
4. **Visualization**: The GUI updates in real-time to reflect new orders and matched trades.

## Project Structure

The project consists of several key components:

- **Order**: Represents a buy or sell order with properties like price, quantity, and order type.
- **Orderbook**: Manages the list of current buy and sell orders using priority queues. Provides methods to add, remove, and retrieve the best orders.
- **OrderMatchingEngine**: Handles the core matching logic, executing orders when prices align and managing partial order fulfillment.
- **OrderbookVisualizer**: Uses SFML to render a graphical representation of the orderbook depth (volume of orders at different price levels).

## Order Matching Logic

The order matching engine works as follows:

1. **Price-Time Priority**: Buy and sell orders are matched based on price. For orders with the same price, time priority (FIFO) is used.
2. **Partial Orders**: If a buy order's quantity is greater than the matching sell order, the remaining quantity of the buy order is re-added to the orderbook, and vice versa.
3. **Execution**: When a match occurs, both orders are removed from the orderbook (either fully or partially).

### Example Matching Scenario

- A buy order for 500 units at \$400 and a sell order for 300 units at \$390 will match because the buy price is greater than or equal to the sell price.
- After the match, the remaining 200 units of the buy order will be left in the orderbook.

