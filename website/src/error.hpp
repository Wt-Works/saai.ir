#ifndef ERROR_HPP
#define ERROR_HPP


namespace SAAIIR {
    class Error;
}

class SAAIIR::Error {
private:
    char *err;

public:
    Error(const char *msg);

    const char *GetReason() const;
};


#endif /* ERROR_HPP */

