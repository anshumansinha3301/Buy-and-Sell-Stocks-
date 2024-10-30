#include <iostream>
#include <map>
#include <string>
#include <iomanip>

using namespace std;  

class Stock {
public:
    string symbol;
    double price;

    Stock() : symbol(""), price(0.0) {} // Default constructor
    Stock(string s, double p) : symbol(s), price(p) {}
};

class Portfolio {
private:
    map<string, pair<int, double>> holdings; // <Stock Symbol, <Quantity, Average Price>>
    double balance;

public:
    Portfolio() : balance(10000.0) {}

    void buyStock(const Stock& stock, int quantity) {
        double totalCost = stock.price * quantity;
        if (totalCost > balance) {
            cout << "Insufficient balance to buy " << quantity << " of " << stock.symbol << ".\n";
            return;
        }
        balance -= totalCost;

        if (holdings.find(stock.symbol) != holdings.end()) {
            int currentQuantity = holdings[stock.symbol].first;
            double averagePrice = holdings[stock.symbol].second;
            averagePrice = (averagePrice * currentQuantity + stock.price * quantity) / (currentQuantity + quantity);
            holdings[stock.symbol] = {currentQuantity + quantity, averagePrice};
        } else {
            holdings[stock.symbol] = {quantity, stock.price};
        }
        cout << "Bought " << quantity << " of " << stock.symbol << " for $" << totalCost << ". New balance: $" << balance << ".\n";
    }

    void sellStock(const Stock& stock, int quantity) {
        if (holdings.find(stock.symbol) == holdings.end() || holdings[stock.symbol].first < quantity) {
            cout << "Not enough shares of " << stock.symbol << " to sell.\n";
            return;
        }
        int currentQuantity = holdings[stock.symbol].first;
        holdings[stock.symbol].first -= quantity;
        double totalRevenue = stock.price * quantity;
        balance += totalRevenue;
        cout << "Sold " << quantity << " of " << stock.symbol << " for $" << totalRevenue << ". New balance: $" << balance << ".\n";

        if (holdings[stock.symbol].first == 0) {
            holdings.erase(stock.symbol);
        }
    }

    void showPortfolio() const {
        cout << "\nCurrent Portfolio:\n";
        if (holdings.empty()) {
            cout << "No stocks in the portfolio.\n";
            return;
        }
        for (const auto& entry : holdings) {
            cout << "Stock: " << entry.first
                 << ", Quantity: " << entry.second.first
                 << ", Average Price: $" << fixed << setprecision(2) << entry.second.second << "\n";
        }
        cout << "Balance: $" << fixed << setprecision(2) << balance << "\n";
    }
};

void showMenu() {
    cout << "\nStock Trading System\n";
    cout << "1. Buy Stock\n";
    cout << "2. Sell Stock\n";
    cout << "3. View Portfolio\n";
    cout << "4. Exit\n";
}

int main() {
    Portfolio portfolio;
    map<string, Stock> stockMarket = {
        {"AAPL", Stock("AAPL", 175.23)},
        {"GOOGL", Stock("GOOGL", 2835.46)},
        {"MSFT", Stock("MSFT", 310.75)},
        {"TSLA", Stock("TSLA", 735.46)},
        {"AMZN", Stock("AMZN", 3472.75)}
    };

    while (true) {
        showMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string symbol;
            int quantity;
            cout << "Enter the stock symbol to buy: ";
            cin >> symbol;
            if (stockMarket.find(symbol) == stockMarket.end()) {
                cout << "Invalid stock symbol.\n";
                continue;
            }
            cout << "Enter quantity to buy: ";
            cin >> quantity;
            if (quantity <= 0) {
                cout << "Quantity must be greater than zero.\n";
                continue;
            }
            portfolio.buyStock(stockMarket[symbol], quantity);
        } else if (choice == 2) {
            string symbol;
            int quantity;
            cout << "Enter the stock symbol to sell: ";
            cin >> symbol;
            if (stockMarket.find(symbol) == stockMarket.end()) {
                cout << "Invalid stock symbol.\n";
                continue;
            }
            cout << "Enter quantity to sell: ";
            cin >> quantity;
            if (quantity <= 0) {
                cout << "Quantity must be greater than zero.\n";
                continue;
            }
            portfolio.sellStock(stockMarket[symbol], quantity);
        } else if (choice == 3) {
            portfolio.showPortfolio();
        } else if (choice == 4) {
            cout << "Exiting the system.\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
