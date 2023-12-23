#include <string_view>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct DBLogLevel;
struct DBConnector;
struct DBHandler;
struct Person;

struct DBInfo {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};
struct PersonInfo{
    int min_age; 
    int max_age;
    string_view name_filter;
};

vector<Person> LoadPersons(DBInfo db_info, PersonInfo person_info) {
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_info.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_info.db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_info.db_connection_timeout);
    }
    if (!db_info.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << person_info.min_age << " and "s << person_info.max_age << " "s
              << "and Name like '%"s << db.Quote(person_info.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}