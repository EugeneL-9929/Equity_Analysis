#ifndef STOCK_H
#define STOCK_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
using namespace std;

namespace AV
{
    // the data capture functions of this namespace are built with the service of alpha vantage.
    struct Stock
    {
    public:
        Stock(const string name, const string function = "TIME_SERIES_INTRADAY", const string outputsize = "full", const string interval = "5min");

        Stock &operator=(const Stock &other);

        Stock(const Stock &other);

        Stock &operator=(Stock &&other);

        Stock(Stock &&other);

        string getMarketData();

        virtual nlohmann::json formatMarketData();

        string getName() const;

        void setApiKey(const string &new_api_key);

        virtual ~Stock();

    protected:
        void resetQueryUrl(string newQueryUrl);
        const string api_url = "https://www.alphavantage.co/query?";
        string api_key = "&apikey=K7G3VVX3F6EAFJL5";

    private:
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *userp);

        string name;
        string symbol{"symbol="};
        string function{"&function="};
        string outputsize{"&outputsize="};
        string interval{"&interval="};
        string query_url;
        string intervalGetPurpose;
    };

}

#endif