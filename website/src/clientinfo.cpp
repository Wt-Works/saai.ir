/*
#include <cstdlib>

#include <GeoIP.h>

const char *ClientInfo::ip = getenv("REMOTE_ADDR");
const char *ClientInfo::browser = getenv("HTTP_USER_AGENT");
const char *ClientInfo::countryCode = GeoIP_country_code_by_name(GeoIP_new(GEOIP_STANDARD), getenv("REMOTE_ADDR"));
const char *ClientInfo::countryName = GeoIP_country_name_by_name(GeoIP_new(GEOIP_STANDARD), getenv("REMOTE_ADDR"));
*/

#include <string>

#include <Wt/WEnvironment>

#include <GeoIP.h>

#include "clientinfo.hpp"
#include "bstring.hpp"

using namespace std;
using namespace Wt;
using namespace SAAIIR;

/*string ClientInfo::ip;
string ClientInfo::browser;
string ClientInfo::countryCode;
string ClientInfo::countryName;
string ClientInfo::referer;*/

ClientInfo::ClientInfo(const WEnvironment& env) {
    ip = env.getCgiValue("REMOTE_ADDR"); //env.clientAddress();
    browser = env.getCgiValue("HTTP_USER_AGENT"); //env.userAgent();
    countryCode = BString::CharToStr(
            GeoIP_country_code_by_name(GeoIP_new(GEOIP_STANDARD), ClientInfo::ip.c_str()));
    countryName = BString::CharToStr(
            GeoIP_country_name_by_name(GeoIP_new(GEOIP_STANDARD), ClientInfo::ip.c_str()));
    referer = env.getCgiValue("HTTP_REFERER"); //env.referer();
}
