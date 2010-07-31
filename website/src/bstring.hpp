#ifndef BSTRING_HPP
#define BSTRING_HPP


#include <string>

namespace SAAIIR {
    class BString;
}

class SAAIIR::BString {
public:
    /*static const char *Join(const int, ...);*/
    static const std::string CharToStr(const char *chStr);
    static const std::wstring CharToStr(const wchar_t *chStr);
    static const std::string IntToStr(const int num);
    static const std::wstring IntToWStr(const int num);
    static const std::string StrFromByte(const unsigned char *arr, const size_t len);
    /*static const std::string StrFromByte(const unsigned wchar_t *arr, const size_t len);*/
    static const int StrToInt(const char *str);
    static const int StrToInt(const std::string& str);
    static const std::wstring StrToWStr(const char *str);
    static const std::wstring StrToWStr(const std::string& str);
    static const std::string WStrToStr(const wchar_t *wStr);
    static const std::string WStrToStr(const std::wstring& wStr);
    static std::string Trim(const std::string str);
    static std::wstring Trim(const std::wstring str);
    /*static const std::string Replace(const std::string& str,
                              const std::string& oldStr, const std::string& newStr);*/
};


#endif /* BSTRING_HPP */
