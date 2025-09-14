#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;

struct Database
{
public:
    Database(const string &dbPath) : dbPath{dbPath}
    {
        int openMark = sqlite3_open(this->dbPath.c_str(), &database);
        if (openMark != SQLITE_OK)
            cout << "failed to open " << this->dbPath << "!" << endl;

        sqlite3_stmt *stmt;
        const char *command = "SELECT COUNT(*) FROM sqlite_master WHERE type='table';";
        int newMark = sqlite3_prepare_v2(database, command, -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            if (sqlite3_column_int(stmt, 0) == 0)
                cout << "there is no table!" << endl;
        sqlite3_finalize(stmt);
        initializeTimeTable();
        initializeStockTable();
        initializeStockNameTable();
        initializeFxTable();
        initializeFxNameTable();
    }

    void initializeTimeTable()
    {
        string command =
            "CREATE TABLE IF NOT EXISTS TIME("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "DATA_TIME DATETIME UNIQUE);"
            "CREATE INDEX IF NOT EXISTS IDX_DATA_TIME ON TIME(DATA_TIME);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish time table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void initializeStockTable()
    {
        string command =
            "CREATE TABLE IF NOT EXISTS STOCK("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "OPEN FLOAT,"
            "CLOSE FLOAT,"
            "HIGH FLOAT,"
            "LOW FLOAT,"
            "VOLUME INTEGER,"
            "TIME_ID INTEGER,"
            "STOCK_ID INTEGER,"
            "FOREIGN KEY (TIME_ID) REFERENCES TIME (ID) ON DELETE CASCADE ON UPDATE CASCADE,"
            "FOREIGN KEY (STOCK_ID) REFERENCES STOCKNAME (ID) ON DELETE CASCADE ON UPDATE CASCADE);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish stock table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void initializeStockNameTable()
    {
        string command =
            "CREATE TABLE IF NOT EXISTS STOCKNAME("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "NAME VARCHAR(20) UNIQUE);"
            "CREATE INDEX IF NOT EXISTS IDX_NAME ON STOCKNAME(NAME);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish stock name table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void initializeFxTable()
    {
        string command =
            "CREATE TABLE IF NOT EXISTS FX("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "OPEN FLOAT,"
            "CLOSE FLOAT,"
            "HIGH FLOAT,"
            "LOW FLOAT,"
            "TIME_ID INTEGER,"
            "FX_ID INTEGER,"
            "FOREIGN KEY (TIME_ID) REFERENCES TIME (ID) ON DELETE CASCADE ON UPDATE CASCADE,"
            "FOREIGN KEY (FX_ID) REFERENCES FXNAME (ID) ON DELETE CASCADE ON UPDATE CASCADE);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish fx table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void initializeFxNameTable()
    {
        string command =
            "CREATE TABLE IF NOT EXISTS FXNAME("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
            "NAME VARCHAR(20) UNIQUE);"
            "CREATE INDEX IF NOT EXISTS IDX_FXNAME ON FXNAME(NAME);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish fx name table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void addStockTable(const nlohmann::json &jsonData, string stockName)
    {

        string command_name = "INSERT OR IGNORE INTO STOCKNAME (NAME) VALUES (?);";
        sqlite3_stmt *stmt_name;
        string command_time = "INSERT OR IGNORE INTO TIME (DATA_TIME) VALUES (?);";
        sqlite3_stmt *stmt_time;
        string command_stock = "INSERT INTO STOCK (OPEN, CLOSE, HIGH, LOW, VOLUME, TIME_ID, STOCK_ID) VALUES (?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt *stmt_stock;
        sqlite3_prepare_v2(this->database, command_name.c_str(), -1, &stmt_name, nullptr);
        sqlite3_prepare_v2(this->database, command_time.c_str(), -1, &stmt_time, nullptr);
        sqlite3_prepare_v2(this->database, command_stock.c_str(), -1, &stmt_stock, nullptr);

        if (getIdByField("STOCKNAME", "NAME", stockName) == -1)
        {
            sqlite3_bind_text(stmt_name, 1, stockName.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt_name) != SQLITE_DONE)
            {
                cout << "insert stock name " << stockName << " failed!" << endl;
                cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
            }
            sqlite3_reset(stmt_name);
        }
        int stockId = this->getIdByField("STOCKNAME", "NAME", stockName);

        for (const auto &data : jsonData.items())
        {
            string dataTime = data.key();
            auto stockData = data.value();
            if (getIdByField("TIME", "DATA_TIME", dataTime) == -1)
            {
                sqlite3_bind_text(stmt_time, 1, dataTime.c_str(), -1, SQLITE_TRANSIENT);
                if (sqlite3_step(stmt_time) != SQLITE_DONE)
                {
                    cout << "insert time " << dataTime << " failed!" << endl;
                    cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
                }
                sqlite3_reset(stmt_time);
            }
            int newId = this->getIdByField("TIME", "DATA_TIME", dataTime);
            vector<string> pairFields{"TIME_ID", "STOCK_ID"};
            vector<string> pairName{to_string(newId), to_string(stockId)};
            if (getIdsByFields("STOCK", pairFields, pairFields, pairName).empty())
            {
                sqlite3_bind_double(stmt_stock, 1, stod(stockData["open"].get<string>()));
                sqlite3_bind_double(stmt_stock, 2, stod(stockData["close"].get<string>()));
                sqlite3_bind_double(stmt_stock, 3, stod(stockData["high"].get<string>()));
                sqlite3_bind_double(stmt_stock, 4, stod(stockData["low"].get<string>()));
                sqlite3_bind_int(stmt_stock, 5, stoi(stockData["volume"].get<string>()));
                sqlite3_bind_int(stmt_stock, 6, newId);
                sqlite3_bind_int(stmt_stock, 7, stockId);
                if (sqlite3_step(stmt_stock) != SQLITE_DONE)
                {
                    cout << "insert data at time " << dataTime << " failed!" << endl;
                }
                sqlite3_reset(stmt_stock);
            }
            else
                cout << "wow! repeat!!!" << endl;
        }
        sqlite3_finalize(stmt_name);
        sqlite3_finalize(stmt_time);
        sqlite3_finalize(stmt_stock);
    }

    void addFxTable(const nlohmann::json &jsonData, string fxName)
    {

        string command_name = "INSERT OR IGNORE INTO FXNAME (NAME) VALUES (?);";
        sqlite3_stmt *stmt_name;
        string command_time = "INSERT OR IGNORE INTO TIME (DATA_TIME) VALUES (?);";
        sqlite3_stmt *stmt_time;
        string command_fx = "INSERT INTO FX (OPEN, CLOSE, HIGH, LOW, TIME_ID, FX_ID) VALUES (?, ?, ?, ?, ?, ?);";
        sqlite3_stmt *stmt_fx;
        sqlite3_prepare_v2(this->database, command_name.c_str(), -1, &stmt_name, nullptr);
        sqlite3_prepare_v2(this->database, command_time.c_str(), -1, &stmt_time, nullptr);
        sqlite3_prepare_v2(this->database, command_fx.c_str(), -1, &stmt_fx, nullptr);

        if (getIdByField("FXNAME", "NAME", fxName) == -1)
        {
            sqlite3_bind_text(stmt_name, 1, fxName.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt_name) != SQLITE_DONE)
            {
                cout << "insert fx name " << fxName << " failed!" << endl;
                cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
            }
            sqlite3_reset(stmt_name);
        }
        int fxId = this->getIdByField("FXNAME", "NAME", fxName);

        for (const auto &data : jsonData.items())
        {
            string dataTime = data.key();
            auto fxData = data.value();
            if (getIdByField("TIME", "DATA_TIME", dataTime) == -1)
            {
                sqlite3_bind_text(stmt_time, 1, dataTime.c_str(), -1, SQLITE_TRANSIENT);
                if (sqlite3_step(stmt_time) != SQLITE_DONE)
                {
                    cout << "insert time " << dataTime << " failed!" << endl;
                    cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
                }
                sqlite3_reset(stmt_time);
            }
            int newId = this->getIdByField("TIME", "DATA_TIME", dataTime);
            vector<string> pairFields{"TIME_ID", "FX_ID"};
            vector<string> pairName{to_string(newId), to_string(fxId)};
            if (getIdsByFields("FX", pairFields, pairFields, pairName).empty())
            {
                sqlite3_bind_double(stmt_fx, 1, stod(fxData["open"].get<string>()));
                sqlite3_bind_double(stmt_fx, 2, stod(fxData["close"].get<string>()));
                sqlite3_bind_double(stmt_fx, 3, stod(fxData["high"].get<string>()));
                sqlite3_bind_double(stmt_fx, 4, stod(fxData["low"].get<string>()));
                sqlite3_bind_int(stmt_fx, 5, newId);
                sqlite3_bind_int(stmt_fx, 6, fxId);
                if (sqlite3_step(stmt_fx) != SQLITE_DONE)
                {
                    cout << "insert data at time " << dataTime << " failed!" << endl;
                }
                sqlite3_reset(stmt_fx);
            }
            else
                cout << "wow! repeat!!!" << endl;
        }
        sqlite3_finalize(stmt_name);
        sqlite3_finalize(stmt_time);
        sqlite3_finalize(stmt_fx);
    }

    void selfDefinedSQLCommand(const string &command)
    {
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        int executeMark = sqlite3_step(stmt);
        if (executeMark == SQLITE_DONE)
            cout << "table alterred!" << endl;
        else if (executeMark == SQLITE_ROW)
        {
        }
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    ~Database()
    {
        sqlite3_close(this->database);
    }

private:
    int latestId(string tableName)
    {
        int returnValue{};
        string command = "SELECT ID FROM " + tableName + " ORDER BY ID DESC LIMIT 1";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            returnValue = sqlite3_column_int(stmt, 0);
            cout << "lastest id is " << returnValue << endl;
        }
        else
        {
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        }
        sqlite3_finalize(stmt);
        return returnValue;
    }

    int getIdByField(string tableName, string field, string name)
    {
        int returnValue{-1};
        string command = "SELECT ID FROM " + tableName + " WHERE " + field + "=?";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            returnValue = sqlite3_column_int(stmt, 0);
            cout << name << " id is " << returnValue << endl;
        }
        else
        {
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        }
        sqlite3_finalize(stmt);
        return returnValue;
    }

    vector<int> getIdsByFields(string tableName, vector<string> observeFields, vector<string> fields, vector<string> names)
    {
        vector<int> returnValues{};
        string command = "SELECT ";
        for (size_t i = 0; i < observeFields.size(); i++)
        {
            command += observeFields[i];
            if (i != observeFields.size() - 1)
                command += ", ";
        }
        command = command + " FROM " + tableName + " WHERE ";
        for (size_t i = 0; i < fields.size(); i++)
        {
            command = command + fields[i] + "=" + names[i];
            if (i != fields.size() - 1)
                command += " AND ";
        }
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        // cout << command <<endl;
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            for (size_t i = 0; i < observeFields.size(); i++)
                returnValues.push_back(sqlite3_column_int(stmt, i));
        }
        else
        {
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        }
        sqlite3_finalize(stmt);
        return returnValues;
    }

    sqlite3 *database;
    const string dbPath;
};