// Enhanced Stock Trading System with Improvements
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>
using namespace std;

// ===== Abstraction Base Class =====
class Account {
public:
    virtual void viewDetails() const = 0;
    virtual ~Account() {}
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

    void setPrice(double pr) { price = pr; }

    void display() const {
        cout << left << setw(10) << symbol << setw(20) << companyName
             << "$" << fixed << setprecision(2) << price << endl;
    }
};

// ===== User Class (Inherits Account) =====
class User : public Account {
private:
    string username;
    string password;
    double balance;
    int portfolio[3];

public:
    User(string uname = "", string pwd = "") : username(uname), password(pwd), balance(10000.0) {
        for (int &p : portfolio) p = 0;
    }

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    double getBalance() const { return balance; }

    void viewDetails() const override {
        cout << "User: " << username << " | Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    void buyStock(const Stock& s, int qty) {
        double total = qty * s.getPrice();
        if (qty <= 0) {
            cout << "Invalid quantity!\n"; return;
        }
        if (balance >= total) {
            balance -= total;
            if (s.getSymbol() == "AAPL") portfolio[0] += qty;
            else if (s.getSymbol() == "GOOG") portfolio[1] += qty;
            else if (s.getSymbol() == "TSLA") portfolio[2] += qty;
            cout << "Purchased " << qty << " shares of " << s.getSymbol() << ".\n";
        } else {
            cout << "Insufficient funds!\n";
        }
    }

    void sellStock(const Stock& s, int qty) {
        if (qty <= 0) {
            cout << "Invalid quantity!\n"; return;
        }
        int index = (s.getSymbol() == "AAPL") ? 0 : (s.getSymbol() == "GOOG") ? 1 : 2;
        if (portfolio[index] >= qty) {
            portfolio[index] -= qty;
            double total = qty * s.getPrice();
            balance += total;
            cout << "Sold " << qty << " shares of " << s.getSymbol() << ".\n";
        } else {
            cout << "Not enough shares!\n";
        }
    }

    void viewPortfolio() const {
        cout << "\n--- Portfolio ---\n";
        cout << "AAPL: " << portfolio[0] << "\n";
        cout << "GOOG: " << portfolio[1] << "\n";
        cout << "TSLA: " << portfolio[2] << "\n";
    }

    void updateCapital(const Stock* market) {
        double total = balance;
        total += portfolio[0] * market[0].getPrice();
        total += portfolio[1] * market[1].getPrice();
        total += portfolio[2] * market[2].getPrice();
        cout << "Total Capital Value: $" << fixed << setprecision(2) << total << endl;
    }

    void saveToFile(ofstream& file) const {
        file << username << " " << password << " " << balance << " "
             << portfolio[0] << " " << portfolio[1] << " " << portfolio[2] << endl;
    }

    void loadFromStream(ifstream& file) {
        file >> username >> password >> balance >> portfolio[0] >> portfolio[1] >> portfolio[2];
    }
};

// ===== Utility Function for Hidden Password =====
string getHiddenPassword() {
    string pwd;
    char ch;
    while ((ch = _getch()) != '\r') {
        pwd.push_back(ch);
        cout << '*';
    }
    cout << endl;
    return pwd;
}

// ===== TradingSystem Class =====
class TradingSystem {
private:
    Stock market[3];
    User users[100];
    int userCount;

public:
    TradingSystem() : userCount(0) {
        market[0] = Stock("AAPL", "Apple Inc.", 150.0);
        market[1] = Stock("GOOG", "Alphabet Inc.", 2800.0);
        market[2] = Stock("TSLA", "Tesla Motors", 700.0);
        loadUsers();
    }

    ~TradingSystem() {
        saveUsers();
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (int i = 0; i < userCount; i++) users[i].saveToFile(file);
        file.close();
    }

    void loadUsers() {
        ifstream file("users.txt");
        while (file && userCount < 100) {
            users[userCount].loadFromStream(file);
            if (file) userCount++;
        }
        file.close();
    }

    void registerUser() {
        if (userCount >= 100) return;
        string uname;
        cout << "Enter username: "; cin >> uname;
        cout << "Enter password: "; string pwd = getHiddenPassword();
        for (int i = 0; i < userCount; i++) {
            if (users[i].getUsername() == uname) {
                cout << "Username exists.\n";
                return;
            }
        }
        users[userCount++] = User(uname, pwd);
        cout << "User registered.\n";
    }

    void loginUser() {
        string uname;
        cout << "Enter username: "; cin >> uname;
        cout << "Enter password: "; string pwd = getHiddenPassword();
        for (int i = 0; i < userCount; i++) {
            if (users[i].getUsername() == uname && users[i].getPassword() == pwd) {
                cout << "Login successful.\n";
                users[i].viewDetails();
                tradingMenu(&users[i]);
                return;
            }
        }
        cout << "Login failed.\n";
    }

    void tradingMenu(User* user) {
        int choice;
        do {
            cout << "\n1. View Market\n2. Buy\n3. Sell\n4. Portfolio\n5. Capital\n6. Logout\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: showMarket(); break;
                case 2: buyStock(user); break;
                case 3: sellStock(user); break;
                case 4: user->viewPortfolio(); break;
                case 5: user->updateCapital(market); break;
                case 6: cout << "Logged out.\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 6);
    }

    void showMarket() const {
        cout << "\n--- Stock Market ---\n";
        for (const auto& s : market) s.display();
    }

    void buyStock(User* user) {
        string sym;
        int qty;
        cout << "Enter stock symbol: "; cin >> sym;
        cout << "Enter quantity: "; cin >> qty;
        for (const auto& s : market) {
            if (s.getSymbol() == sym) {
                user->buyStock(s, qty);
                return;
            }
        }
        cout << "Invalid symbol.\n";
    }

    void sellStock(User* user) {
        string sym;
        int qty;
        cout << "Enter stock symbol: "; cin >> sym;
        cout << "Enter quantity: "; cin >> qty;
        for (const auto& s : market) {
            if (s.getSymbol() == sym) {
                user->sellStock(s, qty);
                return;
            }
        }
        cout << "Invalid symbol.\n";
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
