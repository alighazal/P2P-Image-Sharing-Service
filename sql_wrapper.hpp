#ifndef _SQL_
#define _SQL_

#include <iostream>
#include <sqlite3.h>
#include <cstring>

using namespace std;

class SqlWrapper
{
private:
    sqlite3 *DB;
    string database_name;

    int static callback(void *data, int argc, char **argv, char **azColName)
    {
        int i;
        fprintf(stderr, "%s: ", (const char *)data);

        for (i = 0; i < argc; i++)
        {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }

        printf("\n");
        return 0;
    }

public:
    SqlWrapper(sqlite3 *DB, string database_name) : DB(DB), database_name(database_name)
    {
    }

    int connect()
    {
        int exit = 0;
        exit = sqlite3_open(this->database_name.c_str(), &this->DB);

        if (exit)
        {
            std::cerr << "Error open DB " << sqlite3_errmsg(this->DB) << std::endl;
            return (-1);
        }
        else
            std::cout << "Opened Database Successfully!" << std::endl;

        sqlite3_close(DB);
        return 0;
    }

    int create(string sql_statment)
    {

        int exit = 0;
        exit = sqlite3_open(this->database_name.c_str(), &this->DB);

        char *messaggeError;
        exit = sqlite3_exec(DB, sql_statment.c_str(), NULL, 0, &messaggeError);

        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Create Table" << std::endl;
            sqlite3_free(messaggeError);
            return -1;
        }
        else
            std::cout << "Table created Successfully" << std::endl;

        sqlite3_close(DB);

        return 0;
    }

    int retrieve(bool verbose)
    {

        char *messaggeError;
        int exit = sqlite3_open("example.db", &this->DB);

        string query = "SELECT * FROM PERSON;";

        if (verbose)
        {
            cout << "STATE OF TABLE BEFORE INSERT" << endl;
            sqlite3_exec(DB, query.c_str(), this->callback, NULL, NULL);
        }

        string sql("INSERT INTO PERSON VALUES(1, 'STEVE', 'GATES', 30, 'PALO ALTO', 1000.0);"
                   "INSERT INTO PERSON VALUES(2, 'BILL', 'ALLEN', 20, 'SEATTLE', 300.22);"
                   "INSERT INTO PERSON VALUES(3, 'PAUL', 'JOBS', 24, 'SEATTLE', 9900.0);");

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Insert" << std::endl;
            sqlite3_free(messaggeError);
            return -1;
        }
        else
            std::cout << "Records created Successfully!" << std::endl;

        cout << "STATE OF TABLE AFTER INSERT" << endl;

        sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

        return 0;
    }
};


#endif