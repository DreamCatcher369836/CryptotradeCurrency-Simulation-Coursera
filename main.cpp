#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "wallet.h" // Include the header file for the wallet class

int main()
{
    MerkelMain app{};
    app.init();

    wallet Wallet{}; // Declare a wallet object
    Wallet.insertCurrency("BTC", 1.5);
    std::cout << "Wallet should contain 1.5 BTC now" << std::endl;
    std::cout << Wallet.toString() << std::endl;
    bool result = Wallet.containsCurrency("BTC", 1.5);
    std::cout << "Result should be true " << result << std::endl;
    result = Wallet.removeCurrency("BTC", 2.0);
    std::cout << "Result should be false " << result << std::endl;
    result = Wallet.removeCurrency("BTC", 1.0);
    std::cout << "Result should be true " << result << std::endl;

    return 0; // Ensure to return an exit status
}
