#include <fstream>
#include <sstream>

#include <cstdio>

#include <fastcgi++/manager.hpp>
#include <fastcgi++/request.hpp>

#include <Magick++.h>

#include <dbixx/dbixx.h>

#include <b64/decode.h>

#include "errlogger.hpp"
#include "error.hpp"

using namespace std;
using namespace dbixx;
using namespace Magick;

namespace SAAIIR {
    class ShowPics;
}

class SAAIIR::ShowPics : public Fastcgipp::Request<wchar_t> {
private:
    bool response();
};

bool SAAIIR::ShowPics::response() {
    try {
        string qs(session.queryString.begin(), session.queryString.end());
        string table = qs.substr(0, 10);
        string id = qs.substr(10, 11);
        string field = qs.substr(21, 1);

        if (field == "p") {
            field = "personnel";
        } else if (field == "b") {
            field = "birthcert";
        } else if (field == "n") {
            field = "nationalcard";
        } else if (field == "s") {
            field = "service";
        }

        dbixx::session sql;

        sql.driver("sqlite3");
        sql.param("dbname", "pics.db");
        sql.param("sqlite3_dbdir", "../db/");
        sql.connect();

        row r;
        sql << "SELECT " + field + " FROM [" + table + "] "
                "WHERE id=?;", id, r;

        if(sql.single(r)) {
            string data;
            r >> data;

            string tempB64("../tmp/" + qs + ".b64");
            string tempPic("../tmp/" + qs + ".pic");

            ofstream file(tempB64.c_str(), ios::out | ios::trunc);
            if (file.is_open()) {
                file << data.c_str();
                file.close();
            }
            else {
                throw 0;
            }

            ifstream inStream(tempB64.c_str(), ios_base::in | ios_base::binary);
            if (!inStream.is_open()) {
                throw 0;
            }

            std::ofstream outStream(tempPic.c_str(), ios_base::out | ios_base::binary);
            if (!outStream.is_open()) {
                throw 0;
            }

            base64::decoder dec;
            dec.decode(inStream, outStream);

            if (inStream.is_open())
                inStream.close();
            if (outStream.is_open())
                outStream.close();

            if (field == "personnel") {
                Image image(tempPic);
                image.magick("JPEG");
                image.quality(85);

                Geometry g(144, 192);
                g.aspect(true);
                //Geometry g("96x128!");
                image.scale(g);
                image.write(tempPic);
            }

            ifstream pic(tempPic.c_str());
            if (pic.is_open()) {
                out << "Content-Type: image/jpeg\r\n\r\n";
                out.dump(pic);
                pic.close();
            }
            else {
                throw 0;
            }

            remove(tempB64.c_str());
            remove(tempPic.c_str());
        }
        else {
            throw 0;
        }

        sql.close();
    }
    catch (std::exception& ex) {
        out << "Content-Type: text/html; charset=utf-8\n\n";
        out << "<br /><h1 style=\"color: 990000; text-align:center;\">Internal Server Error...</h1>";
        SAAIIR::ErrLogger::LogError(ex.what());
    }
    catch (...) {
        out << "Content-Type: text/html; charset=utf-8\n\n";
        out << "<br /><h1 style=\"color: 990000; text-align:center;\">Internal Server Error...</h1>";
        SAAIIR::ErrLogger::LogError(" **  Unknown Error!!");
    }

    return true;
}

int main(int argc, char **argv, char **env) {
    try {
        string path(argv[0]);
        path = path.substr(0, path.rfind("/") + 1);
        if (chdir(path.c_str()) == -1) {
            throw Error(("Error changing directory to " + path).c_str());
        }

        Fastcgipp::Manager<SAAIIR::ShowPics> showPics;
        showPics.handler();
    }

    catch (SAAIIR::Error ex) {
        SAAIIR::ErrLogger::LogError(ex.GetReason());
    }

    catch (std::exception& ex) {
        SAAIIR::ErrLogger::LogError(ex.what());
        return -2;
    }

    catch (...) {
        SAAIIR::ErrLogger::LogError(" **  Unknown Error!!");
        return -1;
    }

    return 0;
}
