#ifndef SERVERINFO_H
#define SERVERINFO_H


#include <Wt/WEnvironment>

#include <string>

namespace SAAIIR {
    struct ServerInfo;
}

struct SAAIIR::ServerInfo {
    public:
        ServerInfo(const Wt::WEnvironment& env);

        std::string host;
        std::string url;
        std::string noReply;
};


#endif /* SERVERINFO_H */
