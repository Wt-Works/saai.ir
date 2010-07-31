#ifndef DB_HPP
#define DB_HPP


#include <string>

#include <dbixx/dbixx.h>

namespace SAAIIR {
    class DB;
}

class SAAIIR::DB {
public:
    DB();
    ~DB();

    dbixx::session sql;

    static void Vacuum(const std::string& dbFile);

    void Init(const std::string& dbName);
    void CreateTable(const std::string& tbl, const std::string& fields);
    void DropTable(const std::string& tbl);
    void Insert(const std::string& tbl, const std::string& fields, const int count, ...);
    void Update(const std::string& tbl, const std::string& where, const std::string& value,
                const std::string& set, const int count, ...);
    void Delete(const std::string& tbl, const std::string& where, const std::string& value);
    /*bool FetchRow(const char*, dbixx::row&);
    bool FetchRows(const char*, dbixx::result&);*/
};


#endif /* DB_HPP */
