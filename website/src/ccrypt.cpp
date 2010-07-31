#include <string>

#include <boost/lexical_cast.hpp>

#include <crypto++/aes.h>
#include <crypto++/ccm.h>
#include <crypto++/cryptlib.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>

#include "ccrypt.hpp"
//#include "error.hpp"
#include "errlogger.hpp"

using namespace std;
using CryptoPP::AES;
using CryptoPP::CBC_Mode;
using CryptoPP::Exception;
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using namespace CCrypt;

unsigned char EncDec::key[] = { 0x23, 0x56, 0x31, 0x76, 0x61, 0x4d, 0x61, 0x72, 0x61, 0x6c, 0x3a, 0x2d, 0x2a, 0x2f, 0x2e, 0x3f };
unsigned char EncDec::iv[] = { 0x38, 0x34, 0x32, 0x34, 0x33, 0x30, 0x31, 0x30, 0x34, 0x33, 0x35, 0x39, 0x32, 0x34, 0x2e, 0x34 };

const string EncDec::Encrypt(const string& plainText) {
    string cipher, encoded;

    try {
        CBC_Mode<AES>::Encryption enc;
        enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

        cipher.clear();
        StringSource(plainText, true, new StreamTransformationFilter(enc, new StringSink(cipher)));

        encoded.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
    }
    catch(CryptoPP::Exception& ex) {
        SAAIIR::ErrLogger::LogError(("Encrypt :  " + boost::lexical_cast<string>(ex.what())).c_str());
        //throw Error(ex.what());
    }

    return encoded;
}

const string EncDec::Decrypt(const string& cipherText) {
    string cipher, recovered;

    try {
        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

        cipher.clear();
        StringSource(cipherText, true, new HexDecoder(new StringSink(cipher)));

        recovered.clear();
        StringSource s(cipher, true, new StreamTransformationFilter(dec, new StringSink(recovered)));
    }
    catch(CryptoPP::Exception& ex) {
        SAAIIR::ErrLogger::LogError(("Decrypt :  " + boost::lexical_cast<string>(ex.what())).c_str());
        //throw Error(ex.what());
    }

    return recovered;
}
