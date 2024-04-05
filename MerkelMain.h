#pragma once

#include <vector>
#include "CSVReader.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "wallet.h"

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);
        
        std::string currentTime;

        OrderBook orderbook{"20200317.csv"};

        wallet Wallet;
        

}; 
