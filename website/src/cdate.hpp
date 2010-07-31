#ifndef CDATE_HPP
#define CDATE_HPP


#include <string>

namespace CDate {
    class DateConv;
    class Now;
}

class CDate::DateConv {
public:
    static const std::string CalcToG(const int jYear, const int jMonth, const int jDay, const int dayOfYear);
    static const std::string CalcToJ(const int gYear, const int gMonth, const int gDay, const int dayOfYear);
    static const bool IsRangeValidG(const int gYear, const int gMonth, const int gDay);
    static const bool IsRangeValidJ(const int jYear, const int jMonth, const int jDay);
    static const int DayOfYearG(const int gYear, const int gMonth, const int gDay);
    static const int DayOfYearJ(const int jYear, const int jMonth, const int jDay);
    static const bool IsLeapYearG(const int gYear);
    static const bool IsLeapYearJ(const int jYear);
    static const std::string ToGregorian(const int jYear, const int jMonth, const int jDay);
    static const std::string ToGregorian();
    static const std::string ToJalali(const int gYear, const int gMonth, const int gDay);
    static const std::string ToJalali();
    static const std::string ToGregorian(const CDate::Now& now);
    static const std::string ToJalali(const CDate::Now& now);
    static const std::string Time(const CDate::Now& now);
    static const std::string RawLocalDateTime(const CDate::Now& now);

private:
    static const std::string IntToStr(const int num);
};

class CDate::Now {
public:
    Now();

    int hour;
    int daylightSavingTime;
    int dayOfMonth;
    int minutes;
    int month;
    int seconds;
    int dayOfWeek;
    int dayOfYear;
    int year;
    struct tm *timeInfo;
};


#endif /* CDATE_HPP */

