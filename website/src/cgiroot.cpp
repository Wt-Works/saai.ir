#include <map>

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WEnvironment>
#include <Wt/WFormWidget>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WTable>
#include <Wt/WValidator>
#include <Wt/WWidget>

//#include <GeoIP.h>

#include "clientinfo.hpp"
#include "serverinfo.hpp"

#include "cgiroot.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "cimage.hpp"
#include "db.hpp"
#include "div.hpp"

using namespace std;
using namespace Wt;
using namespace SAAIIR;
using namespace dbixx;

CgiRoot::CgiRoot(const WEnvironment& env) : WApplication(env) {
    InitEnv(env);

    db.Init("root.db");
    dbPics.Init("pics.db");

    root()->clear();

    string acp = IsReqACP(env);
    if (acp != "null") {
        if (acp == "ok")
            Initialize_ACPanel();
        else if (acp == "failed")
            GoAway_ACP();
        return;
    }

    root()->addWidget(IsReqRoot(env) ? Initialize_Root() : Initialize_Home());


////////////////
    /*setTitle(L".:: پنل مدیریت ::.");
    useStyleSheet("root-login.css");

    dbixx::row r;
    db.sql << "SELECT user, lip, lccode, lcname, ldate, ltime FROM [__root] "
            "WHERE user=?", "chroot", r;

   if(db.sql.single(r)) {
        r >> Base::loggedInUser >> Base::lastIP
                >> Base::lastCCode >> Base::lastCName
                >> Base::lastDate >> Base::lastTime;

        Initialize_RP();
   }*/
///////////////
}

CgiRoot::~CgiRoot() {
    delete clientInfo;
    delete serverInfo;
}


void CgiRoot::InitEnv(const WEnvironment& env) {
    clientInfo = new ClientInfo(env);
    serverInfo = new ServerInfo(env);
}

bool CgiRoot::IsReqRoot(const WEnvironment& env) {
    Http::ParameterMap map = env.getParameterMap();
    for (std::map<std::string, Http::ParameterValues>::const_iterator it = map.begin(); it != map.end(); ++it) {
        if (it->first == "root")
            return true;
    }

    return false;
}

string CgiRoot::IsReqACP(const WEnvironment& env) {
    Http::ParameterMap map = env.getParameterMap();
    for (std::map<std::string, Http::ParameterValues>::const_iterator it = map.begin(); it != map.end(); ++it) {
        if (it->first == "acp") {
            if (!env.getParameterValues(it->first).empty()) {
                std::string session = env.getParameterValues(it->first)[0];

                if (AcSessionValidate(session)) {
                    return "ok";
                }
            }

            return "failed";
        }
    }

    return "null";
}

void CgiRoot::GenCap() {
    int seed = (unsigned)time(NULL);
    srand(seed);
    __capResult = ((rand() % 10) + 1) * ((rand() % 10) + 1);
    __capImage = "captcha.fcgi?" + BString::IntToStr(seed - 1984);
}

bool CgiRoot::Validate(WFormWidget *widget) {
    switch (widget->validate()) {
    case WValidator::Valid:
        return true;
        break;
    case WValidator::InvalidEmpty:
        return false;
        break;
    case WValidator::Invalid:
        return false;
        break;
    default:
        return false;
        break;
    }
}

bool CgiRoot::ValidatePic(const string& file) {
    WDialog *dlg = new WDialog(L"ارسال تصویر");
    dlg->setModal(true);

    WContainerWidget *dvPicValidator = new Div(dlg->contents(), "dvPicValidator");

    WTable *tbl = new WTable(dvPicValidator);
    tbl->setStyleClass("tbl");

    tbl->elementAt(0, 0)->addWidget(new WText(L""));
    tbl->elementAt(0, 1)->addWidget(new WText(L"استاندارد"));
    tbl->elementAt(0, 2)->addWidget(new WText(L"تصویر ارسالی"));
    tbl->elementAt(0, 3)->addWidget(new WText(L"توضیحات"));

    tbl->elementAt(1, 0)->addWidget(new WText(L"قالب"));
    tbl->elementAt(1, 1)->addWidget(new WText(L"<div class=\"ltrCell\">JPEG</div>"));
    tbl->elementAt(2, 0)->addWidget(new WText(L"حجم"));
    tbl->elementAt(2, 1)->addWidget(new WText(L"<div class=\"ltrCell\">24 - 96 KB</div>"));
    tbl->elementAt(3, 0)->addWidget(new WText(L"رزولوشن"));
    tbl->elementAt(3, 1)->addWidget(new WText(L"<div class=\"ltrCell\">72 / 96 dpi</div>"));
    tbl->elementAt(4, 0)->addWidget(new WText(L"اندازه"));
    tbl->elementAt(4, 1)->addWidget(new WText(L"<div class=\"ltrCell\">480 x 480 - 720 x 720</div>"));
    tbl->elementAt(5, 0)->addWidget(new WText(L"کیفیت"));
    tbl->elementAt(5, 1)->addWidget(new WText(L"<div class=\"ltrCell\">--</div>"));

    string imgOK = "ok.png";
    string imgFail = "fail.png";

    int successAttempts = 0;
    bool fatal = false;

    if (file != "") {
        if (CImage::IsValidImage(file)) {
            if (CImage::IsValidImageFormat(file)) {
                tbl->elementAt(1, 2)->addWidget(new WImage(imgOK));
                ++successAttempts;
            }
            else {
                tbl->elementAt(1, 2)->addWidget(new WImage(imgFail));
                tbl->elementAt(1, 3)->addWidget(new WText(Base::errPicFormatH));
            }

            if (!CImage::IsImageFileSizeTooHigh(file)) {
                if (!CImage::IsImageFileSizeTooLow(file)) {
                    tbl->elementAt(2, 2)->addWidget(new WImage(imgOK));
                    ++successAttempts;
                }
                else {
                    tbl->elementAt(2, 2)->addWidget(new WImage(imgFail));
                    tbl->elementAt(2, 3)->addWidget(new WText(Base::errPicFileSizeTooLowH));

                    fatal = true;
                }
            }
            else {
                tbl->elementAt(2, 2)->addWidget(new WImage(imgFail));
                tbl->elementAt(2, 3)->addWidget(new WText(Base::errPicFileSizeTooHighH));
            }

            if (!CImage::IsImageResolutionTooHigh(file)) {
                if (!CImage::IsImageResolutionTooLow(file)) {
                    if (CImage::IsValidImageResolution(file)) {
                        tbl->elementAt(3, 2)->addWidget(new WImage(imgOK));
                        ++successAttempts;
                    }
                    else {
                        tbl->elementAt(3, 2)->addWidget(new WImage(imgFail));
                        tbl->elementAt(3, 3)->addWidget(new WText(Base::errPicResRangeH));
                    }
                }
                else {
                    tbl->elementAt(3, 2)->addWidget(new WImage(imgFail));
                    tbl->elementAt(3, 3)->addWidget(new WText(Base::errPicResTooLowH));

                    fatal = true;
                }
            }
            else {
                tbl->elementAt(3, 2)->addWidget(new WImage(imgFail));
                tbl->elementAt(3, 3)->addWidget(new WText(Base::errPicResTooHighH));
            }

            if (!CImage::IsImageSizeTooHigh(file)) {
                if (!CImage::IsImageSizeTooLow(file)) {
                    tbl->elementAt(4, 2)->addWidget(new WImage(imgOK));
                    ++successAttempts;
                }
                else {
                    tbl->elementAt(4, 2)->addWidget(new WImage(imgFail));
                    tbl->elementAt(4, 3)->addWidget(new WText(Base::errPicSizeTooLowH));

                    fatal = true;
                }
            }
            else {
                tbl->elementAt(4, 2)->addWidget(new WImage(imgFail));
                tbl->elementAt(4, 3)->addWidget(new WText(Base::errPicSizeTooHighH));
            }

            if (!fatal) {
                tbl->elementAt(5, 2)->addWidget(new WImage(imgOK));
                ++successAttempts;
            }
            else {
                tbl->elementAt(5, 2)->addWidget(new WImage(imgFail));
                tbl->elementAt(5, 3)->addWidget(new WText(Base::errPicQualityTooLowH));
            }
        }
        else {
            tbl->elementAt(1, 2)->addWidget(new WImage(imgFail));
            tbl->elementAt(1, 3)->addWidget(new WText(Base::errPicInvalidH));

            tbl->elementAt(2, 2)->addWidget(new WImage(imgFail));
            tbl->elementAt(3, 2)->addWidget(new WImage(imgFail));
            tbl->elementAt(4, 2)->addWidget(new WImage(imgFail));
            tbl->elementAt(5, 2)->addWidget(new WImage(imgFail));

            fatal = true;
        }
    }
    else {
        tbl->elementAt(1, 2)->addWidget(new WImage(imgFail));
        tbl->elementAt(1, 3)->addWidget(new WText(Base::errPicNoPicH));

        tbl->elementAt(2, 2)->addWidget(new WImage(imgFail));
        tbl->elementAt(3, 2)->addWidget(new WImage(imgFail));
        tbl->elementAt(4, 2)->addWidget(new WImage(imgFail));
        tbl->elementAt(5, 2)->addWidget(new WImage(imgFail));

        fatal = true;
    }

    if (successAttempts == 5) {
        dvPicValidator->addWidget(new WText(Base::msgPicOKH));
    }
    else {
        if (!fatal) {
            dvPicValidator->addWidget(new WText(Base::msgPicAutoConvH));
            CImage::OptimizeImage(file);
        }
        else {
            dvPicValidator->addWidget(new WText(Base::errPicCannotUpH));
        }
    }

    WContainerWidget *dvBtns = new WContainerWidget(dvPicValidator);
    WPushButton *ok = new WPushButton(L"تائید", dvBtns);
    //ok->clicked().connect(dlg, &WDialog::accept);
    ok->setStyleClass("btnDialogs");

    ValidatePicFlag = !fatal;

    WSignalMapper<WDialog *> *dlgMap = new WSignalMapper<WDialog *>(this);
    dlgMap->mapped().connect(this, &CgiRoot::ValidatePicClose);
    dlgMap->mapConnect(ok->clicked(), dlg);

    new WBreak(dvPicValidator);

    dlg->show();

    if (!fatal)
        return true;
    else
        return false;
}

void CgiRoot::ValidatePicClose(WDialog *sender) {
    delete sender;
    sender = NULL;

    if (ValidatePicFlag) {
        if (ValidatePicPostAction == "GetMCardDialog_ACP") {
            delete dlg;
            dlg = NULL;
            GetMCardDialog_ACP(ValidatePicPostArgs);
        }
    }
}
