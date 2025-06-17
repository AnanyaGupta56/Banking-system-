#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

#define MIN_BALANCE 500
#define SAVINGS_INTEREST_RATE 0.05
#define CHECKING_INTEREST_RATE 0.02

class InsufficientFunds {};
class InvalidInput {};
class InvalidPassword {};

struct Transaction {
    string type;
    float amount;
    float balance;
    time_t timestamp;
    
    Transaction(string t, float a, float b) : type(t), amount(a), balance(b) {
        timestamp = time(nullptr);
    }
};

enum AccountType {
    SAVINGS,
    CHECKING
};

class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    string password;
    float balance;
    AccountType type;
    vector<Transaction> transactions;
    static long nextAccountNumber;
    
public:
    Account() {}
    Account(string fname, string lname, string pwd, float balance, AccountType t);
    long getAccountNumber() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }
    AccountType getType() { return type; }
    string getTypeString() { return type == SAVINGS ? "Savings" : "Checking"; }
    
    void deposit(float amount);
    void withdraw(float amount);
    void addTransaction(string type, float amount);
    void showTransactionHistory();
    void calculateInterest();
    bool verifyPassword(string pwd) { return password == pwd; }
    
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    
    friend ofstream& operator<<(ofstream& ofs, Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
    friend ostream& operator<<(ostream& os, Account& acc);
};

long Account::nextAccountNumber = 0;

class Bank {
private:
    map<long, Account> accounts;
    string adminPassword;
    
public:
    Bank();
    Account openAccount(string fname, string lname, string pwd, float balance, AccountType type);
    Account balanceEnquiry(long accountNumber, string pwd);
    Account deposit(long accountNumber, string pwd, float amount);
    Account withdraw(long accountNumber, string pwd, float amount);
    void closeAccount(long accountNumber, string pwd);
    void showAllAccounts(string adminPwd);
    void calculateInterestForAll();
    void showTransactionHistory(long accountNumber, string pwd);
    bool verifyAdmin(string pwd) { return pwd == adminPassword; }
    ~Bank();
};

// Helper function to get valid input
template<typename T>
T getValidInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please try again.\n";
    }
}

Account::Account(string fname, string lname, string pwd, float balance, AccountType t) {
    nextAccountNumber++;
    accountNumber = nextAccountNumber;
    firstName = fname;
    lastName = lname;
    password = pwd;
    this->balance = balance;
    type = t;
    addTransaction("Initial Deposit", balance);
}

void Account::deposit(float amount) {
    if (amount <= 0) throw InvalidInput();
    balance += amount;
    addTransaction("Deposit", amount);
}

void Account::withdraw(float amount) {
    if (amount <= 0) throw InvalidInput();
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
    addTransaction("Withdrawal", amount);
}

void Account::addTransaction(string type, float amount) {
    transactions.push_back(Transaction(type, amount, balance));
}

void Account::showTransactionHistory() {
    cout << "\nTransaction History for Account " << accountNumber << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& t : transactions) {
        cout << ctime(&t.timestamp);
        cout << "Type: " << t.type << endl;
        cout << "Amount: $" << fixed << setprecision(2) << t.amount << endl;
        cout << "Balance: $" << t.balance << endl;
        cout << "----------------------------------------" << endl;
    }
}

void Account::calculateInterest() {
    float interestRate = (type == SAVINGS) ? SAVINGS_INTEREST_RATE : CHECKING_INTEREST_RATE;
    float interest = balance * interestRate;
    balance += interest;
    addTransaction("Interest", interest);
}

void Account::setLastAccountNumber(long accountNumber) {
    nextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return nextAccountNumber;
}

ofstream& operator<<(ofstream& ofs, Account& acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.password << endl;
    ofs << acc.balance << endl;
    ofs << static_cast<int>(acc.type) << endl;
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc) {
    int type;
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.password;
    ifs >> acc.balance;
    ifs >> type;
    acc.type = static_cast<AccountType>(type);
    return ifs;
}

ostream& operator<<(ostream& os, Account& acc) {
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccountNumber() << endl;
    os << "Account Type: " << acc.getTypeString() << endl;
    os << "Balance: $" << fixed << setprecision(2) << acc.getBalance() << endl;
    return os;
}

Bank::Bank() {
    adminPassword = "admin123"; // In a real system, this would be securely stored
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile) {
        return;
    }
    while (!infile.eof()) {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccountNumber(), account));
    }
    Account::setLastAccountNumber(account.getAccountNumber());
    infile.close();
}

Account Bank::openAccount(string fname, string lname, string pwd, float balance, AccountType type) {
    ofstream outfile;
    Account account(fname, lname, pwd, balance, type);
    accounts.insert(pair<long, Account>(account.getAccountNumber(), account));
    outfile.open("Bank.data", ios::trunc);
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceEnquiry(long accountNumber, string pwd) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) throw InvalidInput();
    if (!itr->second.verifyPassword(pwd)) throw InvalidPassword();
    return itr->second;
}

Account Bank::deposit(long accountNumber, string pwd, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) throw InvalidInput();
    if (!itr->second.verifyPassword(pwd)) throw InvalidPassword();
    itr->second.deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accountNumber, string pwd, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) throw InvalidInput();
    if (!itr->second.verifyPassword(pwd)) throw InvalidPassword();
    itr->second.withdraw(amount);
    return itr->second;
}

void Bank::closeAccount(long accountNumber, string pwd) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) throw InvalidInput();
    if (!itr->second.verifyPassword(pwd)) throw InvalidPassword();
    cout << "Account Deleted: " << itr->second;
    accounts.erase(accountNumber);
}

void Bank::showAllAccounts(string adminPwd) {
    if (!verifyAdmin(adminPwd)) throw InvalidPassword();
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++) {
        cout << "Account " << itr->first << endl << itr->second << endl;
    }
}

void Bank::calculateInterestForAll() {
    for (auto& pair : accounts) {
        pair.second.calculateInterest();
    }
}

void Bank::showTransactionHistory(long accountNumber, string pwd) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) throw InvalidInput();
    if (!itr->second.verifyPassword(pwd)) throw InvalidPassword();
    itr->second.showTransactionHistory();
}

Bank::~Bank() {
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
}

int main() {
    Bank bank;
    Account acc;
    int option;
    string fname, lname, pwd;
    long accountNumber;
    float balance;
    float amount;
    AccountType type;

    cout << "Enhanced Banking System" << endl;
    do {
        cout << "\n\tSelect one option below ";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Show Transaction History";
        cout << "\n\t8 Calculate Interest";
        cout << "\n\t9 Quit";
        cout << "\nEnter your choice: ";
        
        try {
            cin >> option;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (option) {
                case 1: {
                    cout << "Enter First Name: ";
                    getline(cin, fname);
                    cout << "Enter Last Name: ";
                    getline(cin, lname);
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    cout << "Enter Account Type (0 for Savings, 1 for Checking): ";
                    int typeChoice;
                    cin >> typeChoice;
                    type = static_cast<AccountType>(typeChoice);
                    balance = getValidInput<float>("Enter initial Balance: ");
                    
                    acc = bank.openAccount(fname, lname, pwd, balance, type);
                    cout << endl << "Congratulations, Account Created" << endl;
                    cout << acc;
                    break;
                }
                case 2: {
                    accountNumber = getValidInput<long>("Enter Account Number: ");
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    acc = bank.balanceEnquiry(accountNumber, pwd);
                    cout << endl << "Your Account Details" << endl;
                    cout << acc;
                    break;
                }
                case 3: {
                    accountNumber = getValidInput<long>("Enter Account Number: ");
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    amount = getValidInput<float>("Enter Amount: ");
                    acc = bank.deposit(accountNumber, pwd, amount);
                    cout << endl << "Amount Deposited" << endl;
                    cout << acc;
                    break;
                }
                case 4: {
                    accountNumber = getValidInput<long>("Enter Account Number: ");
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    amount = getValidInput<float>("Enter Amount: ");
                    acc = bank.withdraw(accountNumber, pwd, amount);
                    cout << endl << "Amount Withdrawn" << endl;
                    cout << acc;
                    break;
                }
                case 5: {
                    accountNumber = getValidInput<long>("Enter Account Number: ");
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    bank.closeAccount(accountNumber, pwd);
                    cout << endl << "Account Closed" << endl;
                    break;
                }
                case 6: {
                    cout << "Enter Admin Password: ";
                    getline(cin, pwd);
                    bank.showAllAccounts(pwd);
                    break;
                }
                case 7: {
                    accountNumber = getValidInput<long>("Enter Account Number: ");
                    cout << "Enter Password: ";
                    getline(cin, pwd);
                    bank.showTransactionHistory(accountNumber, pwd);
                    break;
                }
                case 8: {
                    cout << "Enter Admin Password: ";
                    getline(cin, pwd);
                    if (bank.verifyAdmin(pwd)) {
                        bank.calculateInterestForAll();
                        cout << "Interest calculated for all accounts" << endl;
                    } else {
                        cout << "Invalid admin password" << endl;
                    }
                    break;
                }
                case 9:
                    break;
                default:
                    cout << "\nEnter correct choice" << endl;
            }
        } catch (InsufficientFunds) {
            cout << "Insufficient funds" << endl;
        } catch (InvalidInput) {
            cout << "Invalid input" << endl;
        } catch (InvalidPassword) {
            cout << "Invalid password" << endl;
        } catch (...) {
            cout << "An error occurred" << endl;
        }
    } while (option != 9);

    return 0;
} 
