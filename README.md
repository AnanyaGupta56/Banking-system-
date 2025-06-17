 # Banking Management System

A comprehensive C++ banking management system that provides secure account management, transaction tracking, and interest calculation features.

## Features

### Account Management
- Create new accounts (Savings or Checking)
- Close existing accounts
- View account details
- Password protection for all accounts
- Minimum balance requirement ($500)

### Transaction Operations
- Deposit funds
- Withdraw funds
- Balance inquiry
- Transaction history tracking
- Interest calculation

### Account Types
- Savings Account (5% interest rate)
- Checking Account (2% interest rate)

### Security Features
- Password protection for individual accounts
- Admin password for system-wide operations
- Input validation
- Secure file storage

### Administrative Features
- View all accounts (admin only)
- Calculate interest for all accounts (admin only)
- Transaction history viewing

## Prerequisites

- C++ compiler (supporting C++11 or later)
- Standard C++ libraries
- File system access for data persistence

## Compilation

To compile the project, use the following command:

```bash
g++ -std=c++11 banking.cpp -o banking
```

## Usage

1. Run the compiled program:
```bash
./banking
```

2. Follow the menu options:
   - 1: Open an Account
   - 2: Balance Enquiry
   - 3: Deposit
   - 4: Withdrawal
   - 5: Close an Account
   - 6: Show All Accounts (Admin)
   - 7: Show Transaction History
   - 8: Calculate Interest (Admin)
   - 9: Quit

### Default Admin Credentials
- Password: admin123

## File Structure

- `banking.cpp`: Main program file
- `Bank.data`: Data storage file (created automatically)
- `README.md`: This documentation file

## Data Storage

The system automatically creates and maintains a `Bank.data` file that stores:
- Account information
- Transaction history
- Account balances
- Account types

## Security Notes

1. The admin password is hardcoded for demonstration purposes. In a production environment, this should be stored securely.
2. All account passwords are stored in plain text. In a production environment, these should be hashed.
3. The minimum balance requirement is set to $500.

## Error Handling

The system handles various error conditions:
- Insufficient funds
- Invalid input
- Invalid passwords
- File I/O errors

## Future Improvements

Potential enhancements for future versions:
1. Implement proper password hashing
2. Add support for multiple currencies
3. Implement account transfer functionality
4. Add support for joint accounts
5. Implement proper database storage
6. Add GUI interface
7. Implement proper logging system
8. Add support for different types of interest calculations

## Contributing

Feel free to contribute to this project by:
1. Forking the repository
2. Creating a feature branch
3. Committing your changes
4. Pushing to the branch
5. Creating a Pull Request


## Author
Ananya Gupta

## Acknowledgments

- Standard C++ libraries
- C++ community for best practices and guidelines
