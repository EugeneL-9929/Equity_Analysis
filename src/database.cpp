#include <iostream>
#include <sqlite3.h>
#include <string>

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
            "FOREIGN KEY (TIME_ID) REFERENCES TIME (ID) ON DELETE CASCADE ON UPDATE CASCADE);";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);
        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish stock table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
    }

    void addStockTable(float open, float close, float high, float low, int volume)
    {
        string command =
            "INSERT OR IGNORE INTO TIME (DATA_TIME) VALUES (?);"
            "INSERT INTO STOCK (OPEN, CLOSE, HIGH, LOW, VOLUME) VALUES (?, ?, ?, ?);";

        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(this->database, command.c_str(), -1, &stmt, nullptr);

        sqlite3_bind_double(stmt, 1, open);
        sqlite3_bind_double(stmt, 2, close);
        sqlite3_bind_double(stmt, 3, high);
        sqlite3_bind_double(stmt, 4, low);
        sqlite3_bind_double(stmt, 5, volume);
        sqlite3_bind_double(stmt, 6, open);

        if (sqlite3_step(stmt) == SQLITE_DONE)
            cout << "finish stock table initialization!" << endl;
        else
            cout << "error lists: " << sqlite3_errmsg(this->database) << endl;
        sqlite3_finalize(stmt);
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
    sqlite3 *database;
    const string dbPath;
};