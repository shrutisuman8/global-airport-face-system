#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

string DATABASE = "airport.db";

int execute(sqlite3* db, const string& query)
{
    char* errMsg = 0;

    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMsg);

    if(rc != SQLITE_OK)
    {
        cout << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return 0;
    }

    return 1;
}

void create_tables(sqlite3* db)
{
    string users_table =
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "name TEXT,"
    "email TEXT UNIQUE,"
    "password TEXT,"
    "face_encoding TEXT"
    ");";

    string flights_table =
    "CREATE TABLE IF NOT EXISTS flights ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "flight_number TEXT UNIQUE,"
    "destination TEXT,"
    "departure_time TEXT"
    ");";

    string bookings_table =
    "CREATE TABLE IF NOT EXISTS bookings ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "user_id INTEGER,"
    "flight_id INTEGER,"
    "status TEXT DEFAULT 'Pending',"
    "FOREIGN KEY(user_id) REFERENCES users(id),"
    "FOREIGN KEY(flight_id) REFERENCES flights(id)"
    ");";

    execute(db, users_table);
    execute(db, flights_table);
    execute(db, bookings_table);

    cout<<"Tables created successfully\n";
}

void insert_dummy_flights(sqlite3* db)
{
    string check_query = "SELECT COUNT(*) FROM flights;";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, check_query.c_str(), -1, &stmt, NULL);

    sqlite3_step(stmt);

    int count = sqlite3_column_int(stmt,0);

    sqlite3_finalize(stmt);

    if(count==0)
    {
        string f1 =
        "INSERT INTO flights (flight_number,destination,departure_time)"
        " VALUES ('AC101','New York (JFK)','2026-03-10 12:00:00');";

        string f2 =
        "INSERT INTO flights (flight_number,destination,departure_time)"
        " VALUES ('BA202','London (LHR)','2026-03-10 15:00:00');";

        execute(db,f1);
        execute(db,f2);

        cout<<"Dummy flights inserted\n";
    }
}

int main()
{
    sqlite3* db;

    int rc = sqlite3_open(DATABASE.c_str(), &db);

    if(rc)
    {
        cout<<"Cannot open database\n";
        return 0;
    }

    create_tables(db);

    insert_dummy_flights(db);

    sqlite3_close(db);

    return 0;
}