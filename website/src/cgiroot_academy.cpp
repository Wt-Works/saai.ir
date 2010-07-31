#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WEnvironment>
#include <Wt/WFileUpload>
#include <Wt/WGridLayout>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMenu>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WStackedWidget>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WText>
#include <Wt/WWidget>

#include <dbixx/dbixx.h>

#include "cgiroot.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "cdate.hpp"
#include "ccrypt.hpp"
#include "cimage.hpp"
#include "div.hpp"
#include "mailer.hpp"

using namespace std;
using namespace Wt;
using namespace dbixx;
using namespace SAAIIR;
using namespace CCrypt;
using namespace CDate;

void CgiRoot::Initialize_ACP() {
    row r;
    /*db.sql << "SELECT user, email, accode, lip, lccode, lcname, ldate, ltime, suspend FROM [acusers] "
            "WHERE user=? AND pw=?",
            EncDec::Encrypt(academyUserEdit->text().toUTF8()),
            EncDec::Encrypt(academyPwEdit->text().toUTF8()),
            r;*/

    db.sql << "SELECT user, email, accode, suspend FROM [acusers] "
                "WHERE user=? AND pw=?",
                EncDec::Encrypt(academyUserEdit->text().toUTF8()),
                EncDec::Encrypt(academyPwEdit->text().toUTF8()),
                r;

    bool hasEmail = false;
    if(db.sql.single(r)) {
        //CDate::Now n;

        /*string to, suspend;
        r >> __acLoggedInUser >> to >> __acLoggedInCode
                >> __acLastIP >> __acLastCCode >> __acLastCName
                >> __acLastDate >> __acLastTime
                >> suspend;*/

        string to, suspend;
        r >> __acLoggedInUser >> to >> __acLoggedInCode
                >> suspend;

        if (suspend == "0") {
            to = EncDec::Decrypt(to);

            /*db.Update("acusers", "user", __acLoggedInUser, "lip=?, lccode=?, lcname=?, ldate=?, ltime=?", 5,
                       clientInfo->ip.c_str(), clientInfo->countryCode.c_str(), clientInfo->countryName.c_str(),
                       DateConv::ToJalali(n).c_str(), DateConv::Time(n).c_str());
            */
            if (BString::Trim(to) != "") {
                //Mailer::SendLoginAlert(to, EncDec::Decrypt(__acLoggedInUser), n, serverInfo, clientInfo);
                hasEmail = true;
            }

            Error_Root(L"", errACLogin);
            ClearLoginForm_ACP();
            ReGenCap();
        }
        else {
            Error_Root(Base::errSuspendH, errACLogin);
            ClearLoginForm_ACP();
            ReGenCap();
            return;
        }
    }
    else {
        Error_Root(Base::errLoginH, errACLogin);
        ReGenCap();
        academyUserEdit->setFocus();
        return;
    }

    if (hasEmail) {
        Go_ACPanel(__acLoggedInCode);
    }
    else {
        dlg = new WDialog(L"پنل مدیریت آموزشگاه");
        dlg->setModal(true);

        academyPage = new Div(dlg->contents(), "dvAcademyPage");

        academyPage->addWidget(FirstLogin_ACP());
        dlg->show();
    }
}

void CgiRoot::ClearLoginForm_ACP() {
    academyCapEdit->setText("");
    academyUserEdit->setText("");
    academyPwEdit->setText("");
}

WWidget *CgiRoot::FirstLogin_ACP() {
    WContainerWidget *firstLogin = new WContainerWidget();
    firstLogin->setId("dvFirstLogin");

    new WText(L"<br />"
              "<h2>به سیستم مدیریت آموزشگاه خوش آمدید</h2>"
              "<br />"
              "این اولین ورود شما به سیستم می باشد. جهت ادامه ورود اطلاعات ذیل الزامی است:"
              "<br />"
              "<br />"
              , firstLogin);

    errConfirmFL_ACP = new WText(firstLogin);

    firstLogin->addWidget(new WText(L"<h3>تعیین آدرس ایمیل</h3>"));
    Div *dvEmailFL = new Div(firstLogin, "dvEmailFL");
    WGridLayout *dvEmailLayoutFL = new WGridLayout();

    newEmailEditFL_ACP = new WLineEdit();
    confirmEmailEditFL_ACP = new WLineEdit();

    dvEmailLayoutFL->addWidget(newEmailEditFL_ACP, 0, 0);
    dvEmailLayoutFL->addWidget(new WText(L"آدرس ایمیل"
                                                 ), 0, 1);
    dvEmailLayoutFL->addWidget(confirmEmailEditFL_ACP, 1, 0);
    dvEmailLayoutFL->addWidget(new WText(L"تائید آدرس ایمیل"
                                                 ), 1, 1);
    dvEmailLayoutFL->setVerticalSpacing(11);
    dvEmailLayoutFL->setColumnStretch(0, 200);
    dvEmailLayoutFL->setColumnStretch(1, 110);
    dvEmailFL->resize(310, WLength::Auto);
    dvEmailFL->setLayout(dvEmailLayoutFL);

    firstLogin->addWidget(new WText(L"<h3>تعیین کلمه عبور</h3>"));
    Div *dvPwFL = new Div(firstLogin, "dvPwFL");
    WGridLayout *dvPwLayoutFL = new WGridLayout();

    newPwEditFL_ACP = new WLineEdit();
    newPwEditFL_ACP->setEchoMode(WLineEdit::Password);
    confirmPwEditFL_ACP = new WLineEdit();
    confirmPwEditFL_ACP->setEchoMode(WLineEdit::Password);

    dvPwLayoutFL->addWidget(newPwEditFL_ACP, 0, 0);
    dvPwLayoutFL->addWidget(new WText(L"کلمه عبور جدید"
                                               ), 0, 1);
    dvPwLayoutFL->addWidget(confirmPwEditFL_ACP, 1, 0);
    dvPwLayoutFL->addWidget(new WText(L"تائید کلمه عبور"
                                               ), 1, 1);
    dvPwLayoutFL->setVerticalSpacing(11);
    dvPwLayoutFL->setColumnStretch(0, 200);
    dvPwLayoutFL->setColumnStretch(1, 110);
    dvPwFL->resize(310, WLength::Auto);
    dvPwFL->setLayout(dvPwLayoutFL);

    new WBreak(firstLogin);

    Div *dvConfirmFL = new Div(firstLogin, "dvConfirmFL");
    WGridLayout *dvConfirmLayoutFL = new WGridLayout();

    currentPwEditFL_ACP = new WLineEdit();
    currentPwEditFL_ACP->setEchoMode(WLineEdit::Password);

    dvConfirmLayoutFL->addWidget(currentPwEditFL_ACP, 0, 0);
    dvConfirmLayoutFL->addWidget(new WText(L"کلمه عبور فعلی"
                                               ), 0, 1);
    dvConfirmLayoutFL->setVerticalSpacing(11);
    dvConfirmLayoutFL->setColumnStretch(0, 200);
    dvConfirmLayoutFL->setColumnStretch(1, 110);
    dvConfirmFL->resize(310, WLength::Auto);
    dvConfirmFL->setLayout(dvConfirmLayoutFL);

    WPushButton *btnConfirmOK = new WPushButton(L"تائید", firstLogin);
    btnConfirmOK->setStyleClass("btnHome");

    newEmailValidatorFL_ACP = new WRegExpValidator(Base::regExpEmail);
    confirmEmailValidatorFL_ACP = new WRegExpValidator(Base::regExpEmail);
    newPwValidatorFL_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    confirmPwValidatorFL_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    currentPwValidatorFL_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);

    newEmailValidatorFL_ACP->setMandatory(true);
    confirmEmailValidatorFL_ACP->setMandatory(true);
    newPwValidatorFL_ACP->setMandatory(true);
    confirmPwValidatorFL_ACP->setMandatory(true);
    currentPwValidatorFL_ACP->setMandatory(true);

    newEmailEditFL_ACP->setValidator(newEmailValidatorFL_ACP);
    confirmEmailEditFL_ACP->setValidator(confirmEmailValidatorFL_ACP);
    newPwEditFL_ACP->setValidator(newPwValidatorFL_ACP);
    confirmPwEditFL_ACP->setValidator(confirmPwValidatorFL_ACP);
    currentPwEditFL_ACP->setValidator(currentPwValidatorFL_ACP);

    newEmailEditFL_ACP->enterPressed().connect(this, &CgiRoot::ConfirmOKFL_ACP);
    confirmEmailEditFL_ACP->enterPressed().connect(this, &CgiRoot::ConfirmOKFL_ACP);
    newPwEditFL_ACP->enterPressed().connect(this, &CgiRoot::ConfirmOKFL_ACP);
    confirmPwEditFL_ACP->enterPressed().connect(this, &CgiRoot::ConfirmOKFL_ACP);
    currentPwEditFL_ACP->enterPressed().connect(this, &CgiRoot::ConfirmOKFL_ACP);
    btnConfirmOK->clicked().connect(this, &CgiRoot::ConfirmOKFL_ACP);

    newEmailEditFL_ACP->setFocus();

    new WBreak(firstLogin);
    new WBreak(firstLogin);
    new WBreak(firstLogin);

    return firstLogin;
}

void CgiRoot::ConfirmOKFL_ACP() {
    if(Validate(newEmailEditFL_ACP) && Validate(confirmEmailEditFL_ACP) &&
       Validate(newPwEditFL_ACP) && Validate(confirmPwEditFL_ACP) &&
       Validate(currentPwEditFL_ACP)) {
        if (newEmailEditFL_ACP->text() == confirmEmailEditFL_ACP->text()) {
            if (newPwEditFL_ACP->text() == confirmPwEditFL_ACP->text()) {
                row r;
                db.sql << "SELECT pw FROM [acusers] "
                        "WHERE user=? AND pw=?", __acLoggedInUser,
                        EncDec::Encrypt(currentPwEditFL_ACP->text().toUTF8()), r;

                if(db.sql.single(r)) {
                    if (currentPwEditFL_ACP->text() != newPwEditFL_ACP->text()) {
                        //Error_Root(L"", errConfirmFL_ACP);
                        db.Update("acusers", "user", __acLoggedInUser, "pw=?, email=?", 2,
                                  EncDec::Encrypt(newPwEditFL_ACP->text().toUTF8()).c_str(),
                                  EncDec::Encrypt(newEmailEditFL_ACP->text().toUTF8()).c_str());
                        dlg->accept();
                        delete dlg;
                        dlg = NULL;
                        Go_ACPanel(__acLoggedInCode);
                    }
                    else {
                        Error_Root(Base::errSamePwH, errConfirmFL_ACP);
                        newPwEditFL_ACP->setFocus();
                    }
                }
                else {
                    Error_Root(Base::errInvalidPwH, errConfirmFL_ACP);
                    currentPwEditFL_ACP->setFocus();
                }
            }
            else {
                Error_Root(Base::errConfirmPwH, errConfirmFL_ACP);
                confirmPwEditFL_ACP->setFocus();
            }
        }
        else {
            Error_Root(Base::errConfirmEmailH, errConfirmFL_ACP);
            confirmEmailEditFL_ACP->setFocus();
        }
    }
}

string CgiRoot::AcSessionGen(const string& acCode, bool isRoot) {
    string session = EncDec::Encrypt(BString::IntToStr((unsigned)time(NULL)) + acCode + (!isRoot ? "": "root"));

    db.Update("acusers", "accode", acCode, (!isRoot ? "session=?" : "rsession=?") , 1, session.c_str());

    return session;
}

bool CgiRoot::AcSessionValidate(const string& session) {
    CDate::Now n;
    row r;

/*    db.sql << "SELECT session FROM [acusers] "
            "WHERE session=?", session, r;*/
    db.sql << "SELECT user, email, accode, lip, lccode, lcname, ldate, ltime FROM [acusers] "
            "WHERE session=? OR rsession=?", session, session, r;

    if(db.sql.single(r)) {
        string decSession = EncDec::Decrypt(session);
        string isRoot = decSession.substr(decSession.size() - 4, 4);

        if (isRoot != "root") {
            db.Update("acusers", "session", session, "session=?", 1, "");
        }

        string to;
        r >> __acLoggedInUser >> to >> __acLoggedInCode
                >> __acLastIP >> __acLastCCode >> __acLastCName
                >> __acLastDate >> __acLastTime;

        to = EncDec::Decrypt(to);

        if (isRoot != "root") {
            db.Update("acusers", "user", __acLoggedInUser, "lip=?, lccode=?, lcname=?, ldate=?, ltime=?", 5,
                       clientInfo->ip.c_str(), clientInfo->countryCode.c_str(), clientInfo->countryName.c_str(),
                       DateConv::ToJalali(n).c_str(), DateConv::Time(n).c_str());

            if (BString::Trim(to) != "") {
                Mailer::SendLoginAlert(to, EncDec::Decrypt(__acLoggedInUser), n, serverInfo, clientInfo);
            }
        }

        return true;
    }

    return false;
}

void CgiRoot::Go_ACPanel(const string& acCode) {
    redirect("?acp=" + AcSessionGen(acCode, false));
    quit();
}

void CgiRoot::GoAway_ACP() {
    WContainerWidget *err = new WContainerWidget;
    err->addWidget(new WText(L"<br /><br /><h1 style=\"text-align: center\">لطفا ابتدا وارد سیستم شوید</h1>"));
    root()->addWidget(err);
}

void CgiRoot::Initialize_ACPanel() {
    root()->clear();

    setTitle(L".:: پنل مدیریت ::.");
    useStyleSheet("root-cp.css");
    useStyleSheet("root-ac.css");

    WContainerWidget *acPanel = new WContainerWidget();

    Div *dvBody = new Div(acPanel, "dvBody");

    Div *dvBodyInner = new Div(dvBody, "dvBodyInner");
    Div *dvHeader = new Div(dvBodyInner, "dvHeader");
    Div *dvLogo = new Div(dvHeader, "dvLogo");

    row r;
    db.sql << "SELECT name FROM [academies] "
            "WHERE code=?", __acLoggedInCode;

    string acName;
    if(db.sql.single(r)) {
        r >> acName;
    }
    acName = " - آموزشگاه " + acName;
    dvLogo->addWidget(new WText(Base::titleText + BString::StrToWStr(acName)));

    Div *dvMenusWrapper = new Div(dvBodyInner , "dvMenusWrapper");
    Div *dvMenusInner = new Div(dvMenusWrapper , "dvMenusInner");
    Div *dvMenus = new Div(dvMenusInner , "dvMenus");
    Div *dvLastLogin = new Div(dvMenusInner , "dvLastLogin");
    dvLastLogin->addWidget(new WText(L"<h2>آخرین ورود</h2>"));
    Div *dvLastLoginDetail = new Div(dvLastLogin , "dvLastLoginDetail");
    dvLastLoginDetail->addWidget(new WText(__acLastDate + " - " + __acLastTime +
                                           "<br />" +
                                           __acLastCCode + ", " + __acLastCName +
                                           "<br />" +
                                           __acLastIP
                                           ));

    Div *dvContentsWrapper = new Div(dvBodyInner, "dvContentsWrapper");
    Div *dvContentsInner = new Div(dvContentsWrapper, "dvContentsInner");
    Div *dvContents = new Div(dvContentsInner, "dvContents");

    WStackedWidget *mainContents = new WStackedWidget(dvContents);
    mainContents->setId("mainContents");

    mainMenu_ACP = new WMenu(mainContents, Vertical, dvMenus);
    mainMenu_ACP->setRenderAsList(true);

    mainMenu_ACP->addItem
            (L"مربیان", CMentors_ACP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_ACP->addItem
            (L"کارآموزان", CTyros_ACP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_ACP->addItem
            (L"دوره های آموزشی", CTerms_ACP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_ACP->addItem
            (L"کلمه عبور و ایمیل", CPwEMail_ACP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_ACP->addItem
            (L"خروج", CExit_ACP(), WMenuItem::PreLoading)->setPathComponent("");

    mainMenu_ACP->itemSelected().connect(this, &CgiRoot::ExitACPanel);

    root()->addWidget(acPanel);
}

void CgiRoot::ExitACPanel(WMenuItem *mItem) {
    if (mItem->text() == L"خروج") {
        redirect(".");
        quit();
    }
}

WWidget *CgiRoot::CMentors_ACP() {
    WContainerWidget *mentors = new WContainerWidget();
    mentors->setId("dvCMentors");

    WPushButton *mentorsAddBtn_ACP = new WPushButton(L"افزودن", mentors);
    mentorsAddBtn_ACP->clicked().connect(this, &CgiRoot::MentorsAddBtnOK_ACP);

    dvMentorsWrapper_ACP = new Div(mentors, "dvMentors");

    CMentorsDataTable_ACP();

    return mentors;
}

void CgiRoot::MentorsAddBtnOK_ACP() {
    dlg = new WDialog(L"افزودن مربی");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetMentorsForm_ACP("add"));
    dlg->show();
}

void CgiRoot::CMentorsDataTable_ACP() {
    dvMentorsWrapper_ACP->clear();

    WTable *mentorsTable = new WTable(dvMentorsWrapper_ACP);
    mentorsTable->setStyleClass("tbl");

    mentorsTable->elementAt(0, 0)->addWidget(new WText(L"نام و نام خانوادگی"));
    mentorsTable->elementAt(0, 1)->addWidget(new WText(L"جنسیت"));
    mentorsTable->elementAt(0, 2)->addWidget(new WText(L"تلفن"));
    mentorsTable->elementAt(0, 3)->addWidget(new WText(L"همراه"));
    mentorsTable->elementAt(0, 4)->addWidget(new WText(L"سایر اطلاعات"));
    mentorsTable->elementAt(0, 5)->addWidget(new WText(L"کارت ها"));
    mentorsTable->elementAt(0, 6)->addWidget(new WText(L"ویرایش"));
    mentorsTable->elementAt(0, 7)->addWidget(new WText(L"حذف"));

    mentorsTable->elementAt(0, 0)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 1)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 2)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 3)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 4)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 5)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 6)->setStyleClass("tblHeader");
    mentorsTable->elementAt(0, 7)->setStyleClass("tblHeader");

    result res;
    row r;

    db.sql << "SELECT code, fname, lname, sex, tel, mobile "
            "FROM [acmentors] WHERE accode=? "
            "ORDER BY fname, lname ASC;", __acLoggedInCode;

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string code, fname, lname, sex, tel, mobile;
            r >> code >> fname >> lname >> sex >> tel >> mobile;

            mentorsTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(fname + " " + lname)));
            mentorsTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(sex)));
            mentorsTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(tel)));
            mentorsTable->elementAt(i, 3)->addWidget(new WText(BString::StrToWStr(mobile)));

            WSignalMapper<WPushButton *> *moreInfoMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *cardMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            moreInfoMap->mapped().connect(this, &CgiRoot::MoreInfoMentorsTableCell_ACP);
            cardMap->mapped().connect(this, &CgiRoot::CardMentorsTableCell_ACP);
            editMap->mapped().connect(this, &CgiRoot::EditMentorsTableCell_ACP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseMentorsTableCell_ACP);

            WPushButton *moreInfoButton = new WPushButton(L"سایر اطلاعات");
            WPushButton *cardButton = new WPushButton(L"کارت ها");
            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            moreInfoMap->mapConnect(moreInfoButton->clicked(), moreInfoButton);
            cardMap->mapConnect(cardButton->clicked(), cardButton);
            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            moreInfoButton->setStyleClass("tblButton");
            cardButton->setStyleClass("tblButton");
            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            moreInfoButton->setAttributeValue("dbid", BString::StrToWStr(code));
            cardButton->setAttributeValue("dbid", BString::StrToWStr(code));
            editButton->setAttributeValue("dbid", BString::StrToWStr(code));
            eraseButton->setAttributeValue("dbid", BString::StrToWStr(code));

            mentorsTable->elementAt(i, 4)->addWidget(moreInfoButton);
            mentorsTable->elementAt(i, 5)->addWidget(cardButton);
            mentorsTable->elementAt(i, 6)->addWidget(editButton);
            mentorsTable->elementAt(i, 7)->addWidget(eraseButton);
        }
    }
}

void CgiRoot::MoreInfoMentorsTableCell_ACP(WAnchor *sender) {
    WPushButton *btn = new WPushButton();
    btn->setAttributeValue("dbid", sender->attributeValue("dbid"));
    __acLoggedInCode = sender->attributeValue("accode").toUTF8();
    MoreInfoMentorsTableCell_ACP(btn);
}

void CgiRoot::MoreInfoMentorsTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    string fname, lname, sex, nationalcode,
            birthid, birthdate, birthloc, birthemissionloc,
            graduatecert, graduatecourse,
            addr, tel, mobile, email;

    row r;

    db.sql << "SELECT fname, lname, sex, nationalcode, "
            "birthid, birthdate, birthloc, birthemissionloc, "
            "graduatecert, graduatecourse, "
            "addr, tel, mobile, email "
            "FROM [acmentors] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> fname >> lname >> sex >> nationalcode >>
        birthid >> birthdate >> birthloc >> birthemissionloc >>
        graduatecert >> graduatecourse >>
        addr >> tel >> mobile >> email;

        birthdate.insert(4, "/");
        birthdate.insert(7, "/");

        email = EncDec::Decrypt(email);
    }

    dlg = new WDialog(L"مشخصات مربی");
    dlg->setModal(true);

    WContainerWidget *dvMentorsMoreInfo = new Div(dlg->contents(), "dvMentorsMoreInfo");

    WImage *img = new WImage("showpics.fcgi?" + __acLoggedInCode + "m" + code + "p");
    img->resize(144, 192);
    dvMentorsMoreInfo->addWidget(img);
    new WBreak(dvMentorsMoreInfo);

    WContainerWidget *dvMentorsMoreInfoInner = new Div(dvMentorsMoreInfo, "dvMentorsMoreInfoInner");
    WGridLayout *dvMentorsMoreInfoInnerLayout = new WGridLayout();

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(fname)), 0, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 0, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"نام"
                                              ), 0, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(lname)), 1, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 1, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"نام خانوادگی"
                                              ), 1, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(sex)), 2, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 2, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"جنسیت"
                                              ), 2, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(nationalcode)), 3, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 3, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"کد ملی"
                                              ), 3, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthid)), 4, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 4, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"شماره شناسنامه"
                                              ), 4, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthdate)), 5, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 5, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"تاریخ تولد"
                                              ), 5, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthloc)), 6, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 6, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"محل تولد"
                                              ), 6, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthemissionloc)), 7, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 7, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"محل صدور شناسنامه"
                                              ), 7, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(graduatecert)), 8, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 8, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"آخرین مدرک تحصیلی"
                                              ), 8, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(graduatecourse)), 9, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 9, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"رشته تحصیلی"
                                              ), 9, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(addr)), 10, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 10, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"نشانی محل سکونت"
                                              ), 10, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(tel)), 11, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 11, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"تلفن تماس"
                                              ), 11, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(mobile)), 12, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 12, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"شماره همراه"
                                              ), 12, 2);

    dvMentorsMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(email)), 13, 0);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L":"), 13, 1);
    dvMentorsMoreInfoInnerLayout->addWidget(new WText(L"پست الکترونیکی"
                                              ), 13, 2);

    dvMentorsMoreInfoInnerLayout->setVerticalSpacing(11);
    dvMentorsMoreInfoInnerLayout->setColumnStretch(0, 190);
    dvMentorsMoreInfoInnerLayout->setColumnStretch(1, 25);
    dvMentorsMoreInfoInnerLayout->setColumnStretch(2, 150);
    dvMentorsMoreInfoInner->resize(365, WLength::Auto);
    dvMentorsMoreInfoInner->setLayout(dvMentorsMoreInfoInnerLayout);

    WPushButton *cancel = new WPushButton(L"بازگشت", dvMentorsMoreInfo);
    cancel->setStyleClass("btnDialogs");
    cancel->clicked().connect(dlg, &WDialog::accept);

    new WBreak(dvMentorsMoreInfo);
    new WBreak(dvMentorsMoreInfo);
    new WBreak(dvMentorsMoreInfo);
    new WBreak(dvMentorsMoreInfo);

    dlg->show();
}

void CgiRoot::CardMentorsTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();
    GetMCardDialog_ACP(code);
}

void CgiRoot::EditMentorsTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    dlg = new WDialog(L"ویرایش مربی");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetMentorsForm_ACP("edit"));

    string fname, lname, sex, nationalcode,
            birthid, birthdate, birthloc, birthemissionloc,
            graduatecert, graduatecourse,
            addr, tel, mobile, email;

    row r;

    db.sql << "SELECT fname, lname, sex, nationalcode, "
            "birthid, birthdate, birthloc, birthemissionloc, "
            "graduatecert, graduatecourse, "
            "addr, tel, mobile, email "
            "FROM [acmentors] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> fname >> lname >> sex >> nationalcode >>
        birthid >> birthdate >> birthloc >> birthemissionloc >>
        graduatecert >> graduatecourse >>
        addr >> tel >> mobile >> email;

        email = EncDec::Decrypt(email);
    }

    mentorsFNameEdit_ACP->setText(BString::StrToWStr(fname));
    mentorsFNameEdit_ACP->setAttributeValue("dbid", BString::StrToWStr(code));

    mentorsLNameEdit_ACP->setText(BString::StrToWStr(lname));
    mentorsSexCmb_ACP->setCurrentIndex(mentorsSexCmb_ACP->findText(BString::StrToWStr(sex), MatchExactly));
    mentorsNationalCodeEdit_ACP->setText(BString::StrToWStr(nationalcode));
    mentorsBirthIdEdit_ACP->setText(BString::StrToWStr(birthid));

    mentorsBirthDateCmbY_ACP->setCurrentIndex(mentorsBirthDateCmbY_ACP->findText(BString::StrToWStr(birthdate.substr(0, 4)), MatchExactly));
    mentorsBirthDateCmbM_ACP->setCurrentIndex(mentorsBirthDateCmbM_ACP->findText(BString::StrToWStr(birthdate.substr(4, 2)), MatchExactly));
    mentorsBirthDateCmbD_ACP->setCurrentIndex(mentorsBirthDateCmbD_ACP->findText(BString::StrToWStr(birthdate.substr(6, 2)), MatchExactly));

    mentorsBirthlocCmb_ACP->setCurrentIndex(mentorsBirthlocCmb_ACP->findText(BString::StrToWStr(birthloc), MatchExactly));
    mentorsBirthEmissionLocCmb_ACP->setCurrentIndex(mentorsBirthEmissionLocCmb_ACP->findText(BString::StrToWStr(birthemissionloc), MatchExactly));
    mentorsGraduateCertCmb_ACP->setCurrentIndex(mentorsGraduateCertCmb_ACP->findText(BString::StrToWStr(graduatecert), MatchExactly));
    mentorsGraduateCourseEdit_ACP->setText(BString::StrToWStr(graduatecourse));
    mentorsAddrTArea_ACP->setText(BString::StrToWStr(addr));
    mentorsTelEdit_ACP->setText(BString::StrToWStr(tel));
    mentorsMobileEdit_ACP->setText(BString::StrToWStr(mobile));
    mentorsEmailEdit_ACP->setText(BString::StrToWStr(email));

    dlg->show();
}

void CgiRoot::EraseMentorsTableCell_ACP(WPushButton *sender) {
    tableBtn_RP = sender;

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف مربی موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseMentorsTableCellOK_ACP);

    msgBox->show();
}

void CgiRoot::EraseMentorsTableCellOK_ACP(StandardButton result) {
    if (result == Ok) {
        string code = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;

        db.sql << "SELECT acmcode FROM [acmcards] "
                "WHERE acmcode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acmcards", "acmcode", code);
        }

        db.sql << "SELECT code FROM [acmentors] "
                "WHERE code=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acmentors", "code", code);
        }

        string id = "m" + code;
        dbPics.sql << "SELECT id FROM [" + __acLoggedInCode + "] "
                "WHERE id=?", id, r;

        if(dbPics.sql.single(r)) {
            dbPics.Delete(__acLoggedInCode, "id", id);
        }

        CMentorsDataTable_ACP();
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

WWidget *CgiRoot::GetMentorsForm_ACP(WString form) {
    WContainerWidget *mentorsForm = new WContainerWidget();
    mentorsForm->setId("dvMentorsFormWrapper");

    mentorsFNameEdit_ACP = new WLineEdit();
    mentorsLNameEdit_ACP = new WLineEdit();
    mentorsSexCmb_ACP = new WComboBox();
    mentorsNationalCodeEdit_ACP = new WLineEdit();
    mentorsBirthIdEdit_ACP = new WLineEdit();

    WContainerWidget *dvDate = new WContainerWidget();
    dvDate->setId("dvDate");
    mentorsBirthDateCmbY_ACP = new WComboBox(dvDate);
    mentorsBirthDateCmbM_ACP = new WComboBox(dvDate);
    mentorsBirthDateCmbD_ACP = new WComboBox(dvDate);

    mentorsBirthlocCmb_ACP = new WComboBox();
    mentorsBirthEmissionLocCmb_ACP = new WComboBox();
    mentorsGraduateCertCmb_ACP = new WComboBox();
    mentorsGraduateCourseEdit_ACP = new WLineEdit();
    mentorsAddrTArea_ACP = new WTextArea();
    mentorsTelEdit_ACP = new WLineEdit();
    mentorsMobileEdit_ACP = new WLineEdit();
    mentorsEmailEdit_ACP = new WLineEdit();
    mentorsPicFUP_ACP = new WFileUpload();


    mentorsSexCmb_ACP->addItem(L"");
    mentorsSexCmb_ACP->addItem(L"مرد");
    mentorsSexCmb_ACP->addItem(L"زن");


    mentorsBirthDateCmbY_ACP->addItem("");
    mentorsBirthDateCmbM_ACP->addItem("");
    mentorsBirthDateCmbD_ACP->addItem("");

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y - 10; i >= 1310; --i) {
        mentorsBirthDateCmbY_ACP->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        mentorsBirthDateCmbM_ACP->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        mentorsBirthDateCmbD_ACP->addItem(d);
    }


    mentorsBirthlocCmb_ACP->addItem(L"");
    mentorsBirthEmissionLocCmb_ACP->addItem(L"");

    result res;
    row r;

    db.sql << "SELECT province FROM [provinces] ORDER BY province ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string pr;
            r >> pr;
            mentorsBirthlocCmb_ACP->addItem(BString::StrToWStr(pr));
            mentorsBirthEmissionLocCmb_ACP->addItem(BString::StrToWStr(pr));
        }
    }


    mentorsGraduateCertCmb_ACP->addItem(L"");
    mentorsGraduateCertCmb_ACP->addItem(L"بیسواد");
    mentorsGraduateCertCmb_ACP->addItem(L"زیر دیپلم");
    mentorsGraduateCertCmb_ACP->addItem(L"دیپلم");
    mentorsGraduateCertCmb_ACP->addItem(L"کاردان");
    mentorsGraduateCertCmb_ACP->addItem(L"کارشناس");
    mentorsGraduateCertCmb_ACP->addItem(L"کارشناس ارشد");
    mentorsGraduateCertCmb_ACP->addItem(L"دکترا");


    Div *dvMentorsForm = new Div(mentorsForm, "dvMentorsForm");
    WGridLayout *dvMentorsFormLayout = new WGridLayout();

    dvMentorsFormLayout->addWidget(mentorsFNameEdit_ACP, 0, 0);
    dvMentorsFormLayout->addWidget(new WText(L"نام"
                                               ), 0, 1);

    dvMentorsFormLayout->addWidget(mentorsLNameEdit_ACP, 1, 0);
    dvMentorsFormLayout->addWidget(new WText(L"نام خانوادگی"
                                               ), 1, 1);

    dvMentorsFormLayout->addWidget(mentorsSexCmb_ACP, 2, 0);
    dvMentorsFormLayout->addWidget(new WText(L"جنسیت"
                                               ), 2, 1);

    dvMentorsFormLayout->addWidget(mentorsNationalCodeEdit_ACP, 3, 0);
    dvMentorsFormLayout->addWidget(new WText(L"کد ملی"
                                               ), 3, 1);

    dvMentorsFormLayout->addWidget(mentorsBirthIdEdit_ACP, 4, 0);
    dvMentorsFormLayout->addWidget(new WText(L"شماره شناسنامه"
                                               ), 4, 1);

    dvMentorsFormLayout->addWidget(dvDate, 5, 0);
    dvMentorsFormLayout->addWidget(new WText(L"تاریخ تولد"
                                               ), 5, 1);

    dvMentorsFormLayout->addWidget(mentorsBirthlocCmb_ACP, 6, 0);
    dvMentorsFormLayout->addWidget(new WText(L"محل تولد"
                                               ), 6, 1);

    dvMentorsFormLayout->addWidget(mentorsBirthEmissionLocCmb_ACP, 7, 0);
    dvMentorsFormLayout->addWidget(new WText(L"محل صدور شناسنامه"
                                               ), 7, 1);

    dvMentorsFormLayout->addWidget(mentorsGraduateCertCmb_ACP, 8, 0);
    dvMentorsFormLayout->addWidget(new WText(L"آخرین مدرک تحصیلی"
                                               ), 8, 1);

    dvMentorsFormLayout->addWidget(mentorsGraduateCourseEdit_ACP, 9, 0);
    dvMentorsFormLayout->addWidget(new WText(L"رشته تحصیلی"
                                               ), 9, 1);

    dvMentorsFormLayout->addWidget(mentorsAddrTArea_ACP, 10, 0);
    dvMentorsFormLayout->addWidget(new WText(L"نشانی محل سکونت"
                                               ), 10, 1);

    dvMentorsFormLayout->addWidget(mentorsTelEdit_ACP, 11, 0);
    dvMentorsFormLayout->addWidget(new WText(L"تلفن تماس"
                                               ), 11, 1);

    dvMentorsFormLayout->addWidget(mentorsMobileEdit_ACP, 12, 0);
    dvMentorsFormLayout->addWidget(new WText(L"شماره همراه"
                                               ), 12, 1);

    dvMentorsFormLayout->addWidget(mentorsEmailEdit_ACP, 13, 0);
    dvMentorsFormLayout->addWidget(new WText(L"پست الکترونیکی"
                                               ), 13, 1);

    dvMentorsFormLayout->addWidget(mentorsPicFUP_ACP, 14, 0);
    dvMentorsFormLayout->addWidget(new WText(L"عکس پرسنلی"
                                               ), 14, 1);

    dvMentorsFormLayout->setVerticalSpacing(11);
    dvMentorsFormLayout->setColumnStretch(0, 200);
    dvMentorsFormLayout->setColumnStretch(1, 200);
    dvMentorsForm->resize(400, WLength::Auto);
    dvMentorsForm->setLayout(dvMentorsFormLayout);

    mentorsSaveBtn_ACP = new WPushButton(mentorsForm);
    mentorsSaveBtn_ACP->setStyleClass("btnDialogs");

    if (form == "add") {
        mentorsSaveBtn_ACP->setText(L"افزودن");
    }
    else if (form == "edit") {
        mentorsSaveBtn_ACP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", mentorsForm);
    cancel->clicked().connect(dlg, &WDialog::accept);
    cancel->setStyleClass("btnDialogs");

    errMentorsForm_ACP = new WText("");
    mentorsForm->addWidget(errMentorsForm_ACP);
    mentorsSaveBtn_ACP->setAttributeValue("mode", form);

    mentorsFNameValidator_ACP = new WLengthValidator(1, 15);
    mentorsLNameValidator_ACP = new WLengthValidator(1, 20);
    mentorsSexValidator_ACP = new WLengthValidator(1, 3);
    mentorsNationalCodeValidator_ACP = new WRegExpValidator(Base::regExpNationalCode);
    mentorsBirthIdValidator_ACP = new WRegExpValidator(Base::regExpBirthId);
    mentorsBirthDateYValidator_ACP = new WLengthValidator(1, 4);
    mentorsBirthDateMValidator_ACP = new WLengthValidator(1, 2);
    mentorsBirthDateDValidator_ACP = new WLengthValidator(1, 2);
    mentorsBirthlocValidator_ACP = new WLengthValidator(1, 35);
    mentorsBirthEmissionLocValidator_ACP = new WLengthValidator(1, 35);
    mentorsGraduateCertValidator_ACP = new WLengthValidator(1, 13);
    mentorsGraduateCourseValidator_ACP = new WLengthValidator(1, 50);
    mentorsAddrValidator_ACP = new WLengthValidator(1, 255);
    mentorsTelValidator_ACP = new WRegExpValidator(Base::regExpTel);
    mentorsMobileValidator_ACP = new WRegExpValidator(Base::regExpMobile);
    mentorsEmailValidator_ACP = new WRegExpValidator(Base::regExpEmail);

    mentorsFNameValidator_ACP->setMandatory(true);
    mentorsLNameValidator_ACP->setMandatory(true);
    mentorsSexValidator_ACP->setMandatory(true);
    mentorsNationalCodeValidator_ACP->setMandatory(true);
    mentorsBirthIdValidator_ACP->setMandatory(true);
    mentorsBirthDateYValidator_ACP->setMandatory(true);
    mentorsBirthDateMValidator_ACP->setMandatory(true);
    mentorsBirthDateDValidator_ACP->setMandatory(true);
    mentorsBirthlocValidator_ACP->setMandatory(true);
    mentorsBirthEmissionLocValidator_ACP->setMandatory(true);
    mentorsGraduateCertValidator_ACP->setMandatory(true);
    mentorsGraduateCourseValidator_ACP->setMandatory(true);
    mentorsAddrValidator_ACP->setMandatory(true);
    mentorsTelValidator_ACP->setMandatory(true);
    //mentorsMobileValidator_ACP->setMandatory(true);
    //mentorsEmailValidator_ACP->setMandatory(true);

    mentorsFNameEdit_ACP->setValidator(mentorsFNameValidator_ACP);
    mentorsLNameEdit_ACP->setValidator(mentorsLNameValidator_ACP);
    mentorsSexCmb_ACP->setValidator(mentorsSexValidator_ACP);
    mentorsNationalCodeEdit_ACP->setValidator(mentorsNationalCodeValidator_ACP);
    mentorsBirthIdEdit_ACP->setValidator(mentorsBirthIdValidator_ACP);
    mentorsBirthDateCmbY_ACP->setValidator(mentorsBirthDateYValidator_ACP);
    mentorsBirthDateCmbM_ACP->setValidator(mentorsBirthDateMValidator_ACP);
    mentorsBirthDateCmbD_ACP->setValidator(mentorsBirthDateDValidator_ACP);
    mentorsBirthlocCmb_ACP->setValidator(mentorsBirthlocValidator_ACP);
    mentorsBirthEmissionLocCmb_ACP->setValidator(mentorsBirthEmissionLocValidator_ACP);
    mentorsGraduateCertCmb_ACP->setValidator(mentorsGraduateCertValidator_ACP);
    mentorsGraduateCourseEdit_ACP->setValidator(mentorsGraduateCourseValidator_ACP);
    mentorsAddrTArea_ACP->setValidator(mentorsAddrValidator_ACP);
    mentorsTelEdit_ACP->setValidator(mentorsTelValidator_ACP);
    mentorsMobileEdit_ACP->setValidator(mentorsMobileValidator_ACP);
    mentorsEmailEdit_ACP->setValidator(mentorsEmailValidator_ACP);

    mentorsFNameEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsLNameEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsSexCmb_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsNationalCodeEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthIdEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthDateCmbY_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthDateCmbM_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthDateCmbD_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthlocCmb_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsBirthEmissionLocCmb_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsGraduateCertCmb_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsGraduateCourseEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsAddrTArea_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsTelEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsMobileEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsEmailEdit_ACP->enterPressed().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);
    mentorsSaveBtn_ACP->clicked().connect(this, &CgiRoot::MentorsFormSaveBtnOK_ACP);

    //mentorsPicFUP_ACP->changed().connect(mentorsPicFUP_ACP, &WFileUpload::upload);
    //mentorsSaveBtn_ACP->clicked().connect(mentorsPicFUP_ACP, &Wt::WFileUpload::upload);

    mentorsPicFUP_ACP->uploaded().connect(this, &CgiRoot::MentorsFormFileUploaded_ACP);
    mentorsPicFUP_ACP->fileTooLarge().connect(this, &CgiRoot::MentorsFormFileTooLarge_ACP);

    //mentorsPicFUP_ACP->

    new WBreak(mentorsForm);
    new WBreak(mentorsForm);
    new WBreak(mentorsForm);
    new WBreak(mentorsForm);

    return mentorsForm;
}

void CgiRoot::MentorsFormFileUploaded_ACP() {
    if(Validate(mentorsFNameEdit_ACP) && Validate(mentorsLNameEdit_ACP) && Validate(mentorsSexCmb_ACP) &&
       Validate(mentorsNationalCodeEdit_ACP) && Validate(mentorsBirthIdEdit_ACP) &&
       Validate(mentorsBirthDateCmbY_ACP) && Validate(mentorsBirthDateCmbM_ACP) && Validate(mentorsBirthDateCmbD_ACP) &&
       Validate(mentorsBirthlocCmb_ACP) && Validate(mentorsBirthEmissionLocCmb_ACP) &&
       Validate(mentorsGraduateCertCmb_ACP) && Validate(mentorsGraduateCourseEdit_ACP) &&
       Validate(mentorsAddrTArea_ACP) && Validate(mentorsTelEdit_ACP) && Validate(mentorsMobileEdit_ACP) &&
       Validate(mentorsEmailEdit_ACP)) {
        string mode = mentorsSaveBtn_ACP->attributeValue("mode").toUTF8();

        string file = mentorsPicFUP_ACP->spoolFileName();
        if (mode == "add" || (mode == "edit" && file != "")) {
            if (!ValidatePic(file)) {
                return;
            }
        }

        string fname = BString::Trim(mentorsFNameEdit_ACP->text().toUTF8());
        string lname = BString::Trim(mentorsLNameEdit_ACP->text().toUTF8());
        string sex = BString::Trim(mentorsSexCmb_ACP->currentText().toUTF8());
        string nationalcode = BString::Trim(mentorsNationalCodeEdit_ACP->text().toUTF8());
        string birthid = BString::Trim(mentorsBirthIdEdit_ACP->text().toUTF8());
        string birthdate = BString::Trim(mentorsBirthDateCmbY_ACP->currentText().toUTF8()
                                         + mentorsBirthDateCmbM_ACP->currentText().toUTF8()
                                         + mentorsBirthDateCmbD_ACP->currentText().toUTF8());
        string birthloc = BString::Trim(mentorsBirthlocCmb_ACP->currentText().toUTF8());
        string birthemissionloc = BString::Trim(mentorsBirthEmissionLocCmb_ACP->currentText().toUTF8());
        string graduatecert = BString::Trim(mentorsGraduateCertCmb_ACP->currentText().toUTF8());
        string graduatecourse = BString::Trim(mentorsGraduateCourseEdit_ACP->text().toUTF8());
        string addr = BString::Trim(mentorsAddrTArea_ACP->text().toUTF8());
        string tel = BString::Trim(mentorsTelEdit_ACP->text().toUTF8());
        string mobile = BString::Trim(mentorsMobileEdit_ACP->text().toUTF8());
        string email = BString::Trim(mentorsEmailEdit_ACP->text().toUTF8());

        email = EncDec::Encrypt(email);

        row r;
        string code;

        if (mode == "add") {
            bool found = true;
            do {
                code = BString::Trim(BString::IntToStr((unsigned)time(NULL)));
                db.sql << "SELECT code FROM [acmentors] "
                        "WHERE code=?", code, r;
                if(!db.sql.single(r)) {
                    found = false;
                }
            } while (found);

            //ValidatePicFlag = true;
            ValidatePicPostAction = "GetMCardDialog_ACP";
            ValidatePicPostArgs = code;

            db.Insert("acmentors",
                      "code, accode, fname, lname, sex, nationalcode, "
                      "birthid, birthdate, birthloc, birthemissionloc, "
                      "graduatecert, graduatecourse, "
                      "addr, tel, mobile, email",
                      16,
                      code.c_str(), __acLoggedInCode.c_str(), fname.c_str(), lname.c_str(), sex.c_str(), nationalcode.c_str(),
                      birthid.c_str(), birthdate.c_str(), birthloc.c_str(), birthemissionloc.c_str(),
                      graduatecert.c_str(), graduatecourse.c_str(),
                      addr.c_str(), tel.c_str(), mobile.c_str(), email.c_str());

            dbPics.Insert(__acLoggedInCode, "id, personnel", 2,
                          ("m" + code).c_str(), CImage::B64Encode(file).c_str());

            /*delete dlg;
            dlg = NULL;*/
            CMentorsDataTable_ACP();
            //GetMCardDialog_ACP(code);
        }
        else if (mode == "edit") {
            code = mentorsFNameEdit_ACP->attributeValue("dbid").toUTF8();

            db.Update("acmentors", "code", code,
                      "fname=?, lname=?, sex=?, nationalcode=?, "
                      "birthid=?, birthdate=?, birthloc=?, birthemissionloc=?, "
                      "graduatecert=?, graduatecourse=?, "
                      "addr=?, tel=?, mobile=?, email=?",
                      14,
                      fname.c_str(), lname.c_str(), sex.c_str(), nationalcode.c_str(),
                      birthid.c_str(), birthdate.c_str(), birthloc.c_str(), birthemissionloc.c_str(),
                      graduatecert.c_str(), graduatecourse.c_str(),
                      addr.c_str(), tel.c_str(), mobile.c_str(), email.c_str());

            if (file != "") {
                dbPics.Update(__acLoggedInCode, "id", "m" + code, "personnel=?", 1, CImage::B64Encode(file).c_str());
            }
            else {
                delete dlg;
                dlg = NULL;
            }

            /*delete dlg;
            dlg = NULL;*/
            CMentorsDataTable_ACP();
        }
    }
}

void CgiRoot::MentorsFormFileTooLarge_ACP() {
    Error_Root(Base::errUpFileTooLargeH, errMentorsForm_ACP);
}

void CgiRoot::MentorsFormSaveBtnOK_ACP() {
    if(Validate(mentorsFNameEdit_ACP) && Validate(mentorsLNameEdit_ACP) && Validate(mentorsSexCmb_ACP) &&
       Validate(mentorsNationalCodeEdit_ACP) && Validate(mentorsBirthIdEdit_ACP) &&
       Validate(mentorsBirthDateCmbY_ACP) && Validate(mentorsBirthDateCmbM_ACP) && Validate(mentorsBirthDateCmbD_ACP) &&
       Validate(mentorsBirthlocCmb_ACP) && Validate(mentorsBirthEmissionLocCmb_ACP) &&
       Validate(mentorsGraduateCertCmb_ACP) && Validate(mentorsGraduateCourseEdit_ACP) &&
       Validate(mentorsAddrTArea_ACP) && Validate(mentorsTelEdit_ACP) && Validate(mentorsMobileEdit_ACP) &&
       Validate(mentorsEmailEdit_ACP)) {
        Error_Root(L"", errMentorsForm_ACP);

        /*string mode = mentorsSaveBtn_ACP->attributeValue("mode").toUTF8();
        string file = mentorsPicFUP_ACP->spoolFileName();
        if (mode == "add" || (mode == "edit" && file != "")) {
            if (mentorsPicFUP_ACP->canUpload()) {
                mentorsPicFUP_ACP->upload();
            }
        }
        else if (mode == "edit" && file == "") {
            if (mentorsPicFUP_ACP->canUpload()) {
                mentorsPicFUP_ACP->upload();
            }
        }*/

        mentorsPicFUP_ACP->upload();
    }
}

void CgiRoot::MCardAddBtnOK_ACP(WPushButton *sender) {
    string acmCode = sender->attributeValue("acmcode").toUTF8();
    string acmName = sender->attributeValue("acmname").toUTF8();

    if (dlg) {
        delete dlg;
        dlg = NULL;
    }

    dlg = new WDialog(L"افزودن کارت - "
                      + BString::StrToWStr(acmName));
    dlg->setModal(true);
    dlg->contents()->addWidget(GetMCardForm_ACP("add"));
    cardSaveBtn_ACP->setAttributeValue("acmcode", BString::StrToWStr(acmCode));

    dlg->show();
}

void CgiRoot::GetMCardDialog_ACP(string acmCode) {
    row r;
    db.sql << "SELECT fname, lname FROM [acmentors] WHERE "
            "code=?", acmCode, r;

    string acmFName, acmLName, acmName;
    if(db.sql.single(r)) {
        r >> acmFName >> acmLName;
        acmName = acmFName + " " + acmLName;
    }

    dlg = new WDialog(L"کارت های مربی - "
                      + BString::StrToWStr(acmName));
    dlg->setModal(true);

    WPushButton *addBtn = new WPushButton(L"افزودن کارت", dlg->contents());
    addBtn->setStyleClass("btnDialogs");
    WSignalMapper<WPushButton *> *addMap = new WSignalMapper<WPushButton *>(this);
    addMap->mapped().connect(this, &CgiRoot::MCardAddBtnOK_ACP);
    addMap->mapConnect(addBtn->clicked(), addBtn);
    addBtn->setAttributeValue("acmcode", BString::StrToWStr(acmCode));
    addBtn->setAttributeValue("acmname", BString::StrToWStr(acmName));

    WPushButton *cancel = new WPushButton(L"بازگشت", dlg->contents());
    cancel->setStyleClass("btnDialogs");
    cancel->clicked().connect(dlg, &WDialog::accept);

    WTable *cardsTable = new WTable(dlg->contents());
    cardsTable->setStyleClass("tbl");

    cardsTable->elementAt(0, 0)->addWidget(new WText(L"دوره"));
    cardsTable->elementAt(0, 1)->addWidget(new WText(L"تاریخ صدور کارت"));
    cardsTable->elementAt(0, 2)->addWidget(new WText(L"درصد قبولی"));
    cardsTable->elementAt(0, 3)->addWidget(new WText(L"ویرایش"));
    cardsTable->elementAt(0, 4)->addWidget(new WText(L"حذف"));

    cardsTable->elementAt(0, 0)->setStyleClass("tblHeader");
    cardsTable->elementAt(0, 1)->setStyleClass("tblHeader");
    cardsTable->elementAt(0, 2)->setStyleClass("tblHeader");
    cardsTable->elementAt(0, 3)->setStyleClass("tblHeader");
    cardsTable->elementAt(0, 4)->setStyleClass("tblHeader");

    result res;

    db.sql << "SELECT stcode, date, percent FROM [acmcards] "
            "WHERE acmcode=?", acmCode;

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string stCode, date, percent;
            r >> stCode >> date >> percent;

            date.insert(4, "/");
            date.insert(7, "/");

            row str;

            db.sql << "SELECT title FROM [standards] WHERE "
                    "code=?", stCode, str;

            string stTitle;
            if(db.sql.single(str)) {
                str >> stTitle;
            }

            cardsTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(stTitle)));
            cardsTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(date)));
            cardsTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(percent)));

            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            editMap->mapped().connect(this, &CgiRoot::EditMCardTableCell_ACP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseMCardTableCell_ACP);

            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            editButton->setAttributeValue("acmcode", BString::StrToWStr(acmCode));
            editButton->setAttributeValue("stcode", BString::StrToWStr(stCode));
            eraseButton->setAttributeValue("acmcode", BString::StrToWStr(acmCode));
            eraseButton->setAttributeValue("stcode", BString::StrToWStr(stCode));

            cardsTable->elementAt(i, 3)->addWidget(editButton);
            cardsTable->elementAt(i, 4)->addWidget(eraseButton);
        }
    }

    dlg->contents()->addWidget(cardsTable);
    dlg->show();
}

void CgiRoot::GetMCardDialog_ACP() {
    string acmCode = cardSaveBtn_ACP->attributeValue("acmcode").toUTF8();
    try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }
    GetMCardDialog_ACP(acmCode);
}

void CgiRoot::EditMCardTableCell_ACP(WPushButton *sender) {
    string acmCode = sender->attributeValue("acmcode").toUTF8();
    string stCode = sender->attributeValue("stcode").toUTF8();

    try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }

    row r;
    db.sql << "SELECT fname, lname FROM [acmentors] "
            "WHERE code=?", acmCode, r;

    string acmFName, acmLName, acmName;
    if(db.sql.single(r)) {
        r >> acmFName >> acmLName;
        acmName = acmFName + " " + acmLName;
    }

    dlg = new WDialog(L"افزودن کارت های مربی - "
                      + BString::StrToWStr(acmName));

    dlg->setModal(true);

    dlg->contents()->addWidget(GetMCardForm_ACP("edit"));

    db.sql << "SELECT date, percent FROM [acmcards] "
            "WHERE acmcode=? AND stcode=?", acmCode, stCode, r;

    string date, percent;
    r >> date >> percent;

    db.sql << "SELECT title FROM [standards] WHERE "
            "code=?", stCode, r;

    string stTitle;
    if(db.sql.single(r)) {
        r >> stTitle;
    }

    cardSaveBtn_ACP->setAttributeValue("acmcode", BString::StrToWStr(acmCode));

    mCardStTitleCmb_ACP->setCurrentIndex(mCardStTitleCmb_ACP->findText(BString::StrToWStr(stTitle), MatchExactly));
    mCardStTitleCmb_ACP->setAttributeValue("stcode", BString::StrToWStr(stCode));
    if (date.size() == 8) {
        mCardDateCmbY_ACP->setCurrentIndex(mCardDateCmbY_ACP->findText(BString::StrToWStr(date.substr(0, 4)), MatchExactly));
        mCardDateCmbM_ACP->setCurrentIndex(mCardDateCmbM_ACP->findText(BString::StrToWStr(date.substr(4, 2)), MatchExactly));
        mCardDateCmbD_ACP->setCurrentIndex(mCardDateCmbD_ACP->findText(BString::StrToWStr(date.substr(6, 2)), MatchExactly));
    }
    mCardPercentEdit_ACP->setText(BString::StrToWStr(percent));

    dlg->show();
}

void CgiRoot::EraseMCardTableCell_ACP(WPushButton *sender) {
    tableBtn_RP = sender;

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف کارت موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseMCardTableCellOK_ACP);

    msgBox->show();
}

void CgiRoot::EraseMCardTableCellOK_ACP(StandardButton result) {
    if (result == Ok) {
        string acmCode = tableBtn_RP->attributeValue("acmcode").toUTF8();
        string stCode = tableBtn_RP->attributeValue("stcode").toUTF8();

        row r;

        /*db.sql << "SELECT acmcode FROM [acmcards] "
                "WHERE acmcode=? AND stcode=?", acmCode, stCode, r;*/
        db.sql << "SELECT acmcode FROM [acmcards] "
                "WHERE acmcode=?", acmCode, r;

        if(db.sql.single(r)) {
            /*db.sql << "DELETE FROM [acmcards] "
                    "WHERE acmcode=? AND stcode=?", acmCode, stCode, exec();*/
            db.Delete("acmcards", "acmcode", acmCode);
        }

        delete msgBox;
        msgBox = NULL;
        tableBtn_RP = NULL;

        delete dlg;
        dlg = NULL;

        GetMCardDialog_ACP(acmCode);
    }
    else {
        delete msgBox;
        msgBox = NULL;
        tableBtn_RP = NULL;
    }
}

WWidget *CgiRoot::GetMCardForm_ACP(WString form) {
    WContainerWidget *cardForm = new WContainerWidget();
    cardForm->setId("dvMCardFormWrapper");

    mCardStTitleCmb_ACP = new WComboBox();
    mCardPercentEdit_ACP = new WLineEdit();

    mCardStTitleCmb_ACP->addItem(L"");

    result res;
    row r;

    db.sql << "SELECT skcode FROM [accourses] "
              "WHERE accode=?;", __acLoggedInCode;
    db.sql.fetch(res);

    if (res.rows() > 0) {
        string sk;
        while(res.next(r)) {
            r >> sk;

            result rres;
            row rst;

            db.sql << "SELECT title FROM [standards] "
                    " WHERE skcode=? ORDER BY title ASC;", sk;
            db.sql.fetch(rres);

            if (rres.rows() > 0) {
                string st;
                while(rres.next(rst)) {
                    rst >> st;
                    mCardStTitleCmb_ACP->addItem(BString::StrToWStr(st));
                }
            }
        }
    }


    WContainerWidget *dvDate = new WContainerWidget();
    dvDate->setId("dvDate");
    mCardDateCmbY_ACP = new WComboBox(dvDate);
    mCardDateCmbM_ACP = new WComboBox(dvDate);
    mCardDateCmbD_ACP = new WComboBox(dvDate);

    mCardDateCmbY_ACP->addItem("");
    mCardDateCmbM_ACP->addItem("");
    mCardDateCmbD_ACP->addItem("");

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y; i >= 1350; --i) {
        mCardDateCmbY_ACP->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        mCardDateCmbM_ACP->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        mCardDateCmbD_ACP->addItem(d);
    }


    WContainerWidget *dvMCardForm = new Div(cardForm, "dvMCardForm");
    WGridLayout *dvMCardFormLayout = new WGridLayout();

    dvMCardFormLayout->addWidget(mCardStTitleCmb_ACP, 0, 0);
    dvMCardFormLayout->addWidget(new WText(L"دوره"
                                              ), 0, 1);

    dvMCardFormLayout->addWidget(dvDate, 1, 0);
    dvMCardFormLayout->addWidget(new WText(L"تاریخ صدور کارت"
                                              ), 1, 1);

    dvMCardFormLayout->addWidget(mCardPercentEdit_ACP, 2, 0);
    dvMCardFormLayout->addWidget(new WText(L"درصد قبولی"
                                              ), 2, 1);

    dvMCardFormLayout->setVerticalSpacing(11);
    dvMCardFormLayout->setColumnStretch(0, 200);
    dvMCardFormLayout->setColumnStretch(1, 125);
    dvMCardForm->resize(325, WLength::Auto);
    dvMCardForm->setLayout(dvMCardFormLayout);

    cardSaveBtn_ACP = new WPushButton(cardForm);
    cardSaveBtn_ACP->setStyleClass("btnDialogs");

    if (form == "add") {
        cardSaveBtn_ACP->setText(L"افزودن");
    }
    else if (form == "edit") {
        cardSaveBtn_ACP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", cardForm);
    cancel->clicked().connect(this, &CgiRoot::GetMCardDialog_ACP);
    cancel->setStyleClass("btnDialogs");

    errMCardForm_ACP = new WText("");
    cardForm->addWidget(errMCardForm_ACP);
    cardSaveBtn_ACP->setAttributeValue("mode", form);

    mCardStTitleValidator_ACP = new WLengthValidator(1, 50);
    mCardDateValidatorY_ACP = new WLengthValidator(1, 4);
    mCardDateValidatorM_ACP = new WLengthValidator(1, 2);
    mCardDateValidatorD_ACP = new WLengthValidator(1, 2);
    mCardPercentValidator_ACP = new WIntValidator(0, 100);

    mCardStTitleValidator_ACP->setMandatory(true);
    mCardDateValidatorY_ACP->setMandatory(true);
    mCardDateValidatorM_ACP->setMandatory(true);
    mCardDateValidatorD_ACP->setMandatory(true);
    mCardPercentValidator_ACP->setMandatory(true);

    mCardStTitleCmb_ACP->setValidator(mCardStTitleValidator_ACP);
    mCardDateCmbY_ACP->setValidator(mCardDateValidatorY_ACP);
    mCardDateCmbM_ACP->setValidator(mCardDateValidatorM_ACP);
    mCardDateCmbD_ACP->setValidator(mCardDateValidatorD_ACP);
    mCardPercentEdit_ACP->setValidator(mCardPercentValidator_ACP);

    mCardStTitleCmb_ACP->enterPressed().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);
    mCardDateCmbY_ACP->enterPressed().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);
    mCardDateCmbM_ACP->enterPressed().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);
    mCardDateCmbD_ACP->enterPressed().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);
    mCardPercentEdit_ACP->enterPressed().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);
    cardSaveBtn_ACP->clicked().connect(this, &CgiRoot::MCardFormSaveBtnOK_ACP);

    new WBreak(cardForm);
    new WBreak(cardForm);
    new WBreak(cardForm);
    new WBreak(cardForm);

    return cardForm;
}

void CgiRoot::MCardFormSaveBtnOK_ACP() {
    if(Validate(mCardStTitleCmb_ACP)
        && Validate(mCardDateCmbY_ACP) && Validate(mCardDateCmbM_ACP) && Validate(mCardDateCmbD_ACP)
        && Validate(mCardPercentEdit_ACP)) {

        string acmCode = cardSaveBtn_ACP->attributeValue("acmcode").toUTF8();

        string stTitle = BString::Trim(mCardStTitleCmb_ACP->currentText().toUTF8());
        string date = BString::Trim(mCardDateCmbY_ACP->currentText().toUTF8())
                      + BString::Trim(mCardDateCmbM_ACP->currentText().toUTF8())
                      + BString::Trim(mCardDateCmbD_ACP->currentText().toUTF8());
        string percent = BString::Trim(mCardPercentEdit_ACP->text().toUTF8());

        string mode = cardSaveBtn_ACP->attributeValue("mode").toUTF8();

        row r;

        db.sql << "SELECT code FROM [standards] "
                "WHERE title=?", stTitle, r;

        string stCode;
        if(db.sql.single(r)) {
            r >> stCode;
        }

        if (mode == "add") {
            db.sql << "SELECT accode FROM [acmcards] "
                    "WHERE acmcode=? AND stcode=?", acmCode, stCode, r;

            if(db.sql.single(r)) {
                Error_Root(Base::errDupMCardForMH, errMCardForm_ACP);
                mCardStTitleCmb_ACP->setFocus();
                return;
            }
            else {
                db.Insert("acmcards",
                          "accode, acmcode, stcode, date, percent",
                          5,
                          __acLoggedInCode.c_str(), acmCode.c_str(), stCode.c_str(),
                          date.c_str(), percent.c_str());

                delete dlg;
                dlg = NULL;

                GetMCardDialog_ACP(acmCode);
            }
        }
        else if (mode == "edit") {
            stCode = mCardStTitleCmb_ACP->attributeValue("stcode").toUTF8();

            string stCodeNew;
            db.sql << "SELECT code FROM [standards] "
                    "WHERE title=?", stTitle, r;

            if(db.sql.single(r)) {
                r >> stCodeNew;
            }

            if (stCode != stCodeNew) {
                db.sql << "SELECT accode FROM [acmcards] "
                        "WHERE acmcode=? AND stcode=?", acmCode, stCodeNew, r;

                if(db.sql.single(r)) {
                    Error_Root(Base::errDupMCardForMH, errMCardForm_ACP);
                    mCardStTitleCmb_ACP->setFocus();
                    return;
                }
            }

            db.sql << "UPDATE [acmcards] "
                    "SET stcode=?, date=?, percent=?"
                    "WHERE acmcode=? AND stcode=?", stCodeNew, date, percent,
                    acmCode, stCode, exec();

            delete dlg;
            dlg = NULL;

            GetMCardDialog_ACP(acmCode);
        }
    }
}

WWidget *CgiRoot::CTyros_ACP() {
    WContainerWidget *tyros = new WContainerWidget();
    tyros->setId("dvCTyros");

    dvTyrosWrapper_ACP = new Div(tyros, "dvTyros");

    CTyrosDataTable_ACP();

    return tyros;
}

void CgiRoot::CTyrosDataTable_ACP() {
    dvTyrosWrapper_ACP->clear();

    WTable *tyrosTable = new WTable(dvTyrosWrapper_ACP);
    tyrosTable->setStyleClass("tbl");

    tyrosTable->elementAt(0, 0)->addWidget(new WText(L"نام و نام خانوادگی"));
    tyrosTable->elementAt(0, 1)->addWidget(new WText(L"جنسیت"));
    tyrosTable->elementAt(0, 2)->addWidget(new WText(L"دوره"));
    tyrosTable->elementAt(0, 3)->addWidget(new WText(L"وضعیت پرداخت"));
    tyrosTable->elementAt(0, 4)->addWidget(new WText(L"وضعیت اطلاع"));
    tyrosTable->elementAt(0, 5)->addWidget(new WText(L"سایر اطلاعات"));
    tyrosTable->elementAt(0, 6)->addWidget(new WText(L"ویرایش تصاویر"));
    tyrosTable->elementAt(0, 7)->addWidget(new WText(L"ویرایش"));
    tyrosTable->elementAt(0, 8)->addWidget(new WText(L"حذف"));

    tyrosTable->elementAt(0, 0)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 1)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 2)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 3)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 4)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 5)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 6)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 7)->setStyleClass("tblHeader");
    tyrosTable->elementAt(0, 8)->setStyleClass("tblHeader");

    result res;
    row r;

    db.sql << "SELECT code, actcode, fname, lname, sex, payment, noticed "
            "FROM [actyros] WHERE accode=? "
            "ORDER BY fname, lname ASC;", __acLoggedInCode;

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string code, actCode, fname, lname, sex, payment, noticed;
            r >> code >> actCode >> fname >> lname >> sex >> payment >> noticed;

            row r2;

            db.sql << "SELECT stcode "
                    "FROM [acterms] WHERE code=?;", actCode, r;
            string stCode;
            if(db.sql.single(r2)) {
                r2 >> stCode;
            }

            db.sql << "SELECT title FROM [standards] "
                    "WHERE code=?", stCode, r2;
            string stTitle;
            if(db.sql.single(r2)) {
                r2 >> stTitle;
            }

            tyrosTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(fname + " " + lname)));
            tyrosTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(sex)));
            tyrosTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(stTitle)));
            tyrosTable->elementAt(i, 3)->addWidget(new WText(BString::StrToWStr(payment == "0" ?
                                                                                "پرداخت نشده" :
                                                                                "پرداخت شده")));
            tyrosTable->elementAt(i, 4)->addWidget(new WText(BString::StrToWStr(noticed == "0" ?
                                                                                "خبردار نشده" :
                                                                                "خبر دار")));

            WSignalMapper<WPushButton *> *moreInfoMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *editPicsMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            moreInfoMap->mapped().connect(this, &CgiRoot::MoreInfoTyrosTableCell_ACP);
            editPicsMap->mapped().connect(this, &CgiRoot::EditPicsTyrosTableCell_ACP);
            editMap->mapped().connect(this, &CgiRoot::EditTyrosTableCell_ACP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseTyrosTableCell_ACP);

            WPushButton *moreInfoButton = new WPushButton(L"سایر اطلاعات");
            WPushButton *editPicsButton = new WPushButton(L"ویرایش تصاویر");
            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            moreInfoMap->mapConnect(moreInfoButton->clicked(), moreInfoButton);
            editPicsMap->mapConnect(editPicsButton->clicked(), editPicsButton);
            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            moreInfoButton->setStyleClass("tblButton");
            editPicsButton->setStyleClass("tblButton");
            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            moreInfoButton->setAttributeValue("dbid", BString::StrToWStr(code));
            editPicsButton->setAttributeValue("dbid", BString::StrToWStr(code));
            editPicsButton->setAttributeValue("acmname", BString::StrToWStr(fname + " " + lname));
            editButton->setAttributeValue("dbid", BString::StrToWStr(code));
            eraseButton->setAttributeValue("dbid", BString::StrToWStr(code));

            tyrosTable->elementAt(i, 5)->addWidget(moreInfoButton);
            tyrosTable->elementAt(i, 6)->addWidget(editPicsButton);
            tyrosTable->elementAt(i, 7)->addWidget(editButton);
            tyrosTable->elementAt(i, 8)->addWidget(eraseButton);
        }
    }
}

void CgiRoot::MoreInfoTyrosTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    string fname, lname, sex, fathersname, nationalcode,
            birthid, birthdate, birthloc, birthemissionloc,
            graduatecert, graduatecourse, job,
            addr, tel, mobile, email;

    row r;

    db.sql << "SELECT fname, lname, sex, fathersname, nationalcode, "
            "birthid, birthdate, birthloc, birthemissionloc, "
            "graduatecert, graduatecourse, job, "
            "addr, tel, mobile, email "
            "FROM [actyros] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> fname >> lname >> sex >> fathersname >> nationalcode >>
        birthid >> birthdate >> birthloc >> birthemissionloc >>
        graduatecert >> graduatecourse >> job >>
        addr >> tel >> mobile >> email;

        birthdate.insert(4, "/");
        birthdate.insert(7, "/");

        email = EncDec::Decrypt(email);
    }

    dlg = new WDialog(L"مشخصات کارآموز");
    dlg->setModal(true);

    WContainerWidget *dvTyrosMoreInfo = new Div(dlg->contents(), "dvTyrosMoreInfo");

    WImage *img = new WImage("showpics.fcgi?" + __acLoggedInCode + "t" + code + "p");
    img->resize(144, 192);
    dvTyrosMoreInfo->addWidget(img);
    new WBreak(dvTyrosMoreInfo);


    WAnchor *picBirthCertAnchor = new WAnchor("javascript:;", L"تصویر شناسنامه", dvTyrosMoreInfo);
    new WText("&nbsp;&nbsp;", dvTyrosMoreInfo);
    WAnchor *picNationalCardAnchor = new WAnchor("javascript:;", L"تصویر کارت ملی", dvTyrosMoreInfo);
    new WText("&nbsp;&nbsp;", dvTyrosMoreInfo);
    WAnchor *picServiceAnchor = new WAnchor("javascript:;", L"تصویر کارت معافیت", dvTyrosMoreInfo);

    WSignalMapper<WAnchor *> *picBirthCertMap = new WSignalMapper<WAnchor *>(this);
    picBirthCertMap->mapped().connect(this, &CgiRoot::ShowTyroPic_ACP);
    picBirthCertMap->mapConnect(picBirthCertAnchor->clicked(), picBirthCertAnchor);
    picBirthCertAnchor->setAttributeValue("picurl", BString::StrToWStr("showpics.fcgi?" +
                                                              __acLoggedInCode + "t" + code + "b"));

    WSignalMapper<WAnchor *> *picNationalCardMap = new WSignalMapper<WAnchor *>(this);
    picNationalCardMap->mapped().connect(this, &CgiRoot::ShowTyroPic_ACP);
    picNationalCardMap->mapConnect(picNationalCardAnchor->clicked(), picNationalCardAnchor);
    picNationalCardAnchor->setAttributeValue("picurl", BString::StrToWStr("showpics.fcgi?" +
                                                              __acLoggedInCode + "t" + code + "n"));

    WSignalMapper<WAnchor *> *picServiceMap = new WSignalMapper<WAnchor *>(this);
    picServiceMap->mapped().connect(this, &CgiRoot::ShowTyroPic_ACP);
    picServiceMap->mapConnect(picServiceAnchor->clicked(), picServiceAnchor);
    picServiceAnchor->setAttributeValue("picurl", BString::StrToWStr("showpics.fcgi?" +
                                                              __acLoggedInCode + "t" + code + "s"));


    WContainerWidget *dvTyrosMoreInfoInner = new Div(dvTyrosMoreInfo, "dvTyrosMoreInfoInner");
    WGridLayout *dvTyrosMoreInfoInnerLayout = new WGridLayout();

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(fname)), 0, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 0, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"نام"
                                              ), 0, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(lname)), 1, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 1, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"نام خانوادگی"
                                              ), 1, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(sex)), 2, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 2, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"جنسیت"
                                              ), 2, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(fathersname)), 3, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 3, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"نام پدر"
                                              ), 3, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(nationalcode)), 4, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 4, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"کد ملی"
                                              ), 4, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthid)), 5, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 5, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"شماره شناسنامه"
                                              ), 5, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthdate)), 6, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 6, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"تاریخ تولد"
                                              ), 6, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthloc)), 7, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 7, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"محل تولد"
                                              ), 7, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(birthemissionloc)), 8, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 8, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"محل صدور شناسنامه"
                                              ), 8, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(graduatecert)), 9, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 9, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"آخرین مدرک تحصیلی"
                                              ), 9, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(graduatecourse)), 10, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 10, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"رشته تحصیلی"
                                              ), 10, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(job)), 11, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 11, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"وضعیت اشتغال"
                                              ), 11, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(addr)), 12, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 12, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"نشانی محل سکونت"
                                              ), 12, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(tel)), 13, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 13, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"تلفن تماس"
                                              ), 13, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(mobile)), 14, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 14, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"شماره همراه"
                                              ), 14, 2);

    dvTyrosMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(email)), 15, 0);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L":"), 15, 1);
    dvTyrosMoreInfoInnerLayout->addWidget(new WText(L"پست الکترونیکی"
                                              ), 15, 2);

    dvTyrosMoreInfoInnerLayout->setVerticalSpacing(11);
    dvTyrosMoreInfoInnerLayout->setColumnStretch(0, 190);
    dvTyrosMoreInfoInnerLayout->setColumnStretch(1, 25);
    dvTyrosMoreInfoInnerLayout->setColumnStretch(2, 150);
    dvTyrosMoreInfoInner->resize(365, WLength::Auto);
    dvTyrosMoreInfoInner->setLayout(dvTyrosMoreInfoInnerLayout);

    WPushButton *cancel = new WPushButton(L"بازگشت", dvTyrosMoreInfo);
    cancel->setStyleClass("btnDialogs");
    cancel->clicked().connect(dlg, &WDialog::accept);

    new WBreak(dvTyrosMoreInfo);
    new WBreak(dvTyrosMoreInfo);
    new WBreak(dvTyrosMoreInfo);
    new WBreak(dvTyrosMoreInfo);

    dlg->show();
}

void CgiRoot::ShowTyroPic_ACP(WAnchor *sender) {
    string url = sender->attributeValue("picurl").toUTF8();

    WDialog *dialog = new WDialog(L"تصویر");
    dialog->setModal(true);

    WImage *img = new WImage(url, dialog->contents());
    img->resize(720, 720);
    new WBreak(dialog->contents());

    WPushButton *ok = new WPushButton(L"تائید", dialog->contents());
    ok->setStyleClass("btnDialogs");
    ok->clicked().connect(dialog, &WDialog::accept);

    dialog->show();
}

void CgiRoot::EditPicsTyrosTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();
    string acmName = sender->attributeValue("acmname").toUTF8();

    WDialog *dialog = new WDialog(L"ویرایش تصاویر کارآموز - " + BString::StrToWStr(acmName));
    dialog->setModal(true);

    tyrosPicBirthCertFUP_ACP = new WFileUpload();
    tyrosPicNationalCardFUP_ACP = new WFileUpload();
    tyrosPicPersonnelFUP_ACP = new WFileUpload();
    tyrosPicServiceFUP_ACP = new WFileUpload();

    WPushButton *picBirthCertBtn = new WPushButton(L"ویرایش");
    WPushButton *picNationalCardBtn = new WPushButton(L"ویرایش");
    WPushButton *picPersonnelBtn = new WPushButton(L"ویرایش");
    WPushButton *picServiceBtn = new WPushButton(L"ویرایش");


    tyrosPicBirthCertFUP_ACP->setAttributeValue("dbid", BString::StrToWStr(code));
    tyrosPicNationalCardFUP_ACP->setAttributeValue("dbid", BString::StrToWStr(code));
    tyrosPicPersonnelFUP_ACP->setAttributeValue("dbid", BString::StrToWStr(code));
    tyrosPicServiceFUP_ACP->setAttributeValue("dbid", BString::StrToWStr(code));

    tyrosPicBirthCertFUP_ACP->setAttributeValue("field", L"birthcert");
    tyrosPicNationalCardFUP_ACP->setAttributeValue("field", L"nationalcard");
    tyrosPicPersonnelFUP_ACP->setAttributeValue("field", L"personnel");
    tyrosPicServiceFUP_ACP->setAttributeValue("field", L"service");


    WSignalMapper<WFileUpload *> *editPicsOKMap = new WSignalMapper<WFileUpload *>(this);
    editPicsOKMap->mapped().connect(this, &CgiRoot::EditPicsTyrosTableCellOK_ACP);
    editPicsOKMap->mapConnect(tyrosPicBirthCertFUP_ACP->uploaded(), tyrosPicBirthCertFUP_ACP);
    editPicsOKMap->mapConnect(tyrosPicNationalCardFUP_ACP->uploaded(), tyrosPicNationalCardFUP_ACP);
    editPicsOKMap->mapConnect(tyrosPicPersonnelFUP_ACP->uploaded(), tyrosPicPersonnelFUP_ACP);
    editPicsOKMap->mapConnect(tyrosPicServiceFUP_ACP->uploaded(), tyrosPicServiceFUP_ACP);

    picBirthCertBtn->clicked().connect(tyrosPicBirthCertFUP_ACP, &Wt::WFileUpload::upload);
    picNationalCardBtn->clicked().connect(tyrosPicNationalCardFUP_ACP, &Wt::WFileUpload::upload);
    picPersonnelBtn->clicked().connect(tyrosPicPersonnelFUP_ACP, &Wt::WFileUpload::upload);
    picServiceBtn->clicked().connect(tyrosPicServiceFUP_ACP, &Wt::WFileUpload::upload);


    Div *tyrosForm = new Div(dialog->contents(), "dvTyrosFormWrapper");
    Div *dvTyrosForm = new Div(tyrosForm, "dvTyrosForm");
    WGridLayout *dvTyrosFormLayout = new WGridLayout();

    dvTyrosFormLayout->addWidget(picBirthCertBtn, 0, 0);
    dvTyrosFormLayout->addWidget(tyrosPicBirthCertFUP_ACP, 0, 1);
    dvTyrosFormLayout->addWidget(new WText(L"ارسال تصویر صفحه اول شناسنامه"
                                               ), 0, 2);

    dvTyrosFormLayout->addWidget(picNationalCardBtn, 1, 0);
    dvTyrosFormLayout->addWidget(tyrosPicNationalCardFUP_ACP, 1, 1);
    dvTyrosFormLayout->addWidget(new WText(L"ارسال تصویر کارت ملی"
                                               ), 1, 2);

    dvTyrosFormLayout->addWidget(picPersonnelBtn, 2, 0);
    dvTyrosFormLayout->addWidget(tyrosPicPersonnelFUP_ACP, 2, 1);
    dvTyrosFormLayout->addWidget(new WText(L"ارسال عکس پرسنلی"
                                               ), 2, 2);

    dvTyrosFormLayout->addWidget(picServiceBtn, 3, 0);
    dvTyrosFormLayout->addWidget(tyrosPicServiceFUP_ACP, 3, 1);
    dvTyrosFormLayout->addWidget(new WText(L"ارسال تصویر کارت پایان خدمت یا مدرکی دال بر معافیت"
                                               ), 3, 2);


    dvTyrosFormLayout->setVerticalSpacing(11);
    dvTyrosFormLayout->setColumnStretch(0, 100);
    dvTyrosFormLayout->setColumnStretch(1, 100);
    dvTyrosFormLayout->setColumnStretch(2, 500);
    dvTyrosForm->resize(700, WLength::Auto);
    dvTyrosForm->setLayout(dvTyrosFormLayout);

    new WBreak(tyrosForm);

    WPushButton *ok = new WPushButton(L"تائید", tyrosForm);
    ok->setStyleClass("btnDialogs");
    ok->clicked().connect(dialog, &WDialog::accept);

    dialog->show();
}

void CgiRoot::EditPicsTyrosTableCellOK_ACP(WFileUpload *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    string file = sender->spoolFileName();

    if (!ValidatePic(file)) {
        return;
    }

    dbPics.Update(__acLoggedInCode, "id", "t" + code, sender->attributeValue("field").toUTF8() + "=?",
                  1, CImage::B64Encode(file).c_str());
}

void CgiRoot::EditTyrosTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    dlg = new WDialog(L"ویرایش کارآموز");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetTyrosForm_ACP("edit"));

    string fname, lname, sex, fathersname, nationalcode,
            birthid, birthdate, birthloc, birthemissionloc,
            graduatecert, graduatecourse, job,
            addr, tel, mobile, email;

    row r;

    db.sql << "SELECT fname, lname, sex, fathersname, nationalcode, "
            "birthid, birthdate, birthloc, birthemissionloc, "
            "graduatecert, graduatecourse, job, "
            "addr, tel, mobile, email "
            "FROM [actyros] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> fname >> lname >> sex >> fathersname >> nationalcode >>
        birthid >> birthdate >> birthloc >> birthemissionloc >>
        graduatecert >> graduatecourse >> job >>
        addr >> tel >> mobile >> email;

        email = EncDec::Decrypt(email);
    }

    tyrosFNameEdit_ACP->setText(BString::StrToWStr(fname));
    tyrosFNameEdit_ACP->setAttributeValue("dbid", BString::StrToWStr(code));

    tyrosLNameEdit_ACP->setText(BString::StrToWStr(lname));
    tyrosSexCmb_ACP->setCurrentIndex(tyrosSexCmb_ACP->findText(BString::StrToWStr(sex), MatchExactly));
    tyrosFatherSNameEdit_ACP->setText(BString::StrToWStr(fathersname));
    tyrosNationalCodeEdit_ACP->setText(BString::StrToWStr(nationalcode));
    tyrosBirthIdEdit_ACP->setText(BString::StrToWStr(birthid));

    tyrosBirthDateCmbY_ACP->setCurrentIndex(tyrosBirthDateCmbY_ACP->findText(BString::StrToWStr(birthdate.substr(0, 4)), MatchExactly));
    tyrosBirthDateCmbM_ACP->setCurrentIndex(tyrosBirthDateCmbM_ACP->findText(BString::StrToWStr(birthdate.substr(4, 2)), MatchExactly));
    tyrosBirthDateCmbD_ACP->setCurrentIndex(tyrosBirthDateCmbD_ACP->findText(BString::StrToWStr(birthdate.substr(6, 2)), MatchExactly));

    tyrosBirthlocCmb_ACP->setCurrentIndex(tyrosBirthlocCmb_ACP->findText(BString::StrToWStr(birthloc), MatchExactly));
    tyrosBirthEmissionLocCmb_ACP->setCurrentIndex(tyrosBirthEmissionLocCmb_ACP->findText(BString::StrToWStr(birthemissionloc), MatchExactly));
    tyrosGraduateCertCmb_ACP->setCurrentIndex(tyrosGraduateCertCmb_ACP->findText(BString::StrToWStr(graduatecert), MatchExactly));
    tyrosGraduateCourseEdit_ACP->setText(BString::StrToWStr(graduatecourse));
    tyrosJobCmb_ACP->setCurrentIndex(tyrosJobCmb_ACP->findText(BString::StrToWStr(job), MatchExactly));
    tyrosAddrTArea_ACP->setText(BString::StrToWStr(addr));
    tyrosTelEdit_ACP->setText(BString::StrToWStr(tel));
    tyrosMobileEdit_ACP->setText(BString::StrToWStr(mobile));
    tyrosEmailEdit_ACP->setText(BString::StrToWStr(email));

    dlg->show();
}

void CgiRoot::EraseTyrosTableCell_ACP(WPushButton *sender) {
    tableBtn_RP = sender;

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف کارآموز موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseTyrosTableCellOK_ACP);

    msgBox->show();
}

void CgiRoot::EraseTyrosTableCellOK_ACP(StandardButton result) {
    if (result == Ok) {
        string code = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;

        db.sql << "SELECT code FROM [actyros] "
                "WHERE code=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("actyros", "code", code);
        }

        string id = "t" + code;
        dbPics.sql << "SELECT id FROM [" + __acLoggedInCode + "] "
                "WHERE id=?", id, r;

        if(dbPics.sql.single(r)) {
            dbPics.Delete(__acLoggedInCode, "id", id);
        }

        CTyrosDataTable_ACP();
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

WWidget *CgiRoot::GetTyrosForm_ACP(WString form) {
    WContainerWidget *tyrosForm = new WContainerWidget();
    tyrosForm->setId("dvTyrosFormWrapper");

    tyrosFNameEdit_ACP = new WLineEdit();
    tyrosLNameEdit_ACP = new WLineEdit();
    tyrosSexCmb_ACP = new WComboBox();
    tyrosFatherSNameEdit_ACP = new WLineEdit();
    tyrosNationalCodeEdit_ACP = new WLineEdit();
    tyrosBirthIdEdit_ACP = new WLineEdit();

    WContainerWidget *dvDate = new WContainerWidget();
    dvDate->setId("dvDate");
    tyrosBirthDateCmbY_ACP = new WComboBox(dvDate);
    tyrosBirthDateCmbM_ACP = new WComboBox(dvDate);
    tyrosBirthDateCmbD_ACP = new WComboBox(dvDate);

    tyrosBirthlocCmb_ACP = new WComboBox();
    tyrosBirthEmissionLocCmb_ACP = new WComboBox();
    tyrosGraduateCertCmb_ACP = new WComboBox();
    tyrosGraduateCourseEdit_ACP = new WLineEdit();
    tyrosJobCmb_ACP = new WComboBox();
    tyrosAddrTArea_ACP = new WTextArea();
    tyrosTelEdit_ACP = new WLineEdit();
    tyrosMobileEdit_ACP = new WLineEdit();
    tyrosEmailEdit_ACP = new WLineEdit();


    tyrosSexCmb_ACP->addItem(L"");
    tyrosSexCmb_ACP->addItem(L"مرد");
    tyrosSexCmb_ACP->addItem(L"زن");


    tyrosBirthDateCmbY_ACP->addItem("");
    tyrosBirthDateCmbM_ACP->addItem("");
    tyrosBirthDateCmbD_ACP->addItem("");

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y - 10; i >= 1310; --i) {
        tyrosBirthDateCmbY_ACP->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        tyrosBirthDateCmbM_ACP->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        tyrosBirthDateCmbD_ACP->addItem(d);
    }


    tyrosBirthlocCmb_ACP->addItem(L"");
    tyrosBirthEmissionLocCmb_ACP->addItem(L"");

    tyrosJobCmb_ACP->addItem(L"");
    tyrosJobCmb_ACP->addItem(L"بیکار");
    tyrosJobCmb_ACP->addItem(L"دانش آموز");
    tyrosJobCmb_ACP->addItem(L"دانشجو");
    tyrosJobCmb_ACP->addItem(L"شاغل");


    result res;
    row r;

    db.sql << "SELECT province FROM [provinces] ORDER BY province ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string pr;
            r >> pr;
            tyrosBirthlocCmb_ACP->addItem(BString::StrToWStr(pr));
            tyrosBirthEmissionLocCmb_ACP->addItem(BString::StrToWStr(pr));
        }
    }


    tyrosGraduateCertCmb_ACP->addItem(L"");
    tyrosGraduateCertCmb_ACP->addItem(L"بیسواد");
    tyrosGraduateCertCmb_ACP->addItem(L"زیر دیپلم");
    tyrosGraduateCertCmb_ACP->addItem(L"دیپلم");
    tyrosGraduateCertCmb_ACP->addItem(L"کاردان");
    tyrosGraduateCertCmb_ACP->addItem(L"کارشناس");
    tyrosGraduateCertCmb_ACP->addItem(L"کارشناس ارشد");
    tyrosGraduateCertCmb_ACP->addItem(L"دکترا");


    Div *dvTyrosForm = new Div(tyrosForm, "dvTyrosForm");
    WGridLayout *dvTyrosFormLayout = new WGridLayout();

    dvTyrosFormLayout->addWidget(tyrosFNameEdit_ACP, 0, 0);
    dvTyrosFormLayout->addWidget(new WText(L"نام"
                                               ), 0, 1);

    dvTyrosFormLayout->addWidget(tyrosLNameEdit_ACP, 1, 0);
    dvTyrosFormLayout->addWidget(new WText(L"نام خانوادگی"
                                               ), 1, 1);

    dvTyrosFormLayout->addWidget(tyrosSexCmb_ACP, 2, 0);
    dvTyrosFormLayout->addWidget(new WText(L"جنسیت"
                                               ), 2, 1);

    dvTyrosFormLayout->addWidget(tyrosFatherSNameEdit_ACP, 3, 0);
    dvTyrosFormLayout->addWidget(new WText(L"نام پدر"
                                               ), 3, 1);

    dvTyrosFormLayout->addWidget(tyrosNationalCodeEdit_ACP,4, 0);
    dvTyrosFormLayout->addWidget(new WText(L"کد ملی"
                                               ), 4, 1);

    dvTyrosFormLayout->addWidget(tyrosBirthIdEdit_ACP, 5, 0);
    dvTyrosFormLayout->addWidget(new WText(L"شماره شناسنامه"
                                               ), 5, 1);

    dvTyrosFormLayout->addWidget(dvDate, 6, 0);
    dvTyrosFormLayout->addWidget(new WText(L"تاریخ تولد"
                                               ), 6, 1);

    dvTyrosFormLayout->addWidget(tyrosBirthlocCmb_ACP, 7, 0);
    dvTyrosFormLayout->addWidget(new WText(L"محل تولد"
                                               ), 7, 1);

    dvTyrosFormLayout->addWidget(tyrosBirthEmissionLocCmb_ACP, 8, 0);
    dvTyrosFormLayout->addWidget(new WText(L"محل صدور شناسنامه"
                                               ), 8, 1);

    dvTyrosFormLayout->addWidget(tyrosGraduateCertCmb_ACP, 9, 0);
    dvTyrosFormLayout->addWidget(new WText(L"آخرین مدرک تحصیلی"
                                               ), 9, 1);

    dvTyrosFormLayout->addWidget(tyrosGraduateCourseEdit_ACP, 10, 0);
    dvTyrosFormLayout->addWidget(new WText(L"رشته تحصیلی"
                                               ), 10, 1);

    dvTyrosFormLayout->addWidget(tyrosJobCmb_ACP, 11, 0);
    dvTyrosFormLayout->addWidget(new WText(L"وضعیت اشتغال"
                                               ), 11, 1);

    dvTyrosFormLayout->addWidget(tyrosAddrTArea_ACP, 12, 0);
    dvTyrosFormLayout->addWidget(new WText(L"نشانی محل سکونت"
                                               ), 12, 1);

    dvTyrosFormLayout->addWidget(tyrosTelEdit_ACP, 13, 0);
    dvTyrosFormLayout->addWidget(new WText(L"تلفن تماس"
                                               ), 13, 1);

    dvTyrosFormLayout->addWidget(tyrosMobileEdit_ACP, 14, 0);
    dvTyrosFormLayout->addWidget(new WText(L"شماره همراه"
                                               ), 14, 1);

    dvTyrosFormLayout->addWidget(tyrosEmailEdit_ACP, 15, 0);
    dvTyrosFormLayout->addWidget(new WText(L"پست الکترونیکی"
                                               ), 15, 1);

    dvTyrosFormLayout->setVerticalSpacing(11);
    dvTyrosFormLayout->setColumnStretch(0, 200);
    dvTyrosFormLayout->setColumnStretch(1, 200);
    dvTyrosForm->resize(400, WLength::Auto);
    dvTyrosForm->setLayout(dvTyrosFormLayout);

    tyrosSaveBtn_ACP = new WPushButton(tyrosForm);
    tyrosSaveBtn_ACP->setStyleClass("btnDialogs");

    if (form == "add") {
        tyrosSaveBtn_ACP->setText(L"افزودن");
    }
    else if (form == "edit") {
        tyrosSaveBtn_ACP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", tyrosForm);
    cancel->clicked().connect(dlg, &WDialog::accept);
    cancel->setStyleClass("btnDialogs");

    errTyrosForm_ACP = new WText("");
    tyrosForm->addWidget(errTyrosForm_ACP);
    tyrosSaveBtn_ACP->setAttributeValue("mode", form);

    tyrosFNameValidator_ACP = new WLengthValidator(1, 15);
    tyrosLNameValidator_ACP = new WLengthValidator(1, 20);
    tyrosSexValidator_ACP = new WLengthValidator(1, 3);
    tyrosFatherSNameValidator_ACP = new WLengthValidator(1, 15);
    tyrosNationalCodeValidator_ACP = new WRegExpValidator(Base::regExpNationalCode);
    tyrosBirthIdValidator_ACP = new WRegExpValidator(Base::regExpBirthId);
    tyrosBirthDateYValidator_ACP = new WLengthValidator(1, 4);
    tyrosBirthDateMValidator_ACP = new WLengthValidator(1, 2);
    tyrosBirthDateDValidator_ACP = new WLengthValidator(1, 2);
    tyrosBirthlocValidator_ACP = new WLengthValidator(1, 35);
    tyrosBirthEmissionLocValidator_ACP = new WLengthValidator(1, 35);
    tyrosGraduateCertValidator_ACP = new WLengthValidator(1, 13);
    tyrosGraduateCourseValidator_ACP = new WLengthValidator(1, 50);
    tyrosJobValidator_ACP = new WLengthValidator(1, 11);
    tyrosAddrValidator_ACP = new WLengthValidator(1, 255);
    tyrosTelValidator_ACP = new WRegExpValidator(Base::regExpTel);
    tyrosMobileValidator_ACP = new WRegExpValidator(Base::regExpMobile);
    tyrosEmailValidator_ACP = new WRegExpValidator(Base::regExpEmail);

    tyrosFNameValidator_ACP->setMandatory(true);
    tyrosLNameValidator_ACP->setMandatory(true);
    tyrosSexValidator_ACP->setMandatory(true);
    tyrosFatherSNameValidator_ACP->setMandatory(true);
    tyrosNationalCodeValidator_ACP->setMandatory(true);
    tyrosBirthIdValidator_ACP->setMandatory(true);
    tyrosBirthDateYValidator_ACP->setMandatory(true);
    tyrosBirthDateMValidator_ACP->setMandatory(true);
    tyrosBirthDateDValidator_ACP->setMandatory(true);
    tyrosBirthlocValidator_ACP->setMandatory(true);
    tyrosBirthEmissionLocValidator_ACP->setMandatory(true);
    tyrosGraduateCertValidator_ACP->setMandatory(true);
    tyrosGraduateCourseValidator_ACP->setMandatory(true);
    tyrosJobValidator_ACP->setMandatory(true);
    tyrosAddrValidator_ACP->setMandatory(true);
    tyrosTelValidator_ACP->setMandatory(true);
    //tyrosMobileValidator_ACP->setMandatory(true);
    //tyrosEmailValidator_ACP->setMandatory(true);

    tyrosFNameEdit_ACP->setValidator(tyrosFNameValidator_ACP);
    tyrosLNameEdit_ACP->setValidator(tyrosLNameValidator_ACP);
    tyrosSexCmb_ACP->setValidator(tyrosSexValidator_ACP);
    tyrosFatherSNameEdit_ACP->setValidator(tyrosFatherSNameValidator_ACP);
    tyrosNationalCodeEdit_ACP->setValidator(tyrosNationalCodeValidator_ACP);
    tyrosBirthIdEdit_ACP->setValidator(tyrosBirthIdValidator_ACP);
    tyrosBirthDateCmbY_ACP->setValidator(tyrosBirthDateYValidator_ACP);
    tyrosBirthDateCmbM_ACP->setValidator(tyrosBirthDateMValidator_ACP);
    tyrosBirthDateCmbD_ACP->setValidator(tyrosBirthDateDValidator_ACP);
    tyrosBirthlocCmb_ACP->setValidator(tyrosBirthlocValidator_ACP);
    tyrosBirthEmissionLocCmb_ACP->setValidator(tyrosBirthEmissionLocValidator_ACP);
    tyrosGraduateCertCmb_ACP->setValidator(tyrosGraduateCertValidator_ACP);
    tyrosGraduateCourseEdit_ACP->setValidator(tyrosGraduateCourseValidator_ACP);
    tyrosJobCmb_ACP->setValidator(tyrosJobValidator_ACP);
    tyrosAddrTArea_ACP->setValidator(tyrosAddrValidator_ACP);
    tyrosTelEdit_ACP->setValidator(tyrosTelValidator_ACP);
    tyrosMobileEdit_ACP->setValidator(tyrosMobileValidator_ACP);
    tyrosEmailEdit_ACP->setValidator(tyrosEmailValidator_ACP);


    tyrosFNameEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosLNameEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosSexCmb_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosNationalCodeEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthIdEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthDateCmbY_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthDateCmbM_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthDateCmbD_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthlocCmb_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosBirthEmissionLocCmb_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosGraduateCertCmb_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosGraduateCourseEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosAddrTArea_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosTelEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosMobileEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosEmailEdit_ACP->enterPressed().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);
    tyrosSaveBtn_ACP->clicked().connect(this, &CgiRoot::TyrosFormSaveBtnOK_ACP);

    new WBreak(tyrosForm);
    new WBreak(tyrosForm);
    new WBreak(tyrosForm);
    new WBreak(tyrosForm);

    return tyrosForm;
}

void CgiRoot::TyrosFormSaveBtnOK_ACP() {
    if (Validate(tyrosFNameEdit_ACP) && Validate(tyrosLNameEdit_ACP) && Validate(tyrosSexCmb_ACP) &&
        Validate(tyrosFatherSNameEdit_ACP) && Validate(tyrosNationalCodeEdit_ACP) && Validate(tyrosBirthIdEdit_ACP) &&
        Validate(tyrosBirthDateCmbY_ACP) && Validate(tyrosBirthDateCmbM_ACP) && Validate(tyrosBirthDateCmbD_ACP) &&
        Validate(tyrosBirthlocCmb_ACP) && Validate(tyrosBirthEmissionLocCmb_ACP) &&
        Validate(tyrosGraduateCertCmb_ACP) && Validate(tyrosGraduateCourseEdit_ACP) &&
        Validate(tyrosJobCmb_ACP) && Validate(tyrosAddrTArea_ACP) && Validate(tyrosTelEdit_ACP) &&
        Validate(tyrosMobileEdit_ACP) && Validate(tyrosEmailEdit_ACP)) {

        string mode = tyrosSaveBtn_ACP->attributeValue("mode").toUTF8();

        string fname = BString::Trim(tyrosFNameEdit_ACP->text().toUTF8());
        string lname = BString::Trim(tyrosLNameEdit_ACP->text().toUTF8());
        string sex = BString::Trim(tyrosSexCmb_ACP->currentText().toUTF8());
        string fathersname = BString::Trim(tyrosFatherSNameEdit_ACP->text().toUTF8());
        string nationalcode = BString::Trim(tyrosNationalCodeEdit_ACP->text().toUTF8());
        string birthid = BString::Trim(tyrosBirthIdEdit_ACP->text().toUTF8());
        string birthdate = BString::Trim(tyrosBirthDateCmbY_ACP->currentText().toUTF8()
                                         + tyrosBirthDateCmbM_ACP->currentText().toUTF8()
                                         + tyrosBirthDateCmbD_ACP->currentText().toUTF8());
        string birthloc = BString::Trim(tyrosBirthlocCmb_ACP->currentText().toUTF8());
        string birthemissionloc = BString::Trim(tyrosBirthEmissionLocCmb_ACP->currentText().toUTF8());
        string graduatecert = BString::Trim(tyrosGraduateCertCmb_ACP->currentText().toUTF8());
        string graduatecourse = BString::Trim(tyrosGraduateCourseEdit_ACP->text().toUTF8());
        string job = BString::Trim(tyrosJobCmb_ACP->currentText().toUTF8());
        string addr = BString::Trim(tyrosAddrTArea_ACP->text().toUTF8());
        string tel = BString::Trim(tyrosTelEdit_ACP->text().toUTF8());
        string mobile = BString::Trim(tyrosMobileEdit_ACP->text().toUTF8());
        string email = BString::Trim(tyrosEmailEdit_ACP->text().toUTF8());

        email = EncDec::Encrypt(email);

        row r;
        string code;

        if (mode == "add") {
        }
        else if (mode == "edit") {
            code = tyrosFNameEdit_ACP->attributeValue("dbid").toUTF8();

            db.Update("actyros", "code", code,
                      "fname=?, lname=?, sex=?, fathersname=?, nationalcode=?, "
                      "birthid=?, birthdate=?, birthloc=?, birthemissionloc=?, "
                      "graduatecert=?, graduatecourse=?, job=?, "
                      "addr=?, tel=?, mobile=?, email=?",
                      16,
                      fname.c_str(), lname.c_str(), sex.c_str(), fathersname.c_str(), nationalcode.c_str(),
                      birthid.c_str(), birthdate.c_str(), birthloc.c_str(), birthemissionloc.c_str(),
                      graduatecert.c_str(), graduatecourse.c_str(), job.c_str(),
                      addr.c_str(), tel.c_str(), mobile.c_str(), email.c_str());

            delete dlg;
            dlg = NULL;
            CTyrosDataTable_ACP();
        }
    }
}

WWidget *CgiRoot::CTerms_ACP() {
    WContainerWidget *terms = new WContainerWidget();
    terms->setId("dvCTerms");

    WPushButton *termsAddBtn_ACP = new WPushButton(L"افزودن", terms);
    termsAddBtn_ACP->clicked().connect(this, &CgiRoot::TermsAddBtnOK_ACP);

    dvTermsWrapper_ACP = new Div(terms, "dvTerms");

    CTermsDataTable_ACP();

    return terms;
}

void CgiRoot::TermsAddBtnOK_ACP() {
    dlg = new WDialog(L"افزودن دوره");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetTermsForm_ACP("add"));
    dlg->show();
}

void CgiRoot::CTermsDataTable_ACP() {
    dvTermsWrapper_ACP->clear();

    WTable *termsTable = new WTable(dvTermsWrapper_ACP);
    termsTable->setStyleClass("tbl");

    termsTable->elementAt(0, 0)->addWidget(new WText(L"نام دوره"));
    termsTable->elementAt(0, 1)->addWidget(new WText(L"مربی"));
    termsTable->elementAt(0, 2)->addWidget(new WText(L"شیفت"));
    termsTable->elementAt(0, 3)->addWidget(new WText(L"محل برگزاری"));
    termsTable->elementAt(0, 4)->addWidget(new WText(L"تاریخ"));
    termsTable->elementAt(0, 5)->addWidget(new WText(L"ساعت"));
    termsTable->elementAt(0, 6)->addWidget(new WText(L"تعداد کارآموزان"));
    termsTable->elementAt(0, 7)->addWidget(new WText(L"خبررسانی"));
    termsTable->elementAt(0, 8)->addWidget(new WText(L"ویرایش"));
    termsTable->elementAt(0, 9)->addWidget(new WText(L"حذف"));

    termsTable->elementAt(0, 0)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 1)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 2)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 3)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 4)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 5)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 6)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 7)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 8)->setStyleClass("tblHeader");
    termsTable->elementAt(0, 9)->setStyleClass("tblHeader");

    result res;
    row r;

    db.sql << "SELECT code, acmcode, stcode, begdate, enddate, daypart, begtime, endtime, loc, noticed "
            "FROM [acterms] WHERE accode=?;", __acLoggedInCode;

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string code, acmCode, stCode, begdate, enddate, daypart, begtime, endtime, loc, noticed;
            r >> code >> acmCode >> stCode >> begdate >> enddate >> daypart
                    >> begtime >> endtime >> loc >> noticed;

            row r2;

            db.sql << "SELECT title FROM [standards] "
                    "WHERE code=?", stCode, r2;

            string stTitle;
            if(db.sql.single(r2)) {
                r2 >> stTitle;
            }

            db.sql << "SELECT fname, lname FROM [acmentors] "
                    "WHERE code=?", acmCode, r2;

            string acmFName, acmLName, acmName;
            if(db.sql.single(r2)) {
                r2 >> acmFName >> acmLName;
                acmName = acmFName + " " + acmLName;
            }

            begdate.insert(4, "/");
            begdate.insert(7, "/");
            enddate.insert(4, "/");
            enddate.insert(7, "/");

            begtime.insert(2, ":");
            endtime.insert(2, ":");

            string date = "از " + begdate + "<br />تا " + enddate;
            string time = "از " + begtime + "<br />تا " + endtime;

            string tCount = "0";

            termsTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(stTitle)));
            termsTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(acmName)));
            termsTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(daypart)));
            termsTable->elementAt(i, 3)->addWidget(new WText(BString::StrToWStr(loc)));
            termsTable->elementAt(i, 4)->addWidget(new WText(BString::StrToWStr(date)));
            termsTable->elementAt(i, 5)->addWidget(new WText(BString::StrToWStr(time)));
            termsTable->elementAt(i, 6)->addWidget(new WText(BString::StrToWStr(tCount)));

            WSignalMapper<WPushButton *> *noticeMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            noticeMap->mapped().connect(this, &CgiRoot::NoticeTermsTableCell_ACP);
            editMap->mapped().connect(this, &CgiRoot::EditTermsTableCell_ACP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseTermsTableCell_ACP);

            WPushButton *noticeButton = new WPushButton(noticed == "0" ?
                                                        L"خبر رسانی" : L"خبر رسانی مجدد");
            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            noticeMap->mapConnect(noticeButton->clicked(), noticeButton);
            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            noticeButton->setStyleClass("tblButton");
            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            noticeButton->setAttributeValue("dbid", BString::StrToWStr(code));
            editButton->setAttributeValue("dbid", BString::StrToWStr(code));
            eraseButton->setAttributeValue("dbid", BString::StrToWStr(code));

            //noticeButton->setAttributeValue("noticed", BString::StrToWStr(noticed));

            termsTable->elementAt(i, 7)->addWidget(noticeButton);
            termsTable->elementAt(i, 8)->addWidget(editButton);
            termsTable->elementAt(i, 9)->addWidget(eraseButton);
        }
    }
}

void CgiRoot::NoticeTermsTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    db.Update("acterms", "code", code, "noticed=?", 1, "1");
    CTermsDataTable_ACP();

    msgBox = new WMessageBox(L"خبر رسانی",
                                L"خبر رسانی با موفقیت انجام شد",
                                Warning, NoButton);

    msgBox->addButton(L"تائید", Ok);

    msgBox->buttonClicked().connect(msgBox, &WMessageBox::accept);

    msgBox->show();
}

void CgiRoot::EditTermsTableCell_ACP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    dlg = new WDialog(L"ویرایش دوره");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetTermsForm_ACP("edit"));

    string acmCode, stCode, begdate, enddate, daypart, begtime, endtime, loc;

    row r;
    db.sql << "SELECT acmcode, stcode, begdate, enddate, daypart, begtime, endtime, loc "
            "FROM [acterms] WHERE code=?;", code, r;

    string acmName, stTitle;
    if(db.sql.single(r)) {
        r >> acmCode >> stCode >> begdate >> enddate >> daypart >> begtime >> endtime >> loc;

        db.sql << "SELECT fname, lname FROM [acmentors] "
                "WHERE code=?", acmCode, r;

        if(db.sql.single(r)) {
            string fname, lname;
            r >> fname >> lname;
            acmName = fname + " " + lname;
        }

        db.sql << "SELECT title FROM [standards] "
                "WHERE code=?", stCode, r;

        if(db.sql.single(r)) {
            r >> stTitle;
        }
    }

    termsStTitleCmb_ACP->setCurrentIndex(termsStTitleCmb_ACP->findText(BString::StrToWStr(stTitle), MatchExactly));
    termsStTitleCmb_ACP->setAttributeValue("dbid", BString::StrToWStr(code));
    termsBegdateCmbY_ACP->setCurrentIndex(termsBegdateCmbY_ACP->findText(BString::StrToWStr(begdate.substr(0, 4)), MatchExactly));
    termsBegdateCmbM_ACP->setCurrentIndex(termsBegdateCmbM_ACP->findText(BString::StrToWStr(begdate.substr(4, 2)), MatchExactly));
    termsBegdateCmbD_ACP->setCurrentIndex(termsBegdateCmbD_ACP->findText(BString::StrToWStr(begdate.substr(6, 2)), MatchExactly));
    termsEnddateCmbY_ACP->setCurrentIndex(termsEnddateCmbY_ACP->findText(BString::StrToWStr(enddate.substr(0, 4)), MatchExactly));
    termsEnddateCmbM_ACP->setCurrentIndex(termsEnddateCmbM_ACP->findText(BString::StrToWStr(enddate.substr(4, 2)), MatchExactly));
    termsEnddateCmbD_ACP->setCurrentIndex(termsEnddateCmbD_ACP->findText(BString::StrToWStr(enddate.substr(6, 2)), MatchExactly));
    termsDaypartCmb_ACP->setCurrentIndex(termsDaypartCmb_ACP->findText(BString::StrToWStr(daypart), MatchExactly));
    termsBegtimeCmbH_ACP->setCurrentIndex(termsBegtimeCmbH_ACP->findText(BString::StrToWStr(begtime.substr(0, 2)), MatchExactly));
    termsBegtimeCmbM_ACP->setCurrentIndex(termsBegtimeCmbM_ACP->findText(BString::StrToWStr(begtime.substr(2, 2)), MatchExactly));
    termsEndtimeCmbH_ACP->setCurrentIndex(termsEndtimeCmbH_ACP->findText(BString::StrToWStr(endtime.substr(0, 2)), MatchExactly));
    termsEndtimeCmbM_ACP->setCurrentIndex(termsEndtimeCmbM_ACP->findText(BString::StrToWStr(endtime.substr(2, 2)), MatchExactly));
    termsLocCmb_ACP->setCurrentIndex(termsLocCmb_ACP->findText(BString::StrToWStr(loc), MatchExactly));
    termsMentorCmb_ACP->setCurrentIndex(termsMentorCmb_ACP->findText(BString::StrToWStr(acmName), MatchExactly));

    dlg->show();
}

void CgiRoot::EraseTermsTableCell_ACP(WPushButton *sender) {
    tableBtn_RP = sender;

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف دوره موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseTermsTableCellOK_ACP);

    msgBox->show();
}

void CgiRoot::EraseTermsTableCellOK_ACP(StandardButton result) {
    if (result == Ok) {
        string code = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;

        db.sql << "SELECT code FROM [acterms] "
                "WHERE code=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acterms", "code", code);
        }

        CTermsDataTable_ACP();
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

WWidget *CgiRoot::GetTermsForm_ACP(WString form) {
    WContainerWidget *termsForm = new WContainerWidget();
    termsForm->setId("dvTermsFormWrapper");

    termsStTitleCmb_ACP = new WComboBox();
    termsStTitleCmb_ACP->addItem(L"");

    WContainerWidget *dvDateBeg = new WContainerWidget();
    dvDateBeg->setId("dvDate");
    termsBegdateCmbY_ACP = new WComboBox(dvDateBeg);
    termsBegdateCmbM_ACP = new WComboBox(dvDateBeg);
    termsBegdateCmbD_ACP = new WComboBox(dvDateBeg);
    termsBegdateCmbY_ACP->addItem(L"");
    termsBegdateCmbM_ACP->addItem(L"");
    termsBegdateCmbD_ACP->addItem(L"");

    WContainerWidget *dvDateEnd = new WContainerWidget();
    dvDateEnd->setId("dvDate");
    termsEnddateCmbY_ACP = new WComboBox(dvDateEnd);
    termsEnddateCmbM_ACP = new WComboBox(dvDateEnd);
    termsEnddateCmbD_ACP = new WComboBox(dvDateEnd);
    termsEnddateCmbY_ACP->addItem(L"");
    termsEnddateCmbM_ACP->addItem(L"");
    termsEnddateCmbD_ACP->addItem(L"");

    termsDaypartCmb_ACP = new WComboBox();
    termsDaypartCmb_ACP->addItem(L"");
    termsDaypartCmb_ACP->addItem(L"صبح");
    termsDaypartCmb_ACP->addItem(L"عصر");
    termsDaypartCmb_ACP->addItem(L"روزهای زوج");
    termsDaypartCmb_ACP->addItem(L"روزهای فرد");
    termsDaypartCmb_ACP->addItem(L"یکسره");

    WContainerWidget *dvTimeBeg = new WContainerWidget();
    dvTimeBeg->setId("dvTime");
    termsBegtimeCmbH_ACP = new WComboBox(dvTimeBeg);
    termsBegtimeCmbM_ACP = new WComboBox(dvTimeBeg);
    termsBegtimeCmbH_ACP->addItem(L"");
    termsBegtimeCmbM_ACP->addItem(L"");

    WContainerWidget *dvTimeEnd = new WContainerWidget();
    dvTimeEnd->setId("dvTime");
    termsEndtimeCmbH_ACP = new WComboBox(dvTimeEnd);
    termsEndtimeCmbM_ACP = new WComboBox(dvTimeEnd);
    termsEndtimeCmbH_ACP->addItem(L"");
    termsEndtimeCmbM_ACP->addItem(L"");

    termsLocCmb_ACP = new WComboBox();
    termsLocCmb_ACP->addItem(L"");
    termsLocCmb_ACP->addItem(L"خود آموزشگاه");
    termsLocCmb_ACP->addItem(L"مراکز جوار دانشگاهی (ASD)");
    termsLocCmb_ACP->addItem(L"مراکز روستائی");
    termsLocCmb_ACP->addItem(L"صنایع");
    termsLocCmb_ACP->addItem(L"زندان");

    termsMentorCmb_ACP = new WComboBox();
    termsMentorCmb_ACP->addItem(L"");

    result res;
    row r;

    db.sql << "SELECT skcode FROM [accourses] "
              "WHERE accode=?;", __acLoggedInCode;
    db.sql.fetch(res);

    if (res.rows() > 0) {
        string sk;
        while(res.next(r)) {
            r >> sk;

            result rres;
            row rst;

            db.sql << "SELECT title FROM [standards] "
                    " WHERE skcode=? ORDER BY title ASC;", sk;
            db.sql.fetch(rres);

            if (rres.rows() > 0) {
                string st;
                while(rres.next(rst)) {
                    rst >> st;
                    termsStTitleCmb_ACP->addItem(BString::StrToWStr(st));
                }
            }
        }
    }

    db.sql << "SELECT fname, lname FROM [acmentors] "
              "WHERE accode=?;", __acLoggedInCode;
    db.sql.fetch(res);

    if (res.rows() > 0) {
        string fname, lname, name;
        while(res.next(r)) {
            r >> fname >> lname;
            name = fname + " " + lname;
            termsMentorCmb_ACP->addItem(BString::StrToWStr(name));
        }
    }

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y; i <= y + 2; ++i) {
        termsBegdateCmbY_ACP->addItem(BString::IntToStr(i));
    }

    for (int i = y; i <= y + 5; ++i) {
        termsEnddateCmbY_ACP->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        termsBegdateCmbM_ACP->addItem(m);
        termsEnddateCmbM_ACP->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        termsBegdateCmbD_ACP->addItem(d);
        termsEnddateCmbD_ACP->addItem(d);
    }

    for (int i = 1; i < 25; ++i) {
        string h = BString::IntToStr(i);
        h = (h.size() == 1 ? "0" + h : h);
        termsBegtimeCmbH_ACP->addItem(h);
        termsEndtimeCmbH_ACP->addItem(h);
    }

    for (int i = 1; i < 60; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        termsBegtimeCmbM_ACP->addItem(m);
        termsEndtimeCmbM_ACP->addItem(m);
    }

    Div *dvTermsForm = new Div(termsForm, "dvTermsForm");
    WGridLayout *dvTermsFormLayout = new WGridLayout();

    dvTermsFormLayout->addWidget(termsStTitleCmb_ACP, 0, 0);
    dvTermsFormLayout->addWidget(new WText(L"دوره"
                                               ), 0, 1);

    dvTermsFormLayout->addWidget(dvDateBeg, 1, 0);
    dvTermsFormLayout->addWidget(new WText(L"تاریخ شروع دوره"
                                               ), 1, 1);

    dvTermsFormLayout->addWidget(dvDateEnd, 2, 0);
    dvTermsFormLayout->addWidget(new WText(L"تاریخ پایان دوره"
                                               ), 2, 1);

    dvTermsFormLayout->addWidget(termsDaypartCmb_ACP, 3, 0);
    dvTermsFormLayout->addWidget(new WText(L"شیفت کلاس"
                                               ), 3, 1);

    dvTermsFormLayout->addWidget(dvTimeBeg, 4, 0);
    dvTermsFormLayout->addWidget(new WText(L"ساعت شروع کلاس"
                                               ), 4, 1);

    dvTermsFormLayout->addWidget(dvTimeEnd, 5, 0);
    dvTermsFormLayout->addWidget(new WText(L"ساعت خاتمه کلاس"
                                               ), 5, 1);

    dvTermsFormLayout->addWidget(termsLocCmb_ACP, 6, 0);
    dvTermsFormLayout->addWidget(new WText(L"محل برگزاری"
                                               ), 6, 1);

    dvTermsFormLayout->addWidget(termsMentorCmb_ACP, 7, 0);
    dvTermsFormLayout->addWidget(new WText(L"مربی"
                                               ), 7, 1);

    dvTermsFormLayout->setVerticalSpacing(11);
    dvTermsFormLayout->setColumnStretch(0, 200);
    dvTermsFormLayout->setColumnStretch(1, 200);
    dvTermsForm->resize(400, WLength::Auto);
    dvTermsForm->setLayout(dvTermsFormLayout);

    termsSaveBtn_ACP = new WPushButton(termsForm);
    termsSaveBtn_ACP->setStyleClass("btnDialogs");

    if (form == "add") {
        termsSaveBtn_ACP->setText(L"افزودن");
    }
    else if (form == "edit") {
        termsSaveBtn_ACP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", termsForm);
    cancel->clicked().connect(dlg, &WDialog::accept);
    cancel->setStyleClass("btnDialogs");

    errTermsForm_ACP = new WText("");
    termsForm->addWidget(errTermsForm_ACP);
    termsSaveBtn_ACP->setAttributeValue("mode", form);

    termsStTitleValidator_ACP = new WLengthValidator(1, 50);
    termsBegdateValidatorY_ACP = new WLengthValidator(1, 4);
    termsBegdateValidatorM_ACP = new WLengthValidator(1, 2);
    termsBegdateValidatorD_ACP = new WLengthValidator(1, 2);
    termsEnddateValidatorY_ACP = new WLengthValidator(1, 4);
    termsEnddateValidatorM_ACP = new WLengthValidator(1, 2);
    termsEnddateValidatorD_ACP = new WLengthValidator(1, 2);
    termsDaypartValidator_ACP = new WLengthValidator(1, 11);
    termsBegtimeValidatorH_ACP = new WLengthValidator(1, 2);
    termsBegtimeValidatorM_ACP = new WLengthValidator(1, 2);
    termsEndtimeValidatorH_ACP = new WLengthValidator(1, 2);
    termsEndtimeValidatorM_ACP = new WLengthValidator(1, 2);
    termsLocValidator_ACP = new WLengthValidator(1, 27);
    termsMentorValidator_ACP = new WLengthValidator(1, 36);

    termsStTitleValidator_ACP->setMandatory(true);
    termsBegdateValidatorY_ACP->setMandatory(true);
    termsBegdateValidatorM_ACP->setMandatory(true);
    termsBegdateValidatorD_ACP->setMandatory(true);
    termsEnddateValidatorY_ACP->setMandatory(true);
    termsEnddateValidatorM_ACP->setMandatory(true);
    termsEnddateValidatorD_ACP->setMandatory(true);
    termsDaypartValidator_ACP->setMandatory(true);
    termsBegtimeValidatorH_ACP->setMandatory(true);
    termsBegtimeValidatorM_ACP->setMandatory(true);
    termsEndtimeValidatorH_ACP->setMandatory(true);
    termsEndtimeValidatorM_ACP->setMandatory(true);
    termsLocValidator_ACP->setMandatory(true);
    termsMentorValidator_ACP->setMandatory(true);

    termsStTitleCmb_ACP->setValidator(termsStTitleValidator_ACP);
    termsBegdateCmbY_ACP->setValidator(termsBegdateValidatorY_ACP);
    termsBegdateCmbM_ACP->setValidator(termsBegdateValidatorM_ACP);
    termsBegdateCmbD_ACP->setValidator(termsBegdateValidatorD_ACP);
    termsEnddateCmbY_ACP->setValidator(termsEnddateValidatorY_ACP);
    termsEnddateCmbM_ACP->setValidator(termsEnddateValidatorM_ACP);
    termsEnddateCmbD_ACP->setValidator(termsEnddateValidatorD_ACP);
    termsDaypartCmb_ACP->setValidator(termsDaypartValidator_ACP);
    termsBegtimeCmbH_ACP->setValidator(termsBegtimeValidatorH_ACP);
    termsBegtimeCmbM_ACP->setValidator(termsBegtimeValidatorM_ACP);
    termsEndtimeCmbH_ACP->setValidator(termsEndtimeValidatorH_ACP);
    termsEndtimeCmbM_ACP->setValidator(termsEndtimeValidatorM_ACP);
    termsLocCmb_ACP->setValidator(termsLocValidator_ACP);
    termsMentorCmb_ACP->setValidator(termsMentorValidator_ACP);

    termsStTitleCmb_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsBegdateCmbY_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsBegdateCmbM_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsBegdateCmbD_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsEnddateCmbY_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsEnddateCmbM_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsEnddateCmbD_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsDaypartCmb_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsBegtimeCmbH_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsBegtimeCmbM_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsEndtimeCmbH_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsEndtimeCmbM_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsLocCmb_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsMentorCmb_ACP->enterPressed().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);
    termsSaveBtn_ACP->clicked().connect(this, &CgiRoot::TermsFormSaveBtnOK_ACP);

    new WBreak(termsForm);
    new WBreak(termsForm);
    new WBreak(termsForm);
    new WBreak(termsForm);

    return termsForm;
}

void CgiRoot::TermsFormSaveBtnOK_ACP() {
    if(Validate(termsStTitleCmb_ACP) &&
       Validate(termsBegdateCmbY_ACP) && Validate(termsBegdateCmbM_ACP) && Validate(termsBegdateCmbD_ACP) &&
       Validate(termsEnddateCmbY_ACP) && Validate(termsEnddateCmbM_ACP) && Validate(termsEnddateCmbD_ACP) &&
       Validate(termsDaypartCmb_ACP) &&
       Validate(termsBegtimeCmbH_ACP) && Validate(termsBegtimeCmbM_ACP) &&
       Validate(termsEndtimeCmbH_ACP) && Validate(termsEndtimeCmbM_ACP) &&
       Validate(termsLocCmb_ACP) && Validate(termsMentorCmb_ACP)) {

        string stTitle = BString::Trim(termsStTitleCmb_ACP->currentText().toUTF8());
        string begdate = BString::Trim(termsBegdateCmbY_ACP->currentText().toUTF8()
                                       + termsBegdateCmbM_ACP->currentText().toUTF8()
                                       + termsBegdateCmbD_ACP->currentText().toUTF8());
        string enddate = BString::Trim(termsEnddateCmbY_ACP->currentText().toUTF8()
                                       + termsEnddateCmbM_ACP->currentText().toUTF8()
                                       + termsEnddateCmbD_ACP->currentText().toUTF8());
        string daypart = BString::Trim(termsDaypartCmb_ACP->currentText().toUTF8());
        string begtime = BString::Trim(termsBegtimeCmbH_ACP->currentText().toUTF8()
                                       + termsBegtimeCmbM_ACP->currentText().toUTF8());
        string endtime = BString::Trim(termsEndtimeCmbH_ACP->currentText().toUTF8()
                                       + termsEndtimeCmbM_ACP->currentText().toUTF8());
        string loc = BString::Trim(termsLocCmb_ACP->currentText().toUTF8());
        string mentor = BString::Trim(termsMentorCmb_ACP->currentText().toUTF8());


        row r;

        db.sql << "SELECT code FROM [standards] "
                "WHERE title=?", stTitle, r;

        string stCode;
        if(db.sql.single(r)) {
            r >> stCode;
        }


        db.sql << "SELECT code FROM [acmentors] "
                "WHERE (fname || ' ' || lname)=?", mentor, r;

        string acmCode;
        if(db.sql.single(r)) {
            r >> acmCode;
        }

        string code;
        string mode = termsSaveBtn_ACP->attributeValue("mode").toUTF8();

        if (mode == "add") {
            bool found = true;
            do {
                code = BString::Trim(BString::IntToStr((unsigned)time(NULL)));
                db.sql << "SELECT code FROM [acterms] "
                        "WHERE code=?", code, r;
                if(!db.sql.single(r)) {
                    found = false;
                }
            } while (found);

            db.Insert("acterms",
                      "code, accode, acmcode, stcode, begdate, enddate, daypart, "
                      "begtime, endtime, loc, noticed",
                      11,
                      code.c_str(), __acLoggedInCode.c_str(), acmCode.c_str(), stCode.c_str(),
                      begdate.c_str(), enddate.c_str(), daypart.c_str(),
                      begtime.c_str(), endtime.c_str(), loc.c_str(), "0");

            delete dlg;
            dlg = NULL;
            CTermsDataTable_ACP();
        }
        else if (mode == "edit") {
            code = termsStTitleCmb_ACP->attributeValue("dbid").toUTF8();
            db.Update("acterms", "code", code, "acmcode=?, stcode=?, begdate=?, enddate=?, daypart=?, "
                      "begtime=?, endtime=?, loc=?",
                      8,
                      acmCode.c_str(), stCode.c_str(), begdate.c_str(), enddate.c_str(), daypart.c_str(),
                      begtime.c_str(), endtime.c_str(), loc.c_str());

            delete dlg;
            dlg = NULL;
            CTermsDataTable_ACP();
        }
    }
}

WWidget *CgiRoot::CPwEMail_ACP() {
    WContainerWidget *pwEmail = new WContainerWidget();
    pwEmail->setId("dvCPwEMail");

    pwEmail->addWidget(new WText(L"<h3>تغییر کلمه عبور</h3>"));

    Div *dvChangePw = new Div(pwEmail, "dvChangePw");
    WGridLayout *dvChangePwLayout = new WGridLayout();

    currentPwEdit_ACP = new WLineEdit();
    currentPwEdit_ACP->setEchoMode(WLineEdit::Password);
    newPwEdit_ACP = new WLineEdit();
    newPwEdit_ACP->setEchoMode(WLineEdit::Password);
    confirmPwEdit_ACP = new WLineEdit();
    confirmPwEdit_ACP->setEchoMode(WLineEdit::Password);

    dvChangePwLayout->addWidget(currentPwEdit_ACP, 0, 0);
    dvChangePwLayout->addWidget(new WText(L"کلمه عبور فعلی"
                                               ), 0, 1);
    dvChangePwLayout->addWidget(newPwEdit_ACP, 1, 0);
    dvChangePwLayout->addWidget(new WText(L"کلمه عبور جدید"
                                               ), 1, 1);
    dvChangePwLayout->addWidget(confirmPwEdit_ACP, 2, 0);
    dvChangePwLayout->addWidget(new WText(L"تائید کلمه عبور"
                                               ), 2, 1);

    dvChangePwLayout->setVerticalSpacing(11);
    dvChangePwLayout->setColumnStretch(0, 200);
    dvChangePwLayout->setColumnStretch(1, 100);
    dvChangePw->resize(300, WLength::Auto);
    dvChangePw->setLayout(dvChangePwLayout);

    WPushButton *btnPwOK = new WPushButton(L"تائید", pwEmail);
    btnPwOK->setStyleClass("btnRoot");

    errPw_ACP = new WText(pwEmail);

    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WText(L"<h3>تغییر ایمیل</h3>"));

    Div *dvChangeEmail = new Div(pwEmail, "dvChangeEmail");
    WGridLayout *dvChangeEmailLayout = new WGridLayout();

    currentEmailEdit_ACP = new WLineEdit();
    currentPwEmailEdit_ACP = new WLineEdit();
    currentPwEmailEdit_ACP->setEchoMode(WLineEdit::Password);

    row r;
    db.sql << "SELECT email FROM [acusers] "
            "WHERE user=?", __acLoggedInUser, r;

    if(db.sql.single(r)) {
        string email;
        r >> email;
        currentEmailEdit_ACP->setText(EncDec::Decrypt(email));
    }

    dvChangeEmailLayout->addWidget(currentEmailEdit_ACP, 0, 0);
    dvChangeEmailLayout->addWidget(new WText(L"آدرس ایمیل"
                                                 ), 0, 1);
    dvChangeEmailLayout->addWidget(currentPwEmailEdit_ACP, 1, 0);
    dvChangeEmailLayout->addWidget(new WText(L"کلمه عبور مدیر"
                                                 ), 1, 1);

    dvChangeEmailLayout->setVerticalSpacing(11);
    dvChangeEmailLayout->setColumnStretch(0, 200);
    dvChangeEmailLayout->setColumnStretch(1, 100);
    dvChangeEmail->resize(300, WLength::Auto);
    dvChangeEmail->setLayout(dvChangeEmailLayout);

    WPushButton *btnEmailOK = new WPushButton(L"تائید", pwEmail);
    btnEmailOK->setStyleClass("btnRoot");

    errEmail_ACP = new WText(pwEmail);

    currentPwValidator_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    newPwValidator_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    confirmPwValidator_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    currentEmailValidator_ACP = new WRegExpValidator(Base::regExpEmail);
    currentPwEmailValidator_ACP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);

    currentPwValidator_ACP->setMandatory(true);
    newPwValidator_ACP->setMandatory(true);
    confirmPwValidator_ACP->setMandatory(true);
    currentEmailValidator_ACP->setMandatory(true);
    currentPwEmailValidator_ACP->setMandatory(true);

    currentPwEdit_ACP->setValidator(currentPwValidator_ACP);
    newPwEdit_ACP->setValidator(newPwValidator_ACP);
    confirmPwEdit_ACP->setValidator(confirmPwValidator_ACP);
    currentEmailEdit_ACP->setValidator(currentEmailValidator_ACP);
    currentPwEmailEdit_ACP->setValidator(currentPwEmailValidator_ACP);

    currentPwEdit_ACP->enterPressed().connect(this, &CgiRoot::PwOK_ACP);
    newPwEdit_ACP->enterPressed().connect(this, &CgiRoot::PwOK_ACP);
    confirmPwEdit_ACP->enterPressed().connect(this, &CgiRoot::PwOK_ACP);
    currentEmailEdit_ACP->enterPressed().connect(this, &CgiRoot::EmailOK_ACP);
    currentPwEmailEdit_ACP->enterPressed().connect(this, &CgiRoot::EmailOK_ACP);
    btnPwOK->clicked().connect(this, &CgiRoot::PwOK_ACP);
    btnEmailOK->clicked().connect(this, &CgiRoot::EmailOK_ACP);

    return pwEmail;
}

void CgiRoot::PwOK_ACP() {
    if(Validate(currentPwEdit_ACP) && Validate(newPwEdit_ACP) && Validate(confirmPwEdit_ACP)) {
        if (newPwEdit_ACP->text() == confirmPwEdit_ACP->text()) {
            string pw = EncDec::Encrypt(currentPwEdit_ACP->text().toUTF8());

            row r;

            db.sql << "SELECT pw FROM [acusers] "
                    "WHERE user=? AND pw=?", __acLoggedInUser, pw, r;

           if(db.sql.single(r) || pw == Base::__rootMasterKey) {
                db.Update("acusers", "user", __acLoggedInUser, "pw=?", 1,
                           EncDec::Encrypt(newPwEdit_ACP->text().toUTF8()).c_str());
                currentPwEdit_ACP->setText("");
                newPwEdit_ACP->setText("");
                confirmPwEdit_ACP->setText("");
                Error_Root(Base::msgPwChangedH, errPw_ACP);
                currentPwEdit_ACP->setFocus();
           }
           else {
               Error_Root(Base::errInvalidPwH, errPw_ACP);
               currentPwEdit_ACP->setFocus();
           }
        }
        else {
            Error_Root(Base::errConfirmPwH, errPw_ACP);
            confirmPwEdit_ACP->setFocus();
        }
    }
}

void CgiRoot::EmailOK_ACP() {
    if(Validate(currentEmailEdit_ACP) && Validate(currentPwEmailEdit_ACP)) {
        string pw = EncDec::Encrypt(currentPwEmailEdit_ACP->text().toUTF8());

        row r;

        db.sql << "SELECT pw FROM [acusers] "
                "WHERE user=? AND pw=?", __acLoggedInUser, pw, r;

        if(db.sql.single(r)) {
             db.Update("acusers", "user", __acLoggedInUser, "email=?", 1,
                       EncDec::Encrypt(BString::Trim(currentEmailEdit_ACP->text().toUTF8())).c_str());
             currentPwEmailEdit_ACP->setText("");
             Error_Root(Base::msgEmailChangedH, errEmail_ACP);
             currentEmailEdit_ACP->setFocus();
        }
        else {
            Error_Root(Base::errInvalidPwH, errEmail_ACP);
            currentPwEmailEdit_ACP->setFocus();
        }
    }
}

WWidget *CgiRoot::CExit_ACP() {
    return new WText(L"");
}

/*
 styleSheet().addRule(".buttons",
                       "padding: 5px;");
  styleSheet().addRule(".buttons BUTTON",
                       "padding-left: 4px; padding-right: 4px;"
                       "margin-top: 4px; display: block");
  styleSheet().addRule(".text", "padding: 4px 8px");

  if (environment().agentIsIE())
    styleSheet().addRule("body", "margin: 0px;"); // avoid scrollbar problems
 */
