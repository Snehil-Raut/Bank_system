#include<iostream>
#include<string>
#include<fstream>
#include<map>
using namespace std;

class Account{
private:
    string fname;
    string lname;
    int accountNumber;
    double balance;
    static int nextaccountNumber;

public:
    Account(){};
    Account(string fn, string ln, double bal);

    string getfname() { return fname; }
    string getlname() { return lname; }
    double getBalance() { return balance; }
    int getAccountNumber() { return accountNumber; }
    void deposit(double amount);
    void withdraw(double amount);

    static void nextaccountnumber();

    friend ofstream & operator<<(ofstream &ofs, Account &acc);
    friend ifstream & operator>>(ifstream &ifs, Account &acc);

};
int Account::nextaccountNumber = 1; // Initialize static member

class Bank{
private:
    map<int, Account> accounts; // Map to store accounts with account number as key
public:
    Bank();
    Account OpenAccount(string fname, string lname, double balance);
    Account BalanceEnquiry(int accountNumber);
    Account Deposit(int accountNumber, double amount);
    Account Withdraw(int accountNumber, double amount);
    void DisplayAllAccounts();
    Account closeAccount(int accountNumber);
    void SaveToFile();
};

ofstream & operator<<(ofstream &ofs, Account &acc){
    ofs << acc.accountNumber << endl;
    ofs << acc.getfname() << endl;
    ofs << acc.getlname() << endl;
    ofs << acc.getBalance() << endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs, Account &acc){
    ifs >>  acc.accountNumber;
    ifs.ignore();
    ifs >>  acc.fname;
    ifs >>  acc.lname;
    ifs >>  acc.balance;
    return ifs;
}

void Account::nextaccountnumber(){
    ifstream ifs("account.txt");
    int maxAccountNumber = 0;

    if(ifs.is_open()){
        Account temp;
        while (ifs >> temp) {
            if(temp.accountNumber > maxAccountNumber){
                maxAccountNumber = temp.accountNumber;
            }
        }
        ifs.close();
    }
    nextaccountNumber = maxAccountNumber + 1; // Set next account number to max + 1
}

void Account::deposit(double amount){
    if(amount > 0){
        balance += amount;
        cout << "Deposit successful" << endl;
    }
    else{
        cerr << "Deposit amount must be positive" << endl;
    }
}

void Account::withdraw(double amount){
    if(amount > 0 &&  amount <= balance){
        balance -= amount;
        cout << "Withdrawal successful" << endl;
    }
    else if(amount > balance){
        cerr << "Insufficient balance" << endl;
    }
    else{
        cerr << "Withdrawal amount must be positive" << endl;
    }
}

Account::Account(string fn, string ln, double bal){
    fname = fn;
    lname = ln;
    balance = bal;
    accountNumber = nextaccountNumber++;
}

int main(){

    Account::nextaccountnumber(); // Initialize next account number
    Bank bank;

    cout << "Select an option:" << endl;
    cout << "1. Open Account" << endl;
    cout << "2. Balance Enquiry" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Withdraw" << endl;
    cout << "5. Display All Accounts" << endl;
    cout << "6. Close Account" << endl;
    cout << "7. Exit" << endl;

    while (true)
    {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice > 7 || choice < 1 || cin.fail()){
            cerr << "Invalid choice. Please enter a number between 1 and 7." << endl;
            cin.clear();
            cin.ignore(10, '\n'); // Ignore till 10 characters or newline
            continue;
        }
        
        switch(choice) {
            case 1:
            {
                    string fname, lname;
                    double balance;
                    cout << "Enter First Name: ";
                    cin >> fname;
                    cout << "Enter Last Name: ";
                    cin >> lname;
                    cout << "Enter Initial Balance: ";
                    cin >> balance; 
                    Account newAccount = bank.OpenAccount(fname, lname, balance);
                    cout << "Account opened successfully with Account Number: " << newAccount.getAccountNumber() << endl;
                    cout << endl;
                }
                break;
            
            case 2:
                {
                    int accountNumber;
                    cout << "Enter Account Number for Balance Enquiry: ";
                    cin >> accountNumber;
                    Account Balenq = bank.BalanceEnquiry(accountNumber);
                    if (Balenq.getAccountNumber() != 0){
                        cout << "Account Number: " << Balenq.getAccountNumber() << endl;
                        cout << "First Name: " << Balenq.getfname() << endl;
                        cout << "Last Name: " << Balenq.getlname() << endl;
                        cout << "Balance: " << Balenq.getBalance() << endl;
                    }
                    else{
                        cerr << "Account not found." << endl;
                    }
                }
                break;
            
            case 3:
                {
                    int accountNumber;
                    cout << "Enter Account Number to Deposit money: ";
                    cin >> accountNumber;

                    double depositAmount;
                    cout << "Enter Amount to Deposit: ";
                    cin >> depositAmount;

                    Account updatedAccount = bank.Deposit(accountNumber, depositAmount);
                    if(updatedAccount.getAccountNumber() != 0){
                        cout << "Deposit successful. New Balance: " << updatedAccount.getBalance() << endl;
                        cout << endl;
                    }
                    else{
                        cerr << "Account not found." << endl;
                    }
                }
                break;
            
            case 4:
                {
                    int accountNumber;
                    cout << "Enter Account Number to Withdraw money: ";
                    cin >> accountNumber;

                    double withdrawAmount;
                    cout << "Enter Amount to Withdraw: ";
                    cin >> withdrawAmount;

                    Account amountwithdraw = bank.Withdraw(accountNumber, withdrawAmount);
                    if(amountwithdraw.getAccountNumber() != 0){
                        cout << "Withdraw successful. New Balance: " << amountwithdraw.getBalance() << endl;
                        cout << endl;
                    }
                    else{
                        cerr << "Account not found." << endl;
                    }
                }
                break;

            case 5:
                {
                    bank.DisplayAllAccounts();
                }
                break;

            case 6:
                {
                    int accountNumber;
                    cout << "Enter Account Number to Close Account: ";
                    cin >> accountNumber;
                    Account closedAccount = bank.closeAccount(accountNumber);
                    if(closedAccount.getAccountNumber() != 0){
                        cout << "Account closed successfully." << endl;
                        cout << "Account Number: " << closedAccount.getAccountNumber() << endl;
                        cout << "First Name: " << closedAccount.getfname() << endl;
                        cout << "Last Name: " << closedAccount.getlname() << endl;
                        cout << endl;
                    }
                }
                break;
            case 7:
                cout << "Exiting the program." << endl;
                return 0; // Exit the program
        }
    }
}

Bank::Bank(){
    ifstream ifs("account.txt");
    if(ifs.is_open()){
        Account acc;
        while(ifs >> acc){
            accounts[acc.getAccountNumber()] = acc; // Load accounts from file into the map
        }
        ifs.close();
    }
    else{
        cerr << "Error opening account file." << endl;
    }
}

// return type is Account
Account Bank::OpenAccount(string fname, string lname, double balance){
    Account openacc(fname, lname, balance);
    accounts[openacc.getAccountNumber()] = openacc; // Store account in map]
    ofstream ofs("account.txt", ios::app);
    if (!ofs) {
        cerr << "Error opening file for writing." << endl;
        return Account(); // Return an empty account on error
    }
    ofs << openacc; // Write the new account to the file
    return openacc; // Return the newly created account
}

Account Bank::BalanceEnquiry(int accountNumber){
    if(accounts.find(accountNumber) != accounts.end()){
        return accounts[accountNumber]; // Return the account if found
    } else {
        cerr << "Account not found." << endl;
        return Account(); // Return an empty account if not found
    }
}

void Bank::SaveToFile(){
    ofstream ofs("account.txt", ios::out);
    for(auto save : accounts){
        ofs << save.second;
    }
}

Account Bank::Deposit(int accountNumber, double amount){
    if(accounts.find(accountNumber) != accounts.end()){
        accounts[accountNumber].deposit(amount); // Deposit amount into the account
        SaveToFile();
        return accounts[accountNumber]; // Return the updated account
    }
    else{
        cerr << "Account not found." << endl;
        return Account(); // Return an empty account if not found
    }    
}

Account Bank::Withdraw(int accountNumber, double amount){
    if(accounts.find(accountNumber) != accounts.end()){
        accounts[accountNumber].withdraw(amount); // Deposit amount into the account
        SaveToFile();
        return accounts[accountNumber]; // Return the updated account
    }
    else{
        cerr << "Account not found." << endl;
        return Account(); // Return an empty account if not found
    }    
}

void Bank::DisplayAllAccounts(){
    for(auto it = accounts.begin(); it != accounts.end(); ++it){
        Account display = it->second;
        cout << "Account Number: " << display.getAccountNumber() << endl;
        cout << "First Name: " << display.getfname() << endl;
        cout << "Last Name: " << display.getlname() << endl;
        cout << "Balance: " << display.getBalance() << endl;
        cout << "------------------------" << endl; // Separator for multiple entries
    }
}

Account Bank::closeAccount(int accountNumber){
    if(accounts.find(accountNumber) != accounts.end()){
        Account closeacc = accounts[accountNumber];
        accounts.erase(accountNumber); // Remove the account from the map
        SaveToFile();
        return closeacc; // Return the closed account
    }
    else{
        cerr << "Account not found." << endl;
        return Account(); // Return an empty account if not found
    }
}