/*
#include <cstdlib>

#include "bstring.hpp"

const char *ServerInfo::host = getenv("HTTP_HOST");
const char *ServerInfo::url = ("http://" + BString::CharToStr(ServerInfo::host) + BString::CharToStr(getenv("REQUEST_URI"))).c_str();
*/


#include <string>

#include <Wt/WEnvironment>

#include "serverinfo.hpp"

using namespace std;
using namespace Wt;
using namespace SAAIIR;

/*string ServerInfo::host;
string ServerInfo::url;
string ServerInfo::noReply;*/

ServerInfo::ServerInfo(const WEnvironment& env) {
    host = env.getCgiValue("HTTP_HOST"); //env.hostName();
    url = "http://" + host + env.getCgiValue("REQUEST_URI");
    noReply = "no-reply@" + host;
}
