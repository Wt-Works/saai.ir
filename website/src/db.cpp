#include <string>
#include <cstdarg>

#include <sqlite3.h>

#include <dbixx/dbixx.h>

#include "db.hpp"

using namespace std;
using namespace dbixx;
using namespace SAAIIR;

DB::DB() {
}

DB::~DB() {
    sql.close();
}

void DB::Vacuum(const std::string& dbFile) {
    sqlite3 *db;
    int rc = sqlite3_open(dbFile.c_str(), &db);
    if (!rc)
        sqlite3_exec(db, "VACUUM;", 0, 0, 0);
}

void DB::Init(const string &dbName) {
    sql.driver("sqlite3");
    sql.param("dbname", dbName);
    sql.param("sqlite3_dbdir", "../db/");
    sql.connect();
}

/*string DB::Escape(const string& str) {
    return BString::Replace(str, "'", "''");
}*/

void DB::CreateTable(const string& tbl, const string& fields) {
    sql << "CREATE TABLE IF NOT EXISTS [" + tbl + "] ("
            + fields +
            ");";
    sql.exec();
}

void DB::DropTable(const string& tbl) {
    sql << "DROP TABLE IF EXISTS [" + tbl + "];";
    sql.exec();
}

void DB::Insert(const string& tbl, const string& fields, const int count, ...) {
    va_list args;
    va_start(args, count);

    string ph;
    for (int i = 0; i < count; ++i) {
        if (i != 0)
            ph += ", ";
        ph += "?";
    }

    sql << "INSERT OR IGNORE INTO [" + tbl + "] "
           "(" + fields + ") "
           "VALUES (" + ph + ");";

    for(int i = 0; i < count; ++i) {
         sql.bind(va_arg(args, char*));
    }

    va_end(args);

    sql.exec();

    /*Alternative ways
    sql << "INSERT OR IGNORE INTO [__root] (user, pw) VALUES (?, ?);",
             "r00t","t00r/.?",exec();
    sql<<"INSERT INTO [__root] (user,pw) "
         "SELECT 'r00t', 't00r/.?'"
         "WHERE NOT EXISTS (SELECT * FROM [__root] WHERE user='r00t' AND pw='t00r/.?');",
         exec();
    */
}

void DB::Update(const string& tbl, const string& where, const string& value,  const string& set, const int count, ...) {
    va_list args;
    va_start(args, count);

    sql << "UPDATE [" + tbl + "] "
           "SET " + set + " "
           "WHERE " + where + "=?;";

    for(int i = 0; i < count; ++i) {
         sql.bind(va_arg(args, char*));
    }

    va_end(args);

    sql.bind(value);

    sql.exec();
}

void DB::Delete(const string& tbl, const string& where, const string& value) {
    sql << "DELETE FROM [" + tbl + "] "
           "WHERE " + where + "=?;",
           value, exec();
}

/*bool DB::FetchRow(const string& tbl, const string& fields, const string& where, const string& value, dbixx::row& r) {
    sql << "SELECT " + BString::CharToStr(fields) + " FROM [" + BString::CharToStr(tbl) + "] "
        << "WHERE " + BString::CharToStr(where) + "=?";
    sql << query, r;
    return sql.single(r) ? true : false;
}

bool DB::FetchRow(const string& query, dbixx::row& r) {
    sql << query, r;
    return sql.single(r) ? true : false;
    //return !r.isempty() ? true : false;
}

bool DB::FetchRows(const string& query, dbixx::result& res) {
    sql << query;
    sql.fetch(res);
    return res.rows() > 0 ? true : false;
}
*/
