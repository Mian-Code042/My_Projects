#include <iostream>
#include <string>
#include <ctime>
using namespace std;

const int MAX = 100; // max accounts

class Account {
private:
    int accNo;
    string name;
    string type;
    float balance;
public:
    void create(int no, string n, string t, float b) {
        accNo = no;
        name = n;
        type = t;
        balance = b;
    }
    int getAccNo() { return accNo; }
    void display() {
        cout << "Account No: " << accNo << "\nName: " << name << "\nType: " << type << "\nBalance: " << balance << endl;
    }
    void deposit(float amt) {
        balance += amt;
        cout << "Deposited: " << amt << "\nNew Balance: " << balance << endl;
    }
    void withdraw(float amt) {
        if (amt <= balance) {
            balance -= amt;
            cout << "Withdrawn: " << amt << "\nNew Balance: " << balance << endl;
        } else cout << "Insufficient balance!\n";
    }
    float getBalance() { return balance; }
    void update(string newName, string newType) {
        name = newName;
        type = newType;
        cout << "Account updated successfully.\n";
    }
};

Account accList[MAX];
int accCount = 0;

int findAccount(int accNo) {
    for (int i = 0; i < accCount; i++)
        if (accList[i].getAccNo() == accNo) return i;
    return -1;
}

//  ADMIN LOGIN FUNCTION
bool login() {
    string username, password;
    cout << "\n--- Admin Login Required ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if (username == "admin" && password == "123") {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Incorrect username or password!\n";
        return false;
    }
}

// Display Menu
void menu() {
    cout << "\n--- Account Management System ---\n";
    cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Balance Check\n5. Display Account\n";
    cout << "6. Display All\n7. Update\n8. Delete\n9. Exit\nChoose: ";
}

int main() {
    if (!login()) {
        cout << "Access Denied. Exiting program.\n";
        return 0;
    }

    int choice;
    do {
        menu();
        cin >> choice;
        if (choice == 1) {
            int no;
            string name, type;
            float bal;
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
                }
                else if (choice == 3) {
                    float amt; cout << "Amount to Withdraw: "; cin >> amt;
                    accList[idx].withdraw(amt);
                }
                else if (choice == 4) {
                    cout << "Balance: " << accList[idx].getBalance() << endl;
                }
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
            } else cout << "Account not found!\n";
        }
        else if (choice == 8) {
            int no; cout << "Enter Account No to Delete: "; cin >> no;
            int idx = findAccount(no);
            if (idx != -1) {
                for (int i = idx; i < accCount - 1; i++)
                    accList[i] = accList[i + 1];
                accCount--;
                cout << "Account deleted.\n";
            } else cout << "Account not found!\n";
        }
        else if (choice != 9) {
            cout << "Invalid choice! Please select again.\n";
        }
    } while (choice != 9);

    cout << "Thank you for using the system!\n";
    return 0;
}
