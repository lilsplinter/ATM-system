#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;
const string ClientsFileName = "Clients.txt";
void ShowScreen(const string& screenname) {
    cout << "========================================================================================================================" << endl;
    cout << "\n\t\t\t\t\t\t ";
    cout << screenname;
    cout << " \t\t\t\t\t\n\n";
    cout << "========================================================================================================================" << endl;
}
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void PauseScreen() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    double Balance = 0;
};
struct stCurrentClient
{
    stClient Client;
    bool IsLoggedIn = false;
};
enum enMainMenuOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw,
    eDeposit,
    eCheckBalance,
    eLogout
}; 
enum enQuickWithdrawOptions
{
    e20$ = 1,
    e50$,
    e100$,
    eBackToMainMenu
};
void MainMenu(stCurrentClient& CurrentClient);
int ReadNumber(int min, int max, const string& Message) {
    int Number;
    while (true) {
        cout << Message;
        cin >> Number;
        if (!cin.fail() && Number >= min && Number <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return Number;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between " << min << " and " << max << ": " << endl;
    }
}
string ReadString(const string& Message) {
    string Input;
    cout << Message;
    getline(cin >> ws, Input);
    return Input;
}
bool ReadChoice(const string& Message) {
    char Choice;
    while (true) {
        cout << Message;
        cin >> Choice;
        if (Choice == 'Y' || Choice == 'y') {
            return true;
        }
        else if (Choice == 'N' || Choice == 'n') {
            return false;
        }
        else {
            cout << "Invalid input. Please enter Y or N: " << endl;
        }
    }
}
vector <string> GetLinesFromFile(const string& FileName)
{
    vector <string> vClients;
	fstream MyFile(FileName, ios::in);
    string Line;
    if (!MyFile.is_open()) {
		cerr << "Error opening file: " << FileName << endl;
		return vClients;
    }
    while (getline(MyFile,Line)) {
        if (!Line.empty()) {
            vClients.push_back(Line);
        }
	}
	MyFile.close();
	return vClients;
}
vector <string> SplitString(const string& Line, char Delimiter = '#'){
	vector <string> vClient;
    size_t pos = 0;
	string temp = Line;
	pos = temp.find(Delimiter);
    while (temp.find(Delimiter) != temp.npos) {
		vClient.push_back(temp.substr(0, pos));
		temp.erase(0, pos + 1);
		pos = temp.find(Delimiter);
    }
    if (!temp.empty()) {
        vClient.push_back(temp);
	}
	return vClient;
}
stClient CollectVectorDataIntoStructOfClients(const vector <string>& vClient)
{
    stClient Client = { "", "", "", 0.0 };
    if (vClient.size() == 4)
    {
        Client.AccountNumber = vClient[0];
        Client.PinCode = vClient[1];
        Client.Name = vClient[2];
        Client.Balance = stod(vClient[3]);
	}
    return Client;
}
vector <stClient> GetAllClients(const vector<string>& vLines, char Delimiter = '#')
{
    vector <stClient> vClients;
    for (const string& Line : vLines) {
        if (!Line.empty()) {
            vClients.push_back(CollectVectorDataIntoStructOfClients(SplitString(Line)));
        }
    }
    return vClients;
}
string ConvertClientDataToLine(const stClient& Client, char Delimiter = '#') {
    return Client.AccountNumber + Delimiter + Client.PinCode + Delimiter + Client.Name + Delimiter + to_string(Client.Balance);
}
void SaveClientsToFile (const vector<stClient>& vClients, const string& FileName) {
    fstream MyFile(FileName, ios::out);
    if (!MyFile.is_open()) {
        cerr << "Error opening file: " << FileName << endl;
        return;
    }
    for (const stClient& client : vClients) {
		MyFile << ConvertClientDataToLine(client) << endl;
    }
	MyFile.close();
}
void ShowQuickWithdrawOptions() {
    cout << "1. $20" << endl;
    cout << "2. $50" << endl;
    cout << "3. $100" << endl;
    cout << "4. Back to Main Menu" << endl;
}
bool IsAmountGreaterThanBalance(double Amount, double Balance) {
    if (Amount > Balance) {
        return true;
    }
    return false;
}
void WithdrawAmount(double Amount, stCurrentClient& client) {
        if (!IsAmountGreaterThanBalance(Amount, client.Client.Balance)) {
            client.Client.Balance -= Amount;
            cout << "You have withdrawn $" << Amount << ". Your new balance is: $" << fixed << setprecision(2) << client.Client.Balance << endl;
        }
        else {
            cout << "Insufficient balance for this withdrawal." << endl;
        }
}
void QuickWithdraw(stCurrentClient& client) {
		bool WithdrawAgain = true;
        do {
            enQuickWithdrawOptions Option = (enQuickWithdrawOptions)ReadNumber(1, 4, "Please choose an option: ");
            switch (Option)
            {
            case e20$:
                WithdrawAmount(20, client);
                break;
            case e50$:
                WithdrawAmount(50, client);
                break;
            case e100$:
                WithdrawAmount(100, client);
                break;
            case eBackToMainMenu:
				WithdrawAgain = false;
                break;
			}
            if (Option != eBackToMainMenu && WithdrawAgain) {
				WithdrawAgain = ReadChoice("Do you want to perform another quick withdraw? (Y/N): ");
            }
        } while (WithdrawAgain);
}
void NormalWithdraw(stCurrentClient& client) {
	bool WithdrawMore = true;
    do {
        double Amount = (double)ReadNumber(1, 10000, "Please Enter the amount you want to withdraw (max is 10000): ");
        WithdrawAmount(Amount, client);
		WithdrawMore = ReadChoice("Do you want to perform another normal withdraw? (Y/N): ");
	} while (WithdrawMore);
}
void Deposit(stCurrentClient& client) {
	bool DepositMore = true;
    do {
        double Amount = (double)ReadNumber(1, 10000, "Please Enter the amount you want to deposit (max is 10000):");
        client.Client.Balance += Amount;
        cout << "You have deposited $" << Amount << ". Your new balance is: $" << fixed << setprecision(2) << client.Client.Balance << endl;
		DepositMore = ReadChoice("Do you want to perform another deposit? (Y/N): ");
    } while (DepositMore);
}
void CheckBalance(stCurrentClient& client) {
    cout << "Your current balance is: $" << fixed << setprecision(2) << client.Client.Balance << endl;
}
void Logout(stCurrentClient& client) {
    client.IsLoggedIn = false;
    cout << "You have been logged out. Thank you for using our ATM." << endl;
}
void ShowMainMenu() {
    ShowScreen("Main Menu");
    cout << "1. Quick Withdraw" << endl;
    cout << "2. Normal Withdraw" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Check Balance" << endl;
    cout << "5. Logout" << endl;
}
void MainMenu(stCurrentClient& client) {
	enMainMenuOptions Option = static_cast<enMainMenuOptions>(ReadNumber(1, 5, "Please choose an option: "));
    switch (Option)
    {
    case eQuickWithdraw:
		ShowScreen("Quick Withdraw");
		ShowQuickWithdrawOptions();
		QuickWithdraw(client);
		PauseScreen();
        break;
    case eNormalWithdraw:
		ShowScreen("Normal Withdraw");
        NormalWithdraw(client);
		PauseScreen();
        break;
    case eDeposit:
		ShowScreen("Deposit");
		Deposit(client);
		PauseScreen();
        break;
    case eCheckBalance:
		ShowScreen("Check Balance");
		CheckBalance(client);
		PauseScreen();
        break;
    case eLogout:
		ShowScreen("Logout");
		Logout(client);
		PauseScreen();
        break;
    default:
        break;
    }
}
stCurrentClient Login(vector <stClient>& vClients) {
	stCurrentClient CurrentClient;
    ClearScreen();
    ShowScreen("Login");
	int counter = 0;
    do {
		counter++;
        string AccountNumber = ReadString("Please enter your account number: ");
        string PinCode = ReadString("Please enter your pin code: ");
        for (const stClient& client : vClients) {
            if (client.AccountNumber == AccountNumber && client.PinCode == PinCode) {
				CurrentClient.Client = client;
                CurrentClient.IsLoggedIn = true;
				return CurrentClient;
            }
        }
        cout << "Invalid Account Number Or PIN code, Please Try Again." << endl;
        if (counter == 3) {
			cout << "Too many failed login attempts. Exiting the program." << endl;
			exit(0);
        }
    } while (true);
}
void ShowATMScreen() {
    ClearScreen();
    cout << "==============================" << endl;
    cout << "     Welcome to the ATM       " << endl;
    cout << "==============================" << endl;

}
void ATM(vector <stClient>& vClients) {
	ShowATMScreen();
	bool LoginAgain = true;
    do {
        stCurrentClient CurrentClient = Login(vClients);
        do {
            if (CurrentClient.IsLoggedIn) {
                ShowMainMenu();
                MainMenu(CurrentClient);
            }
            for (stClient& client : vClients) {
                if (client.AccountNumber == CurrentClient.Client.AccountNumber) {
                    client.Balance = CurrentClient.Client.Balance;
                    break;
                }
            }
            if (!CurrentClient.IsLoggedIn) {
                SaveClientsToFile(vClients, ClientsFileName);
                break;
            }
		} while (CurrentClient.IsLoggedIn);
		LoginAgain = ReadChoice("Do you want to login with another account? (Y/N): ");
	} while (LoginAgain);
}
int main()
{
	vector <stClient> vClients = GetAllClients(GetLinesFromFile(ClientsFileName));
    if (vClients.empty()) {
        cerr << "Error: No clients found in " << ClientsFileName << endl;
        return 1;
    }
    ATM(vClients);
	return 0;   
}
