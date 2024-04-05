#include <string>
#include <map>
#include "wallet.h"
#include "CSVReader.h"
#include <iostream>

wallet::wallet()
{

}

void wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    if ( amount < 0 )
    {
        std::cout << "Wallet::insertCurrency: Cannot add a negative amount to the wallet!" << std::endl;
        throw std::exception{};                 // If we try to insert a negative value, throw exception
    }
    if (currencies.count(type) == 0)   
    {   
        std::cout << "Wallet::insertCurrency: Currency not found, adding " << type << " to wallet" << std::endl;
        balance = 0;                            // If we dont have this currencie, and it and initialize it to 0
    }
    else{
        balance = currencies[type];             // If we have the currencie then move the value to local var. 
    }

    balance += amount;                          // Increase the local var by amount
    currencies[type] = balance;                 // Update the wallet with the new value.
    std::cout << "Wallet::insertCurrency: Adding " << amount << " " << type << " to the wallet!" << std::endl; 
}
 /** Remove currency from our wallet*/
bool wallet::removeCurrency(std::string type, double amount)
{
    if ( amount < 0 )                           // If amount is less than 0, return false. 
    {
        std::cout << "Wallet::removeCurrency: Cannot remove a negative amount to the wallet!" << std::endl;
        throw std::exception{};                 // If we try to remove a negative value, throw exception  
    }
    if (currencies.count(type) == 0)            // If we dont have this currencie, return false
    {
        std::cout << "Wallet::removeCurrency: Currency not found." << std::endl;
        return false; 
    }
    else{
        if (containsCurrency(type, amount)) 
        {
            currencies[type] -= amount;         // If we do have the currencie and enough cash the we deduct the amount. 
            std::cout << "Wallet::removeCurrency: " << type << " " << amount << " deducted." << std::endl;
            return true; 
        }
        else{
            std::cout << "Wallet::removeCurrency: You dont have enough money to deduct " << type << " " << amount << std::endl;
            return false;                       // If we have the currencie but not enough cash then we also return false..
        }
    }
}


bool wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0)            // If we dont have the currencie, return false .
    {
        std::cout << "Wallet::containsCurrency: You dont have enough money." << std::endl;
        return false;
    }
    else{
        return currencies[type] >= amount;      // If we have it and enough of it, return true 
    }
}

/** Check if the wallet can fulfill an order*/
bool wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');                   // Split the product in substrings i.e. "BTC/ETH" to "BTC", "ETH"
    // Ask 
    if ( order.orderType == OrderBookType::ask)                               
    {
        double amount = order.amount;
        std::string currency = currs[0];                                                        // If ask then look at the first currency. 
        std::cout << "Wallet::canFulfullOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);                                              // If we have enough then it is ok. 
    }

    // Bid 
    if ( order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;                                             // If bid, we need to check the price aswell. 
        std::string currency = currs[1];                                                        // Also we are looking at the second substring, that is what we are buying 
        std::cout << "Wallet::canFulfullOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);                                              // Check if it is ok. 
    }

    return false;
}

/** Update the content of the wallet assumes that the order was done by the user of the wallet*/
void wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');                   // Split the product in substrings i.e. "BTC/ETH" to "BTC", "ETH"
    // Ask 
    if ( sale.orderType == OrderBookType::ask)                               
    {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];   
        double incommingAmount = sale.amount * sale.price;                                   
        std::string incommingCurrency = currs[1];   
         
         
        currencies[incommingCurrency] += incommingAmount;
        currencies[outgoingCurrency]  -= outgoingAmount;                                                                                                                                 // If we have enough then it is ok. 
    }

    // Bid 
    if ( sale.orderType == OrderBookType::bid)
    {
        double incommingAmount = sale.amount;
        std::string incommingCurrency = currs[0];   
        double outgoingAmount = sale.amount * sale.price;                                     
        std::string outgoingCurrency = currs[1];   
         
         
        currencies[incommingCurrency] += incommingAmount;
        currencies[outgoingCurrency]  -= outgoingAmount;                                            
    }
}

std::string wallet::toString()
{
    std::string s; 
    std::cout << "\nWallet::toString: Your wallet contains" << std::endl;
    for (std::pair<std::string, double> pair : currencies)          // Print the type and amount per currency. 
    {
        std::string currency = pair.first;
        double amount = pair.second; 
        s += currency + " : " + std::to_string(amount) + "\n";      // escape n to make it a newline character. 

    }
    return s; 
}
