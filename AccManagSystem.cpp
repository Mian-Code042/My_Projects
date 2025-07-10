#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <conio.h>
using namespace std;

const int MAX = 100; // Max accounts

class Account {
private:
    int accNo;
    string name;
    string type;
    float balance;
    vector<string> history;

public:
    void create(int no, string n, string t, float b) {
        accNo = no;
        name = n;
        type = t;
        balance = b;
        addHistory("Account created with balance: " + to_string(b));
    }

    int getAccNo() { return accNo; }
    float getBalance() { return balance; }
    string getName() { return name; }
    string getType() { return type; }

    void display() {
        cout << "Account No: " << accNo << "\nName: " << name
             << "\nType: " << type << "\nBalance: " << fixed << setprecision(2) << balance << endl;
    }

    void deposit(float amt) {
        if (amt <= 0) {
            cout << "Invalid amount!\n";
            return;
        }
        balance += amt;
        cout << "Deposited: " << amt << "\nNew Balance: " << balance << endl;
        addHistory("Deposited: " + to_string(amt));
    }

    void withdraw(float amt) {
        if (amt <= 0) {
            cout << "Invalid amount!\n";
            return;
        }
        if (amt <= balance) {
            balance -= amt;
            cout << "Withdrawn: " << amt << "\nNew Balance: " << balance << endl;
            addHistory("Withdrawn: " + to_string(amt));
        } else {
            cout << "Insufficient balance!\n";
        }
    }

    void update(string newName, string newType) {
        name = newName;
        type = newType;
        cout << "Account updated successfully.\n";
        addHistory("Account details updated.");
    }

    void addHistory(string msg) {
        time_t now = time(0);
        string dt = ctime(&now);
        dt.pop_back(); // remove newline
        history.push_back("[" + dt + "] " + msg);
    }

    void showHistory() {
        cout << "\n--- Transaction History ---\n";
        for (const string& h : history) cout << h << endl;
    }

    void saveToFile(ofstream& file) {
        file << accNo << '|' << name << '|' << type << '|' << balance << '|';
        file << history.size() << '|';
        for (const string& h : history) file << h << '|';
        file << endl;
    }

    void loadFromStream(ifstream& file) {
        string line;
        if (getline(file, line)) {
            stringstream ss(line);
            string temp;
            getline(ss, temp, '|'); accNo = stoi(temp);
            getline(ss, name, '|');
            getline(ss, type, '|');
            getline(ss, temp, '|'); balance = stof(temp);
            getline(ss, temp, '|'); int histCount = stoi(temp);
            history.clear();
            for (int i = 0; i < histCount; ++i) {
                string entry; getline(ss, entry, '|');
                history.push_back(entry);
            }
        }
    }
};

Account accList[MAX];
int accCount = 0;

int findAccount(int accNo) {
    for (int i = 0; i < accCount; i++)
        if (accList[i].getAccNo() == accNo) return i;
    return -1;
}

void saveAccounts() {
    ofstream file("accounts.txt");
    for (int i = 0; i < accCount; i++) accList[i].saveToFile(file);
    file.close();
}

void loadAccounts() {
    ifstream file("accounts.txt");
    while (file && accCount < MAX) {
        accList[accCount].loadFromStream(file);
        if (!file.eof()) accCount++;
    }
    file.close();
}

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

bool login() {
    string username, password;
    cout << "\n--- Admin Login Required ---\n";
    cout << "Username: "; cin >> username;
    cout << "Password: "; password = getHiddenPassword();
    if (username == "admin" && password == "123") {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Incorrect username or password!\n";
        return false;
    }
}

void menu() {
    cout << "\n--- Account Management System ---\n";
    cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Balance Check\n5. Display Account\n";
    cout << "6. Display All\n7. Update\n8. Delete\n9. Transaction History\n10. Exit\nChoose: ";
}

int main() {
    loadAccounts();
    if (!login()) {
        cout << "Access Denied. Exiting program.\n";
        return 0;
    }

    int choice;
    do {
        menu();
        cin >> choice;
        if (choice == 1) {
            int no; string name, type; float bal;
            cout << "Enter Account No: "; cin >> no;
            if (findAccount(no) != -1) {
                cout << "Account number already exists!\n";
                continue;
            }
            cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            cout << "Enter Type: "; getline(cin, type);
            cout << "Enter Initial Balance: "; cin >> bal;
            accList[accCount++].create(no, name, type, bal);
            cout << "Account Created!\n";
            saveAccounts();
        }
        else if (choice >= 2 && choice <= 4) {
            int no;
            cout << "Enter Account No: "; cin >> no;
            int idx = findAccount(no);
            if (idx == -1) cout << "Account not found!\n";
            else {
                if (choice == 2) {
                    float amt; cout << "Amount to Deposit: "; cin >> amt;
                    accList[idx].deposit(amt);
                } else if (choice == 3) {
                    float amt; cout << "Amount to Withdraw: "; cin >> amt;
                    accList[idx].withdraw(amt);
                } else if (choice == 4) {
                    cout << "Balance: " << accList[idx].getBalance() << endl;
                }
                saveAccounts();
            }
        }
        else if (choice == 5) {
            int no; cout << "Enter Account No: "; cin >> no;
            int idx = findAccount(no);
            if (idx != -1) accList[idx].display();
            else cout << "Account not found!\n";
        }
        else if (choice == 6) {
            for (int i = 0; i < accCount; i++) {
                accList[i].display();
                cout << "----------------\n";
            }
        }
        else if (choice == 7) {
            int no; string newName, newType;
            cout << "Enter Account No: "; cin >> no;
            int idx = findAccount(no);
            if (idx != -1) {
                cin.ignore();
                cout << "Enter New Name: "; getline(cin, newName);
                cout << "Enter New Type: "; getline(cin, newType);
                accList[idx].update(newName, newType);
                saveAccounts();
            } else cout << "Account not found!\n";
        }
        else if (choice == 8) {
            int no; cout << "Enter Account No to Delete: "; cin >> no;
            int idx = findAccount(no);
            if (idx != -1) {
                for (int i = idx; i < accCount - 1; i++) accList[i] = accList[i + 1];
                accCount--;
                cout << "Account deleted.\n";
                saveAccounts();
            } else cout << "Account not found!\n";
        }
        else if (choice == 9) {
            int no; cout << "Enter Account No: "; cin >> no;
            int idx = findAccount(no);
            if (idx != -1) accList[idx].showHistory();
            else cout << "Account not found!\n";
        }
        else if (choice != 10) {
            cout << "Invalid choice! Please select again.\n";
        }
    } while (choice != 10);

    cout << "Thank you for using the system!\n";
    return 0;
}
