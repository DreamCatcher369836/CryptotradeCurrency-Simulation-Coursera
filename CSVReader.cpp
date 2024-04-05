#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            try {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch(const std::exception& e)
            {
                //std::cout << "CSVReader::readCSV bad data"  << std::endl;
            }
        }// end of while
    }    

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries"  << std::endl;
    return entries; 
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    unsigned int start = 0;
    unsigned int end = 0;

    while (start < csvLine.length())
    {
        // Find the position of the separator
        end = csvLine.find(separator, start);

        // Extract the token from the line
        std::string token = csvLine.substr(start, end - start);

        // Add the token to the vector
        tokens.push_back(token);

        // Move to the next position after the separator
        start = (end == std::string::npos) ? csvLine.length() : end + 1;
    }

    return tokens;
}


OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        //std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }
    catch(const std::exception& e){
        std::cout << "Bad float! " << tokens[3]<< std::endl;
        std::cout << "Bad float! " << tokens[4]<< std::endl; 
        throw;        
    }

    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0],
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe; 
}

OrderBookEntry CSVReader::stringsToOBE(std::string product, std::string priceString, std::string amountString, std::string timestamp, OrderBookType orderType)
{
    double price, amount;

    try {
         price = std::stod(priceString);
         amount = std::stod(amountString);
    }
    catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl; 
        throw;        
    }

    OrderBookEntry obe{price, 
                        amount, 
                        timestamp,
                        product, 
                        orderType};
    return obe;
}
 
