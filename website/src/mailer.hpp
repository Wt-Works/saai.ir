#ifndef MAILER_HPP
#define MAILER_HPP


#include <string>

#include "cdate.hpp"
#include "clientinfo.hpp"
#include "serverinfo.hpp"

namespace SAAIIR {
    class Mailer;
}

class SAAIIR::Mailer {
public:
    static void SendMail(const std::string& from, const std::string& to,
                         const std::string& subject, const std::string& body);

    static void SendLoginAlert(const std::string& to, const std::string& user,
                               const CDate::Now& now,
                               const ServerInfo *serverInfo, const ClientInfo *clientInfo);

    static void SendLoginRecovery(const std::string& to, const std::string& user,
                                  const std::string& pw, const CDate::Now& now,
                                  const ServerInfo *serverInfo, const ClientInfo *clientInfo);

    static void SendUserMessage(const std::string& to, const std::string& from,
                                const std::string& user, const std::string& uri,
                                const std::string& subject, const std::string& body,
                                const CDate::Now& now,
                                const ServerInfo *serverInfo, const ClientInfo *clientInfo);
};


#endif /* MAILER_HPP */
