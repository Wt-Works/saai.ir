#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <unistd.h>

#include "base.hpp"
#include "bstring.hpp"
#include "ccrypt.hpp"
#include "db.hpp"
#include "error.hpp"

using namespace std;
using namespace SAAIIR;
using namespace CCrypt;

string Base::curPath;
const string Base::httpHeader = "Content-Type: text/html; charset=utf-8\n\n";
const string Base::errUnknown = " **  Unknown Error!!";
const wstring Base::titleText = L"سیستم ثبت نام در آموزشگاه های آزاد";

const string Base::regExpEmail = "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}";
/*const string Base::regExpEmail = "^([a-zA-Z0-9_\\-\\.]+)@((\[[0-9]{1,3}"
                                      "\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\"
                                      ".)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)$";*/
const string Base::regExpUri = "^(https?://)"
                               "?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?" //user@
                               "(([0-9]{1,3}\\.){3}[0-9]{1,3}" // IP- 199.194.52.184
                               "|" // allows either IP or domain
                               "([0-9a-z_!~*'()-]+\\.)*" // tertiary domain(s)- www.
                               "([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\\." // second level domain
                               "[a-z]{2,6})" // first level domain- .com or .museum
                               "(:[0-9]{1,4})?" // port number- :80
                               "((/?)|" // a slash isn't required if there is no file name
                               "(/[0-9a-z_!~*'().;?:@&=+$,%#-]+)+/?)$";
const string Base::regExpTel = "[0-9]{7,11}";
const string Base::regExpMobile = "[0-9]{11}";
const string Base::regExpNationalCode = "[0-9]{10}";
const string Base::regExpBirthId = "[0-9]{1,15}";

const wstring Base::errHBegin= L"<h4 style=\"font-weight: bold; color: #990000; "
                               "direction: rtl; text-align: center;\">";
const wstring Base::errHEnd = L"</h4>";
const wstring Base::msgHBegin= L"<h4 style=\"font-weight: bold; color: #203070; "
                               "direction: rtl; text-align: center;\">";
const wstring Base::msgHEnd = L"</h4>";

const wstring Base::errNoVote = L"لطفا یک گزینه را انتخاب نمائید";
const wstring Base::errNoVoteH = Base::errHBegin + Base::errNoVote + Base::errHEnd;

const wstring Base::errLogin = L"عدم مطابقت نام کاربری یا کلمه عبور";
const wstring Base::errLoginH = Base::errHBegin + Base::errLogin + Base::errHEnd;
const wstring Base::errSuspend = L"حساب کاربری شما توسط مدیر سیستم غیرفعال شده است";
const wstring Base::errSuspendH = Base::errHBegin + Base::errSuspend + Base::errHEnd;
const wstring Base::errForget = L"کاربر معتبر نمی باشد";
const wstring Base::errForgetH = Base::errHBegin + Base::errForget + Base::errHEnd;
const wstring Base::msgForget = L"کلمه عبور با موفقیت به ایمیل شما ارسال شد";
const wstring Base::msgForgetH = Base::msgHBegin + Base::msgForget + Base::msgHEnd;
const wstring Base::errFollowing = L"کد رهگیری نامعتبر";
const wstring Base::errFollowingH = Base::errHBegin + Base::errFollowing + Base::errHEnd;

const wstring Base::errConfirmPw = L"لطفا کلمه عبور جدید را تائید نمائید";
const wstring Base::errConfirmPwH = Base::errHBegin + Base::errConfirmPw + Base::errHEnd;
const wstring Base::errConfirmEmail = L"لطفا آدرس ایمیل را تائید نمائید";
const wstring Base::errConfirmEmailH = Base::errHBegin + Base::errConfirmEmail + Base::errHEnd;
const wstring Base::errInvalidPw = L"کلمه عبور اشتباه است";
const wstring Base::errInvalidPwH = Base::errHBegin + Base::errInvalidPw + Base::errHEnd;
const wstring Base::errSamePw = L"کلمه عبور جدید و کلمه عبور فعلی یکسان است";
const wstring Base::errSamePwH = Base::errHBegin + Base::errSamePw + Base::errHEnd;
const wstring Base::msgPwChanged = L"کلمه عبور با موفقیت تغییر یافت";
const wstring Base::msgPwChangedH = Base::msgHBegin + Base::msgPwChanged + Base::msgHEnd;
const wstring Base::msgEmailChanged = L"ایمیل با موفقیت تغییر یافت";
const wstring Base::msgEmailChangedH = Base::msgHBegin + Base::msgEmailChanged + Base::msgHEnd;

const wstring Base::errDupContact = L"مخاطب موردنظر قبلا ایجاد شده است";
const wstring Base::errDupContactH = Base::errHBegin + Base::errDupContact + Base::errHEnd;

const wstring Base::msgSendMsgSuccess = L"پیام با موفقیت ارسال شد";
const wstring Base::msgSendMsgSuccessH = Base::msgHBegin + Base::msgSendMsgSuccess + Base::msgHEnd;

const wstring Base::errDupAcUser = L"نام کاربری مورد نظر قبلا ثبت شده است";
const wstring Base::errDupAcUserH = Base::errHBegin + Base::errDupAcUser + Base::errHEnd;
const wstring Base::msgAddAcademySuccess = L"آموزشگاه با موفقیت ثیت شد";
const wstring Base::msgAddAcademySuccessH = Base::msgHBegin + Base::msgAddAcademySuccess + Base::msgHEnd;
const wstring Base::msgEditAcademySuccess = L"آموزشگاه با موفقیت ویرایش شد";
const wstring Base::msgEditAcademySuccessH = Base::msgHBegin + Base::msgEditAcademySuccess + Base::msgHEnd;

const wstring Base::errDupAcCourseForAc = L"رشته مورد نظر قبلا برای آموزشگاه افزوده شده است";
const wstring Base::errDupAcCourseForAcH = Base::errHBegin + Base::errDupAcCourseForAc + Base::errHEnd;

const wstring Base::errPicInvalid = L"فایل ارسالی تصویر نمی باشد";
const wstring Base::errPicInvalidH = Base::errHBegin + Base::errPicInvalid + Base::errHEnd;
const wstring Base::errPicFormat = L"قالب تصویر نامعتبر می باشد";
const wstring Base::errPicFormatH = Base::errHBegin + Base::errPicFormat + Base::errHEnd;
const wstring Base::errPicFileSizeTooHigh = L"تصویر بسیار حجیم می باشد";
const wstring Base::errPicFileSizeTooHighH = Base::errHBegin + Base::errPicFileSizeTooHigh + Base::errHEnd;
const wstring Base::errPicFileSizeTooLow = L"تصویر بسیار کم حجم می باشد";
const wstring Base::errPicFileSizeTooLowH = Base::errHBegin + Base::errPicFileSizeTooLow + Base::errHEnd;
const wstring Base::errPicResRange = L"رزولوشن تصویر نامعتبر می باشد";
const wstring Base::errPicResRangeH = Base::errHBegin + Base::errPicResRange + Base::errHEnd;
const wstring Base::errPicResTooHigh = L"رزولوشن تصویر بسیار بالا است";
const wstring Base::errPicResTooHighH = Base::errHBegin + Base::errPicResTooHigh + Base::errHEnd;
const wstring Base::errPicResTooLow = L"رزولوشن تصویر بسیار پائین است";
const wstring Base::errPicResTooLowH = Base::errHBegin + Base::errPicResTooLow + Base::errHEnd;
const wstring Base::errPicSizeTooHigh = L"اندازه تصویر بسیار بزرگ است";
const wstring Base::errPicSizeTooHighH = Base::errHBegin + Base::errPicSizeTooHigh + Base::errHEnd;
const wstring Base::errPicSizeTooLow = L"اندازه تصویر بسیار کوچک است";
const wstring Base::errPicSizeTooLowH = Base::errHBegin + Base::errPicSizeTooLow + Base::errHEnd;
const wstring Base::errPicQualityTooLow = L"کیفیت تصویر بسیار پائین است";
const wstring Base::errPicQualityTooLowH = Base::errHBegin + Base::errPicQualityTooLow + Base::errHEnd;

const wstring Base::errUpFileTooLarge = L"فایل ارسالی بسیار حجیم می باشد";
const wstring Base::errUpFileTooLargeH = Base::errHBegin + Base::errUpFileTooLarge + Base::errHEnd;
const wstring Base::errPicCannotUp = L"امکان ارسال تصویر وجود ندارد";
const wstring Base::errPicCannotUpH = Base::errHBegin + Base::errPicCannotUp + Base::errHEnd;
const wstring Base::errPicNoPic = L"ارسال تصویر الزامی است";
const wstring Base::errPicNoPicH = Base::errHBegin + Base::errPicNoPic + Base::errHEnd;
const wstring Base::msgPicAutoConv = L"تصویر بصورت خودکار تبدیل خواهد شد";
const wstring Base::msgPicAutoConvH = Base::msgHBegin + Base::msgPicAutoConv + Base::msgHEnd;
const wstring Base::msgPicOK = L"تصویر ارسالی کاملا منطبق است";
const wstring Base::msgPicOKH = Base::msgHBegin + Base::msgPicOK + Base::msgHEnd;

const wstring Base::errDupMCardForM = L"کارت مورد نظر قبلا برای مربی افزوده شده است";
const wstring Base::errDupMCardForMH = Base::errHBegin + Base::errDupMCardForM + Base::errHEnd;

const wstring Base::errFoundNoTerms = L"دوره ای یافت نشد";
const wstring Base::errFoundNoTermsH = Base::errHBegin + Base::errFoundNoTerms + Base::errHEnd;

const wstring Base::errInvalidFollowingCode = L"کد رهگیری نامعتبر می باشد";
const wstring Base::errInvalidFollowingCodeH = Base::errHBegin + Base::errInvalidFollowingCode + Base::errHEnd;


/*const int Base::capTolerance = 1984;
int Base::capSeed = 0;
int Base::capResult = 0;
int Base::capN1 = 0;
int Base::capN2 = 0;
string Base::capImage;*/
const wstring Base::capTip = L"حاصل ضرب را وارد نمائید.";
/*int Base::capRotate = 0;
int Base::capSkew = 0;*/

const unsigned char Base::rootMasterKey[] = { 0x7e, 0x7c, 0x74, 0x5e, 0x30, 0x7D, 0x30, 0x52, 0x40, 0x2e, 0x26, 0x3f, 0x5f }; //"~|t^0}0R@.&?_"
const string Base::__rootMasterKey = EncDec::Encrypt(BString::StrFromByte(rootMasterKey, sizeof(rootMasterKey)));

const int Base::minLenUser = 6;
const int Base::maxLenUser = 16;
const int Base::minLenPw = 8;
const int Base::maxLenPw = 16;

const int Base::minLenEmailSubject = 1;
//const int Base::maxLenEmailSubject = 78;
const int Base::maxLenEmailSubject = 64;
const int Base::minLenEmailBody = 1;
const int Base::maxLenEmailBody = 1792;

const string Base::GetCurPath() {
    return curPath;
}

void Base::InitCurPath(const char *appPath) {
    string path(appPath);
    path = path.substr(0, path.rfind("/") + 1);
    curPath = path;
    if (chdir(curPath.c_str()) == -1) {
        throw Error(("Error changing directory to " + path).c_str());
    }
}

template<typename T, size_t size>
const int Base::GetArrLength(T(&)[size]) {
    return size;
}

/*void Base::GenCap() {
    GenCap((unsigned)time(NULL), false);
}

void Base::GenCap(int seed, bool decode) {
    capSeed = !decode ? seed : seed + capTolerance;
    srand(capSeed);
    capN1 = (rand() % 10) + 1;
    capN2 = (rand() % 10) + 1;
    capResult = capN1 * capN2;

    capRotate = (rand() % 7 - 3);
    capSkew = (rand() % 9 - 4);

    if (!decode) {
        capImage = "captcha.fcgi?" + BString::IntToStr(capSeed - capTolerance);
        //capImage = const_cast<char *>(("captcha.fcgi?" + BString::IntToStr(capSeed - capTolerance)).c_str());
    }
}*/

/*long Base::FileSize(const char *fileName) {
    long begin, end;
    ifstream file(fileName);
    begin = file.tellg();
    file.seekg (0, ios::end);
    end = file.tellg();
    file.close();
    return end - begin;
 }*/

bool Base::FileExists(const char *fileName) {
    ifstream file(fileName);
    if (file.good())
        return true;
    else
        return false;
    /*
#include <sys/stat.h>
    struct stat stFileInfo;
    int intStat;
    if(stat(fileName, &stFileInfo) == 0)
        return true;
    else
        return false;

#include <boost/filesystem.hpp>
bool Base::FileFound(const char *fileName) {
    if (boost::filesystem::exists(fileName))
        return true;
    else
        return false;
}
     */
}

void Base::FileErase(const char *fileName) {
    remove(fileName);
}

/*void Base::FileBinRead(const char *fileName) {
}*/

void Base::FileWrite(const char *fileName, const char *data) {
    ofstream file(fileName, ios::out | ios::trunc);
    //file.open(fileName.c_str(), ios::out | ios::trunc);
    if (file.is_open()) {
        file << data;
        file.close();
    }
}

void Base::Exec(const char *cmd) {
    system(cmd);
}

void Base::VacuumDB() {
    DB::Vacuum("../db/root.db");
    DB::Vacuum("../db/pics.db");
    /*Exec("sqlite3 ../db/root.db 'VACUUM;' & "
         "sqlite3 ../db/pics.db 'VACUUM;' &");*/
}

string Base::GenRandChars() {
        int len = 11;

        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";

        string res;

        srand((unsigned)time(NULL));

        for (int i = 0; i < len; ++i) {
                res += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return res;
}

