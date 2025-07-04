#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool authenticate(int &accNum_out, int &pin_out, float &balance_out) {
    int accNum_input, pin_input;
    int accNum_file, pin_file;
    float balance_file;
    bool found = false;

    cout << "Enter account number: ";
    cin >> accNum_input;

    ifstream file("data.txt");
    while (file >> accNum_file >> pin_file >> balance_file) {
        if (accNum_input == accNum_file) {
            found = true;
            int attempts = 3;
            while (attempts--) {
                cout << "Enter PIN: ";
                cin >> pin_input;
                if (pin_input == pin_file) {
                    accNum_out = accNum_file;
                    pin_out = pin_file;
                    balance_out = balance_file;
                    file.close();
                    return true;
                } else {
                    if (attempts > 0)
                        cout << "Wrong PIN... Try again (" << attempts << " attempts left)\n";
                    else
                        cout << "Access denied. Too many incorrect attempts.\n";
                }
            }
            file.close();
            return false;
        }
    }
    file.close();
    return false;
}

void create() {
    int x = 0, accNum, accNum_file, pin, pin_file;
    float balance, balance_file;
    while(1) {
        cout << "Enter account number: ";
        cin >> accNum;
        ifstream file("data.txt");
        while(file >> accNum_file >> pin_file >> balance_file) {
            if(accNum == accNum_file) {
                cout << "Account number already exists...\n";
                x = 1;
            }
        }
        file.close();
        if(x == 1) {
            x = 0;
            continue;
        }
        cout << "Enter pin: ";
        cin >> pin;
        if(accNum < 1000 || accNum > 9999 || pin < 1000 || pin > 9999) {
            cout << "Enter an account number and pin between 1000 - 9999" << endl;
        } else {
            while(1) {
                cout << "Enter balance: ";
                cin >> balance;
                if(balance < 0) {
                    cout << "Balance cannot be negative\n";
                } else {
                    break;
                }
            }
            break;
        }
    }
    ofstream file("data.txt", ios::app);
    file << accNum << " " << pin << " " << balance << "\n";
    cout << "Account added!" << endl;
    file.close();
}

void view() {
    int accNum, pin;
    float balance;
    if (authenticate(accNum, pin, balance)) {
        cout << "\nAccess granted!\n";
        cout << "Account number: " << accNum << "\n";
        cout << "Balance: $" << balance << "\n";
    } else {
        cout << "Failed to view account.\n";
    }
}

void deposit() {
    int accNum, pin;
    float balance, amt;
    if (authenticate(accNum, pin, balance)) {
        cout << "\nAccess granted!\n";
        cout << "Welcome " << accNum << endl;
        cout << "Enter amount to deposit: ";
        cin >> amt;
        while (true) {
            if (amt > 0 && amt <= 10000) {
                break;
            } else if (amt > 10000) {
                cout << "Can't add more than $10000\nEnter amount: ";
            } else {
                cout << "Enter a positive number\nEnter amount: ";
            }
            cin >> amt;
        }
        vector<int> accNums;
        vector<int> pins;
        vector<float> balances;
        int accNum_file, pin_file;
        float balance_file;
        ifstream infile("data.txt");
        while (infile >> accNum_file >> pin_file >> balance_file) {
            if (accNum == accNum_file) {
                balance_file += amt;
            }
            accNums.push_back(accNum_file);
            pins.push_back(pin_file);
            balances.push_back(balance_file);
        }
        infile.close();
        ofstream outfile("data.txt");
        for (int i = 0; i < accNums.size(); i++) {
            outfile << accNums[i] << " " << pins[i] << " " << balances[i] << "\n";
        }
        outfile.close();

        cout << "Amount deposited successfully!\n";
    } else {
        cout << "Authentication failed.\n";
    }
}


void withdraw() {
    int accNum, pin;
    float balance, amt;
    if (authenticate(accNum, pin, balance)) {
        cout << "\nAccess granted!\n";
        cout << "Welcome " << accNum << endl;
        cout << "Your current balance is: $" << balance << endl;
        cout << "Enter amount to withdraw: ";
        cin >> amt;
        while (true) {
            if (amt <= 0) {
                cout << "Enter a positive amount: ";
            } else if (amt > balance) {
                cout << "Insufficient balance. Enter amount less than or equal to $" << balance << ": ";
            } else {
                break;
            }
            cin >> amt;
        }
        vector<int> accNums;
        vector<int> pins;
        vector<float> balances;
        int accNum_file, pin_file;
        float balance_file;
        ifstream infile("data.txt");
        while (infile >> accNum_file >> pin_file >> balance_file) {
            if (accNum == accNum_file) {
                balance_file -= amt;
            }
            accNums.push_back(accNum_file);
            pins.push_back(pin_file);
            balances.push_back(balance_file);
        }
        infile.close();
        ofstream outfile("data.txt");
        for (int i = 0; i < accNums.size(); i++) {
            outfile << accNums[i] << " " << pins[i] << " " << balances[i] << "\n";
        }
        outfile.close();
        cout << "Amount withdrawn successfully!\n";
        cout << "New balance: $" << (balance - amt) << endl;
    } else {
        cout << "Authentication failed.\n";
    }
}


void list() {
    ifstream file("data.txt");
    int accNum, pin;
    float balance;

    cout << "\n----- All Accounts -----\n\n";
    while (file >> accNum >> pin >> balance) {
        cout << "Account Number: " << accNum << "\n";
        cout << "Balance: $" << balance << "\n";
        cout << "--------------------------\n\n";
    }

    file.close();
}

int main() {
    int choice;
    while(1) {
        cout << "----------Banking System----------\n" << endl;
        cout << "1. Create Account\n2. View Account\n3. Deposit\n4. Withdraw\n5. List All Accounts\n6. Exit\nChoice: ";
        cin >> choice;
        if(choice == 6) {
            cout << "Exiting..." << endl;
            break;
        }
        switch (choice){
            case(1):
                create();
                break;
            case(2):
                view();
                break;
            case(3):
                deposit();
                break;
            case(4):
                withdraw();
                break;
            case(5):
                list();
                break;
            default:
                cout << "Enter a valid choice" << endl;
        }
    }
    return 0;
}
