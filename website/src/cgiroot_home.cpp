#include <Wt/WAnchor>
#include <Wt/WBreak>
#include <Wt/WButtonGroup>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WGroupBox>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMenu>
#include <Wt/Chart/WPieChart>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WRegExpValidator>
#include <Wt/WStackedWidget>
#include <Wt/WStandardItemModel>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>

#include <dbixx/dbixx.h>

#include "cgiroot.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "cdate.hpp"
#include "ccrypt.hpp"
#include "cimage.hpp"
#include "db.hpp"
#include "div.hpp"
#include "mailer.hpp"

using namespace std;
using namespace Wt;
using namespace Wt::Chart;
using namespace dbixx;
using namespace SAAIIR;
using namespace CDate;
using namespace CCrypt;

WWidget *CgiRoot::Initialize_Home() {
    GenCap();

    setTitle(Base::titleText);
    useStyleSheet("root-hm.css");

    WContainerWidget *homePage = new WContainerWidget();

    Div *dvBody = new Div(homePage, "dvBody");

    Div *dvNoScript = new Div(dvBody, "dvNoScript");
    dvNoScript->addWidget(new WText(L"<noscript>"
                                    "<div class=\"dvNoScript\">"
                                    "كاربر گرامي مرورگر شما از جاوا اسكريپت پشتيباني نمي نمايد، "
                                    "و يا اينكه جاوا اسكريپت در مرورگر شما غیر فعال مي باشد؛ "
                                    "جهت بهره گیری از تمامی امکانات وب سایت یکی از مرورگرهای "
                                    "<a href=\"http://www.srware.net/en/software_srware_iron_download.php\" target=\"_blank\">SRWare Iron</a>"
                                    ", "
                                    "<a href=\"http://www.getfirefox.com/\" target=\"_blank\">Mozilla Firefox</a>"
                                    " را توصيه مي نمائيم."
                                    "</div>"
                                    "</noscript>"));

    Div *dvHeader = new Div(dvBody, "dvHeader");
    Div *dvNavMenu = new Div(dvBody, "dvNavMenu");
    Div *dvMainTop = new Div(dvBody, "dvMainTop");
    Div *dvMainBottom = new Div(dvMainTop, "dvMainBottom");
    Div *dvMain = new Div(dvMainBottom, "dvMain");
    Div *dvLogo = new Div(dvMain, "dvLogo");
    dvMainWrapper = new Div(dvMain, "dvMainWrapper");
    Div *dvMainContent = new Div(dvMainWrapper, "dvMainContent");
    Div *dvMainSide = new Div(dvMainWrapper, "dvMainSide");
    Div *dvMainMenu = new Div(dvMainSide, "dvMainMenu");
    Div *dvMainFollowing = new Div(dvMainSide, "dvMainFollowing");
    Div *dvMainAcademies = new Div(dvMainSide, "dvMainAcademies");
    dvMainVote = new Div(dvMainSide, "dvMainVote");
    Div *dvBreak = new Div(dvMainWrapper, "dvBreak");
    Div *dvCopyright = new Div(dvBody, "dvCopyright");
    Div *dvGeoInfo = new Div(dvBody, "dvGeoInfo");
    Div *dvPowered = new Div(dvBody, "dvPowered");

    dvCopyright->addWidget(new WText("&copy; 2010 "
                                     "Iran Technical and Vocational Training Organization.<br />"));

    dvGeoInfo->addWidget(new WText(clientInfo->ip +
                                   "; "
                                   + clientInfo->countryCode +
                                   "; "
                                   + clientInfo->countryName +
                                   ";"));

    dvPowered->addWidget(new WText("<span style=\"color: #ccc\">This website powered entirely by</span> "
                                   "<a href=\"http://en.wikipedia.org/wiki/Free_and_open_source_software\" "
                                   "title=\"Free/Libre and Open Source Software\""
                                   "style=\"color: #000; text-decoration: none;\"> FLOSS</a><br/>"
                                   "<ul> " //"<ul class=\"lstPoweredBy\"> "
                                   "<li>Powered By</li>"
                                   "<li><a href=\"http://www.boost.org/\">Boost</a></li>"
                                   "<li><a href=\"http://www.cmake.org/\">CMake</a></li>"
                                   "<li><a href=\"http://www.cryptopp.com/\">Crypto++</a></li>"
                                   "<li><a href=\"http://art-blog.no-ip.info/wikipp/en/page/ref_dbixx/\">DbiXX</a></li>"
                                   "<li><a href=\"http://www.nongnu.org/fastcgipp/\">fastcgi++</a></li>"
                                   "<li><a href=\"http://www.freebsd.org/\">FreeBSD</a></li>"
                                   "<li><a href=\"http://gcc.gnu.org/\">GCC</a></li>"
                                   "<li><a href=\"http://www.maxmind.com/app/ip-location\">GeoIP</a></li>"
                                   "<li><a href=\"http://sourceforge.net/projects/jwsmtp/\">jwSMTP</a></li>"
                                   "<li><a href=\"http://libb64.sourceforge.net/\">libb64</a></li>"
                                   "<li><a href=\"http://libdbi.sourceforge.net/\">libdbi</a></li>"
                                   "<li><a href=\"http://libdbi-drivers.sourceforge.net/\">libdbi-drivers</a></li>"
                                   "<li><a href=\"http://nginx.org/\">nginx</a></li>"
                                   "<li><a href=\"http://www.imagemagick.org/Magick++/\">Magick++</a></li>"
                                   "<li><a href=\"http://www.perl.org/\">Perl</a></li>"
                                   "<li><a href=\"http://www.openbsd.org/faq/pf/\">PF</a></li>"
                                   "<li><a href=\"http://redmine.lighttpd.net/projects/spawn-fcgi/news\">spawn-fcgi</a></li>"
                                   "<li><a href=\"http://www.sqlite.org/\">SQLite</a></li>"
                                   "<li><a href=\"http://tinymce.moxiecode.com/\">TinyMCE</a></li>"
                                   "<li><a href=\"http://www.webtoolkit.eu/wt\">Wt</a></li>"
                                   "</ul>"
                                   "by "
                                   "<a href=\"http://www.babaei.net/\" "
                                   "title=\"The Official Website of M.S. Babaei\" "
                                   "style=\"color: #ccc; text-decoration: line-through; font-weight: normal; font-family: 'Arial Black'\">"
                                   "M.S. BABAEI</a>"));


    WStackedWidget *mainContents = new WStackedWidget(dvMainContent);
    mainContents->setId("mainContents");
    mainMenu = new WMenu(mainContents, Vertical, dvMainMenu);
    mainMenu->setRenderAsList(true);

    mainMenu->addItem
            (L"صفحه اصلی", CMainMenu(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem
            (L"آموزش نحوه ثبت نام", CHowTo(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem
            (L"سایت های مرتبط", CLinks(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem
            (L"تماس با ما", CContact(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem
            (L"درباره ی ما", CAbout(), WMenuItem::PreLoading)->setPathComponent("");

    dvMainFollowing->addWidget(new WText(L"<h5>پیگیری ثبت نام</h5>"));
    WContainerWidget *dvMainFollowingInner = new Div(dvMainFollowing, "dvMainFollowingInner");
    WTable *dvMainFollowingTable = new WTable(dvMainFollowingInner);
    followEdit = new WLineEdit(dvMainFollowingTable->elementAt(0, 0));
    new WText(L"کد رهگیری", dvMainFollowingTable->elementAt(0, 1));
    followCapEdit = new WLineEdit(dvMainFollowingTable->elementAt(1, 0));
    followCapEdit->setToolTip(Base::capTip);
    new WText(L"کد امنیتی", dvMainFollowingTable->elementAt(1, 1));
    followCaptcha = new WImage(__capImage, dvMainFollowingInner);
    followCaptcha->setStyleClass("capSide");
    followCaptcha->setAlternateText(Base::capTip);
    errFollowing = new WText(dvMainFollowingInner);
    WPushButton *btnMainFollowingOK = new WPushButton(L"تائید", dvMainFollowingInner);
    btnMainFollowingOK->setStyleClass("btnSide");
    WLengthValidator *followValidator = new WLengthValidator(11, 11);
    followValidator->setMandatory(true);
    followEdit->setValidator(followValidator);
    followCapValidator = new WIntValidator(__capResult, __capResult);
    followCapValidator->setMandatory(true);
    followCapEdit->setValidator(followCapValidator);
    followEdit->enterPressed().connect(this, &CgiRoot::FollowingOK);
    followCapEdit->enterPressed().connect(this, &CgiRoot::FollowingOK);
    btnMainFollowingOK->clicked().connect(this, &CgiRoot::FollowingOK);

    dvMainAcademies->addWidget(new WText(L"<h5>آموزشگاه های آزاد</h5>"));
    WContainerWidget *dvMainAcademiesInner = new Div(dvMainAcademies, "dvMainAcademiesInner");
    WTable *dvMainAcademiesTable = new WTable(dvMainAcademiesInner);
    academyUserEdit = new WLineEdit(dvMainAcademiesTable->elementAt(0, 0));
    new WText(L"نام کاربری", dvMainAcademiesTable->elementAt(0, 1));
    academyPwEdit = new WLineEdit(dvMainAcademiesTable->elementAt(1, 0));
    academyPwEdit->setEchoMode(Wt::WLineEdit::Password);
    new WText(L"کلمه عبور", dvMainAcademiesTable->elementAt(1, 1));
    academyCapEdit = new WLineEdit(dvMainAcademiesTable->elementAt(2, 0));
    academyCapEdit->setToolTip(Base::capTip);
    new WText(L"کد امنیتی", dvMainAcademiesTable->elementAt(2, 1));
    academyCaptcha = new WImage(__capImage, dvMainAcademiesInner);
    academyCaptcha->setStyleClass("capSide");
    academyCaptcha->setAlternateText(Base::capTip);
    errACLogin = new WText(dvMainAcademiesInner);
    WPushButton *btnMainAcademiesOK = new WPushButton(L"تائید", dvMainAcademiesInner);
    btnMainAcademiesOK->setStyleClass("btnSide");
    new WBreak(dvMainAcademiesInner);
    WText *forgetLink = new WText(L"<p style=\"text-align: left !important;\">کلمه عبور را فراموش کرده ام</p>", dvMainAcademiesInner);
    forgetLink->setStyleClass("link");
    IsForgetFormShownAcademy = false;
    forgetLink->clicked().connect(SLOT(this, CgiRoot::AcademyForgetForm));
    dvForgetFormWrapperAcademy = new Div(dvMainAcademiesInner, "dvForgetFormAcademy");
    WLengthValidator *academyUserValidator = new WLengthValidator(Base::minLenUser, Base::maxLenUser);
    academyUserValidator->setMandatory(true);
    academyUserEdit->setValidator(academyUserValidator);
    WLengthValidator *academyPwValidator = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    academyPwValidator->setMandatory(true);
    academyPwEdit->setValidator(academyPwValidator);
    academyCapValidator = new WIntValidator(__capResult, __capResult);
    academyCapValidator->setMandatory(true);
    academyCapEdit->setValidator(academyCapValidator);
    academyUserEdit->enterPressed().connect(this, &CgiRoot::AcademiesOK);
    academyPwEdit->enterPressed().connect(this, &CgiRoot::AcademiesOK);
    academyCapEdit->enterPressed().connect(this, &CgiRoot::AcademiesOK);
    btnMainAcademiesOK->clicked().connect(this, &CgiRoot::AcademiesOK);

    enum Vote { NoVote = 0, Standard = 1, Teacher = 2, Workshop = 3, TradeUnion = 4, Surveillance = 5 };
    WContainerWidget *dvMainVoteInner = new Div(dvMainVote, "dvMainVoteInner");
    dvMainVoteInner->addWidget(new WText(L"<h5>نظر سنجی</h5>"));
    WContainerWidget *dvMainVoteInnerRadio = new Div(dvMainVoteInner, "dvMainVoteInnerRadio");
    WVBoxLayout *dvMainVoteLayout = new WVBoxLayout(dvMainVoteInnerRadio);
    WGroupBox *dvMainVoteGroup = new WGroupBox(L"کدام عامل در پربار نمودن دوره های فنی و حرفه ای تاثیر بیشتری دارد؟");
    dvMainVoteLayout->addWidget(dvMainVoteGroup);
    dvMainVoteRadioGroup = new WButtonGroup(dvMainVoteLayout);
    WRadioButton *rdb;
    rdb = new Wt::WRadioButton(L"استانداردهای آموزش", dvMainVoteGroup);
    new Wt::WBreak(dvMainVoteGroup);
    dvMainVoteRadioGroup->addButton(rdb, Standard);
    rdb = new Wt::WRadioButton(L"معلومات مربیان", dvMainVoteGroup);
    new Wt::WBreak(dvMainVoteGroup);
    dvMainVoteRadioGroup->addButton(rdb, Teacher);
    rdb = new Wt::WRadioButton(L"تجهیزات کارگاهها", dvMainVoteGroup);
    new Wt::WBreak(dvMainVoteGroup);
    dvMainVoteRadioGroup->addButton(rdb, Workshop);
    rdb = new Wt::WRadioButton(L"استفاده از تجربه اصناف درآموزش", dvMainVoteGroup);
    new Wt::WBreak(dvMainVoteGroup);
    dvMainVoteRadioGroup->addButton(rdb, TradeUnion);
    rdb = new Wt::WRadioButton(L"نظارت بیشتر بر مراکز آموزشی", dvMainVoteGroup);
    new Wt::WBreak(dvMainVoteGroup);
    dvMainVoteRadioGroup->addButton(rdb, Surveillance);
    dvMainVoteRadioGroup->setCheckedButton(dvMainVoteRadioGroup->button(NoVote));
    dvMainVoteInnerRadio->setLayout(dvMainVoteLayout);
    WTable *dvMainVoteTableCap = new WTable(dvMainVoteInner);
    voteCapEdit = new WLineEdit(dvMainVoteTableCap->elementAt(0, 0));
    voteCapEdit->setToolTip(Base::capTip);
    new WText(L"کد امنیتی", dvMainVoteTableCap->elementAt(0, 1));
    voteCaptcha = new WImage(__capImage, dvMainVoteInner);
    voteCaptcha->setStyleClass("capSide");
    voteCaptcha->setAlternateText(Base::capTip);
    errVote = new WText(dvMainVoteInner);
    WPushButton *btnMainVoteOK = new WPushButton(L"تائید", dvMainVoteInner);
    btnMainVoteOK->setStyleClass("btnSide");
    voteCapValidator = new WIntValidator(__capResult, __capResult);
    voteCapValidator->setMandatory(true);
    voteCapEdit->setValidator(voteCapValidator);
    voteCapEdit->enterPressed().connect(this, &CgiRoot::VoteOK);
    btnMainVoteOK->clicked().connect(this, &CgiRoot::VoteOK);

    return homePage;
}

void CgiRoot::AcademyForgetForm() {
    if (!IsForgetFormShownAcademy) {
        IsForgetFormShownAcademy = true;

        WContainerWidget *rootForget = new WContainerWidget(dvForgetFormWrapperAcademy);
        errForgetAcademy = new WText(rootForget);

        Div *dvForgetForm = new Div(rootForget, "dvForgetForm");

        WTable *dvForgetFormTable = new WTable(dvForgetForm);
        forgetEmailEditAcademy = new WLineEdit(dvForgetFormTable->elementAt(0, 0));
        new WText(L"ایمیل", dvForgetFormTable->elementAt(0, 1));
        forgetCapEditAcademy = new WLineEdit(dvForgetFormTable->elementAt(1, 0));
        forgetCapEditAcademy->setToolTip(Base::capTip);
        new WText(L"کدامنیتی", dvForgetFormTable->elementAt(1, 1));

        WPushButton *btnForget = new WPushButton(L"یادآوری", rootForget);
        btnForget->setStyleClass("btnSide");

        forgetCapValidatorAcademy = new WIntValidator(__capResult, __capResult);
        forgetCapValidatorAcademy->setMandatory(true);
        forgetEmailValidatorAcademy = new WRegExpValidator(Base::regExpEmail);
        forgetEmailValidatorAcademy->setMandatory(true);

        forgetCapEditAcademy->setValidator(forgetCapValidatorAcademy);
        forgetEmailEditAcademy->setValidator(forgetEmailValidatorAcademy);
        forgetCapEditAcademy->enterPressed().connect(this, &CgiRoot::ForgetOKAcademy);
        forgetEmailEditAcademy->enterPressed().connect(this, &CgiRoot::ForgetOKAcademy);
        btnForget->clicked().connect(this, &CgiRoot::ForgetOKAcademy);
    }
    else {
        dvForgetFormWrapperAcademy->clear();
        IsForgetFormShownAcademy = false;
    }
}

void CgiRoot::ForgetOKAcademy() {
    if(Validate(forgetCapEditAcademy) && Validate(forgetEmailEditAcademy)) {
        CDate::Now n;

        row r;
        db.sql << "SELECT user, pw, email FROM [acusers] "
                "WHERE email=?", EncDec::Encrypt(forgetEmailEditAcademy->text().toUTF8()),
                r;

        if(db.sql.single(r)) {
            CDate::Now n;

            string user, pw, to;
            r >> user >> pw >> to;

            user = EncDec::Decrypt(user);
            pw = EncDec::Decrypt(pw);
            to = EncDec::Decrypt(to);

            Mailer::SendLoginRecovery(to, user, pw, n, serverInfo, clientInfo);

            Error_Root(Base::msgForgetH, errForgetAcademy);
            ReGenCap();
            academyUserEdit->setFocus();
        }
        else {
            Error_Root(Base::errForgetH, errForgetAcademy);
            ReGenCap();
            forgetEmailEditAcademy->setFocus();
        }
    }
    else {
       ReGenCap();
    }
}

void CgiRoot::ReGenCap() {
    GenCap();
    followCaptcha->setImageRef(__capImage);
    followCapValidator->setRange(__capResult, __capResult);
    academyCaptcha->setImageRef(__capImage);
    academyCapValidator->setRange(__capResult, __capResult);
    if (voteCaptcha) {
        voteCaptcha->setImageRef(__capImage);
        voteCapValidator->setRange(__capResult, __capResult);
    }
    if (contactCaptcha) {
        contactCaptcha->setImageRef(__capImage);
        contactCapValidator->setRange(__capResult, __capResult);
    }
    if (regTyroCaptcha) {
        regTyroCaptcha->setImageRef(__capImage);
        regTyroCapValidator->setRange(__capResult, __capResult);
    }
}

WWidget *CgiRoot::CMainMenu() {
    //return new WText(GetPageContent("صفحه اصلی"));
    //WContainerWidget *home = new WContainerWidget();
    dvRegister = new WContainerWidget();
    dvRegister->setId("dvRegister");
    dvRegister->addWidget(new WText(L"ثبت نام در آموزشگاه های آزاد"));
    dvRegister->addWidget(new WText(L"<br /><br />"));

    WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
    dvImage->addWidget(new WImage("flow0.png"));

    regProvinceCmb = new WComboBox();
    regCityCmb = new WComboBox();
    regTermTitleEdit = new WLineEdit();

    WContainerWidget *dvDateBeg = new WContainerWidget();
    dvDateBeg->setId("dvDate");
    regTermBegdateCmbY = new WComboBox(dvDateBeg);
    //regTermBegdateCmbY->addItem(L"");
    regTermBegdateCmbM = new WComboBox(dvDateBeg);
    //regTermBegdateCmbM->addItem(L"");
    regTermBegdateCmbD = new WComboBox(dvDateBeg);
    //regTermBegdateCmbD->addItem(L"");

    WContainerWidget *dvDateEnd = new WContainerWidget();
    dvDateEnd->setId("dvDate");
    regTermEnddateCmbY = new WComboBox(dvDateEnd);
    //regTermEnddateCmbY->addItem(L"");
    regTermEnddateCmbM = new WComboBox(dvDateEnd);
    //regTermEnddateCmbM->addItem(L"");
    regTermEnddateCmbD = new WComboBox(dvDateEnd);
    //regTermEnddateCmbD->addItem(L"");

    regTermDaypartCmb = new WComboBox();
    regTermDaypartCmb->addItem(L"");
    regTermDaypartCmb->addItem(L"صبح");
    regTermDaypartCmb->addItem(L"عصر");
    regTermDaypartCmb->addItem(L"روزهای زوج");
    regTermDaypartCmb->addItem(L"روزهای فرد");
    regTermDaypartCmb->addItem(L"یکسره");

    row r;
    result res;

    db.sql << "SELECT province FROM [provinces] ORDER BY province ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string pr;
            r >> pr;
            regProvinceCmb->addItem(BString::StrToWStr(pr));
        }
    }

    db.sql << "SELECT city FROM [cities] WHERE prcode="
            "(SELECT code FROM [provinces] WHERE province=?) "
            "ORDER BY city ASC;", "كرمانشاه";

    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string ct;
            r >> ct;
            regCityCmb->addItem(BString::StrToWStr(ct));
        }
    }

    regProvinceCmb->setCurrentIndex(regProvinceCmb->findText(L"كرمانشاه", MatchExactly));
    regCityCmb->setCurrentIndex(regCityCmb->findText(L"کرمانشاه", MatchExactly));

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y; i <= y + 2; ++i) {
        regTermBegdateCmbY->addItem(BString::IntToStr(i));
    }

    for (int i = y; i <= y + 5; ++i) {
        regTermEnddateCmbY->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        regTermBegdateCmbM->addItem(m);
        regTermEnddateCmbM->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        regTermBegdateCmbD->addItem(d);
        regTermEnddateCmbD->addItem(d);
    }

    string m = now.substr(5, 2).c_str();
    string d = now.substr(8, 2).c_str();
    m = (m.size() == 1 ? "0" + m : m);
    d = (d.size() == 1 ? "0" + d : d);

    regTermBegdateCmbY->setCurrentIndex(regTermBegdateCmbY->findText(BString::IntToWStr(y), MatchExactly));
    //regTermEnddateCmbY->setCurrentIndex(regTermEnddateCmbY->findText(BString::IntToWStr(y + 1), MatchExactly));
    regTermBegdateCmbM->setCurrentIndex(regTermBegdateCmbM->findText(BString::StrToWStr(m), MatchExactly));
    //regTermEnddateCmbM->setCurrentIndex(regTermEnddateCmbM->findText(BString::StrToWStr(m), MatchExactly));
    regTermBegdateCmbD->setCurrentIndex(regTermBegdateCmbD->findText(BString::StrToWStr(d), MatchExactly));
    //regTermEnddateCmbD->setCurrentIndex(regTermEnddateCmbD->findText(BString::StrToWStr(d), MatchExactly));

    regTermEnddateCmbY->setCurrentIndex(regTermEnddateCmbY->count() - 1);
    regTermEnddateCmbM->setCurrentIndex(regTermEnddateCmbM->count() - 1);
    regTermEnddateCmbD->setCurrentIndex(regTermEnddateCmbD->count() - 1);

    WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");
    WContainerWidget *dvRegInner = new Div(dvRegForm, "dvRegInner");
    WGridLayout *dvRegInnerLayout = new WGridLayout();

    dvRegInnerLayout->addWidget(regProvinceCmb, 0, 0);
    dvRegInnerLayout->addWidget(new WText(L"استان"
                                              ), 0, 1);

    dvRegInnerLayout->addWidget(regCityCmb, 1, 0);
    dvRegInnerLayout->addWidget(new WText(L"شهرستان"
                                              ), 1, 1);

    dvRegInnerLayout->addWidget(regTermTitleEdit, 2, 0);
    dvRegInnerLayout->addWidget(new WText(L"رشته موردنظر"
                                              ), 2, 1);

    dvRegInnerLayout->addWidget(dvDateBeg, 3, 0);
    dvRegInnerLayout->addWidget(new WText(L"دوره از"
                                              ), 3, 1);

    dvRegInnerLayout->addWidget(dvDateEnd, 4, 0);
    dvRegInnerLayout->addWidget(new WText(L"تا"
                                              ), 4, 1);

    dvRegInnerLayout->addWidget(regTermDaypartCmb, 5, 0);
    dvRegInnerLayout->addWidget(new WText(L"شیفت"
                                              ), 5, 1);

    regTyroCapEdit = new WLineEdit();
    regTyroCapEdit->setStyleClass("ltrInput");
    regTyroCapValidator = new WIntValidator(__capResult, __capResult);
    regTyroCapValidator->setMandatory(true);
    regTyroCapEdit->setValidator(regTyroCapValidator);
    regTyroCapEdit->enterPressed().connect(this, &CgiRoot::RegStep_2);

    dvRegInnerLayout->addWidget(regTyroCapEdit, 6, 0);
    dvRegInnerLayout->addWidget(new WText(L"کد امنیتی"
                                              ), 6, 1);

    dvRegInnerLayout->setVerticalSpacing(11);
    dvRegInnerLayout->setColumnStretch(0, 200);
    dvRegInnerLayout->setColumnStretch(1, 125);
    dvRegInner->resize(325, WLength::Auto);
    dvRegInner->setLayout(dvRegInnerLayout);


    regTyroCaptcha = new WImage(__capImage, dvRegForm);
    regTyroCaptcha->setStyleClass("capSide");
    regTyroCaptcha->setAlternateText(Base::capTip);


    WPushButton *next = new WPushButton(L"گام بعدی", dvRegForm);
    next->setStyleClass("btnHome");


    dvRegister->addWidget(new WText(L"<br /><br /><br /><hr /><br />"));
    dvRegister->addWidget(new WText(L"اخبار و تازه ها"));
    dvRegister->addWidget(new WText(GetPageContent("صفحه اصلی")));

    regProvinceCmb->sactivated().connect(this, &CgiRoot::RegProvinceCmbChanged);

    regProvinceCmb->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regCityCmb->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermTitleEdit->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermBegdateCmbY->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermBegdateCmbM->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermBegdateCmbD->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermEnddateCmbY->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermEnddateCmbM->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermEnddateCmbD->enterPressed().connect(this, &CgiRoot::RegStep_2);
    regTermDaypartCmb->enterPressed().connect(this, &CgiRoot::RegStep_2);
    next->clicked().connect(this, &CgiRoot::RegStep_2);

    return dvRegister;
}

void CgiRoot::RegProvinceCmbChanged(WString pr) {
    regCityCmb->clear();

    result res;
    row r;

    db.sql << "SELECT city FROM [cities] WHERE prcode="
            "(SELECT code FROM [provinces] WHERE province=?) "
            "ORDER BY city ASC;", pr.toUTF8();

    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string ct;
            r >> ct;
            regCityCmb->addItem(BString::StrToWStr(ct));
        }
    }
}

void CgiRoot::ReturnHome() {
    redirect(".");
    quit();
}

void CgiRoot::RegStep_2() {
    if (!Validate(regTyroCapEdit)) {
        ReGenCap();
        return;
    }

    string regProvince = regProvinceCmb->currentText().toUTF8();
    string regCity = regCityCmb->currentText().toUTF8();
    string regTermTitle = BString::Trim(regTermTitleEdit->text().toUTF8());
    string regTermBegdateY = regTermBegdateCmbY->currentText().toUTF8();
    string regTermBegdateM = regTermBegdateCmbM->currentText().toUTF8();
    string regTermBegdateD = regTermBegdateCmbD->currentText().toUTF8();
    string regTermEnddateY = regTermEnddateCmbY->currentText().toUTF8();
    string regTermEnddateM = regTermEnddateCmbM->currentText().toUTF8();
    string regTermEnddateD = regTermEnddateCmbD->currentText().toUTF8();
    string regTermDaypart = regTermDaypartCmb->currentText().toUTF8();

    dvMainWrapper->clear();

    dvRegister = new WContainerWidget(dvMainWrapper);

    Div *dvReturnHome = new Div(dvRegister, "dvReturnHome");
    WPushButton *returnHome = new WPushButton(L"بازگشت به صفحه اصلی", dvReturnHome);
    returnHome->clicked().connect(this, &CgiRoot::ReturnHome);

    WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
    dvImage->addWidget(new WImage("flow1.png"));

    WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");

    result res;
    row r;

    string prCode, ctCode;

    db.sql << "SELECT code FROM [provinces] "
            "WHERE province=?;",
            regProvince, r;

    if(db.sql.single(r)) {
        r >> prCode;
    }

    db.sql << "SELECT code FROM [cities] "
            "WHERE city=?;",
            regCity, r;

    if(db.sql.single(r)) {
        r >> ctCode;
    }

    db.sql << "SELECT code, name FROM [academies] "
            "WHERE prcode=? AND ctcode=?;", prCode, ctCode;

    db.sql.fetch(res);

    bool found = false;
    bool noCourseMessage = true;

    if (res.rows() > 0) {
        int i = 0;
        WTable *step2Table = new WTable(dvRegForm);
        step2Table->setStyleClass("tbl");

        while(res.next(r)) {
            string acCode, acName;
            r >> acCode >> acName;

            result resSt;
            row rSt;

            db.sql << "SELECT title, code, totaltm FROM [standards] "
                    "WHERE title LIKE ?;", "%" + regTermTitle + "%";

            db.sql.fetch(resSt);

            if (resSt.rows() > 0) {
                while(resSt.next(rSt)) {
                    string stTitle, stCode, totaltm;
                    rSt >> stTitle >> stCode >> totaltm;

                    result resTr;
                    row rTr;

                    db.sql << "SELECT code, acmcode, begdate, enddate, daypart, begtime, endtime, loc "
                            "FROM [acterms] "
                            "WHERE accode=? AND stcode=?;", acCode, stCode;

                    db.sql.fetch(resTr);

                    if (resTr.rows() > 0) {
                        while(resTr.next(rTr)) {
                            string actCode, acmCode, begdate, enddate, daypart, begtime, endtime, loc;
                            rTr >> actCode >> acmCode >> begdate >> enddate >> daypart >>
                                    begtime >> endtime >> loc;

                            int beg = BString::StrToInt((regTermBegdateY + regTermBegdateM + regTermBegdateD));
                            int end = BString::StrToInt(regTermEnddateY + regTermEnddateM + regTermEnddateD);
                            int bd = BString::StrToInt(begdate);
                            int ed = BString::StrToInt(enddate);

                            if ((bd >= beg && ed <= end) &&
                                (regTermDaypart == daypart || regTermDaypart == "")) {
                                found = true;
                                noCourseMessage = false;
                            }

                            if (found) {
                                if (i == 0) {
                                    step2Table->elementAt(0, 0)->addWidget(new WText(L"عنوان دوره"));
                                    step2Table->elementAt(0, 1)->addWidget(new WText(L"ساعت آموزش"));
                                    step2Table->elementAt(0, 2)->addWidget(new WText(L"آموزشگاه"));
                                    step2Table->elementAt(0, 3)->addWidget(new WText(L"مدت دوره"));
                                    step2Table->elementAt(0, 4)->addWidget(new WText(L"شیفت"));
                                    step2Table->elementAt(0, 5)->addWidget(new WText(L"ساعت کلاس"));
                                    step2Table->elementAt(0, 6)->addWidget(new WText(L"مربی"));
                                    step2Table->elementAt(0, 7)->addWidget(new WText(L"محل برگزاری"));
                                    step2Table->elementAt(0, 8)->addWidget(new WText(L"ثبت نام"));

                                    step2Table->elementAt(0, 0)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 1)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 2)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 3)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 4)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 5)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 6)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 7)->setStyleClass("tblHeader");
                                    step2Table->elementAt(0, 8)->setStyleClass("tblHeader");
                                }
                                ++i;

                                row rAcM;

                                db.sql << "SELECT lname FROM [acmentors] "
                                        "WHERE code=?;", acmCode;

                                string acmName;
                                if(db.sql.single(rAcM)) {
                                    rAcM >> acmName;
                                }

                                begdate.insert(4, "/");
                                begdate.insert(7, "/");
                                enddate.insert(4, "/");
                                enddate.insert(7, "/");

                                begtime.insert(2, ":");
                                endtime.insert(2, ":");

                                string date = "از " + begdate + "<br />تا " + enddate;
                                string time = "از " + begtime + "<br />تا " + endtime;

                                step2Table->elementAt(i, 0)->addWidget(
                                        new WText(BString::StrToWStr(stTitle)));
                                step2Table->elementAt(i, 1)->addWidget(
                                        new WText(BString::StrToWStr(totaltm)));
                                step2Table->elementAt(i, 3)->addWidget(
                                        new WText(BString::StrToWStr(date)));
                                step2Table->elementAt(i, 4)->addWidget(
                                        new WText(BString::StrToWStr(daypart)));
                                step2Table->elementAt(i, 5)->addWidget(
                                        new WText(BString::StrToWStr(time)));
                                step2Table->elementAt(i, 7)->addWidget(
                                        new WText(BString::StrToWStr(loc)));

                                WAnchor *acNameAnchor = new WAnchor("javascript:;",
                                                                    BString::StrToWStr(acName));
                                WAnchor *acmNameAnchor = new WAnchor("javascript:;",
                                                                     BString::StrToWStr(acmName));

                                step2Table->elementAt(i, 2)->addWidget(acNameAnchor);
                                step2Table->elementAt(i, 6)->addWidget(acmNameAnchor);

                                WSignalMapper<WAnchor *> *acMap = new WSignalMapper<WAnchor *>(this);
                                acMap->mapped().connect(this, &CgiRoot::MoreInfoAcademiesTableCell_RP);
                                acMap->mapConnect(acNameAnchor->clicked(), acNameAnchor);
                                acNameAnchor->setAttributeValue("dbid", BString::StrToWStr(acCode));

                                WSignalMapper<WAnchor *> *acmMap = new WSignalMapper<WAnchor *>(this);
                                acmMap->mapped().connect(this, &CgiRoot::MoreInfoMentorsTableCell_ACP);
                                acmMap->mapConnect(acmNameAnchor->clicked(), acmNameAnchor);
                                acmNameAnchor->setAttributeValue("dbid", BString::StrToWStr(acmCode));
                                acmNameAnchor->setAttributeValue("accode", BString::StrToWStr(acCode));

                                WSignalMapper<WPushButton *> *regMap =
                                        new WSignalMapper<WPushButton *>(this);
                                regMap->mapped().connect(this, &CgiRoot::RegStep_3);
                                WPushButton *regButton = new WPushButton(L"ثبت نام");
                                regMap->mapConnect(regButton->clicked(), regButton);
                                regButton->setStyleClass("tblButton");

                                regButton->setAttributeValue("accode", BString::StrToWStr(acCode));
                                regButton->setAttributeValue("actcode", BString::StrToWStr(actCode));
                                regButton->setAttributeValue("sttitle", BString::StrToWStr(stTitle));
                                regButton->setAttributeValue("begdate", BString::StrToWStr(begdate));
                                regButton->setAttributeValue("enddate", BString::StrToWStr(enddate));
                                regButton->setAttributeValue("daypart", BString::StrToWStr(daypart));
                                regButton->setAttributeValue("begtime", BString::StrToWStr(begtime));
                                regButton->setAttributeValue("endtime", BString::StrToWStr(endtime));
                                regButton->setAttributeValue("loc", BString::StrToWStr(loc));
                                regButton->setAttributeValue("acmname", BString::StrToWStr(acmName));

                                step2Table->elementAt(i, 8)->addWidget(regButton);
                            }

                            found = false;
                        }
                    }
                }
            }
            else {
                break;
            }
        }
    }

    if (noCourseMessage)
        new WText(Base::errFoundNoTermsH, dvRegForm);
}

void CgiRoot::RegStep_3(WPushButton *sender) {
    regTermAcCode = sender->attributeValue("accode").toUTF8();
    regTermActCode = sender->attributeValue("actcode").toUTF8();
    regTermStTitle = sender->attributeValue("sttitle").toUTF8();
    regTermBegdate = sender->attributeValue("begdate").toUTF8();
    regTermEnddate = sender->attributeValue("enddate").toUTF8();
    regTermDaypart = sender->attributeValue("daypart").toUTF8();
    regTermBegtime = sender->attributeValue("begtime").toUTF8();
    regTermEndtime = sender->attributeValue("endtime").toUTF8();
    regTermLoc = sender->attributeValue("loc").toUTF8();
    regTermAcmName = sender->attributeValue("acmname").toUTF8();

    dvMainWrapper->clear();

    dvRegister = new WContainerWidget(dvMainWrapper);

    Div *dvReturnHome = new Div(dvRegister, "dvReturnHome");
    WPushButton *returnHome = new WPushButton(L"بازگشت به صفحه اصلی", dvReturnHome);
    returnHome->clicked().connect(this, &CgiRoot::ReturnHome);

    WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
    dvImage->addWidget(new WImage("flow2.png"));

    WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");

    WContainerWidget *dvRegNotice = new Div(dvRegForm, "dvRegNotice");

    new WText(L"<br /><br />"
              "دوره انتخابی شما "
              L"<strong>" +
              BString::StrToWStr(regTermStTitle) +
              L"</strong>"
              " تاریخ برگزاری از "
              "<strong>" +
              BString::StrToWStr(regTermBegdate) +
              L"</strong>"
              " تا "
              "<strong>" +
              BString::StrToWStr(regTermEnddate) +
              L"</strong>"
              " شیفت "
              "<strong>" +
              BString::StrToWStr(regTermDaypart) +
              L"</strong>"
              " ساعت "
              "<strong>" +
              BString::StrToWStr(regTermBegtime) +
              L"</strong>"
              " تا "
              "<strong>" +
              BString::StrToWStr(regTermEndtime) +
              L"</strong>"
              " با مربیگری "
              "<strong>" +
              BString::StrToWStr(regTermAcmName) +
              L"</strong>"
              " در محل آموزشگاه "
              "<strong>" +
              BString::StrToWStr(regTermLoc) +
              L"</strong>"
              " می باشد."
              "<br /><br /><br /><br />"
              "کاربر گرامی هزینه دوره انتخابی شما "
              "<strong>"
              "30000"
              "</strong>"
              " ریال می باشد که باید به حساب "
              "<strong>"
              "234567876543 بانک ملت شعبه حافظیه"
              "</strong>"
              " پرداخت شود."
              "<br /><br />", dvRegNotice);

    WPushButton *accept = new WPushButton(L"دوره پیشنهادی پذیرفته می شود و به مرحله ارسال مدارک می روم",
                                          dvRegNotice);
    accept->clicked().connect(this, &CgiRoot::RegStep_4);
}

void CgiRoot::RegStep_4() {
    dvMainWrapper->clear();

    dvRegister = new WContainerWidget(dvMainWrapper);

    Div *dvReturnHome = new Div(dvRegister, "dvReturnHome");
    WPushButton *returnHome = new WPushButton(L"بازگشت به صفحه اصلی", dvReturnHome);
    returnHome->clicked().connect(this, &CgiRoot::ReturnHome);

    WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
    dvImage->addWidget(new WImage("flow3.png"));

    WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");

    tyroFNameEdit = new WLineEdit();
    tyroLNameEdit = new WLineEdit();
    tyroSexCmb = new WComboBox();
    tyroFatherSNameEdit = new WLineEdit();
    tyroNationalCodeEdit = new WLineEdit();
    tyroBirthIdEdit = new WLineEdit();
    WContainerWidget *dvDate = new WContainerWidget();
    dvDate->setId("dvDate");
    tyroBirthDateCmbY = new WComboBox(dvDate);
    tyroBirthDateCmbM = new WComboBox(dvDate);
    tyroBirthDateCmbD = new WComboBox(dvDate);
    tyroBirthlocCmb = new WComboBox();
    tyroBirthEmissionLocCmb = new WComboBox();
    tyroGraduateCertCmb = new WComboBox();
    tyroGraduateCourseEdit = new WLineEdit();
    tyroJobCmb = new WComboBox();
    tyroAddrTArea = new WTextArea();
    tyroTelEdit = new WLineEdit();
    tyroMobileEdit = new WLineEdit();
    tyroEmailEdit = new WLineEdit();

    tyroSexCmb->addItem(L"");
    tyroSexCmb->addItem(L"مرد");
    tyroSexCmb->addItem(L"زن");

    tyroBirthDateCmbY->addItem("");
    tyroBirthDateCmbM->addItem("");
    tyroBirthDateCmbD->addItem("");

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y - 10; i >= 1310; --i) {
        tyroBirthDateCmbY->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        tyroBirthDateCmbM->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        tyroBirthDateCmbD->addItem(d);
    }

    tyroBirthlocCmb->addItem(L"");
    tyroBirthEmissionLocCmb->addItem(L"");

    result res;
    row r;

    db.sql << "SELECT province FROM [provinces] ORDER BY province ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string pr;
            r >> pr;
            tyroBirthlocCmb->addItem(BString::StrToWStr(pr));
            tyroBirthEmissionLocCmb->addItem(BString::StrToWStr(pr));
        }
    }

    tyroGraduateCertCmb->addItem(L"");
    tyroGraduateCertCmb->addItem(L"بیسواد");
    tyroGraduateCertCmb->addItem(L"زیر دیپلم");
    tyroGraduateCertCmb->addItem(L"دیپلم");
    tyroGraduateCertCmb->addItem(L"کاردان");
    tyroGraduateCertCmb->addItem(L"کارشناس");
    tyroGraduateCertCmb->addItem(L"کارشناس ارشد");
    tyroGraduateCertCmb->addItem(L"دکترا");

    tyroJobCmb->addItem(L"");
    tyroJobCmb->addItem(L"بیکار");
    tyroJobCmb->addItem(L"دانش آموز");
    tyroJobCmb->addItem(L"دانشجو");
    tyroJobCmb->addItem(L"شاغل");


    Div *tyroForm = new Div(dvRegForm, "dvTyroFormWarpper");
    Div *dvTyroForm = new Div(tyroForm, "dvTyroForm");
    WGridLayout *dvTyroFormLayout = new WGridLayout();

    dvTyroFormLayout->addWidget(tyroFNameEdit, 0, 0);
    dvTyroFormLayout->addWidget(new WText(L"نام"
                                               ), 0, 1);

    dvTyroFormLayout->addWidget(tyroLNameEdit, 1, 0);
    dvTyroFormLayout->addWidget(new WText(L"نام خانوادگی"
                                               ), 1, 1);

    dvTyroFormLayout->addWidget(tyroSexCmb, 2, 0);
    dvTyroFormLayout->addWidget(new WText(L"جنسیت"
                                               ), 2, 1);

    dvTyroFormLayout->addWidget(tyroFatherSNameEdit, 3, 0);
    dvTyroFormLayout->addWidget(new WText(L"نام پدر"
                                               ), 3, 1);

    dvTyroFormLayout->addWidget(tyroNationalCodeEdit, 4, 0);
    dvTyroFormLayout->addWidget(new WText(L"کد ملی"
                                               ), 4, 1);

    dvTyroFormLayout->addWidget(tyroBirthIdEdit, 5, 0);
    dvTyroFormLayout->addWidget(new WText(L"شماره شناسنامه"
                                               ), 5, 1);

    dvTyroFormLayout->addWidget(dvDate, 6, 0);
    dvTyroFormLayout->addWidget(new WText(L"تاریخ تولد"
                                               ), 6, 1);

    dvTyroFormLayout->addWidget(tyroBirthlocCmb, 7, 0);
    dvTyroFormLayout->addWidget(new WText(L"محل تولد"
                                               ), 7, 1);

    dvTyroFormLayout->addWidget(tyroBirthEmissionLocCmb, 8, 0);
    dvTyroFormLayout->addWidget(new WText(L"محل صدور شناسنامه"
                                               ), 8, 1);

    dvTyroFormLayout->addWidget(tyroGraduateCertCmb, 9, 0);
    dvTyroFormLayout->addWidget(new WText(L"آخرین مدرک تحصیلی"
                                               ), 9, 1);

    dvTyroFormLayout->addWidget(tyroGraduateCourseEdit, 10, 0);
    dvTyroFormLayout->addWidget(new WText(L"رشته تحصیلی"
                                               ), 10, 1);

    dvTyroFormLayout->addWidget(tyroJobCmb, 11, 0);
    dvTyroFormLayout->addWidget(new WText(L"وضعیت اشتغال"
                                               ), 11, 1);

    dvTyroFormLayout->addWidget(tyroAddrTArea, 12, 0);
    dvTyroFormLayout->addWidget(new WText(L"نشانی محل سکونت"
                                               ), 12, 1);

    dvTyroFormLayout->addWidget(tyroTelEdit, 13, 0);
    dvTyroFormLayout->addWidget(new WText(L"تلفن تماس"
                                               ), 13, 1);


    dvTyroFormLayout->addWidget(tyroMobileEdit, 14, 0);
    dvTyroFormLayout->addWidget(new WText(L"شماره همراه"
                                               ), 14, 1);


    dvTyroFormLayout->addWidget(tyroEmailEdit, 15, 0);
    dvTyroFormLayout->addWidget(new WText(L"پست الکترونیکی"
                                               ), 15, 1);

    dvTyroFormLayout->setVerticalSpacing(11);
    dvTyroFormLayout->setColumnStretch(0, 225);
    dvTyroFormLayout->setColumnStretch(1, 175);
    dvTyroForm->resize(400, WLength::Auto);
    dvTyroForm->setLayout(dvTyroFormLayout);

    errTyroForm = new WText("");
    tyroForm->addWidget(errTyroForm);

    WTextArea *agreement = new WTextArea(tyroForm);
    agreement->setReadOnly(true);
    agreement->setStyleClass("agreementBox");
    agreement->setText(
                       L".:: مقررات انضباطی كارآموزان آموزشگاه های آزاد فنی و حرفه ای ::.\n"
                       "1- هر كارآموز موظف است در طی دوره آموزشی موازین اسلامی و اخلاقی و مقررات سازمان و آموزشگاه مربوطه را به ترتیب زیر مراعات نماید .\n"
                       "الف: رعایت ادب و احترام و حسن برخورد نسبت به موسس و مدیر و مربی ... و دیگر كاركنان آموزشگاه\n"
                       "ب: رعایت حسن رفتار و اخلاق و همكاری با سایر كارآموزان و اجتناب از شوخی و حركات غیرمعقول در طول ساعات حضور در آموزشگاه .\n"
                       "ج:  حضور به موقع و مستمر در آموزشگاه ( مطابق برنامه تعیین شده ) و خروج از آن در پایان ساعات مقرر . \n"
                       "2-  خروج كارآموزان از آموزشگاه تنها با موافقت رسمی مسئولین آموزشگاه و در صورت لزوم با درخواست كتبی ولی یا قیم قانونی كارآموز و ارائه دلایل منطقی امكان پذیر می باشد . \n"
                       "3-  كارآموز موظف است مطابق برنامه تدوین شده از طرف آموزشگاه در برنامه های آموزشی (تئوری و عملی) حضور داشته باشد . \n"
                       "4-  كارآموز موظف به رعایت بهداشت و نظافت شخصی و همكاری گروهی با سایر كارآموزان و مسئولین ذیربط (موسس ، مدیر و مربی) در امور زیر می باشد : \n"
                       "الف)  پاكیزه نگهداشتن فضای آموزشگاه.\n"
                       "ب) انجام امور عمومی كارگاه آموزشی ، از قبیل نظافت كارگاه و تجهیزات و لوازم آموزشی برابر استاندارد . \n"
                       "5-  كارآموز موظف به استفاده از لباس مناسب و رعایت شئونات اسلامی می باشد :\n"
                       "الف) كارآموز موظف است از پوشیدن لباس های تنك و چسبان ، آرایش نامناسب و آشكار كردن موی سر و زینت‌آلات زنانه كاملاً اجتناب نماید . \n"
                       "ب)  انتشار و توزیع هرگونه كتاب ، جزوه ، اطلاعیه ، بروشور ، لوح فشرده و سایر موارد مشابه غیرمجاز اكیداً ممنوع است . \n"
                       "ج)  هرنوع انجام تبلیغات و هواداری از پایگاه های اطلاع رسانی و تشكل های غیرقانونی، گروهك های وابسته و منحرف ، فرقه های ضاله و عرفانهای كاذب كاملاً قدغن می باشد . \n"
                       "تبصره : در صورتی كه هر یك از موارد فوق در آموزشگاه های آزاد مشاهده و ثابت شود علاوه بر كارآموز ، موسس و مدیر آموزشگاه نیز تحت پیگرد قانونی قرار خواهند گرفت . \n"
                       "نحوه تشویق : \n"
                       "6-  توجه و اهتمام كارآموز نسبت به وظایف مندرج در ماده 2 این دستورالعمل مثبت تلقی شده و شایسته تقدیر است . بدیهی است كارآموزانی كه وظایف خود را بنحوشایسته انجام دهند از طریق زیر و با رعایت ترتیب مورد تشویق قرار می گیرند . \n"
                       "الف)  تشویق شفاهی در كلاس در حضور كارآموزان آموزشگاه . \n"
                       "ب)  تشویق كتبی و اعلام آن به ولی كارآموز . \n"
                       "ج)  تشویق كتبی و شفاهی در اجتماع كلیه كارآموزان و با حضور ولی وی . \n"
                       "هـ)  اعطای جایزه با لوح تقدیر با امضای موسس ، مدیر آموزشگاه در اجتماع كلیه كارآموزان و با حضور اولیاء كارآموزانی كه تقدیر می شوند . \n"
                       "و)  تشویق كتبی و اعطای لوح تقدیر از طرف مسئولین ذیربط در اداره كل با پیشنهاد موسس و مدیر آموزشگاه .  \n"
                       "مقررات مربوط به غیبت : \n"
                       "7-  عدم حضور كارآموز در آموزشگاه در ساعات تعیین شده جهت فراگیری ، مهارت ، حرفه یا رشته معینی كه قبلاً در آن ثبت نام نموده است ، غیبت محسوب می شود . \n"
                       "عدم حضور ، مكرر و یا متناوب كارآموز بر حسب نوع آن موجب محرومیت هائی خواهد شد كه در دستورالعمل مشخص گردیده است . \n"
                       "8-  غیبت موجه به آن دسته از غیبت هائی اطلاق می گردد كه با اطلاع و اجازه قبلی از آموزشگاه و یا بدون اطلاع و اجازه قبلی و بعلت حوادث پیش بینی شده (بیماری ، فوت بستگان و...) كه پیش آید صورت گیرد . \n"
                       "تبصره : در مواردی كه غیبت با اجازه قبلی آموزشگاه نباشد ارائه گواهی و یا مدرك معتبر الزامی است . \n"
                       "9-  در صورت غیبت موجه ، آموزشگاه همكاری لازم را برای جبران مهارت عقب افتاده كارآموز خواهد نمود . \n"
                       "10-  غیبت غیر موجه به آن دسته از غیبت هائی اطلاق می گردد كه بدون اطلاع و اجازه قبلی آموزشگاه و بدون عذر موجه باشد . \n"
                       "11-  در صورت غیبت غیرموجه (بیش از 3 روز یا 5 جلسه) در طول دوره و اُفت آموزشی و مردودشدن در آزمونهای پایان دوره ، كارآموز هیچگونه حقی در مورد استفاده از دوره های آموزشی آتی و جبرانی نخواهد داشت و مسئولین نیز متوجه آموزشگاه نخواهد بود . \n"
                       "12-  در مواردیكه آموزشگاه بخواهد برای مدتی معین و بنابه دلایلی موجه كه به تائید سازمان رسیده باشد (این مدت زائد بر 2 سال نمی تواند باشد) فعالیت آموزشی خود را متوقف نماید ، موظف است دوره آموزشی جاری خود را با تمام برساند و در صورتیكه آموزشگاه قبل از اختتام دوره آموزشی تعطیل گردد ، ضمن اعلام به سازمان، باید توافق كتبی كلیه كارآموزان را اخذ و نسبت به باز پرداخت شهریه آنان نیز اقدام نماید . \n"
                       "موسس موظف است كلیه مدارك مربوطه را به سازمان ارائه نماید . \n"
                       "13-  آموزشگاه موظف است جهت مطلع نمودن سرپرست یا قیم كارآموز ( چنانچه كارآموز كمتر از 15 سال تمام سن داشته باشد) از غیبتش در هنگام ثبت نام ، آدرس ، شماره تلفن و نمونه امضاء را جهت دریافت گواهی غیبت اخذ نماید . \n"
                       "اصولاً قرارداد كارآموزی در اینگونه موارد فی ما بین آموزشگاه و سرپرست یا قیم كارآموز منعقد خواهد شد . \n"
                       "14-  كارآموز موظف است علت غیبت خود را به آموزشگاه اعلام نماید لذا در صورت موجه بودن با ارائه گواهی در كلاس حضور یابد . \n"
                       "15-  آموزشگاه موظف است بعد از سه روز غیبت موجه (متوالی یا متناوب) كارآموز ، نسبت به تسلیم اخطاریه وی و یا ارسال رونوشت آن به والدین كارآموز (چنانچه كارآموز كمتر از 15 سال سن) داشته باشد اقدام نماید . \n"
                       "تبصره 1 : در صورتیكه كارآموز بعلت بی نظمی و غیبت یا تاخیر ورود و تعجیل در خروج قبلا تا دو اخطار داشته باشد و مجدداً اقدام به غیبت غیر موجه و یا بی نظمی نماید از آموزشگاه اخراج خواهد شد . \n"
                       "تبصره 2 : اخراج كارآموزان خاطی زیر نظر كمیته ای متشكل از موسس ، مدیر آموزشگاه ، مربی و نماینده سازمان صورت خواهد گرفت . \n"
                       "تبصره 3 : حق اعتراض برای كارآموز اخراجی محفوظ است و با ارائه مدارك كافی دال بر رد تخلفات و... به اداره كل بررسی و تصمیم گیری لازم بعمل خواهد آمد . \n"
                       "انصراف از دوره آموزشی\n"
                       "16-  در صورتیكه كارآموزی ، پس از ثبت نام از شركت در دوره انصراف حاصل نماید حداكثر تا یك هفته قبل از شروع دوره فرصت خواهد داشت كه به آموزشگاه مراجعه و و مبالغ  پرداختی را دریافت نماید در صورتیكه از مدت فوق بگذرد پرداخت شهریه به كارآموز منوط به موافقت موسس آموزشگاه خواهد بود . \n"
                       "17-  در صورتیكه كارآموز در حین دوره كارآموزی بنابه دلایلی از ادامه دوره كارآموزی خودداری نماید . \n"
                       "لازم است با ذكر دلایل مراتب را كتباً به آموزشگاه اعلام نماید بدیهی است در صورت وجود دلایل موجه یا ارائه مدارك به سازمان موضوع بررسی و تصمیم گیری خواهد شد . \n"
                       "18-  كوشش و توجه در حفظ و مراقبت از وسایل خویش و اهتمام در حفظ و نگهداری اموال و تجهیزات و ساختمان آموزشی . \n"
                       "تبصره : بدیهی است چنانچه كارآموز خسارتی به تجهیزات و وسایل كمك آموزشی و ساختمان واحد آموزشی یا سایر كارآموزان وارد آورد باید خسارت وارده را جبران نماید . \n"
                       "19-  تذكر و تنبیه : \n"
                       "تقصیر و سهل انگاری نسبت به انجام وظایف مندرج در ماده 2 (وظایف كارآموزان) تخلف محسوب می شود . \n"
                       "20-  موسس- مدیر و مربیان موظفند قبل از اعمال هرگونه تنبیه به وضع و موقعیت كارآموز آگاهی یابند و در جستجوی انگیزه و علت تخلف برآیند و حتی المقدور با راهنمائی و برطرف كردن علت از تنبیه و تكرار تخلف پیشگیری نمایند . \n"
                       "21-  كارآموزان متخلفی كه سایر راهنمائیها و چاره جوئیها تربیتی ، در آنها مفید و موثر نمی افتد ، با رعایت تناسب و ترتیب و جهات مندرج در ماده 8 مورد تنبیه ذیل قرار می گیرند . \n"
                       "1.  تذكر و اخطار شفاهی بطور خصوصی . \n"
                       "2.  تذكر و اخطار شفاهی در حضور چند نفر از كارآموزانی كه شاهد تخلف بوده اند . \n"
                       "3.  اخطار كتبی و اطلاع به ولی كارآموز . \n"
                       "4.  اخراج موقت از واحد آموزشی با اطلاع قبلی ولی كارآموز ، حداكثر برای مدت سه روزه (در اینگونه موارد با گرفتن تعهد از كارآموز و ولی او ، می تواند به كلاس باز گردد) . \n"
                       "5.  اخراج از آموزشگاه در صورت تكرار تخلف با اعلام مراتب به اداره كل یا مركز معین . \n"
                       "22-  در اجرای تنبیهات حیاء كارآموز حفظ شود و آبروی او محفوظ بماند و به همین جهت از تنبیه در حضور جمع در موارد ضروری اجتناب شود . \n"
                       "23-  چنانچه كارآموزی مرتكب جرائم عمومی (جرم یا جنحه و جنایت) شده باشد اقدام و رسیدگی \n"
                       "به وضعیت وی با هماهنگی موسس بر عهده مراجع قانونی ذیصلاح خواهد بود . \n"
                      );

    new WBreak(tyroForm);
    WPushButton *tyroRegBtn = new WPushButton(L"ثبت نام", tyroForm);
    tyroRegBtn->setStyleClass("btnHome");

    tyroFNameValidator = new WLengthValidator(1, 15);
    tyroLNameValidator = new WLengthValidator(1, 20);
    tyroSexValidator = new WLengthValidator(1, 3);
    tyroFatherSNameValidator = new WLengthValidator(1, 15);
    tyroNationalCodeValidator = new WRegExpValidator(Base::regExpNationalCode);
    tyroBirthIdValidator = new WRegExpValidator(Base::regExpBirthId);
    tyroBirthDateYValidator = new WLengthValidator(1, 4);
    tyroBirthDateMValidator = new WLengthValidator(1, 2);
    tyroBirthDateDValidator = new WLengthValidator(1, 2);
    tyroBirthlocValidator = new WLengthValidator(1, 35);
    tyroBirthEmissionLocValidator = new WLengthValidator(1, 35);
    tyroGraduateCertValidator = new WLengthValidator(1, 13);
    tyroGraduateCourseValidator = new WLengthValidator(1, 50);
    tyroJobValidator = new WLengthValidator(1, 11);
    tyroAddrValidator = new WLengthValidator(1, 255);
    tyroTelValidator = new WRegExpValidator(Base::regExpTel);
    tyroMobileValidator = new WRegExpValidator(Base::regExpMobile);
    tyroEmailValidator = new WRegExpValidator(Base::regExpEmail);

    tyroFNameValidator->setMandatory(true);
    tyroLNameValidator->setMandatory(true);
    tyroSexValidator->setMandatory(true);
    tyroFatherSNameValidator->setMandatory(true);
    tyroNationalCodeValidator->setMandatory(true);
    tyroBirthIdValidator->setMandatory(true);
    tyroBirthDateYValidator->setMandatory(true);
    tyroBirthDateMValidator->setMandatory(true);
    tyroBirthDateDValidator->setMandatory(true);
    tyroBirthlocValidator->setMandatory(true);
    tyroBirthEmissionLocValidator->setMandatory(true);
    tyroGraduateCertValidator->setMandatory(true);
    tyroGraduateCourseValidator->setMandatory(true);
    tyroJobValidator->setMandatory(true);
    tyroAddrValidator->setMandatory(true);
    tyroTelValidator->setMandatory(true);
    //tyroMobileValidator->setMandatory(true);
    //tyroEmailValidator->setMandatory(true);

    tyroFNameEdit->setValidator(tyroFNameValidator);
    tyroLNameEdit->setValidator(tyroLNameValidator);
    tyroSexCmb->setValidator(tyroSexValidator);
    tyroFatherSNameEdit->setValidator(tyroFatherSNameValidator);
    tyroNationalCodeEdit->setValidator(tyroNationalCodeValidator);
    tyroBirthIdEdit->setValidator(tyroBirthIdValidator);
    tyroBirthDateCmbY->setValidator(tyroBirthDateYValidator);
    tyroBirthDateCmbM->setValidator(tyroBirthDateMValidator);
    tyroBirthDateCmbD->setValidator(tyroBirthDateDValidator);
    tyroBirthlocCmb->setValidator(tyroBirthlocValidator);
    tyroBirthEmissionLocCmb->setValidator(tyroBirthEmissionLocValidator);
    tyroGraduateCertCmb->setValidator(tyroGraduateCertValidator);
    tyroGraduateCourseEdit->setValidator(tyroGraduateCourseValidator);
    tyroJobCmb->setValidator(tyroJobValidator);
    tyroAddrTArea->setValidator(tyroAddrValidator);
    tyroTelEdit->setValidator(tyroTelValidator);
    tyroMobileEdit->setValidator(tyroMobileValidator);
    tyroEmailEdit->setValidator(tyroEmailValidator);

    tyroFNameEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroLNameEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroSexCmb->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroFatherSNameEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroNationalCodeEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthIdEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthDateCmbY->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthDateCmbM->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthDateCmbD->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthlocCmb->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroBirthEmissionLocCmb->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroGraduateCertCmb->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroGraduateCourseEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroJobCmb->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroAddrTArea->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroTelEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroMobileEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroEmailEdit->enterPressed().connect(this, &CgiRoot::RegStep_5);
    tyroRegBtn->clicked().connect(this, &CgiRoot::RegStep_5);
}

void CgiRoot::RegStep_5() {
    if (Validate(tyroFNameEdit) && Validate(tyroLNameEdit) && Validate(tyroSexCmb) &&
        Validate(tyroFatherSNameEdit) && Validate(tyroNationalCodeEdit) && Validate(tyroBirthIdEdit) &&
        Validate(tyroBirthDateCmbY) && Validate(tyroBirthDateCmbM) && Validate(tyroBirthDateCmbD) &&
        Validate(tyroBirthlocCmb) && Validate(tyroBirthEmissionLocCmb) &&
        Validate(tyroGraduateCertCmb) && Validate(tyroGraduateCourseEdit) &&
        Validate(tyroJobCmb) && Validate(tyroAddrTArea) && Validate(tyroTelEdit) &&
        Validate(tyroMobileEdit) && Validate(tyroEmailEdit)) {

        regTyroFName = tyroFNameEdit->text().toUTF8();
        regTyroLName = tyroLNameEdit->text().toUTF8();
        regTyroSex = tyroSexCmb->currentText().toUTF8();
        regTyroFatherSName = tyroFatherSNameEdit->text().toUTF8();
        regTyroNationalCode = tyroNationalCodeEdit->text().toUTF8();
        regTyroBirthId = tyroBirthIdEdit->text().toUTF8();
        regTyroBirthDateY = tyroBirthDateCmbY->currentText().toUTF8();
        regTyroBirthDateM = tyroBirthDateCmbM->currentText().toUTF8();
        regTyroBirthDateD = tyroBirthDateCmbD->currentText().toUTF8();
        regTyroBirthloc = tyroBirthlocCmb->currentText().toUTF8();
        regTyroBirthEmissionLoc = tyroBirthEmissionLocCmb->currentText().toUTF8();
        regTyroGraduateCert = tyroGraduateCertCmb->currentText().toUTF8();
        regTyroGraduateCourse = tyroGraduateCourseEdit->text().toUTF8();
        regTyroJob = tyroJobCmb->currentText().toUTF8();
        regTyroAddr = tyroAddrTArea->text().toUTF8();
        regTyroTel = tyroTelEdit->text().toUTF8();
        regTyroMobile = tyroMobileEdit->text().toUTF8();
        regTyroEmail = tyroEmailEdit->text().toUTF8();

        regTyroEmail = EncDec::Encrypt(regTyroEmail);

        dvMainWrapper->clear();

        dvRegister = new WContainerWidget(dvMainWrapper);

        Div *dvReturnHome = new Div(dvRegister, "dvReturnHome");
        WPushButton *returnHome = new WPushButton(L"بازگشت به صفحه اصلی", dvReturnHome);
        returnHome->clicked().connect(this, &CgiRoot::ReturnHome);

        WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
        dvImage->addWidget(new WImage("flow4.png"));

        WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");


        tyroPicBirthCertFUP = new WFileUpload();
        tyroPicNationalCardFUP = new WFileUpload();
        tyroPicPersonnelFUP = new WFileUpload();
        tyroPicServiceFUP = new WFileUpload();

        tyroPicBirthCertBtn = new WPushButton(L"ارسال");
        tyroPicNationalCardBtn = new WPushButton(L"ارسال");
        tyroPicPersonnelBtn = new WPushButton(L"ارسال");
        tyroPicServiceBtn = new WPushButton(L"ارسال");

        tyroPicNationalCardFUP->disable();
        tyroPicPersonnelFUP->disable();
        tyroPicServiceFUP->disable();
        tyroPicNationalCardBtn->disable();
        tyroPicPersonnelBtn->disable();
        tyroPicServiceBtn->disable();

        Div *tyroForm = new Div(dvRegForm, "dvTyroFormWrapper");
        Div *dvTyroForm = new Div(tyroForm, "dvTyroForm");
        WGridLayout *dvTyroFormLayout = new WGridLayout();

        dvTyroFormLayout->addWidget(tyroPicBirthCertBtn, 0, 0);
        dvTyroFormLayout->addWidget(tyroPicBirthCertFUP , 0, 1);
        dvTyroFormLayout->addWidget(new WText(L"ارسال تصویر صفحه اول شناسنامه"
                                                   ), 0, 2);

        dvTyroFormLayout->addWidget(tyroPicNationalCardBtn, 1, 0);
        dvTyroFormLayout->addWidget(tyroPicNationalCardFUP, 1, 1);
        dvTyroFormLayout->addWidget(new WText(L"ارسال تصویر کارت ملی"
                                                   ), 1, 2);

        dvTyroFormLayout->addWidget(tyroPicPersonnelBtn, 2, 0);
        dvTyroFormLayout->addWidget(tyroPicPersonnelFUP, 2, 1);
        dvTyroFormLayout->addWidget(new WText(L"ارسال عکس پرسنلی"
                                                   ), 2, 2);

        dvTyroFormLayout->addWidget(tyroPicServiceBtn, 3, 0);
        dvTyroFormLayout->addWidget(tyroPicServiceFUP, 3, 1);
        dvTyroFormLayout->addWidget(new WText(L"ارسال تصویر کارت پایان خدمت یا مدرکی دال بر معافیت"
                                                   ), 3, 2);


        dvTyroFormLayout->setVerticalSpacing(11);
        dvTyroFormLayout->setColumnStretch(0, 100);
        dvTyroFormLayout->setColumnStretch(1, 100);
        dvTyroFormLayout->setColumnStretch(2, 500);
        dvTyroForm->resize(700, WLength::Auto);
        dvTyroForm->setLayout(dvTyroFormLayout);

        new WBreak(tyroForm);
        new WBreak(tyroForm);
        new WBreak(tyroForm);

        tyroRegFinishBtn = new WPushButton(L"پایان ثبت نام و اعلام کد رهگیری", tyroForm);
        tyroRegFinishBtn->disable();
        tyroRegFinishBtn->clicked().connect(this, &CgiRoot::RegStep_6);

        new WBreak(dvRegister);
        new WBreak(dvRegister);
        new WBreak(dvRegister);

        WContainerWidget *dvRegComments = new Div(dvRegister, "dvRegComments");

        new WText(
                L"<h3>دستورات ارایه عکس : </h3><br />"
                "فایل عکس را میتوان از یکی از دو روش زیر تهیه کرد: <br />"
                "گرفتن یک عکس دیجیتالی جدید و یا استفاده از اسکنر دیجیتالی برای اسکن عکس. <br />"
                "<br />"
                "مشخصات ترکیبی عکس :<br />"
                "عکس دیجیتالی ارایه شده می بایست مطابق مشخصات ترکیبی زیر باشد در غیر این صورت مورد تایید نخواهد بود .<br />"
                "<br />"
                "سر شخص :<br />"
                "شخصی که از آن عکس گرفته میشود باید مستقیما رو به دوربین باشد .<br />"
                "- سر شخص نبایست کج ، پایین و یا رو به جهت دیگری باشد .<br />"
                "- سر شخص میبایست حدودا 50 % مساحت عکس را گرفته باشد .<br />"
                "<br />"
                "زمینه عکس :<br />"
                "شخصی که از آن عکس گرفته می شود می بایست روبروی یک زمینه طبیعی و با نور سفید قرار بگیرد.<br />"
                "- زمینه های تاریک یا شکل دار مورد قبول نخواهد بود.<br />"
                "<br />"
                "فوکوس :<br />"
                "- عکس میبایست فوکوس داشته باشد .( عکس نبایست تار باشد )<br />"
                "<br />"
                "وسایل زینتی :<br />"
                "در عکس های که شخص عینک و یا هر موردی که از توجه صورت کم کرده باشد مورد قبول نمی باشد.<br />"
                "- برای آقایان عکسهای با وسایلی نظیر کروات، پاپیون، دستمال گردن و ... قابل قبول نیست.<br />"
                "- برای خانمها عکسها با پوشش حجاب اسلامی بدون مشخص بودن موی سر و در عین حال با مشخص بودن کامل قرص صورت باشد.<br />"
                "<br />"
                "پوشش سر و کلاه :<br />"
                "عکس آقایان با پوشش سر و کلاه مورد قبول نخواهد بود. عکس اشخاص با پوشش سر طایفه ای و غیره مورد قبول نمی‌باشد. عکس ارتشی، خطوط هوایی و هر پوشش سر پرسنلی و کارمندی مورد قبول نخواهد بود.<br />"
                "<br />"
/*                "فرمت فایل عکس :<br />"
                "عکس می بایست در فرمت JPEG باشد .<br />"
                "رزولوشن عکس ( Resolution Image) : باید ارتفاع 600 پیکسل و عرض 600 پیکسل باشد .<br />"
                "<br />"
                "اسکن و ارایه عکس :<br />"
                "قبل از اسکن عکس پرینت شده میبایست مشخصات ساختاری زیر مهیا گردد. اگر عکس پرینت شده مشخصات رنگ و مشخصات ساختاری را دربر داشت . عکس را با مشخصات اسکنر زیر اسکن کنید .<br />"
                "<br />"
                "در فرمت JPEG باشد. اندازه حجم عکس :<br />"
                "بیشترین اندازه ی حجم عکس میبایست (150kb) یعنی 150 کیلو بایت باشد.<br />"
                "رزولوشن عکس ( Resolution Image): باید 600 در 600 پیکسل باشد.<br />"*/
                , dvRegComments);

        /*tyroPicBirthCertBtn->clicked().connect(this, &CgiRoot::RegStep_5_1);
        tyroPicNationalCardBtn->clicked().connect(this, &CgiRoot::RegStep_5_2);
        tyroPicPersonnelBtn->clicked().connect(this, &CgiRoot::RegStep_5_3);
        tyroPicServiceBtn->clicked().connect(this, &CgiRoot::RegStep_6);*/


        tyroPicBirthCertBtn->clicked().connect(tyroPicBirthCertFUP, &Wt::WFileUpload::upload);
        //tyroPicBirthCertFUP->changed().connect(tyroPicBirthCertFUP, &WFileUpload::upload);
        tyroPicBirthCertFUP->uploaded().connect(this, &CgiRoot::RegStep_5_1);

        tyroPicNationalCardBtn->clicked().connect(tyroPicNationalCardFUP, &Wt::WFileUpload::upload);
        //tyroPicNationalCardFUP->changed().connect(tyroPicNationalCardFUP, &WFileUpload::upload);
        tyroPicNationalCardFUP->uploaded().connect(this, &CgiRoot::RegStep_5_2);

        tyroPicPersonnelBtn->clicked().connect(tyroPicPersonnelFUP, &Wt::WFileUpload::upload);
        //tyroPicPersonnelFUP->changed().connect(tyroPicPersonnelFUP, &WFileUpload::upload);
        tyroPicPersonnelFUP->uploaded().connect(this, &CgiRoot::RegStep_5_3);

        tyroPicServiceBtn->clicked().connect(tyroPicServiceFUP, &Wt::WFileUpload::upload);
        //tyroPicServiceFUP->changed().connect(tyroPicServiceFUP, &WFileUpload::upload);
        tyroPicServiceFUP->uploaded().connect(this, &CgiRoot::RegStep_5_4);
    }
}

void CgiRoot::RegStep_5_1() {
    if (!ValidatePic(tyroPicBirthCertFUP->spoolFileName())) {
        return;
    }

    tyroPicBirthCertFUP->disable();
    tyroPicNationalCardFUP->enable();
    tyroPicBirthCertBtn->disable();
    tyroPicNationalCardBtn->enable();
}

void CgiRoot::RegStep_5_2() {
    if (!ValidatePic(tyroPicNationalCardFUP->spoolFileName())) {
        return;
    }
    tyroPicNationalCardFUP->disable();
    tyroPicPersonnelFUP->enable();
    tyroPicNationalCardBtn->disable();
    tyroPicPersonnelBtn->enable();
}

void CgiRoot::RegStep_5_3() {
    if (!ValidatePic(tyroPicPersonnelFUP->spoolFileName())) {
        return;
    }
    tyroPicPersonnelFUP->disable();
    tyroPicPersonnelBtn->disable();

    if (regTyroSex != "زن") {
        tyroPicServiceFUP->enable();
        tyroPicServiceBtn->enable();
    }

    tyroRegFinishBtn->enable();
}

void CgiRoot::RegStep_5_4() {
    if (!ValidatePic(tyroPicServiceFUP->spoolFileName())) {
        return;
    }

    tyroPicServiceFUP->disable();
    tyroPicServiceBtn->disable();
}


void CgiRoot::RegStep_6() {
    string tyroPicBirthCertFile = tyroPicBirthCertFUP->spoolFileName(),
    tyroPicNationalCardFile = tyroPicNationalCardFUP->spoolFileName(),
    tyroPicPersonnelFile = tyroPicPersonnelFUP->spoolFileName(),
    tyroPicService = tyroPicServiceFUP->spoolFileName();

    row r;
    string code, fcode;

    bool found = true;
    do {
        code = BString::Trim(BString::IntToStr((unsigned)time(NULL)));
        db.sql << "SELECT code FROM [actyros] "
                "WHERE code=?", code, r;
        if(!db.sql.single(r)) {
            found = false;
        }
    } while (found);

    found = true;
    do {
        fcode = Base::GenRandChars();
        db.sql << "SELECT fcode FROM [actyros] "
                "WHERE fcode=?", fcode, r;
        if(!db.sql.single(r)) {
            found = false;
        }
    } while (found);


    db.Insert("actyros",
              "code, accode, actcode, fcode,"
              "fname, lname, "
              "sex, fathersname, nationalcode, "
              "birthid, "
              "birthdate, "
              "birthloc, birthemissionloc, "
              "graduatecert, graduatecourse, job, "
              "addr, tel, mobile, email, payment, noticed",
              22, code.c_str(), regTermAcCode.c_str(), regTermActCode.c_str(), fcode.c_str(),
              regTyroFName.c_str(), regTyroLName.c_str(),
              regTyroSex.c_str(), regTyroFatherSName.c_str(), regTyroNationalCode.c_str(),
              regTyroBirthId.c_str(),
              (regTyroBirthDateY + regTyroBirthDateM + regTyroBirthDateD).c_str(),
              regTyroBirthloc.c_str(), regTyroBirthEmissionLoc.c_str(),
              regTyroGraduateCert.c_str(), regTyroGraduateCourse.c_str(), regTyroJob.c_str(),
              regTyroAddr.c_str(), regTyroTel.c_str(), regTyroMobile.c_str(), regTyroEmail.c_str(),
              "0", "0"
              );

    dbPics.Insert(regTermAcCode, "id, personnel, birthcert, nationalcard, service", 5,
                  ("t" + code).c_str(),
                  CImage::B64Encode(tyroPicPersonnelFile).c_str(),
                  CImage::B64Encode(tyroPicBirthCertFile).c_str(),
                  CImage::B64Encode(tyroPicNationalCardFile).c_str(),
                  CImage::B64Encode(tyroPicService).c_str()
                  );

    dvMainWrapper->clear();

    dvRegister = new WContainerWidget(dvMainWrapper);

    WContainerWidget *dvImage =  new Div(dvRegister, "dvImgCentered");
    dvImage->addWidget(new WImage("flow5.png"));

    WContainerWidget *dvRegForm = new Div(dvRegister, "dvRegForm");

    WContainerWidget *dvRegNotice = new Div(dvRegForm, "dvRegNotice");

    new WText(
            L"ثبت نام شما در دوره های آموزشگاه های آزاد با موفقیت انجام شد. "
            "لطفاً کد رهگیری خود را تا پایان فرایند ثبت نام و شروع دوره آموزشی نزد خود نگاه دارید "
            "برای ادامه با شما از طرف آموزشگاه تماس گرفته می شود "
            "و یا از طریق پیامک اطلاع رسانی میگردد."
            "<br /><br /><br /><br /><br />"
            "دوره انتخابی شما "
            L"<strong>" +
            BString::StrToWStr(regTermStTitle) +
            L"</strong>"
            " تاریخ برگزاری از "
            "<strong>" +
            BString::StrToWStr(regTermBegdate) +
            L"</strong>"
            " تا "
            "<strong>" +
            BString::StrToWStr(regTermEnddate) +
            L"</strong>"
            " شیفت "
            "<strong>" +
            BString::StrToWStr(regTermDaypart) +
            L"</strong>"
            " ساعت "
            "<strong>" +
            BString::StrToWStr(regTermBegtime) +
            L"</strong>"
            " تا "
            "<strong>" +
            BString::StrToWStr(regTermEndtime) +
            L"</strong>"
            " با مربیگری "
            "<strong>" +
            BString::StrToWStr(regTermAcmName) +
            L"</strong>"
            " در محل آموزشگاه "
            "<strong>" +
            BString::StrToWStr(regTermLoc) +
            L"</strong>"
            " می باشد."
            "<br /><br />"
            "کاربر گرامی هزینه دوره انتخابی شما "
            "<strong>"
            "30000"
            "</strong>"
            " ریال می باشد که باید به حساب "
            "<strong>"
            "234567876543 بانک ملت شعبه حافظیه"
            "</strong>"
            " پرداخت شود."
            "<br /><br /><br /><br />"
            "کد رهگیری: <span style=\"font-weight: bold; color: #990000; "
            "direction: rtl; text-align: center;\">"
            + BString::StrToWStr(fcode)
            + L"</span>"
            "<br/><br/>"
            "<span style=\"font-weight: bold; color: #990000; "
            "direction: rtl; text-align: center;\">"
            "توجه: کد رهگیری به حروف کوچک و بزرگ حساس می باشد."
            "</span>"
            , dvRegNotice);


    new WBreak(dvRegNotice);
    new WBreak(dvRegNotice);
    new WBreak(dvRegNotice);

    WPushButton *returnHome = new WPushButton(L"پایان", dvRegNotice);
    returnHome->clicked().connect(this, &CgiRoot::ReturnHome);
}

WWidget *CgiRoot::CHowTo(){
    return new WText(GetPageContent("آموزش نحوه ثبت نام"));
}

WWidget *CgiRoot::CLinks(){
    return new WText(GetPageContent("سایت های مرتبط"));
}

WWidget *CgiRoot::CContact(){
    WContainerWidget *contact = new WContainerWidget();
    contact->setId("dvCContact");

    errContactForm = new WText(contact);

    Div *dvContactForm = new Div(contact, "dvContactForm");
    WGridLayout *dvContactFormLayout = new WGridLayout();

    contactToCmb = new WComboBox();
    contactToCmb->addItem(L".: تماس با :.");

    result res;
    row r;

    db.sql << "SELECT name FROM [contact] ORDER BY name ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string name;
            r >> name;
            contactToCmb->addItem(BString::StrToWStr(name));
        }
    }

    contactFromEdit = new WLineEdit();
    contactEmailEdit = new WLineEdit();
    contactEmailEdit->setStyleClass("ltrInput");
    contactUrlEdit = new WLineEdit();
    contactUrlEdit->setStyleClass("ltrInput");
    contactSubjectEdit = new WLineEdit();
    contactBodyTArea = new WTextArea();
    contactBodyTArea->resize(WLength::Auto, 100);

    dvContactFormLayout->addWidget(contactToCmb, 0, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"مخاطب"
                                               ), 0, 1);

    dvContactFormLayout->addWidget(contactFromEdit, 1, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"نام فرستنده"
                                               ), 1, 1);

    dvContactFormLayout->addWidget(contactEmailEdit, 2, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"آدرس ایمیل"
                                               ), 2, 1);

    dvContactFormLayout->addWidget(contactUrlEdit, 3, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"صفحه ی وب"
                                               ), 3, 1);

    dvContactFormLayout->addWidget(contactSubjectEdit, 4, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"موضوع پیام"
                                               ), 4, 1);

    dvContactFormLayout->addWidget(contactBodyTArea, 5, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"متن پیام"
                                                 ), 5, 1);

    contactCapEdit = new WLineEdit();
    contactCaptcha = new WImage(__capImage);
    contactCaptcha->setStyleClass("capSide");
    contactCaptcha->setAlternateText(Base::capTip);

    dvContactFormLayout->addWidget(contactCapEdit, 6, 0);
    dvContactFormLayout->addWidget(new Wt::WText(L"کد امنیتی"
                                                 ), 6, 1);

    dvContactFormLayout->addWidget(contactCaptcha, 7, 0);

    Div *dvContactFormButtons = new Div(contact, "dvContactFormButtons");
    contactSendBtn = new WPushButton(L"ارسال", dvContactFormButtons);
    contactClearBtn = new WPushButton(L"پیام جدید", dvContactFormButtons);
    contactClearBtn->setStyleClass("btnHome");
    contactSendBtn->setStyleClass("btnHome");
    dvContactFormLayout->addWidget(dvContactFormButtons, 8, 0);

    dvContactFormLayout->setVerticalSpacing(11);
    dvContactFormLayout->setColumnStretch(0, 250);
    dvContactFormLayout->setColumnStretch(1, 100);
    dvContactForm->resize(350, WLength::Auto);
    dvContactForm->setLayout(dvContactFormLayout);

    SetCContactForm(false);

    contactFromValidator = new WLengthValidator(1, 35);
    contactEmailValidator = new WRegExpValidator(Base::regExpEmail);
    contactUrlValidator = new WRegExpValidator(Base::regExpUri);
    contactSubjectValidator = new WLengthValidator(Base::minLenEmailSubject, Base::maxLenEmailSubject);
    contactBodyValidator = new WLengthValidator(Base::minLenEmailBody, Base::maxLenEmailBody);
    contactCapValidator = new WIntValidator(__capResult, __capResult);

    contactFromValidator->setMandatory(true);
    contactEmailValidator->setMandatory(true);
    contactSubjectValidator->setMandatory(true);
    contactBodyValidator->setMandatory(true);
    contactCapValidator->setMandatory(true);

    contactFromEdit->setValidator(contactFromValidator);
    contactEmailEdit->setValidator(contactEmailValidator);
    contactUrlEdit->setValidator(contactUrlValidator);
    contactSubjectEdit->setValidator(contactSubjectValidator);
    contactBodyTArea->setValidator(contactBodyValidator);
    contactCapEdit->setValidator(contactCapValidator);

    contactToCmb->sactivated().connect(this, &CgiRoot::ContactToCmbChanged);
    contactClearBtn->clicked().connect(this, &CgiRoot::ContactClearOK_RP);

    contactFromEdit->enterPressed().connect(this, &CgiRoot::SendMessageOK);
    contactEmailEdit->enterPressed().connect(this, &CgiRoot::SendMessageOK);
    contactUrlEdit->enterPressed().connect(this, &CgiRoot::SendMessageOK);
    contactSubjectEdit->enterPressed().connect(this, &CgiRoot::SendMessageOK);
    contactCapEdit->enterPressed().connect(this, &CgiRoot::SendMessageOK);
    contactSendBtn->clicked().connect(this, &CgiRoot::SendMessageOK);

    return contact;
}

void CgiRoot::SetCContactForm(bool flag) {
    contactToCmb->setDisabled(flag);
    contactFromEdit->setDisabled(!flag);
    contactEmailEdit->setDisabled(!flag);
    contactUrlEdit->setDisabled(!flag);
    contactSubjectEdit->setDisabled(!flag);
    contactBodyTArea->setDisabled(!flag);
    contactCapEdit->setDisabled(!flag);
    contactSendBtn->setDisabled(!flag);
    contactClearBtn->setDisabled(!flag);

    if (flag) {
        Error_Root(L"", errContactForm);
        contactFromEdit->setFocus();
    }
    else {
        contactFromEdit->setText("");
        contactEmailEdit->setText("");
        contactUrlEdit->setText("");
        contactSubjectEdit->setText("");
        contactBodyTArea->setText("");
        contactCapEdit->setText("");
        contactToCmb->setCurrentIndex(0);
        contactToCmb->setFocus();
    }
}

void CgiRoot::ContactToCmbChanged(WString to) {
    if (to != L".: تماس با :.") {
        SetCContactForm(true);
    }
}

void CgiRoot::ContactClearOK_RP() {
    Error_Root(L"", errContactForm);
    SetCContactForm(false);
}

void CgiRoot::SendMessageOK() {
    if(Validate(contactFromEdit) && Validate(contactEmailEdit) && Validate(contactUrlEdit) &&
       Validate(contactSubjectEdit) && Validate(contactBodyTArea) && Validate(contactCapEdit)) {
        row r;
        db.sql << "SELECT addr FROM [contact] "
                "WHERE name=?", BString::Trim(contactToCmb->currentText().toUTF8()), r;

        if(db.sql.single(r)) {
            CDate::Now n;

            string to;
            r >> to;
            to = EncDec::Decrypt(to);

            Mailer::SendUserMessage(to,
                                    BString::Trim(contactEmailEdit->text().toUTF8()),
                                    BString::Trim(contactFromEdit->text().toUTF8()),
                                    BString::Trim(contactUrlEdit->text().toUTF8()),
                                    BString::Trim(contactSubjectEdit->text().toUTF8()),
                                    BString::Trim(contactBodyTArea->text().toUTF8()),
                                    n, serverInfo, clientInfo);
        }
        SetCContactForm(false);
        ReGenCap();
        Error_Root(Base::msgSendMsgSuccessH, errContactForm);
    }
    else {
       ReGenCap();
    }
}

WWidget *CgiRoot::CAbout(){
    return new WText(GetPageContent("درباره ی ما"));
}

void CgiRoot::FollowingOK() {
    if(Validate(followEdit) && Validate(followCapEdit)) {
        row r;

        db.sql << "SELECT actcode FROM [actyros] "
                "WHERE fcode=?;", followEdit->text().toUTF8(), r;

        if(!db.sql.single(r)) {
            Error_Root(Base::errInvalidFollowingCodeH, errFollowing);
            return;
        }

        followEdit->setText("");
        followCapEdit->setText("");
        ReGenCap();


        string actCode;
        r >> actCode;

        db.sql << "SELECT acmcode, stcode, begdate, enddate, daypart, begtime, endtime, loc "
                "FROM [acterms] WHERE code=?;", actCode, r;

        string acmCode, stCode, begdate, enddate, daypart, begtime, endtime, loc;
        if(db.sql.single(r)) {
            r >> acmCode >> stCode >> begdate >> enddate >> daypart >> begtime >> endtime >> loc;

            begdate.insert(4, "/");
            begdate.insert(7, "/");
            enddate.insert(4, "/");
            enddate.insert(7, "/");

            begtime.insert(2, ":");
            endtime.insert(2, ":");
        }

        db.sql << "SELECT fname, lname FROM [acmentors] "
                "WHERE code=?;", acmCode, r;

        string acmName;
        if(db.sql.single(r)) {
            string fname, lname;
            r >> fname >> lname;
            acmName = fname  +" " + lname;
        }

        db.sql << "SELECT title FROM [standards] "
                "WHERE code=?;", stCode, r;

        string stTitle;
        if(db.sql.single(r)) {
            r >> stTitle;
        }


        WDialog *dialog = new WDialog(L"پیگیری ثبت نام");
        dialog->setModal(true);

        WContainerWidget *dvRegNotice = new Div(dialog->contents(), "dvRegNotice");

        new WText(
                L"<br /><br />"
                "دوره انتخابی شما "
                L"<strong>" +
                BString::StrToWStr(stTitle) +
                L"</strong>"
                " تاریخ برگزاری از "
                "<strong>" +
                BString::StrToWStr(begdate) +
                L"</strong>"
                " تا "
                "<strong>" +
                BString::StrToWStr(enddate) +
                L"</strong>"
                " شیفت "
                "<strong>" +
                BString::StrToWStr(daypart) +
                L"</strong>"
                " ساعت "
                "<strong>" +
                BString::StrToWStr(begtime) +
                L"</strong>"
                " تا "
                "<strong>" +
                BString::StrToWStr(endtime) +
                L"</strong>"
                " با مربیگری "
                "<strong>" +
                BString::StrToWStr(acmName) +
                L"</strong>"
                " در محل آموزشگاه "
                "<strong>" +
                BString::StrToWStr(loc) +
                L"</strong>"
                " می باشد."
                "<br /><br />"
                "کاربر گرامی هزینه دوره انتخابی شما "
                "<strong>"
                "30000"
                "</strong>"
                " ریال می باشد که باید به حساب "
                "<strong>"
                "234567876543 بانک ملت شعبه حافظیه"
                "</strong>"
                " پرداخت شود."
                "<br /><br />"
                , dvRegNotice);

        WPushButton *ok = new WPushButton(L"تائید", dialog->contents());
        ok->clicked().connect(dialog, &WDialog::accept);
        ok->setStyleClass("btnDialogs");

        dialog->show();
    }
    else {
       ReGenCap();
    }
}

void CgiRoot::AcademiesOK() {
    if(Validate(academyCapEdit) && Validate(academyUserEdit) && Validate(academyPwEdit)) {
        Initialize_ACP();
        ReGenCap();
    }
    else {
       ReGenCap();
    }
}

void CgiRoot::VoteOK() {
    if(Validate(voteCapEdit)) {
        string field;

        switch (dvMainVoteRadioGroup->selectedButtonIndex()) {
        case 0:
            field = "standards";
            break;
        case 1:
            field = "teachers";
            break;
        case 2:
            field = "equipment";
            break;
        case 3:
            field = "tradexp";
            break;
        case 4:
            field = "supervision";
            break;
        default:
            Error_Root(Base::errNoVoteH, errVote);
            break;
        }

        if (field != "") {
            db.sql << "UPDATE [vote] "
                    "SET " + field + "=" + field + "+1 "
                    "WHERE r=0;", exec();

            //Error_Root(L"", errVote);

            delete voteCaptcha;
            voteCaptcha = NULL;
            dvMainVote->clear();
            VoteChart();
        }

        ReGenCap();
    }
    else {
       ReGenCap();
    }
}

void CgiRoot::VoteChart() {
    WDialog *dialog = new WDialog(L"نتیجه نظرسنجی");
    dialog->setModal(true);

    int standards, teachers, equipment, tradexp, supervision;
    row r;

    db.sql << "SELECT standards, teachers, equipment, tradexp, supervision FROM [vote] "
            "WHERE r=0;", r;

    if(db.sql.single(r)) {
        r >> standards >> teachers >> equipment >> tradexp >> supervision;
    }

    WStandardItemModel *model = new WStandardItemModel(dialog->contents());

    model->insertColumns(model->columnCount(), 2);
    model->setHeaderData(0, boost::any(WString("Item")));
    model->setHeaderData(1, boost::any(WString("Value")));

    model->insertRows(model->rowCount(), 5);
    int row = 0; // ???
    model->setData(row, 0, boost::any(WString(L"استانداردهای آموزش")));
    model->setData(row, 1, boost::any(standards));
    row++;
    model->setData(row, 0, boost::any(WString(L"معلومات مربیان")));
    model->setData(row, 1, boost::any(teachers));
    row++;
    model->setData(row, 0, boost::any(WString(L"تجهیزات کارگاهها")));
    model->setData(row, 1, boost::any(equipment));
    row++;
    model->setData(row, 0, boost::any(WString(L"استفاده از تجربه اصناف درآموزش")));
    model->setData(row, 1, boost::any(tradexp));
    row++;
    model->setData(row, 0, boost::any(WString(L"نظارت بیشتر بر مراکز آموزشی")));
    model->setData(row, 1, boost::any(supervision));
    row++;

    WPieChart *chart = new WPieChart(dialog->contents());
    chart->setModel(model);       // set the model
    chart->setLabelsColumn(0);    // set the column that holds the labels
    chart->setDataColumn(1);      // set the column that holds the data

    // configure location and type of labels
    chart->setDisplayLabels(Outside | TextLabel | TextPercentage);

    // enable a 3D and shadow effect
    chart->setPerspectiveEnabled(true, 0.2);
    chart->setShadowEnabled(true);


    //////
    int max = 0, min = 2147483647;
    int vals[5] = { standards, teachers, equipment, tradexp, supervision };

    for (int i = 0; i < 5; ++i) {
        if (max < vals[i])
            max = vals[i];
        if (min > vals[i])
            min = vals[i];
    }
    //////

    // explode the first item
    //chart->setExplode(0, 0.3);
    for (int i = 0; i < 5; ++i) {
        if (vals[i] == max/* || vals[i] == min*/)
            chart->setExplode(i, 0.3);
    }

    chart->resize(800, 300); // WPaintedWidget must be given an explicit size

    chart->setMargin(10, Top | Bottom);            // add margin vertically
    chart->setMargin(WLength::Auto, Left | Right); // center horizontally

    WPushButton *ok = new WPushButton(L"تائید", dialog->contents());
    ok->clicked().connect(dialog, &WDialog::accept);
    ok->setStyleClass("btnDialogs");

    dialog->show();
}

