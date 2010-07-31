#ifndef CLIENTINFO_H
#define CLIENTINFO_H


#include <Wt/WEnvironment>

#include <string>

namespace SAAIIR {
    struct ClientInfo;
}

struct SAAIIR::ClientInfo {
public:
    ClientInfo(const Wt::WEnvironment& env);

    std::string ip;
    std::string browser;
    std::string countryCode;
    std::string countryName;
    std::string referer;
};


#endif /* CLIENTINFO_H */
