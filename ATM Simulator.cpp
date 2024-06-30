#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <conio.h>
using namespace std;

struct User {
    string name;
    string pin;
    double balance;
    vector<string> transactionHistory;
};

class ATM {
private:
    map<string, User> users;
    User* currentUser;
public:
    ATM() : currentUser(nullptr) {}

    void createUser(string name, string pin, double initialBalance) {
        string accountNumber = generateAccountNumber();
        users[accountNumber] = {name, pin, initialBalance, {}};
        if (currentUser == nullptr) {
            currentUser = &users[accountNumber];
        }
    }

    bool login(string accountNumber, string pin) {
        auto user = users.find(accountNumber);
        if (user != users.end() && user->second.pin == pin) {
            currentUser = &user->second;
            cout << "Welcome " << currentUser->name << "!" << endl;
            return true;
        }
        cout << "Invalid account number or PIN. Please try again." << endl;
        return false;
    }

    void checkBalance() {
        cout << "Current Balance: Rs." << fixed << setprecision(2) << currentUser->balance << endl;
    }

    void deposit(double amount) {
        currentUser->balance += amount;
        string transaction = "Deposited Rs." + to_string(amount);
        currentUser->transactionHistory.push_back(transaction);
        cout << endl;
        cout << transaction << " successfully." << endl << "Current Balance: Rs." << fixed << setprecision(2) << currentUser->balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= currentUser->balance) {
            currentUser->balance -= amount;
            string transaction = "Withdrawn Rs." + to_string(amount);
            currentUser->transactionHistory.push_back(transaction);
            cout << endl;
            cout << transaction << " successfully." << endl << "Current Balance: Rs." << fixed << setprecision(2) << currentUser->balance << endl;
        } else {
            cout << "Insufficient funds." << endl;
        }
    }

    void transfer(string recipientAccount, double amount) {
        auto recipientUser = users.find(recipientAccount);
        if (recipientUser != users.end()) {
            if (recipientAccount != getCurrentAccountNumber()) {
                if (amount <= currentUser->balance) {
                    currentUser->balance -= amount;
                    recipientUser->second.balance += amount;
                    string transaction = "Transferred Rs." + to_string(amount) + " to " + recipientUser->second.name;
                    currentUser->transactionHistory.push_back(transaction);
                    cout << endl;
                    cout << transaction << " successfully." << endl;
                } else {
                    cout << "Insufficient funds." << endl;
                }
            } else {
                cout << "Cannot transfer funds to your own account." << endl;
            }
        } else {
            cout << "Recipient account not found." << endl;
        }
    }

    void printTransactionHistory() {
        cout << endl;
        cout << "TRANSACTION HISTORY:" << endl;
        for (const auto& transaction : currentUser->transactionHistory) {
            cout << transaction << endl;
        }
    }

private:
    string generateAccountNumber() {
        static int nextAccountNumber = 1;
        return "ACC" + to_string(nextAccountNumber++);
    }

    string getCurrentAccountNumber() {
        for (const auto& pair : users) {
            if (&pair.second == currentUser) {
                return pair.first;
            }
        }
        return "";
    }
};

string getHiddenPIN() {
    string pin;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!pin.empty()) {
                cout << "\b \b";
                pin.pop_back();
            }
        } else {
            pin.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return pin;
}

int main() {
    ATM atm;
    atm.createUser("Vinayak Mahadev", "1234", 1000.0);
    atm.createUser("Prithvi Raj", "5678", 500.0);
    string accountNumber, pin;
    cout << "Enter account number: ";
    cin >> accountNumber;
    cout << "Enter PIN: ";
    pin = getHiddenPIN();
    if (atm.login(accountNumber, pin)) {
        int choice;
        do {
            cout << "\n===== ATM Menu =====" << endl;
            cout << "1. Check Balance" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Transfer Funds" << endl;
            cout << "5. View Transaction History" << endl;
            cout << "6. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    atm.checkBalance();
                    break;
                case 2: {
                    double amount;
                    cout << "Enter amount to deposit: Rs.";
                    cin >> amount;
                    atm.deposit(amount);
                    break;
                }
                case 3: {
                    double amount;
                    cout << "Enter amount to withdraw: Rs.";
                    cin >> amount;
                    atm.withdraw(amount);
                    break;
                }
                case 4: {
                    string recipientAccount;
                    double amount;
                    cout << "Enter recipient's account number: ";
                    cin >> recipientAccount;
                    cout << "Enter amount to transfer: Rs.";
                    cin >> amount;
                    atm.transfer(recipientAccount, amount);
                    break;
                }
                case 5:
                    atm.printTransactionHistory();
                    break;
                case 6:
                    cout << endl;
                    cout << "Thank You! Visit Again :)" << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 6);
    }
}
