#include "error.hpp"

using namespace SAAIIR;

Error::Error(const char *msg) {
    err = const_cast<char *>(msg);
}

const char *Error::GetReason() const {
    return err;
}
