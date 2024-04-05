#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class wallet
{
    public:
        wallet();
        /** Insert currency into our wallet*/
        void insertCurrency(std::string type, double amount);
        /** Remove currency from our wallet*/
        bool removeCurrency(std::string type, double amount);
        
        /** Check if or wallet contains enoughf currency or not*/
        bool containsCurrency(std::string type, double amount);

        /** Check if the wallet can fulfill an order*/
        bool canFulfillOrder(OrderBookEntry order);

        /** Update the content of the wallet assumes that the order was done by the user of the wallet*/
        void processSale(OrderBookEntry& sale);
        /** Generate a sting representation of the wallet.*/
        std::string toString();


    private: 

       std::map<std::string, double> currencies; 

}; 