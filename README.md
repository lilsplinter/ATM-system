# ATM System (C++)

*Deutsch: Konsolen-Geldautomat in C++ mit Login (Kontonummer und PIN), Einzahlung, Abhebung, Schnellabhebung und Kontostand. Die Kundendaten werden in einer Textdatei gespeichert.*

A console ATM simulation written in C++. A client logs in with an account number and a PIN, then can withdraw money, deposit money and check the balance. All balances are stored in a text file, so they are still there the next time the program starts.

I built this after my [Bank System](https://github.com/lilsplinter/Bank-System) project, to practice the other side of a bank: one client working with their own account.

## Features

- **Login** with account number and PIN. After 3 wrong attempts the program closes.
- **Quick Withdraw:** choose $20, $50 or $100, then repeat if you want.
- **Normal Withdraw:** any whole amount from $1 to $10,000.
- **Deposit:** any whole amount from $1 to $10,000.
- **Check Balance.**
- **Insufficient balance check:** a withdrawal bigger than the balance is refused.
- **Input validation:** letters or numbers outside the menu range are rejected and the user is asked again.
- **Several accounts:** after logout you can log in with another account.
- **Automatic saving:** the balance is written to `Clients.txt` after every action.

## Menu

```
Main Menu
1. Quick Withdraw
2. Normal Withdraw
3. Deposit
4. Check Balance
5. Logout
```

## Example session

```
Please enter your account number: A1594
Please enter your pin code: 1234

Please choose an option: 3
Please enter the amount you want to deposit (max is 10000): 100
You have deposited $100.00. Your new balance is: $5100.00

Please choose an option: 1
Please choose an option: 2
You have withdrawn $50.00. Your new balance is: $5050.00

Please choose an option: 2
Please enter the amount you want to withdraw (max is 10000): 9000
Insufficient balance for this withdrawal.

Please choose an option: 5
You have been logged out. Thank you for using our ATM.
```

## Data file

Clients are stored in `Clients.txt`, one client per line, separated by `#`:

```
AccountNumber#PinCode#Name#Balance
A1594#1234#Ahmed#5050.00
```

The file contains fake test data only.

## How to run

1. Open `ATMsystem.sln` in Visual Studio (Windows).
2. Press **F5**.

`Clients.txt` must stay in the project folder next to `ATMsystem.cpp`, because the program reads it from there.

## How the code is organized

- `struct stClient` holds one client, and `struct stCurrentClient` holds the logged-in client.
- `enum` types name the menu choices (`enMainMenuOptions`, `enQuickWithdrawOptions`).
- Small functions with one job each: reading input (`ReadNumber`, `ReadString`, `ReadChoice`), file handling (`GetLinesFromFile`, `SplitString`, `SaveClientsToFile`) and the ATM actions (`Deposit`, `WithdrawAmount`, `CheckBalance`, ...).
- Clients are loaded into a `vector<stClient>` when the program starts and saved back after every action.

## Skills practiced

C++, structs, enums, vectors, functions, text file reading and writing, string splitting, input validation, program flow with menus.

## Possible improvements

- Store PINs as hashes instead of plain text
- Lock an account after too many failed logins, not only close the program
- Use whole cents instead of `double` for money
- Split the code into several files and classes
- Add a transaction history
- Add unit tests
