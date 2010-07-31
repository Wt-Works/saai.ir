#ifndef CIMAGE_HPP
#define CIMAGE_HPP


#include <string>

namespace SAAIIR {
    class CImage;
}

class SAAIIR::CImage {
public:
    static bool IsValidImage(const std::string& fileName);
    static bool IsValidImageFormat(const std::string& fileName);
    static unsigned long ImageFileSize(const std::string& fileName);
    static bool IsImageFileSizeTooHigh(const std::string& fileName);
    static bool IsImageFileSizeTooLow(const std::string& fileName);
    static bool IsImageResolutionTooHigh(const std::string& fileName);
    static bool IsImageResolutionTooLow(const std::string& fileName);
    static bool IsValidImageResolution(const std::string& fileName);
    static bool IsImageSizeTooHigh(const std::string& fileName);
    static bool IsImageSizeTooLow(const std::string& fileName);
    static void OptimizeImage(const std::string& fileName);
    static std::string B64Encode(const std::string& fileName);
private:
    static unsigned int qDef;
    static unsigned int dpiMin;
    static unsigned int dpiMax;
    static unsigned int sizeMin;
    static unsigned int sizeMax;
    static unsigned int pixMin;
    static unsigned int pixMax;
};


#endif /* CIMAGE_HPP */
