#include <fstream>
#include <sstream>

#include <cstdlib>
#include <ctime>

/****/
#include <boost/lexical_cast.hpp>
/****/

#include <fastcgi++/manager.hpp>
#include <fastcgi++/request.hpp>

#include <Magick++.h>

/****/
//#include "base.hpp"
//#include "bstring.hpp"
/****/
#include "errlogger.hpp"
#include "error.hpp"

using namespace std;
using namespace boost;
using namespace Magick;

namespace SAAIIR {
    class CapRoot;
}

class SAAIIR::CapRoot : public Fastcgipp::Request<wchar_t> {
private:
    bool response();
};

bool SAAIIR::CapRoot::response() {
    try {
        /****/
        //Base::GenCap(BString::StrToInt(BString::WStrToStr(session.queryString).c_str()), true);
        /****/

        /*string captcha(" ");
        captcha += BString::IntToStr(Base::capN1);
        captcha += " X ";
        captcha += BString::IntToStr(Base::capN2);
        captcha += " = ";*/

        /****/
        //string captcha(BString::IntToStr(Base::capN1));
        //captcha += " X ";
        //captcha += BString::IntToStr(Base::capN2);
        /****/

        /****/
        //int seed = boost::lexical_cast<int>(session.queryString) + 1984; //IT SUCKS
        wstringstream ss(session.queryString);
        int seed;
        ss >> seed;
        seed += 1984;
        //

        srand(seed);
        int n1 = (rand() % 10) + 1;
        int n2 = (rand() % 10) + 1;
        int rotate = (rand() % 7 - 3);
        int skew = (rand() % 9 - 4);

        string captcha(boost::lexical_cast<std::string>(n1));
        captcha += " X ";
        captcha += boost::lexical_cast<std::string>(n2);
        /****/

        Image img(Geometry(115, 35), Color("white"));
        list<Drawable> drawList;

        drawList.push_back(DrawableTextAntialias(true));
        drawList.push_back(DrawableFont("../fnt/HAZELN.TTF"));
        drawList.push_back(DrawablePointSize(32));
        drawList.push_back(DrawableStrokeColor(Color("black")));
        drawList.push_back(DrawableFillColor(Color(0, 0, 0, MaxRGB)));
        drawList.push_back(DrawableTextDecoration(UnderlineDecoration));
        drawList.push_back(DrawableGravity(CenterGravity));
        /****/
        //drawList.push_back(DrawableRotation(Base::capRotate));
        //drawList.push_back(DrawableRotation(Base::capSkew));
        drawList.push_back(DrawableRotation(rotate));
        drawList.push_back(DrawableRotation(skew));
        /****/
        drawList.push_back(DrawableText(0, 0, captcha));

        img.draw(drawList);
        //img.blur(2, 0.5);

        string path("../tmp/captcha-");
        /****/
        //path += BString::IntToStr(rand());
        path += boost::lexical_cast<std::string>(rand());
        /****/
        path += ".png";

        img.write(path);

        ifstream file(path.c_str());
        if (file.is_open()) {
            out << "Content-Type: image/png\r\n\r\n";
            out.dump(file);
            file.close();
            /****/
            //Base::EraseFile(path.c_str());
            remove(path.c_str());
            /****/
        }
    }
    catch (std::exception& ex) {
        /****/
        //out << Base::httpHeader.c_str();
        out << "Content-Type: text/html; charset=utf-8\n\n";
        /****/
        //out << ex.what();
        out << "<br /><h1 style=\"color: 990000; text-align:center;\">Internal Server Error...</h1>";
        SAAIIR::ErrLogger::LogError(ex.what());
    }
    catch (...) {
        /****/
        //out << Base::httpHeader.c_str();
        //out << Base::errUnknown.c_str();
        //SAAIIR::ErrLogger::LogError(ex.what());
        out << "Content-Type: text/html; charset=utf-8\n\n";
        out << "<br /><h1 style=\"color: 990000; text-align:center;\">Internal Server Error...</h1>";
        SAAIIR::ErrLogger::LogError(" **  Unknown Error!!");
        /****/
    }

    return true;
}

int main(int argc, char **argv, char **env) {
    try {
        /****/
        //SAAIIR::Base::InitCurPath(argv[0]);
        string path(argv[0]);
        path = path.substr(0, path.rfind("/") + 1);
        if (chdir(path.c_str()) == -1) {
            throw Error(("Error changing directory to " + path).c_str());
        }
        /****/

        Fastcgipp::Manager<SAAIIR::CapRoot> capRoot;
        capRoot.handler();
    }

    catch (SAAIIR::Error ex) {
        SAAIIR::ErrLogger::LogError(ex.GetReason());
    }

    catch (std::exception& ex) {
        SAAIIR::ErrLogger::LogError(ex.what());
        return -2;
    }

    catch (...) {
        /****/
        //SAAIIR::ErrLogger::LogError(SAAIIR::Base::errUnknown.c_str());
        SAAIIR::ErrLogger::LogError(" **  Unknown Error!!");
        /****/
        return -1;
    }

    return 0;
}


///////// CGI Way
/*

#include <iostream>
#include <fstream>

using namespace std;

int main () {
        ifstream file("test.png", ios::in | ios::binary | ios::ate);

        if (file.is_open()) {
//		cout << "Content-Disposition: attachment; filename=captcha.png\n\n";
                cout << "Content-Type: image/png;\n\n";

                ifstream::pos_type size = (int)file.tellg();
                char *buffer = new char [size];
                file.seekg (0, ios::beg);
                file.read (buffer, size);
                cout.write(buffer, size);
                //cout << buffer;
                delete[] buffer;
        }

        return 0;
}

*/
