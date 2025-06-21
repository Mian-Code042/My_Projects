#include <iostream>
#include <string>
using namespace std;

const int MAX = 100;

class Book {
public:
    string ISBN, title, author, publisher;
    int quantity;

    void input() {
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Publisher: ";
        getline(cin, publisher);
        cout << "Enter Quantity: ";
        while (!(cin >> quantity) || quantity < 0) {
            cout << "Invalid! Enter a positive number: ";
            cin.clear(); cin.ignore(10000, '\n');
        }
    }

    void display() {
        cout << "\nISBN: " << ISBN << "\nTitle: " << title
             << "\nAuthor: " << author << "\nPublisher: " << publisher
             << "\nQuantity: " << quantity << "\n";
    }
};

class Member {
public:
    string memberID, name, address, contact;

    void input() {
        cout << "Enter Member ID: ";
        cin >> memberID;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Contact: ";
        getline(cin, contact);
    }

    void display() {
        cout << "\nID: " << memberID << "\nName: " << name
             << "\nAddress: " << address << "\nContact: " << contact << "\n";
    }
};

// Global book and member arrays
Book books[MAX];
Member members[MAX];
int bookCount = 0, memberCount = 0;

void addBook() {
    if (bookCount < MAX) {
        books[bookCount++].input();
    } else {
        cout << "Book limit reached.\n";
    }
}

void displayBooks() {
    if (bookCount == 0) cout << "No books found.\n";
    for (int i = 0; i < bookCount; i++) {
        books[i].display();
    }
}

void searchBook() {
    string key;
    cout << "Enter ISBN or Title: ";
    cin >> key;
    bool found = false;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].ISBN == key || books[i].title == key) {
            books[i].display();
            found = true;
        }
    }
    if (!found) cout << "Book not found.\n";
}

void addMember() {
    if (memberCount < MAX) {
        members[memberCount++].input();
    } else {
        cout << "Member limit reached.\n";
    }
}

void displayMembers() {
    if (memberCount == 0) cout << "No members found.\n";
    for (int i = 0; i < memberCount; i++) {
        members[i].display();
    }
}

void searchMember() {
    string key;
    cout << "Enter Member ID or Name: ";
    cin >> key;
    bool found = false;
    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberID == key || members[i].name == key) {
            members[i].display();
            found = true;
        }
    }
    if (!found) cout << "Member not found.\n";
}

void menu() {
    int choice;
    do {
        cout << "\n--- Library Management Menu ---\n";
        cout << "1. Add Book\n2. Display All Books\n3. Search Book\n";
        cout << "4. Add Member\n5. Display Members\n6. Search Member\n";
        cout << "0. Exit\nChoose: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: addMember(); break;
            case 5: displayMembers(); break;
            case 6: searchMember(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
