#include "OrderBook.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>


OrderBook::OrderBook(std::string filename)
{
    // Constructor implementation
    orders = CSVReader::readCSV(filename);


}
        
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::map <std::string, bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    //Now flatten the map to a string of vectors
    std::vector<std::string> products;
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }
    
    // Method implementation
    return products;
}
      
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e: orders)
    {
        if (e.orderType == type && e.product == product && e.timestamp == timestamp  )
        {
            orders_sub.push_back(e);
        }
    }
    // Method implementation
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector <OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max) max = e.price;
        
    }

    return max;
}

double OrderBook::getLowPrice(std::vector <OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min) min = e.price;
        
    }

    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for(OrderBookEntry& e: orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }

        if (next_timestamp == "")
        {
            next_timestamp = orders[0].timestamp;
        }
    }

    return next_timestamp;
}
//Compute the average price by passing in a vector of entries.
double OrderBook::getAveragePrice(std::vector<OrderBookEntry>& entries)
{
    double sum = 0;
    for (OrderBookEntry& e : entries)
    {
        sum = e.price; // the sum of all entries 
    }

    return (sum / entries.size()); //Return the sum by dividing it by the number of entries in the entries vector (entries.size()).
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // Get the asks and bids. 
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp); 
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp); 

    // completed sales
    std::vector<OrderBookEntry> sales;

    // sort all asks and bids on price before starting
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    // Main matching engine
    for (OrderBookEntry& ask : asks)
    {
        for (OrderBookEntry& bid : bids)
        {
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::sale, };
                

                // start comparing the bids and asks

                // If the bid amount and the ask amount is the same. 
                // in this case the the bid and ask will clear out. 
                if ( bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }

                // If the bid amount is greater than the ask. 
                // in this case the ask will be consumed and the bid will still exist to some extent. The bid will go to the next ask and try match with that.
                // When breaking, we will exit this bid loop, and start with next ask in the outer loop, we will then hit this same bid again for processing.  
                if ( bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                    
                }
                // If the bid amount is less than the ask amount. 
                // In this case the bid will be consumed and the ask will still remain. Instead of breaking we will do a continue with the next bid.
                if ( bid.amount < ask.amount && bid.amount > 0)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;

                    bid.amount = 0;

                    continue;


                }
            }
        }
    }

    return sales;

}