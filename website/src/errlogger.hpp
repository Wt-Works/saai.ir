#ifndef ERRLOGGER_HPP
#define ERRLOGGER_HPP


namespace SAAIIR {
    class ErrLogger;
}

class SAAIIR::ErrLogger {
public:
    static void LogError(const char *msg);
};


#endif /* ERRLOGGER_HPP */

