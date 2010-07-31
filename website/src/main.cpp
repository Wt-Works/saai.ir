#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include "base.hpp"
#include "cgiroot.hpp"
#include "errlogger.hpp"
#include "error.hpp"

using namespace Wt;
using namespace SAAIIR;

WApplication *CreateApplication(const WEnvironment& env);

int main(int argc, char **argv, char **env) {
    try {
        Base::InitCurPath(argv[0]);
        Base::VacuumDB();

        return WRun(argc, argv, &CreateApplication);
    }

    catch (Error ex) {
        ErrLogger::LogError(ex.GetReason());
        return 0;
    }

    catch (std::exception& ex) {
        ErrLogger::LogError(ex.what());
        return -2;
    }

    catch (...) {
        ErrLogger::LogError(Base::errUnknown.c_str());
        return -1;
    }
}

WApplication *CreateApplication(const WEnvironment& env) {
    WApplication *app = new CgiRoot(env);
    return app;
}
