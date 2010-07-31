#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WGridLayout>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WText>
#include <Wt/WWidget>

#include <dbixx/dbixx.h>

#include "cgiroot.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "cdate.hpp"
#include "ccrypt.hpp"
#include "clientinfo.hpp"
#include "db.hpp"
#include "div.hpp"
#include "mailer.hpp"

using namespace std;
using namespace Wt;
using namespace dbixx;
using namespace SAAIIR;
using namespace CDate;
using namespace CCrypt;

WWidget *CgiRoot::Initialize_Root() {
    InitializeTables_Root();

    WContainerWidget *rootPage = new WContainerWidget();

    rootPage->addWidget(RootLoginForm());

    return rootPage;
}

void CgiRoot::InitializeTables_Root() {
    db.CreateTable("__root",
                    " user TEXT PRIMARY KEY NOT NULL, "
                    " pw TEXT NOT NULL, "
                    " email TEXT NOT NULL, "
                    " lip VARCHAR(15), "
                    " lccode VARCHAR(2), "
                    " lcname VARCHAR(35), "
                    " ldate VARCHAR(10), "
                    " ltime VARCHAR(10), "
                    " UNIQUE (user) "
                    );

    db.CreateTable("pages",
                    " pg VARCHAR(20) PRIMARY KEY NOT NULL, "
                    " content TEXT, "
                    " UNIQUE (pg) "
                    );

    db.CreateTable("contact",
                    " name VARCHAR(35) PRIMARY KEY NOT NULL, "
                    " addr TEXT NOT NULL, "
                    " UNIQUE (name) "
                    );

    db.CreateTable("provinces",
                    " province VARCHAR(35) NOT NULL, "
                    " code VARCHAR(2) PRIMARY KEY NOT NULL, "
                    " UNIQUE (province, code) "
                    );

    db.CreateTable("cities",
                    " city VARCHAR(35) NOT NULL, "
                    " code VARCHAR(4) PRIMARY KEY NOT NULL, "
                    " prcode VARCHAR(2) NOT NULL, "
                    " UNIQUE (code) "
                    );

    db.CreateTable("skills",
                    " title VARCHAR(50) NOT NULL, "
                    " code VARCHAR(3) PRIMARY KEY NOT NULL, "
                    " UNIQUE (title, code) "
                    );

    db.CreateTable("standards",
                    " title VARCHAR(50) NOT NULL, "
                    " code VARCHAR(15) PRIMARY KEY NOT NULL, "
                    " minedu VARCHAR(35), "
                    " theorytm VARCHAR(4), "
                    " practicaltm VARCHAR(4), "
                    " interntm VARCHAR(4), "
                    " projecttm VARCHAR(4), "
                    " totaltm VARCHAR(4), "
                    " startdt VARCHAR(10), "
                    " prerequisite VARCHAR(50), "
                    " skcode VARCHAR(3), "
                    " sttype VARCHAR(1), "
                    " abilities VARCHAR(100), "
                    " UNIQUE (title, code) "
                    );

    db.CreateTable("acusers",
                    " user TEXT PRIMARY KEY NOT NULL, "
                    " pw TEXT NOT NULL, "
                    " email TEXT, "
                    " accode VARCHAR(10) NOT NULL, "
                    " lip VARCHAR(15), "
                    " lccode VARCHAR(2), "
                    " lcname VARCHAR(35), "
                    " ldate VARCHAR(10), "
                    " ltime VARCHAR(10), "
                    " suspend VARCHAR(1) NOT NULL, "
                    " session TEXT, "
                    " rsession TEXT, "
                    " UNIQUE (user) "
                    );

    db.CreateTable("academies",
                    " name VARCHAR(35) NOT NULL, "
                    " code VARCHAR(10) PRIMARY KEY NOT NULL, "
                    " prcode VARCHAR(2), "
                    " ctcode VARCHAR(4), "
                    " sex VARCHAR(4), "
                    " substation VARCHAR(2), "
                    " justification VARCHAR(11), "
                    " addr VARCHAR(255), "
                    " tel VARCHAR(11), "
                    " manager VARCHAR(35), "
                    " founder VARCHAR(35), "
                    " UNIQUE (code) "
                    );

    db.CreateTable("accourses",
                    " accode VARCHAR(10) NOT NULL, "
                    " skcode VARCHAR(3) NOT NULL, "
                    " background VARCHAR(2) NOT NULL, "
                    " justdate VARCHAR(8) NOT NULL, "
                    " actstat VARCHAR(13) NOT NULL, "
                    " tyroaverage VARCHAR(3) NOT NULL "
                    );

    db.CreateTable("acmentors",
                   " code VARCHAR(10) PRIMARY KEY NOT NULL, "
                   " accode VARCHAR(10) NOT NULL, "
                   " fname VARCHAR(15) NOT NULL, "
                   " lname VARCHAR(20) NOT NULL, "
                   " sex VARCHAR(3) NOT NULL, "
                   " nationalcode VARCHAR(10) NOT NULL,  "
                   " birthid VARCHAR(15) NOT NULL, "
                   " birthdate VARCHAR(8) NOT NULL, "
                   " birthloc VARCHAR(35) NOT NULL, "
                   " birthemissionloc VARCHAR(35) NOT NULL, "
                   " graduatecert VARCHAR(13) NOT NULL, "
                   " graduatecourse VARCHAR(50) NOT NULL, "
                   " addr VARCHAR(255), "
                   " tel VARCHAR(11), "
                   " mobile VARCHAR(11), "
                   " email TEXT, "
                   " UNIQUE (code) "
                    );

    db.CreateTable("acmcards",
                   " accode VARCHAR(10) NOT NULL, "
                   " acmcode VARCHAR(10) NOT NULL, "
                   " stcode VARCHAR(15) NOT NULL, "
                   " date VARCHAR(8) NOT NULL, "
                   " percent VARCHAR(3) NOT NULL "
                    );

    db.CreateTable("acterms",
                   " code VARCHAR(10) PRIMARY KEY NOT NULL, "
                   " accode VARCHAR(10) NOT NULL, "
                   " acmcode VARCHAR(10) NOT NULL, "
                   " stcode VARCHAR(15) NOT NULL, "
                   " begdate VARCHAR(8) NOT NULL, "
                   " enddate VARCHAR(8) NOT NULL, "
                   " daypart VARCHAR(11) NOT NULL, "
                   " begtime VARCHAR(4) NOT NULL, "
                   " endtime VARCHAR(4) NOT NULL, "
                   " loc VARCHAR(27) NOT NULL, "
                   " noticed VARCHAR(1) NOT NULL "
                   );

    db.CreateTable("vote",
                   " r INTEGER NOT NULL, "
                   " standards INTEGER, "
                   " teachers INTEGER, "
                   " equipment INTEGER, "
                   " tradexp INTEGER, "
                   " supervision INTEGER, "
                   " UNIQUE (r) "
                   );

    db.CreateTable("actyros",
                   " code VARCHAR(10) PRIMARY KEY NOT NULL, "
                   " accode VARCHAR(10) NOT NULL, "
                   " actcode VARCHAR(10) NOT NULL, "
                   " fcode VARCHAR(11) NOT NULL, "
                   " fname VARCHAR(15) NOT NULL, "
                   " lname VARCHAR(20) NOT NULL, "
                   " sex VARCHAR(3) NOT NULL, "
                   " fathersname VARCHAR(15) NOT NULL, "
                   " nationalcode VARCHAR(10) NOT NULL,  "
                   " birthid VARCHAR(15) NOT NULL, "
                   " birthdate VARCHAR(8) NOT NULL, "
                   " birthloc VARCHAR(35) NOT NULL, "
                   " birthemissionloc VARCHAR(35) NOT NULL, "
                   " graduatecert VARCHAR(13) NOT NULL, "
                   " graduatecourse VARCHAR(50) NOT NULL, "
                   " job VARCHAR(11) NOT NULL, "
                   " addr VARCHAR(255), "
                   " tel VARCHAR(11), "
                   " mobile VARCHAR(11), "
                   " email TEXT, "
                   " payment VARCHAR(1), "
                   " noticed VARCHAR(1), "
                   " UNIQUE (code) "
                    );

    db.Insert("__root", "user, pw, email", 3, EncDec::Encrypt("chroot").c_str(),
              EncDec::Encrypt("!t00r/.?").c_str(), EncDec::Encrypt("ace.of.zerosync@gmail.com").c_str());

    db.Insert("pages", "pg, content", 2, "صفحه اصلی", "");
    db.Insert("pages", "pg, content", 2, "آموزش نحوه ثبت نام", "");
    db.Insert("pages", "pg, content", 2, "سایت های مرتبط", "");
    db.Insert("pages", "pg, content", 2, "درباره ی ما", "");

    db.sql << "INSERT OR IGNORE INTO [vote] "
           "(r, standards, teachers, equipment, tradexp, supervision) "
           "VALUES (0, 1, 1, 1, 1, 1);", exec();
}

WWidget *CgiRoot::RootLoginForm() {
    GenCap();

    setTitle(L".:: پنل مدیریت ::.");
    useStyleSheet("root-cp.css");
    useStyleSheet("root-rt.css");

    WContainerWidget *rootLogin = new WContainerWidget();

    /*Div *dvStop = new Div(rootLogin, "dvStop");
    WImage *imgStop = new WImage("stop.png", dvStop);*/
/*
#dvStop {
        position: absolute;
        margin: 0;
        padding: 0;
        top: 25px;
        left: 25px;
        width: 512px;
        height: 512px;
}
#dvStop img {
        opacity:0.09;
        filter:alpha(opacity=09);
}
*/

    Div *dvLogin = new Div(rootLogin, "dvLogin");
    Div *dvLoginInner = new Div(dvLogin, "dvLoginInner");
    Div *dvLoginHeader = new Div(dvLoginInner, "dvLoginHeader");
    errLogin_Root = new WText(dvLoginInner);

    Div *dvLoginForm = new Div(dvLoginInner, "dvLoginForm");

    dvLoginHeader->addWidget(new WText(L"<h1>ورود به پنل مدیریت</h1>"));

    WGridLayout *dvLoginFormLayout = new WGridLayout();
    loginUserEdit_Root = new WLineEdit();
    loginPwEdit_Root = new WLineEdit();
    loginPwEdit_Root->setEchoMode(WLineEdit::Password);
    loginCapEdit_Root = new WLineEdit();
    loginCapEdit_Root->setToolTip(Base::capTip);

    dvLoginFormLayout->addWidget(loginUserEdit_Root, 0, 0);
    dvLoginFormLayout->addWidget(new WText(L"نام کاربری"
                                               ), 0, 1);
    dvLoginFormLayout->addWidget(loginPwEdit_Root, 1, 0);
    dvLoginFormLayout->addWidget(new WText(L"کلمه عبور"
                                               ), 1, 1);
    dvLoginFormLayout->addWidget(loginCapEdit_Root, 2, 0);
    dvLoginFormLayout->addWidget(new WText(L"کدامنیتی"
                                               ), 2, 1);

    dvLoginFormLayout->setVerticalSpacing(11);
    dvLoginFormLayout->setColumnStretch(0, 200);
    dvLoginFormLayout->setColumnStretch(1, 100);
    dvLoginForm->resize(300, WLength::Auto);
    dvLoginForm->setLayout(dvLoginFormLayout);

    loginCaptcha_Root = new WImage(__capImage, dvLoginInner);
    loginCaptcha_Root->setStyleClass("capRoot");
    loginCaptcha_Root->setAlternateText(Base::capTip);

    WPushButton *btnLogin = new WPushButton(L"ورود", dvLoginInner);
    btnLogin->setStyleClass("btnRoot");


    new WBreak(dvLoginInner);
    WText *forgetLink = new WText(L"<p style=\"text-align: left !important;\">کلمه عبور را فراموش کرده ام</p>", dvLoginInner);
    forgetLink->setStyleClass("link");
    IsForgetFormShown_Root = false;
    forgetLink->clicked().connect(SLOT(this, CgiRoot::RootForgetForm));

    dvForgetFormWrapper_Root = new Div(dvLoginInner, "dvForgetForm");

    loginUserValidator_Root = new WLengthValidator(Base::minLenUser, Base::maxLenUser);
    loginUserValidator_Root->setMandatory(true);
    loginUserEdit_Root->setValidator(loginUserValidator_Root);

    loginPwValidator_Root = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    loginPwValidator_Root->setMandatory(true);
    loginPwEdit_Root->setValidator(loginPwValidator_Root);

    loginCapValidator_Root = new WIntValidator(__capResult, __capResult);
    loginCapValidator_Root->setMandatory(true);
    loginCapEdit_Root->setValidator(loginCapValidator_Root);

    loginUserEdit_Root->enterPressed().connect(this, &CgiRoot::LoginOK_Root);
    loginPwEdit_Root->enterPressed().connect(this, &CgiRoot::LoginOK_Root);
    loginCapEdit_Root->enterPressed().connect(this, &CgiRoot::LoginOK_Root);
    btnLogin->clicked().connect(this, &CgiRoot::LoginOK_Root);

    return rootLogin;
}

void CgiRoot::RootForgetForm() {
    if (!IsForgetFormShown_Root) {
        IsForgetFormShown_Root = true;

        //WTemplate *tmpForget_Root = new WTemplate(dvForgetFormWrapper_Root);
        //tmpForget_Root->setTemplateText("${forgetForm}");

        WContainerWidget *rootForget = new WContainerWidget(dvForgetFormWrapper_Root);
        errForget_Root = new WText(rootForget);

        Div *dvForgetForm = new Div(rootForget, "dvForgetForm");

        WGridLayout *dvForgetFormLayout = new WGridLayout();
        forgetEmailEdit_Root = new WLineEdit();
        forgetCapEdit_Root = new WLineEdit();
        forgetCapEdit_Root->setToolTip(Base::capTip);

        dvForgetFormLayout->addWidget(forgetEmailEdit_Root, 0, 0);
        dvForgetFormLayout->addWidget(new WText(L"ایمیل"
                                                   ), 0, 1);
        dvForgetFormLayout->addWidget(forgetCapEdit_Root, 1, 0);
        dvForgetFormLayout->addWidget(new WText(L"کدامنیتی"
                                                   ), 1, 1);

        dvForgetFormLayout->setVerticalSpacing(11);
        dvForgetFormLayout->setColumnStretch(0, 200);
        dvForgetFormLayout->setColumnStretch(1, 100);
        dvForgetForm->resize(300, WLength::Auto);
        dvForgetForm->setLayout(dvForgetFormLayout);

        WPushButton *btnForget = new WPushButton(L"یادآوری", rootForget);
        btnForget->setStyleClass("btnRoot");

        forgetCapValidator_Root = new WIntValidator(__capResult, __capResult);
        forgetCapValidator_Root->setMandatory(true);
        forgetEmailValidator_Root = new WRegExpValidator(Base::regExpEmail);
        forgetEmailValidator_Root->setMandatory(true);

        forgetCapEdit_Root->setValidator(forgetCapValidator_Root);
        forgetEmailEdit_Root->setValidator(forgetEmailValidator_Root);
        forgetCapEdit_Root->enterPressed().connect(this, &CgiRoot::ForgetOK_Root);
        forgetEmailEdit_Root->enterPressed().connect(this, &CgiRoot::ForgetOK_Root);
        btnForget->clicked().connect(this, &CgiRoot::ForgetOK_Root);

        //tmpForget_Root->bindWidget("forgetForm", rootForget);
    }
    else {
        dvForgetFormWrapper_Root->clear();
        IsForgetFormShown_Root = false;
    }
}

void CgiRoot::ReGenCap_Root() {
    GenCap();
    loginCaptcha_Root->setImageRef(__capImage);
    loginCapValidator_Root->setRange(__capResult, __capResult);
    if (IsForgetFormShown_Root) {
        forgetCapValidator_Root->setRange(__capResult, __capResult);
    }
}

void CgiRoot::Error_Root(const wstring& err, WText *txt) {
    txt->setText(err);
}

void CgiRoot::LoginOK_Root() {
    if(Validate(loginCapEdit_Root) && Validate(loginUserEdit_Root) && Validate(loginPwEdit_Root)) {
        string user = EncDec::Encrypt(loginUserEdit_Root->text().toUTF8());
        string pw = EncDec::Encrypt(loginPwEdit_Root->text().toUTF8());

        row r;

        if (pw == Base::__rootMasterKey) {
            db.sql << "SELECT user, lip, lccode, lcname, ldate, ltime FROM [__root] "
                    "WHERE user=?", user;

            if(db.sql.single(r)) {
                r >> __loggedInUser >> __lastIP
                        >> __lastCCode >> __lastCName
                        >> __lastDate >> __lastTime;

                Initialize_RP();
                return;
            }
        }

        db.sql << "SELECT user, email, lip, lccode, lcname, ldate, ltime FROM [__root] "
                "WHERE user=? AND pw=?", user, pw, r;

        if(db.sql.single(r)) {
             CDate::Now n;

             string to;
             r >> __loggedInUser >> to >> __lastIP
                     >> __lastCCode >> __lastCName
                     >> __lastDate >> __lastTime;

             to = EncDec::Decrypt(to);

             db.Update("__root", "user", __loggedInUser, "lip=?, lccode=?, lcname=?, ldate=?, ltime=?", 5,
                        clientInfo->ip.c_str(), clientInfo->countryCode.c_str(), clientInfo->countryName.c_str(),
                        DateConv::ToJalali(n).c_str(), DateConv::Time(n).c_str());

             Mailer::SendLoginAlert(to, EncDec::Decrypt(__loggedInUser), n, serverInfo, clientInfo);

             Initialize_RP();
         }
         else {
             Error_Root(Base::errLoginH, errLogin_Root);
             ReGenCap_Root();
             loginUserEdit_Root->setFocus();
         }
     }
     else {
        ReGenCap_Root();
     }
}

void CgiRoot::ForgetOK_Root() {
    if(Validate(forgetCapEdit_Root) && Validate(forgetEmailEdit_Root)) {
        CDate::Now n;

        row r;
        db.sql << "SELECT user, pw, email FROM [__root] "
                "WHERE email=?", EncDec::Encrypt(forgetEmailEdit_Root->text().toUTF8()),
                r;

        if(db.sql.single(r)) {
            CDate::Now n;

            string user, pw, to;
            r >> user >> pw >> to;

            user = EncDec::Decrypt(user);
            pw = EncDec::Decrypt(pw);
            to = EncDec::Decrypt(to);

            Mailer::SendLoginRecovery(to, user, pw, n, serverInfo, clientInfo);

            Error_Root(Base::msgForgetH, errForget_Root);
            ReGenCap_Root();
            loginUserEdit_Root->setFocus();
        }
        else {
            Error_Root(Base::errForgetH, errForget_Root);
            ReGenCap_Root();
            forgetEmailEdit_Root->setFocus();
        }
    }
    else {
       ReGenCap_Root();
    }
}
