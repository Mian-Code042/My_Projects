#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ===== Abstraction Base Class =====
class Account {
public:
    virtual void viewDetails() const = 0; // Pure virtual function
    virtual ~Account() {} // Virtual destructor
};

// ===== Stock Class =====
class Stock {
private:
    string symbol;
    string companyName;
    double price;

public:
    Stock(string sym = "", string name = "", double pr = 0.0)
        : symbol(sym), companyName(name), price(pr) {}

    string getSymbol() const { return symbol; }
    string getCompany() const { return companyName; }
    double getPrice() const { return price; }

    void display() const {
        cout << left << setw(10) << symbol << setw(20) << companyName
             << "$" << fixed << setprecision(2) << price << endl;
    }
};

// ===== User Class (Inherits Account) =====
class User : public Account {
private:
    string username;
    string password; // Added password field
    double balance;
    int* portfolioAAPL;
    int* portfolioGOOG;
    int* portfolioTSLA;

    void allocate() {
        portfolioAAPL = new int(0);
        portfolioGOOG = new int(0);
        portfolioTSLA = new int(0);
    }

public:
    User(string uname = "", string pwd = "") : username(uname), password(pwd), balance(10000.0) {
        allocate();
    }

    // Copy Constructor
    User(const User& other)
        : username(other.username), password(other.password), balance(other.balance) {
        allocate();
        *portfolioAAPL = *other.portfolioAAPL;
        *portfolioGOOG = *other.portfolioGOOG;
        *portfolioTSLA = *other.portfolioTSLA;
    }

    // Assignment Operator
    User& operator=(const User& other) {
        if (this != &other) {
            username = other.username;
            password = other.password;
            balance = other.balance;
            *portfolioAAPL = *other.portfolioAAPL;
            *portfolioGOOG = *other.portfolioGOOG;
            *portfolioTSLA = *other.portfolioTSLA;
        }
        return *this;
    }

    ~User() {
        delete portfolioAAPL;
        delete portfolioGOOG;
        delete portfolioTSLA;
    }

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    double getBalance() const { return balance; }

    // Override from Account
    void viewDetails() const override {
        cout << "User: " << username << " | Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    void buyStock(string symbol, int qty, double price) {
        double total = qty * price;
        if (balance >= total) {
            balance -= total;

            if (symbol == "AAPL") *portfolioAAPL += qty;
            else if (symbol == "GOOG") *portfolioGOOG += qty;
            else if (symbol == "TSLA") *portfolioTSLA += qty;

            cout << "Purchased " << qty << " shares of " << symbol << ".\n";
        } else {
            cout << "Insufficient funds!\n";
        }
    }

    void sellStock(string symbol, int qty, double price) {
        int* port = nullptr;
        if (symbol == "AAPL") port = portfolioAAPL;
        else if (symbol == "GOOG") port = portfolioGOOG;
        else if (symbol == "TSLA") port = portfolioTSLA;

        if (port && *port >= qty) {
            *port -= qty;
            double total = qty * price;
            balance += total;
            cout << "Sold " << qty << " shares of " << symbol << ".\n";
        } else {
            cout << "Not enough shares!\n";
        }
    }

    void viewPortfolio() const {
        cout << "\n--- Portfolio ---\n";
        cout << "AAPL: " << *portfolioAAPL << "\n";
        cout << "GOOG: " << *portfolioGOOG << "\n";
        cout << "TSLA: " << *portfolioTSLA << "\n";
    }

    void updateCapital(const Stock* market) {
        double total = balance;
        total += *portfolioAAPL * market[0].getPrice();
        total += *portfolioGOOG * market[1].getPrice();
        total += *portfolioTSLA * market[2].getPrice();

        cout << "Total Capital Value: $" << fixed << setprecision(2) << total << endl;
    }
};

// ===== TradingSystem Class =====
class TradingSystem {
private:
    Stock market[3];
    User users[100];     // Fixed array to store multiple users
    int userCount;
    User* currentUser;

public:
    TradingSystem() : userCount(0), currentUser(nullptr) {
        market[0] = Stock("AAPL", "Apple Inc.", 150.0);
        market[1] = Stock("GOOG", "Alphabet Inc.", 2800.0);
        market[2] = Stock("TSLA", "Tesla Motors", 700.0);
    }

    ~TradingSystem() {
        // Nothing dynamic to delete here (Users managed in array)
    }

    void registerUser() {
        if (userCount >= 100) {
            cout << "User limit reached, cannot register more users.\n";
            return;
        }

        string uname, pwd;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pwd;

        // Check if username already exists
        for (int i = 0; i < userCount; i++) {
            if (users[i].getUsername() == uname) {
                cout << "Username already exists! Try again.\n";
                return;
            }
        }

        users[userCount++] = User(uname, pwd);
        cout << "User registered successfully.\n";
    }

    void loginUser() {
        string uname, pwd;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pwd;

        for (int i = 0; i < userCount; i++) {
            if (users[i].getUsername() == uname && users[i].getPassword() == pwd) {
                cout << "Login successful!\n";
                currentUser = &users[i];
                currentUser->viewDetails();
                tradingMenu(currentUser);
                currentUser = nullptr; // logout after tradingMenu ends
                return;
            }
        }
        cout << "Login failed. Incorrect username or password.\n";
    }

    void showMarket() const {
        cout << "\n--- Stock Market ---\n";
        for (const auto& s : market) s.display();
    }

    void buyStock(User* user) {
        string sym;
        int qty;
        cout << "Enter stock symbol: ";
        cin >> sym;
        cout << "Enter quantity: ";
        cin >> qty;
        for (const auto& s : market) {
            if (s.getSymbol() == sym) {
                user->buyStock(sym, qty, s.getPrice());
                return;
            }
        }
        cout << "Invalid symbol.\n";
    }

    void sellStock(User* user) {
        string sym;
        int qty;
        cout << "Enter stock symbol: ";
        cin >> sym;
        cout << "Enter quantity: ";
        cin >> qty;
        for (const auto& s : market) {
            if (s.getSymbol() == sym) {
                user->sellStock(sym, qty, s.getPrice());
                return;
            }
        }
        cout << "Invalid symbol.\n";
    }

    void tradingMenu(User* user) {
        int choice;
        do {
            cout << "\n1. View Market\n2. Buy\n3. Sell\n4. View Portfolio\n5. Update Capital\n6. Logout\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: showMarket(); break;
                case 2: buyStock(user); break;
                case 3: sellStock(user); break;
                case 4: user->viewPortfolio(); break;
                case 5: user->updateCapital(market); break;
                case 6: cout << "Logging out.\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 6);
    }

    void mainMenu() {
        int choice;
        do {
            cout << "\n--- Main Menu ---\n1. Register\n2. Login\n3. Exit\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: registerUser(); break;
                case 2: loginUser(); break;
                case 3: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 3);
    }
};

// ===== Main =====
int main() {
    TradingSystem ts;
    ts.mainMenu();
    return 0;
}