#ifndef ENCDEC_HPP
#define ENCDEC_HPP


#include <string>

namespace CCrypt {
    class EncDec;
}

class CCrypt::EncDec {
private:
    static unsigned char key[];
    static unsigned char iv[];
public:
    static const std::string Encrypt(const std::string& plainText);
    static const std::string Decrypt(const std::string& cipherText);
};


#endif /* ENCDEC_HPP */
