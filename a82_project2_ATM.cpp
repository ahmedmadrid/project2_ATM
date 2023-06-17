#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


enum enClientChoice{ enQUICK_WITHDRAW = 1, enREGULAR_WITHDRAW = 2, enDEPOSIT = 3, enCHECK_BALANCE = 4, enLOGOUT = 5 };

enum enWithdrawalChoice {en20 = 1, en50 = 2, en100 = 3, en200 = 4, en400 = 5, en600 = 6, en800 = 7, en1000 =8, enExit = 9};

enum enUserAnswer {enVALID_PROCEDURE = 1, enINVALID_PROCEDURE = 3, enSAID_NO = 2};

struct stClient {
    string accountNumber{};
    string pinCode{};
    string name{};
    string mobilePhone{};
    double balance{};
};

// global variables are seen during the run time by all functions!
const string CLIENTS_FILE_NAME = "clients.txt";
const string CLIENTS_FILE_DELIMETER = "/$/";
stClient whologgedIn{};


/*========================================== title screen =====================================================*/

string repeatString(const string& STR, int times) {
    string temp{};
    while (times--)
        temp += STR;
    return temp;
}

void drawTitleOnScreen(const string& TITLE) {
    cout << repeatString("=", 60) << "\n\n";
    cout << "\t" << TITLE << "\n\n";
    cout << repeatString("=", 60) << endl;
}

string readString(const string& MESSAGE) {
    string temp{};
    cout << MESSAGE << " -> ";
    cin >> temp;
    return temp;
}

/*========================================= read client data base ==========================================*/

// get tokens
vector<string> getTokensFromString(string str, const string& DELIEMETER) {
    // declare a container
    vector<string> vTokens{};
    // locate delimeter position
    int delimPos{};
    // store tokens
    string token{};

    while ( (delimPos = str.find(DELIEMETER)) != std::string::npos ) {
        token = str.substr(0, delimPos);
        // ignore pre-element and adjacent delimeters
        if (token != "")
            vTokens.push_back(token);
        // remove old token
        str.erase(0, delimPos + DELIEMETER.length());
    }
    // ignore post-element delimeter
    if (str != "")
        vTokens.push_back(str);
    return vTokens;
}

// get client date
stClient convertLineToClient(const vector<string>& vRECORD) {
    stClient client{};

    client.accountNumber = vRECORD.at(0);
    client.pinCode = vRECORD.at(1);
    client.name = vRECORD.at(2);
    client.mobilePhone = vRECORD.at(3);
    client.balance = stod(vRECORD.at(4));

    return client;
}


vector<stClient> readClientsFromFile(const string& FILE_NAME, const string& DELIMETER) {
    // declare file
    fstream file;
    // declare container
    vector<stClient> vClients{};
    // open file in reading mode
    file.open(FILE_NAME, ios::in);
    string line{};
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line != "") {
                // convert line to stClient
                vClients.push_back(convertLineToClient(getTokensFromString(line, DELIMETER)));
            }
        }
    }
    else
        file.close();
    return vClients;
}

/*=================================== search for clients ==============================================*/

int searchForClient(const vector<stClient>& vCLIENTS,const string& ACCOUNT_NUMBER, const string& PIN_CODE) {
    for (int i = 0; i < vCLIENTS.size(); i++) {
        if (vCLIENTS.at(i).accountNumber == ACCOUNT_NUMBER && vCLIENTS.at(i).pinCode == PIN_CODE)
            return i;
    }
    return -1;
}

/*==================================== login screen =======================================================*/

bool logIn(vector<stClient>& vClients) {
    drawTitleOnScreen("LogIn Screen");
    string accountNumber{}, pinCode{};
    int idx{};
    do {
        accountNumber = readString("Enter account number");
        pinCode = readString("Enter pin code"); // validate to be consist of 4 digits, and not letters!
        vClients = readClientsFromFile(CLIENTS_FILE_NAME, CLIENTS_FILE_DELIMETER);
        idx = searchForClient(vClients, accountNumber, pinCode);
        if (idx != -1)
            whologgedIn = vClients.at(idx);
        else {
            system("cls");
            drawTitleOnScreen("logIn Screen");
            cout << "\nInvalid account number/pin code!\n\n";
        }
            
    } while (idx == -1);
    return idx != -1;
}

/*=============================== welocme screen =============================================================*/

void welcomeScreen(stClient client) {
    drawTitleOnScreen(client.name + ", Welcome to ATM Menu Screen!");
    cout << "\t\t[1] Quick withdraw\n";
    cout << "\t\t[2] Regular withdraw\n";
    cout << "\t\t[3] Deposit\n";
    cout << "\t\t[4] Check Balance\n";
    cout << "\t\t[5] Logout.\n";
    cout << repeatString("=", 60) << endl;
}

int readInRange(const string& MESSAGE, const int& FROM, const int& TO) {
    int choice{};
    bool isString{}, isOutOfRange{};
    do {
        cout << MESSAGE << " -> ";
        cin >> choice;
        // avoid string
        if (isString = cin.fail()) {
            // clear the buffer
            cin.clear();
            // ignore error
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nString is Invalid input!!\n";
        }
        // out of range
        else if (isOutOfRange = (choice < FROM || choice > TO))
            cout << "\nNumber you entered is out of range!\n";

    } while (isString || isOutOfRange);
    return choice;
}

/*================================================= balance screen ================================================*/

double getClientBalance(stClient client) {
    return client.balance;
}

void showClientBalance(const double& BALANCE) {
    cout << "your balance is $" << BALANCE << endl;
}

void goBackScreen(const string& MESSAGE) {
    cout << "\n\n" << MESSAGE;
    system("pause >0");
    system("cls");
}

/*=================================================== quick withdraw =============================================*/
void withdrawMenu() {
    cout << right << setw(10) << "[1]" << " 20" << right << setw(11) << "[2]" << " 50\n";
    cout << right << setw(10) << "[3]" << " 100" << right << setw(10) << "[4]" << " 500\n";
    cout << right << setw(10) << "[5]" << " 400" << right << setw(10) << "[6]" << " 600\n";
    cout << right << setw(10) << "[7]" << " 800" << right << setw(10) << "[8]" << " 1000\n";
    cout << right << setw(10) << "[9]" << " Exist" << endl;
}

double readPositiveNumber(const string& MESSAGE) {
    double amount{1};
    bool isString{};
    do {
        cout << MESSAGE << " -> ";
        cin >> amount;

        if (isString = cin.fail()) {
            // clear the buffer
            cin.clear();
            // ignore error
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\nString is Invalid input!!\n";
        }
        else if (amount < 0)
            cout << "\nAmount can not be 0 or less!!\n";

    } while (isString || amount < 0);
    return amount;
}

void makeADeposit(stClient& client, double amount) {
    client.balance += amount;
}

bool canWithdraw(stClient& client, double amount) {
    if (client.balance >= amount) {
        makeADeposit(client, amount * -1);
        return true;
    }
    else
        cout << "\nWithdrawal amount can't exceed your balance. Make another choice!\n";
    return false;
}

bool isUpperChar(const char& C) {
    return (C >= 65 && C <= 90);
}

char toLowerChar(char c) {
    if (isUpperChar(c))
        return c + 32;
    return c;
}

bool saidYes(const string& MESSGAE) {
    char ans;
    cout << MESSGAE << " -> ";
    cin >> ans;
    return (toLowerChar(ans) == 'y') ? true : false;
}

double getAmountOfQuickWithdraw(enWithdrawalChoice choice) {
    
    double amount{};
    switch (choice) {
    case enWithdrawalChoice::en20:
        amount = 20;
        break;
    case enWithdrawalChoice::en50:
        amount = 50;
        break;
    case enWithdrawalChoice::en100:
        amount = 100;
        break;
    case enWithdrawalChoice::en200:
        amount = 200;
        break;
    case enWithdrawalChoice::en400:
        amount = 400;
        break;
    case enWithdrawalChoice::en600:
        amount = 600;
        break;
    case enWithdrawalChoice::en800:
        amount = 800;
        break;
    case enWithdrawalChoice::en1000:
        amount = 1000;
        break;
    }
    return amount;
}

// this function will check on amount of quick or normal withdraw
enUserAnswer checkUserAnswer(stClient& client, double amount, bool isDepoist = false) {
    if (saidYes("Are you sure you want to perform this transaction?(y-n)?")) {

        if(!isDepoist)
            // said yes to valid or invalid procedure!!
            return canWithdraw(client, amount) ? enUserAnswer::enVALID_PROCEDURE : enINVALID_PROCEDURE;
        else {
            makeADeposit(client, amount);
            // two options: valid or no
            return enUserAnswer::enVALID_PROCEDURE;
        } 
    }

    return enUserAnswer::enSAID_NO;
}

// created this to avoid repeating the code 3 times in functions quickWithdraw, regularWithdraw, and deposit
void switchOnUserAnswer(stClient& client, enUserAnswer& userAns) {
    switch (userAns) {
    case enUserAnswer::enVALID_PROCEDURE:
        cout << "\n\nDone successfully. Your Balance now becomes $" << client.balance << endl;
        break;
    case enUserAnswer::enINVALID_PROCEDURE:
        goBackScreen("Press any key to continue...");
        break;
    }
}

void quickWithdraw(stClient& client) {
    enUserAnswer userAns{};
    double amount{};
    do {
        drawTitleOnScreen("Quick Withdraw SCreen");
        withdrawMenu();
        showClientBalance(getClientBalance(client));
        amount = getAmountOfQuickWithdraw((enWithdrawalChoice)readInRange("Choose which amount to withdraw? [1-8]", 1, 8));
        userAns = checkUserAnswer(client, amount);
        switchOnUserAnswer(client,userAns);  
    } while (userAns == enUserAnswer::enINVALID_PROCEDURE);
}

/*================================================== normal withdraw =============================================*/

bool isMultipleOfN(int number, int multiple) {
    return (number % multiple) == 0;
}

void regularWithdraw(stClient& client) {
    drawTitleOnScreen("Regular withdraw screen");
    // multiples of 5
    int amount{};
    bool isMultiple{};
    enUserAnswer userAns{};
    do {
        amount = readPositiveNumber("Enter a number to be a multiple of 5");
        isMultiple = isMultipleOfN(amount, 5);
        
        if (isMultiple = isMultipleOfN(amount, 5))
            switchOnUserAnswer(client, userAns = checkUserAnswer(client, amount));

    } while (!isMultiple || userAns == enUserAnswer::enINVALID_PROCEDURE);
}

/*=================================================== Deposit ======================================================*/

void deposit(stClient& client) {
    drawTitleOnScreen("Deposit Screen");
    double amount{};
    enUserAnswer userAns{};
    amount = readPositiveNumber("Enter a deposit amount");
    switchOnUserAnswer(client, userAns = checkUserAnswer(client, amount, true));
}

/*===================================================== update client data ========================================*/

string convertstClientToLine(const stClient& CLIENT, const string& DELIMETER) {
    string record{};
    record += CLIENT.accountNumber + DELIMETER;
    record += CLIENT.pinCode + DELIMETER;
    record += CLIENT.name + DELIMETER;
    record += CLIENT.mobilePhone + DELIMETER;
    return record += to_string(CLIENT.balance);
}

void writestClientsInFile(const vector<stClient>& vCLIENTS, const string& FILE_NAME, const string& DELIMETER) {
    // define file
    fstream file;
    // open file in writing mode
    file.open(FILE_NAME, ios::out);
    // avoid closed files
    if (file.is_open() && !vCLIENTS.empty()) {
        for (const stClient& CLIENT : vCLIENTS)
            file << convertstClientToLine(CLIENT,DELIMETER) << endl;
    }
    file.close();
}

/*============================================ run ===============================================================*/

void updateClientData(vector<stClient>& vClients, stClient& whoLoggedIn) {
    int idx = searchForClient(vClients, whologgedIn.accountNumber, whologgedIn.pinCode);
   if(idx != -1)
       vClients.at(idx) = whologgedIn;
}

void run(vector<stClient>& vClients) {
    bool logOut{};
    enClientChoice choice;
    while (!logOut) {
        welcomeScreen(whologgedIn);

        choice = (enClientChoice) readInRange("Choose what do you want to do? [1-5]", 1, 5);
        system("cls");

        switch (choice) {
        case enClientChoice::enQUICK_WITHDRAW:
            quickWithdraw(whologgedIn);
            goBackScreen("Press any key to go back to main menu....");
            break;
        case enClientChoice::enREGULAR_WITHDRAW:
            regularWithdraw(whologgedIn);
            goBackScreen("Press any key to go back to main menu....");
            break;
        case enClientChoice::enDEPOSIT:
            deposit(whologgedIn);
            goBackScreen("Press any key to go back to main menu....");
            break;
        case enClientChoice::enCHECK_BALANCE:
            showClientBalance(getClientBalance(whologgedIn));
            goBackScreen("Press any key to go back to main menu....");
            break;
        case enClientChoice::enLOGOUT:
            logOut = true;
            break;
        }
    }
    // rewrite client info
    updateClientData(vClients, whologgedIn);
    writestClientsInFile(vClients, CLIENTS_FILE_NAME, CLIENTS_FILE_DELIMETER);
}

void start() {
    vector<stClient> vClients{};
    do {
        system("cls");
        if (logIn(vClients)) {
            system("cls");
            run(vClients);
        }
       
    } while (saidYes("Do you want to log in in a different user?(y-n)?"));
}

int main()
{
    start();

}
