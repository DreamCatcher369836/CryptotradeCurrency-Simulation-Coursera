#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <limits>
#include <ios>


MerkelMain::MerkelMain()
{


}

void MerkelMain::init()
{
    int input;
    currentTime = orderbook.getEarliestTime();
    Wallet.insertCurrency("BTC", 10);
    while(true)
    {
       
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p: orderbook.getKnownProducts())
    {
        std::cout << "Products " << p << std::endl;
        std::vector <OrderBookEntry> entries = orderbook.getOrders(OrderBookType::ask, p ,currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Low ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Mean ask: " << orderbook.getAveragePrice(entries) << std::endl; // Call the function using the orderbook object

        
    }
   /*std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    unsigned int bids = 0;
    unsigned int asks = 0;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == OrderBookType::ask)
        {
            asks ++;
        }
        if (e.orderType == OrderBookType::bid)
        {
            bids ++;
        }  
    }    
    std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;
    */ 
}

void MerkelMain::enterAsk()
{
    
    std::cout << "Place an ask - enter the amount: product, price, amount e.g ETH/BTC, 10, 50" << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // Clear the input buffer before we can get the input value. 
    std::getline(std::cin, input); 

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkleMain::enterAsk: Bad input, need exactly 3 inputs" << std::endl;

    }
    else
    {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);

            obe.username = "simuser"; 

            if(Wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks ok, adding order" << std::endl;
                orderbook.insertOrder(obe);
            }
            else
            {
                std::cout << "Not enough funds for processing order" << std::endl;   
            }
            
        } catch( const std::exception& e)
        {
            std::cout << "MerkleMain::enterAsk: Bad input" << std::endl;
        }   
    }

    std::cout << "You typed: " << input << std::endl;

}


void MerkelMain::enterBid() 
{
    std::cout << "Place an bid - enter the amount: product, price, amount e.g ETH/BTC, 10, 50" << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // Clear the input buffer before we can get the input value. 
    std::getline(std::cin, input); 

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkleMain::enterBid: Bad input, need exactly 3 inputs" << std::endl;

    }
    else
    {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid); 

            obe.username = "simuser"; 

            if(Wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks ok, adding order" << std::endl;
                orderbook.insertOrder(obe);
            }
            else
            {
                std::cout << "Not enough funds for processing order" << std::endl;   
            }
            
        } catch( const std::exception& e)
        {
            std::cout << "MerkleMain::enterBid: Bad input" << std::endl;
        }   
    }

    std::cout << "You typed: " << input << std::endl;

}




void MerkelMain::printWallet()
{
    std::cout << "Displaying your lack of money in your wallet...." << std::endl;
    std::cout << Wallet.toString() << std::endl; 
    std::cout << "Returning to Main menu \n" << std::endl;
    
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    currentTime = orderbook.getNextTime(currentTime);
    std::vector<OrderBookEntry> sales =  orderbook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl; 
    for (OrderBookEntry& sale : sales)
    {
        std::cout << "Sale price: " << sale.amount << " amount: " << sale.amount << std::endl;
        if (sale.username == "simuser")
        {
            Wallet.processSale(sale);
        }
    }

    // Keep track of the last timefram to calculate the price change. 
    std::cout << "Returning to Main menu \n" << std::endl;

}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    }

    catch(const std::exception& e)
    {
        
    }
    //std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }       
}
 
