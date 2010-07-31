#ifndef BASE_HPP
#define BASE_HPP


#include <string>

namespace SAAIIR {
    class Base;
}

class SAAIIR::Base {
private:
    static const unsigned char rootMasterKey[];

public:
    static std::string curPath;
    static const std::string httpHeader;
    static const std::wstring titleText;
    static const std::string errUnknown;

    static const std::string regExpEmail;
    static const std::string regExpUri;
    static const std::string regExpTel;
    static const std::string regExpMobile;
    static const std::string regExpNationalCode;
    static const std::string regExpBirthId;

    static const std::wstring errHBegin;
    static const std::wstring errHEnd;
    static const std::wstring msgHBegin;
    static const std::wstring msgHEnd;

    static const std::wstring errNoVote;
    static const std::wstring errNoVoteH;

    static const std::wstring errLogin;
    static const std::wstring errLoginH;
    static const std::wstring errSuspend;
    static const std::wstring errSuspendH;
    static const std::wstring errForget;
    static const std::wstring errForgetH;
    static const std::wstring msgForget;
    static const std::wstring msgForgetH;
    static const std::wstring errFollowing;
    static const std::wstring errFollowingH;

    static const std::wstring errConfirmPw;
    static const std::wstring errConfirmPwH;
    static const std::wstring errConfirmEmail;
    static const std::wstring errConfirmEmailH;
    static const std::wstring errInvalidPw;
    static const std::wstring errInvalidPwH;
    static const std::wstring errSamePw;
    static const std::wstring errSamePwH;
    static const std::wstring msgPwChanged;
    static const std::wstring msgPwChangedH;
    static const std::wstring msgEmailChanged;
    static const std::wstring msgEmailChangedH;

    static const std::wstring errDupContact;
    static const std::wstring errDupContactH;

    static const std::wstring msgSendMsgSuccess;
    static const std::wstring msgSendMsgSuccessH;

    static const std::wstring errDupAcUser;
    static const std::wstring errDupAcUserH;
    static const std::wstring msgAddAcademySuccess;
    static const std::wstring msgAddAcademySuccessH;
    static const std::wstring msgEditAcademySuccess;
    static const std::wstring msgEditAcademySuccessH;

    static const std::wstring errDupAcCourseForAc;
    static const std::wstring errDupAcCourseForAcH;

    static const std::wstring errPicInvalid;
    static const std::wstring errPicInvalidH;
    static const std::wstring errPicFormat;
    static const std::wstring errPicFormatH;
    static const std::wstring errPicFileSizeTooHigh;
    static const std::wstring errPicFileSizeTooHighH;
    static const std::wstring errPicFileSizeTooLow;
    static const std::wstring errPicFileSizeTooLowH;
    static const std::wstring errPicResRange;
    static const std::wstring errPicResRangeH;
    static const std::wstring errPicResTooHigh;
    static const std::wstring errPicResTooHighH;
    static const std::wstring errPicResTooLow;
    static const std::wstring errPicResTooLowH;
    static const std::wstring errPicSizeTooHigh;
    static const std::wstring errPicSizeTooHighH;
    static const std::wstring errPicSizeTooLow;
    static const std::wstring errPicSizeTooLowH;
    static const std::wstring errPicQualityTooLow;
    static const std::wstring errPicQualityTooLowH;

    static const std::wstring errUpFileTooLarge;
    static const std::wstring errUpFileTooLargeH;
    static const std::wstring errPicCannotUp;
    static const std::wstring errPicCannotUpH;
    static const std::wstring errPicNoPic;
    static const std::wstring errPicNoPicH;
    static const std::wstring msgPicAutoConv;
    static const std::wstring msgPicAutoConvH;
    static const std::wstring msgPicOK;
    static const std::wstring msgPicOKH;

    static const std::wstring errDupMCardForM;
    static const std::wstring errDupMCardForMH;

    static const std::wstring errFoundNoTerms;
    static const std::wstring errFoundNoTermsH;

    static const std::wstring errInvalidFollowingCode;
    static const std::wstring errInvalidFollowingCodeH;

    static const std::string GetCurPath();
    static void InitCurPath(const char *appPath);

    template<typename T, size_t size>
    static const int GetArrLength(T(&)[size]);

    /*static const int capTolerance;
    static int capSeed;
    static int capResult;
    static int capN1;
    static int capN2;
    static std::string capImage;*/
    static const std::wstring capTip;
    /*static void GenCap();
    static void GenCap(int seed, bool decode);
    static int capRotate;
    static int capSkew;*/

    static const std::string __rootMasterKey;
    static const int minLenUser;
    static const int maxLenUser;
    static const int minLenPw;
    static const int maxLenPw;

    static const int minLenEmailSubject;
    static const int maxLenEmailSubject;
    static const int minLenEmailBody;
    static const int maxLenEmailBody;

    static bool FileExists(const char *fileName);
    //static void FileSize(const char *fileName);
    static void FileErase(const char *fileName);
    static void FileWrite(const char *fileName, const char *data);
    static void Exec(const char *cmd);
    static void VacuumDB();
    static std::string GenRandChars();
};


#endif /* BASE_HPP */
