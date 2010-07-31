#include <sstream>
#include <string>

#include <cstdarg>
//#include <cstdlib>

#include <boost/algorithm/string.hpp>
//#include <boost/lexical_cast.hpp>

#include <Wt/WString>

#include "bstring.hpp"

using namespace std;
using namespace boost;
using namespace Wt;
using namespace SAAIIR;

const string BString::CharToStr(const char *chStr) {
    return chStr != NULL ? chStr : "";
}

const wstring BString::CharToStr(const wchar_t *chStr) {
    return chStr != NULL ? chStr : L"";
}

const string BString::IntToStr(const int num) {
    //return boost::lexical_cast<std::string>(n);
    stringstream ss;
    ss << num;
    return ss.str();
}

const wstring BString::IntToWStr(const int num) {
    //return boost::lexical_cast<std::wstring>(n);
    wstringstream ss;
    ss << num;
    return ss.str();
}

const string BString::StrFromByte(const unsigned char *arr, const size_t len) {
    ostringstream oss;

    for (size_t i = 0; i < len; ++i) {
        oss << arr[i];
    }

    return oss.str();
}

/*const string BString::StrFromByte(const unsigned wchar_t *arr, const size_t len) {
    wostringstream oss;

    for (size_t i = 0; i < len; ++i) {
        oss << (wchar_t) arr[i];
    }

    return WStrToStr(oss.str());
}*/

const int BString::StrToInt(const char *str) {
    //return boost::lexical_cast<int>(str);
    //return atoi(str);
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

const int BString::StrToInt(const string& str) {
    return StrToInt(str.c_str());
}

const wstring BString::StrToWStr(const char *str) {
    string tempStr(str);
    return StrToWStr(tempStr);
}

const wstring BString::StrToWStr(const string& str) {
    /*wstring wStr(str.begin(), str.end());
    return wStr;*/
    return WString::fromUTF8(str);
}

const string BString::WStrToStr(const wchar_t *wStr) {
    wstring tempWStr(wStr);
    return WStrToStr(tempWStr);
}

const string BString::WStrToStr(const wstring& wStr) {
    /*string str(wStr.begin(), wStr.end());
    return str;*/
    WString wstr(wStr);
    return wstr.toUTF8();
}

string BString::Trim(const string str) {
    return algorithm::trim_copy(str);
}

wstring BString::Trim(const wstring str) {
    return algorithm::trim_copy(str);
}





/*
const char *BString::Join(const int count, ...) {
    string res;

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        res += CharToStr(va_arg(args, char*));
    }

    va_end(args);

    return res.c_str();
}
*/

/*
const string BString::Replace(const string& str, const string& oldStr, const string& newStr) {
    string res(str);

    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != string::npos) {
       res.replace(pos, oldStr.length(), newStr);
       pos += newStr.length();
    }

    return res;
}
*/
