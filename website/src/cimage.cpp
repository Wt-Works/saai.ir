#include <fstream>
#include <string>

#include <cstdlib>

#include <boost/lexical_cast.hpp>

#include <Magick++.h>

#include <b64/encode.h>
//#include <b64/decode.h>

#include "cimage.hpp"

using namespace std;
using namespace Magick;
using namespace SAAIIR;

unsigned int CImage::qDef = 85;
unsigned int CImage::dpiMin = 72;
unsigned int CImage::dpiMax = 96;
unsigned int CImage::sizeMin = 24576;
unsigned int CImage::sizeMax = 98304;
unsigned int CImage::pixMin = 480;
unsigned int CImage::pixMax = 720;

bool CImage::IsValidImage(const string& fileName) {
    try {
        Image image(fileName);
        return true;
    }
    catch(...) {
    }

    return false;
}

bool CImage::IsValidImageFormat(const string& fileName) {
    try {
        Image image(fileName);
        if ((image.format().find("JFIF") != string::npos)) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

unsigned long CImage::ImageFileSize(const string& fileName) {
    try {
        Image image(fileName);
        return image.fileSize();
    }
    catch(...) {
    }

    return 0;
}

bool CImage::IsImageFileSizeTooHigh(const string& fileName) {
    if (ImageFileSize(fileName) <= sizeMax) {
        return false;
    }

    return true;
}

bool CImage::IsImageFileSizeTooLow(const string& fileName) {
    if (ImageFileSize(fileName) >= sizeMin) {
        return false;
    }

    return true;
}

bool CImage::IsImageResolutionTooHigh(const string& fileName) {
    try {
        Image image(fileName);
        if (image.xResolution() <= dpiMax && image.yResolution() <= dpiMax) {
            return false;
        }
    }
    catch(...) {
    }

    return true;
}

bool CImage::IsImageResolutionTooLow(const string& fileName) {
    try {
        Image image(fileName);
        if (image.xResolution() >= dpiMin && image.yResolution() >= dpiMin) {
            return false;
        }
    }
    catch(...) {
    }

    return true;
}

bool CImage::IsValidImageResolution(const string& fileName) {
    try {
        Image image(fileName);
        if ((image.xResolution() == dpiMin && image.yResolution() == dpiMin) ||
            (image.xResolution() == dpiMax && image.yResolution() == dpiMax )) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

bool CImage::IsImageSizeTooLow(const string& fileName) {
    try {
        Image image(fileName);
        if (image.columns() >= pixMin && image.rows() >= pixMin) {
            return false;
        }
    }
    catch(...) {
    }

    return true;
}

bool CImage::IsImageSizeTooHigh(const string& fileName) {
    try {
        Image image(fileName);
        if (image.columns() <= pixMax && image.rows() <= pixMax) {
            return false;
        }
    }
    catch(...) {
    }

    return true;
}

void CImage::OptimizeImage(const string& fileName) {
    try {
        Image image(fileName);
        image.magick("JPEG");
        image.quality(qDef);


        int xRes = 0, yRes = 0;
        if (image.xResolution() > dpiMax) {
            xRes = dpiMax;
        }
        else if (image.xResolution() > dpiMin && image.xResolution() < dpiMax) {
            xRes = dpiMin;
        }
        else {
            xRes = image.xResolution();
        }

        if (image.yResolution() > dpiMax) {
            yRes = dpiMax;
        }
        else if (image.yResolution() > dpiMin && image.yResolution() < dpiMax) {
            yRes = dpiMin;
        }
        else {
            yRes = image.yResolution();
        }

        if (xRes != yRes) {
            xRes = dpiMin;
            yRes = dpiMin;
        }

        image.resolutionUnits(PixelsPerInchResolution);
        image.density(Geometry(xRes, yRes));


        unsigned int x = 0, y = 0;

        if ((image.columns() >= pixMin && image.columns() <= pixMax) &&
            (image.rows() >= pixMin && image.rows() <= pixMax)) {
            x = image.columns();
            y = image.rows();
        }
        else {
            bool found = false;
            for (unsigned int i = pixMin; i <= pixMax; ++i) {
                float div = image.columns() / (float)i;
                x = image.columns() / div;
                y = image.rows() / div;

                if ((x >= pixMin && x <= pixMax) &&
                    (y >= pixMin && y <= pixMax)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                x = pixMax;
                y = pixMax;
            }
        }


        Geometry g(x, y);
        //g.aspect(true);
        //image.resize(g);
        image.scale(g);
        image.write(fileName);
    }
    catch(...) {
    }
}

string CImage::B64Encode(const string& fileName) {
    try {
        ifstream inStream(fileName.c_str(), ios_base::in | ios_base::binary);
        if (!inStream.is_open()) {
            return "";
        }

        srand((unsigned)time(NULL));
        string tempFile("../tmp/pic-");
        tempFile += boost::lexical_cast<std::string>(rand());
        tempFile += ".b64";

        ofstream outStream(tempFile.c_str(), ios_base::out | ios_base::binary);
        if (!outStream.is_open()) {
            return "";
        }

        base64::encoder enc;
        enc.encode(inStream, outStream);

        if (inStream.is_open())
            inStream.close();
        if (outStream.is_open())
            outStream.close();

        string b64;
        ifstream file(tempFile.c_str());
        if (file.is_open()) {
            string line;
            while (!file.eof()) {
                getline (file, line);
                b64 += line + "\n";
            }
            file.close();
        }

        remove(tempFile.c_str());

        return b64;

        /*ifstream file(fileName.c_str(), ios::in | ios::binary | ios::ate);
        if (file.is_open()) {
            ifstream::pos_type size;
            char *buffer;

            size = file.tellg();
            buffer = new char [size];
            file.seekg (0, ios::beg);
            file.read (buffer, size);
            file.close();

            char *res;
            base64::encoder enc(16777216);
            enc.encode(buffer, size, res);

            delete[] buffer;

            return res;
        }*/
    }
    catch (...) {
    }

    return "";
}
/*
char *CImage::B64Decode(const string& data) {
    try {

    }
    catch (...) {

    }

    return "";
}
*/
