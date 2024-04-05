#pragma once 
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>

class OrderBook
{
    public:
        //Construct, reading a csv data file
        OrderBook(std::string filename);
        //Return vector of all known products
        std::vector <std::string> getKnownProducts();
        //Return vector of orders according to the sent filters
        std::vector <OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);
        std::string getEarliestTime();

        void insertOrder(OrderBookEntry& order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        std::string getNextTime(std::string timestamp);
        static double getHighPrice(std::vector <OrderBookEntry>& orders);
        static double getLowPrice(std::vector <OrderBookEntry>& orders);
        static double getAveragePrice(std::vector<OrderBookEntry>& entries);
;
    
    private:
        std::vector <OrderBookEntry> orders;




};

