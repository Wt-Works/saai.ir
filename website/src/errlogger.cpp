#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "errlogger.hpp"

using namespace std;
using namespace boost;
using namespace SAAIIR;

void ErrLogger::LogError(const char *msg) {
    static ofstream error;

    if(!error.is_open()) {
        error.open("../log/error_log", ios_base::out | ios_base::app);
        error.imbue(locale(error.getloc(), new posix_time::time_facet()));
    }

    error << '[' << posix_time::second_clock::local_time() << "] " << msg << endl;
}
