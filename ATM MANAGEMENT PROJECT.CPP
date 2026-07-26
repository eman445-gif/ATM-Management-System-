#include <iostream>
#include <fstream>
using namespace std;

const int MAX = 20;
int accountId[MAX];
int pin[MAX];
int balance[MAX];
int transactions[MAX][5];
int transCount[MAX];
int totalAccounts = 0;

void loadData() {
    ifstream file("atmdata.txt");
    if (!file) return;

    file >> totalAccounts;
    for (int i = 0; i < totalAccounts; i++) {
        file >> accountId[i] >> pin[i] >> balance[i];
        file >> transCount[i];
        for (int j = 0; j < transCount[i]; j++) {
            file >> transactions[i][j];
        }
    }
    file.close();
}

void saveData() {
    ofstream file("atmdata.txt");
    file << totalAccounts << endl;
    for (int i = 0; i < totalAccounts; i++) {
        file << accountId[i] << " " << pin[i] << " " << balance[i] << endl;
        file << transCount[i] << endl;
        for (int j = 0; j < transCount[i]; j++) {
            file << transactions[i][j] << " ";
        }
        file << endl;
    }
    file.close();
}

int findAccount(int id) {
    for (int i = 0; i < totalAccounts; i++) {
        if (accountId[i] == id) return i;
    }
    return -1;
}

void addTransaction(int index, int amount) {
    if (transCount[index] < 5) {
        transactions[index][transCount[index]] = amount;
        transCount[index]++;
    } else {
        for (int i = 0; i < 4; i++) {
            transactions[index][i] = transactions[index][i + 1];
        }
        transactions[index][4] = amount;
    }
}

void createAccount() {
    if (totalAccounts >= MAX) {
        cout << "Account limit reached" << endl;
        return;
    }

    cout << "Enter new account id: ";
    cin >> accountId[totalAccounts];

    if (findAccount(accountId[totalAccounts]) != -1) {
        cout << "Account already exists" << endl;
        return;
    }

    cout << "Set PIN: ";
    cin >> pin[totalAccounts];

    cout << "Enter initial balance: ";
    cin >> balance[totalAccounts];

    transCount[totalAccounts] = 0;
    totalAccounts++;
    saveData();
    cout << "Account created successfully" << endl;
}

int login() {
    int id;
    int p;

    cout << "Enter account id: ";
    cin >> id;

    int index = findAccount(id);
    if (index == -1) {
        cout << "Account not found" << endl;
        return -1;
    }

    cout << "Enter PIN: ";
    cin >> p;

    if (p != pin[index]) {
        cout << "Invalid PIN" << endl;
        return -1;
    }

    return index;
}

void changePin(int index) {
    int oldPin;
    int newPin;

    cout << "Enter old PIN: ";
    cin >> oldPin;

    if (oldPin != pin[index]) {
        cout << "Invalid PIN" << endl;
        return;
    }

    cout << "Enter new PIN: ";
    cin >> newPin;
    pin[index] = newPin;
    saveData();
    cout << "PIN changed successfully" << endl;
}

void miniStatement(int index) {
    cout << "Last transactions" << endl;
    for (int i = 0; i < transCount[index]; i++) {
        cout << transactions[index][i] << endl;
    }
}

void atmMenu(int index) {
    int choice;
    int amount;
    int targetId;
    int targetIndex;
    int dailyLimit = 20000;

    do {
        cout << endl;
        cout << "ATM MENU" << endl;
        cout << "1 Balance Inquiry" << endl;
        cout << "2 Cash Deposit" << endl;
        cout << "3 Cash Withdrawal" << endl;
        cout << "4 Money Transfer" << endl;
        cout << "5 Mini Statement" << endl;
        cout << "6 Change PIN" << endl;
        cout << "7 Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Current balance is " << balance[index] << endl;
        }
        else if (choice == 2) {
            cout << "Enter deposit amount: ";
            cin >> amount;
            balance[index] += amount;
            addTransaction(index, amount);
            saveData();
            cout << "Deposit successful" << endl;
        }
        else if (choice == 3) {
            cout << "Enter withdrawal amount: ";
            cin >> amount;
            if (amount > balance[index] || amount > dailyLimit) {
                cout << "Transaction not allowed" << endl;
            } else {
                balance[index] -= amount;
                addTransaction(index, -amount);
                saveData();
                cout << "Please collect your cash" << endl;
            }
        }
        else if (choice == 4) {
            cout << "Enter receiver account id: ";
            cin >> targetId;
            targetIndex = findAccount(targetId);

            if (targetIndex == -1) {
                cout << "Receiver account not found" << endl;
            } else {
                cout << "Enter transfer amount: ";
                cin >> amount;

                if (amount > balance[index]) {
                    cout << "Insufficient balance" << endl;
                } else {
                    balance[index] -= amount;
                    balance[targetIndex] += amount;
                    addTransaction(index, -amount);
                    addTransaction(targetIndex, amount);
                    saveData();
                    cout << "Transfer successful" << endl;
                }
            }
        }
        else if (choice == 5) {
            miniStatement(index);
        }
        else if (choice == 6) {
            changePin(index);
        }
        else if (choice == 7) {
            cout << "Logged out successfully" << endl;
        }
        else {
            cout << "Invalid option" << endl;
        }

    } while (choice != 7);
}

int main() {
    loadData();

    int choice;
    int userIndex;

    cout << "Insert your ATM card" << endl;

    do {
        cout << endl;
        cout << "1 Create Account" << endl;
        cout << "2 Login" << endl;
        cout << "3 Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            createAccount();
        }
        else if (choice == 2) {
            userIndex = login();
            if (userIndex != -1) {
                atmMenu(userIndex);
            }
        }
        else if (choice == 3) {
            saveData();
            cout << "Thank you for using ATM" << endl;
        }
        else {
            cout << "Invalid option" << endl;
        }

    } while (choice != 3);

    return 0;
}

