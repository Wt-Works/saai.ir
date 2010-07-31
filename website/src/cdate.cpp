#include <sstream>
#include <string>

#include <ctime>

#include "cdate.hpp"

using namespace std;
using namespace CDate;

Now::Now() {
    time_t rawTime;

    time(&rawTime);
    timeInfo = localtime(&rawTime);
    // Full date+time
    //asctime(timeInfo);

    hour = timeInfo->tm_hour; //  hour (0 - 23)
    daylightSavingTime = timeInfo->tm_isdst; //  Daylight saving time enabled (> 0), disabled (= 0), or unknown (< 0)
    dayOfMonth = timeInfo->tm_mday; //  day of the month (1 - 31)
    minutes = timeInfo->tm_min; //  minutes (0 - 59)
    month = timeInfo->tm_mon + 1; //  month (0 - 11, 0 = January)
    seconds = timeInfo->tm_sec != 60 ? timeInfo->tm_sec : 59; //  seconds (0 - 60, 60 = Leap second)
    /*
    A leap second is a plus or minus one-second adjustment to the Coordinated Universal Time (UTC) time scale that keeps it close to mean solar time.
    When a positive leap second is added at 23:59:60 UTC, it delays the start of the following UTC day (at 00:00:00 UTC) by one second, effectively slowing the UTC clock.
    */
    dayOfWeek = timeInfo->tm_wday + 1; //  day of the week (0 - 6, 0 = Sunday)
    dayOfYear = timeInfo->tm_yday + 1; //  day of the year (0 - 365)
    year = timeInfo->tm_year + 1900; //  year since 1900
}

const string DateConv::IntToStr(const int num) {
    //return boost::lexical_cast<std::string>(n);
    stringstream ss;
    ss << num;
    return ss.str();
}

const string DateConv::CalcToG(const int jYear, const int jMonth, const int jDay, const int dayOfYear) {
    bool isLeapYear = IsLeapYearJ(jYear);
    int dayMatch[13] = { !isLeapYear ? 287 : 288, !isLeapYear ? 318 : 319, !isLeapYear && !IsLeapYearJ(jYear + 1) ? 346 : 347, !isLeapYear ? 12 : 13, !isLeapYear ? 42 : 43, !isLeapYear ? 73 : 74, !isLeapYear ? 103 : 104, !isLeapYear ? 134 : 135, !isLeapYear ? 165 : 166, !isLeapYear ? 195 : 196, !isLeapYear ? 226 : 227, !isLeapYear ? 256 : 257, 999 };

    string gDay;
    string gMonth;

    for (int i = 0; i < 12; ++i)
        if ((dayOfYear >= dayMatch[i] && dayOfYear < dayMatch[i + 1]) || ((dayOfYear >= dayMatch[i] || dayOfYear < dayMatch[i + 1]) && (i == 2))) {
            gDay = IntToStr(dayOfYear >= dayMatch[i] ? dayOfYear - dayMatch[i] + 1 : !isLeapYear ? dayOfYear + 20 : dayOfYear + 19);
            gMonth = IntToStr(i + 1);
            break;
        }

    return IntToStr(dayOfYear < dayMatch[0] ? jYear + 621 : jYear + 622) + "/" +
            (gMonth.size() == 1 ? "0" + gMonth : gMonth)
            + "/" +
            (gDay.size() != 1 ? gDay : "0" + gDay);
}

const string DateConv::CalcToJ(const int gYear, const int gMonth, const int gDay, const int dayOfYear) {
    bool isLeapYear = IsLeapYearG(gYear - 1);
    int dayMatch[13] = { 80, 111, 142, 173, 204, 235, 266, 296, 326, 356, !isLeapYear ? 21 : 20, !isLeapYear ? 51 : 50, 999 };

    string jDay;
    string jMonth;

    for (int i = 0; i < 12; ++i)
        if ((dayOfYear >= dayMatch[i] && dayOfYear < dayMatch[i + 1]) || ((dayOfYear >= dayMatch[i] || dayOfYear < dayMatch[i + 1]) && (i == 9))) {
            jDay = IntToStr(dayOfYear >= dayMatch[i] ? dayOfYear - dayMatch[i] + 1 : !isLeapYear ? dayOfYear + 10 : dayOfYear + 11);
            jMonth = IntToStr(i + 1);
            break;
        }

    return IntToStr(dayOfYear > 79 ? gYear - 621 : gYear - 622) + "/" +
            (jMonth.size() == 1 ? "0" + jMonth : jMonth)
            + "/" +
            (jDay.size() != 1 ? jDay : "0" + jDay);
}

const bool DateConv::IsRangeValidG(const int gYear, const int gMonth, const int gDay) {
    int gMonths[12] = { 31, !IsLeapYearG(gYear) ? 28 : 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((gYear < 10000) && (gYear > -10000))
        if ((gMonth < 13) && (gMonth > 0))
            if ((gDay <= gMonths[gMonth - 1]) && (gDay > 0))
                return true;

    return false;
}

const bool DateConv::IsRangeValidJ(const int jYear, const int jMonth, const int jDay) {
    int jMonths[12] = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, !IsLeapYearJ(jYear) ? 29 : 30 };

    if ((jYear < 10000) && (jYear > -100))
        if ((jMonth < 13) && (jMonth > 0))
            if ((jDay <= jMonths[jMonth - 1]) && (jDay > 0))
                return true;

    return false;
}

const int DateConv::DayOfYearG(const int gYear, const int gMonth, const int gDay) {
    int gMonths[12] = { 31, !IsLeapYearG(gYear) ? 28 : 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int result = 0;

    for (int i = 0; i < gMonth - 1; ++i)
        result += gMonths[i];

    return result + gDay;
}

const int DateConv::DayOfYearJ(const int jYear, const int jMonth, const int jDay) {
    int jMonths[12] = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, !IsLeapYearJ(jYear) ? 29 : 30 };
    int result = 0;

    for (int i = 0; i < jMonth - 1; ++i)
        result += jMonths[i];

    return result + jDay;
}

const bool DateConv::IsLeapYearG(const int gYear) {
    int modulus = gYear % 4;

    return gYear > 0 && modulus == 0 ? true : gYear < 0 && modulus == 0 ? true : gYear == 0 ? true : false;
}

const bool DateConv::IsLeapYearJ(const int jYear) {
    int modulus = jYear % 4;

    return jYear > 0 && modulus == 3 ? true : jYear < 0 && modulus == -1 ? true : false;
}

const string DateConv::ToGregorian(const int jYear, const int jMonth, const int jDay) {
    if (!IsRangeValidJ(jYear, jMonth, jDay))
        return "";

    return CalcToG(jYear, jMonth, jDay, DayOfYearJ(jYear, jMonth, jDay));
}

const string DateConv::ToGregorian() {
    Now n;

    string m = IntToStr(n.month);
    string d = IntToStr(n.dayOfMonth);

    return IntToStr(n.year) + "/" +
            (m.size() == 1 ? "0" + m : m)
            + "/" +
            (d.size() != 1 ? d : "0" + d);
}

const string DateConv::ToJalali(const int gYear, const int gMonth, const int gDay) {
    if (!IsRangeValidG(gYear, gMonth, gDay))
        return "";

    return CalcToJ(gYear, gMonth, gDay, DayOfYearG(gYear, gMonth, gDay));
}

const string DateConv::ToJalali() {
    Now n;
    return CalcToJ(n.year, n.month, n.dayOfMonth, n.dayOfYear);
}

const string DateConv::ToGregorian(const CDate::Now& now) {
    string m = IntToStr(now.month);
    string d = IntToStr(now.dayOfMonth);

    return IntToStr(now.year) + "/" +
            (m.size() == 1 ? "0" + m : m)
            + "/" +
            (d.size() != 1 ? d : "0" + d);
}

const string DateConv::ToJalali(const CDate::Now& now) {
    return CalcToJ(now.year, now.month, now.dayOfMonth, now.dayOfYear);
}

const string DateConv::Time(const CDate::Now& now) {
    string s = IntToStr(now.seconds);
    string m = IntToStr(now.minutes);
    string h = IntToStr(now.hour);

    return (h.size() == 1 ? "0" + h : h) + ":" +
            (m.size() == 1 ? "0" + m : m)
            + ":" +
            (s.size() != 1 ? s : "0" + s);
}

const string DateConv::RawLocalDateTime(const CDate::Now& now) {
    return asctime(now.timeInfo);
}
