#include <ctime>

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WEnvironment>
#include <Wt/WGridLayout>
#include <Wt/WInPlaceEdit>
#include <Wt/WLabel>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMenu>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WSignalMapper>
#include <Wt/WStackedWidget>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WText>
#include <Wt/WTextEdit>
#include <Wt/WWidget>

#include <dbixx/dbixx.h>

#include "cgiroot.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "ccrypt.hpp"
#include "cdate.hpp"
#include "db.hpp"
#include "div.hpp"

using namespace std;
using namespace Wt;
using namespace dbixx;
using namespace SAAIIR;
using namespace CCrypt;
using namespace CDate;

void CgiRoot::Initialize_RP() {
    root()->clear();

    WContainerWidget *rootPanel = new WContainerWidget();

    Div *dvBody = new Div(rootPanel, "dvBody");

    Div *dvBodyInner = new Div(dvBody, "dvBodyInner");
    Div *dvHeader = new Div(dvBodyInner, "dvHeader");
    Div *dvLogo = new Div(dvHeader, "dvLogo");
    dvLogo->addWidget(new WText(Base::titleText));

    Div *dvMenusWrapper = new Div(dvBodyInner , "dvMenusWrapper");
    Div *dvMenusInner = new Div(dvMenusWrapper , "dvMenusInner");
    Div *dvMenus = new Div(dvMenusInner , "dvMenus");
    Div *dvLastLogin = new Div(dvMenusInner , "dvLastLogin");
    dvLastLogin->addWidget(new WText(L"<h2>آخرین ورود</h2>"));
    Div *dvLastLoginDetail = new Div(dvLastLogin , "dvLastLoginDetail");
    dvLastLoginDetail->addWidget(new WText(__lastDate + " - " + __lastTime +
                                           "<br />" +
                                           __lastCCode + ", " + __lastCName +
                                           "<br />" +
                                           __lastIP
                                           ));

    Div *dvContentsWrapper = new Div(dvBodyInner, "dvContentsWrapper");
    Div *dvContentsInner = new Div(dvContentsWrapper, "dvContentsInner");
    Div *dvContents = new Div(dvContentsInner, "dvContents");

    WStackedWidget *mainContents = new WStackedWidget(dvContents);
    mainContents->setId("mainContents");

    mainMenu_RP = new WMenu(mainContents, Vertical, dvMenus);
    mainMenu_RP->setRenderAsList(true);

    //mainMenu_RP->setInternalPathEnabled("/");

    /*mainMenu_RP->addItem
            (L"صفحه آغازین", CHome_RP(), WMenuItem::PreLoading)->setPathComponent("");*/
    /*mainMenu_RP->addItem
            (L"اطلاعات پایه", CBase_RP(), WMenuItem::PreLoading)->setPathComponent("");*/
    mainMenu_RP->addItem
            (L"آموزشگاه ها", CAcademies_RP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_RP->addItem
            (L"مدیر صفحات", CPages_RP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_RP->addItem
            (L"تماس با", CContact_RP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_RP->addItem
            (L"کلمه عبور و ایمیل", CPwEMail_RP(), WMenuItem::PreLoading)->setPathComponent("");
    mainMenu_RP->addItem
            (L"خروج", CExit_RP(), WMenuItem::PreLoading)->setPathComponent("");

    mainMenu_RP->itemSelected().connect(this, &CgiRoot::ExitRootPanel);


    root()->addWidget(rootPanel);
}

void CgiRoot::ExitRootPanel(WMenuItem *mItem) {
    if (mItem->text() == L"خروج") {
        db.sql << "UPDATE [acusers] "
                "SET rsession=? "
                "WHERE rsession!=?", "", "", exec();
        redirect("?root");
        quit();
    }
}

/*
WWidget *CgiRoot::CHome_RP() {
    return new WText(L"");
}

WWidget *CgiRoot::CBase_RP() {
    return new WText(L"");
}
*/

WWidget *CgiRoot::CAcademies_RP() {
    WContainerWidget *academies = new WContainerWidget();
    academies->setId("dvCAcademies");

    academiesProvinceCmb_RP = new WComboBox(academies);
    academies->addWidget(new WText("&nbsp;&nbsp;&nbsp;"));
    academiesCityCmb_RP = new WComboBox(academies);

    academiesProvinceCmb_RP->addItem(L".: استان :.");
    academiesCityCmb_RP->addItem(L".: شهرستان :.");

    academiesProvinceCmb_RP->setCurrentIndex(0);
    academiesCityCmb_RP->setCurrentIndex(0);

    academies->addWidget(new WText("&nbsp;&nbsp;&nbsp;&nbsp;"));
    academiesAddBtn_RP = new WPushButton(L"افزودن", academies);
    //academies->addWidget(new WText("&nbsp;&nbsp;"));
    //academiesReturnBtn_RP = new WPushButton(L"بازگشت", academies);

    SetCAcademiesForm_RP(0);

    result res;
    row r;

    db.sql << "SELECT province FROM [provinces] ORDER BY province ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        while(res.next(r)) {
            string pr;
            r >> pr;
            academiesProvinceCmb_RP->addItem(BString::StrToWStr(pr));
        }
    }

    academiesProvinceCmb_RP->sactivated().connect(this, &CgiRoot::AcademiesProvinceCmbChanged_RP);
    academiesCityCmb_RP->sactivated().connect(this, &CgiRoot::AcademiesCityCmbChanged_RP);
    academiesAddBtn_RP->clicked().connect(this, &CgiRoot::AcademiesAddBtnOK_RP);
    //academiesReturnBtn_RP->clicked().connect(this, &CgiRoot::AcademiesReturnBtnOK_RP);

    dvAcademiesWrapper_RP = new Div(academies, "dvAcademies");
/*    dvAcademiesAddForm_RP = new Div(, "");
    dvAcademiesJustForm_RP = new Div(, "");
    dvAcademiesJust_RP = new Div(, "");
    dvAcademiesTableWrapper_RP = new Div(, "");*/

    return academies;
}

void CgiRoot::SetCAcademiesForm_RP(int flag) {
    switch(flag) {
    case 0:
        academiesProvinceCmb_RP->enable();
        academiesCityCmb_RP->enable();
        academiesAddBtn_RP->hide();
        //academiesReturnBtn_RP->hide();
        break;
    case 1:
        academiesProvinceCmb_RP->enable();
        academiesCityCmb_RP->enable();
        academiesAddBtn_RP->show();
        //academiesReturnBtn_RP->hide();
        break;
/*    case 2:
        academiesProvinceCmb_RP->disable();
        academiesCityCmb_RP->disable();
        academiesAddBtn_RP->hide();
        academiesReturnBtn_RP->show();
        break;*/
    }
}

void CgiRoot::AcademiesProvinceCmbChanged_RP(WString pr) {
    academiesCityCmb_RP->clear();
    academiesCityCmb_RP->addItem(L".: شهرستان :.");
    dvAcademiesWrapper_RP->clear();
    SetCAcademiesForm_RP(0);

    if (pr.toUTF8() != ".: استان :.") {
        result res;
        row r;

        /*db.sql << "SELECT city FROM [cities] WHERE prcode IN "
                "(SELECT code FROM [provinces] WHERE province=?) "
                "ORDER BY city ASC;", pr.toUTF8();*/

        db.sql << "SELECT city FROM [cities] WHERE prcode="
                "(SELECT code FROM [provinces] WHERE province=?) "
                "ORDER BY city ASC;", pr.toUTF8();

        db.sql.fetch(res);

        if (res.rows() > 0) {
            while(res.next(r)) {
                string ct;
                r >> ct;
                academiesCityCmb_RP->addItem(BString::StrToWStr(ct));
            }
        }
    }

    CAcademiesDataTable_RP();
}

void CgiRoot::AcademiesCityCmbChanged_RP(WString ct) {
    if (ct.toUTF8() != ".: شهرستان :.") {
        SetCAcademiesForm_RP(1);
    }
    else {
        SetCAcademiesForm_RP(0);
    }
    CAcademiesDataTable_RP();
}

void CgiRoot::AcademiesAddBtnOK_RP() {
    dlg = new WDialog(L"افزودن آموزشگاه");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetAcademiesForm_RP("add"));
    dlg->show();
}

/*void CgiRoot::AcademiesReturnBtnOK_RP() {
    if (academiesCityCmb_RP->currentIndex() != -1 && academiesCityCmb_RP->currentIndex() != 0) {
        SetCAcademiesForm_RP(1);
    }
    else {
        SetCAcademiesForm_RP(0);
    }
    CAcademiesDataTable_RP();
}*/

void CgiRoot::CAcademiesDataTable_RP() {
    dvAcademiesWrapper_RP->clear();

    if (academiesProvinceCmb_RP->currentIndex() == -1 || academiesProvinceCmb_RP->currentIndex() == 0) {
        return;
    }

    WTable *academiesTable = new WTable(dvAcademiesWrapper_RP);
    academiesTable->setStyleClass("tbl");

    academiesTable->elementAt(0, 0)->addWidget(new WText(L"نام آموزشگاه"));
    academiesTable->elementAt(0, 1)->addWidget(new WText(L"جنسیت پذیرش"));
    academiesTable->elementAt(0, 2)->addWidget(new WText(L"تلفن"));
    academiesTable->elementAt(0, 3)->addWidget(new WText(L"مدیر"));
    academiesTable->elementAt(0, 4)->addWidget(new WText(L"سایر اطلاعات"));
    academiesTable->elementAt(0, 5)->addWidget(new WText(L"رشته ها"));
    academiesTable->elementAt(0, 6)->addWidget(new WText(L"مدیریت"));
    academiesTable->elementAt(0, 7)->addWidget(new WText(L"تعلیق"));
    academiesTable->elementAt(0, 8)->addWidget(new WText(L"ویرایش"));
    academiesTable->elementAt(0, 9)->addWidget(new WText(L"حذف"));

    academiesTable->elementAt(0, 0)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 1)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 2)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 3)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 4)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 5)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 6)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 7)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 8)->setStyleClass("tblHeader");
    academiesTable->elementAt(0, 9)->setStyleClass("tblHeader");

    result res;
    row r;

    if (academiesCityCmb_RP->currentIndex() != -1 && academiesCityCmb_RP->currentIndex() != 0) {
        db.sql << "SELECT name, code, sex, tel, manager "
                "FROM [academies] WHERE "
                "prcode=(SELECT code FROM [provinces] WHERE province=?) AND "
                "ctcode=(SELECT code FROM [cities] WHERE city=?) "
                "ORDER BY name ASC", academiesProvinceCmb_RP->currentText().toUTF8(),
                academiesCityCmb_RP->currentText().toUTF8();
    }
    else {
        db.sql << "SELECT name, code, sex, tel, manager "
                "FROM [academies] WHERE "
                "prcode=(SELECT code FROM [provinces] WHERE province=?) "
                "ORDER BY name ASC", academiesProvinceCmb_RP->currentText().toUTF8();
    }

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string name, code, sex, tel, manager;
            r >> name >> code >> sex >> tel >> manager;

            academiesTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(name)));
            academiesTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(sex)));
            academiesTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(tel)));
            academiesTable->elementAt(i, 3)->addWidget(new WText(BString::StrToWStr(manager)));

            WSignalMapper<WPushButton *> *moreInfoMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *courseMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *suspendMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            moreInfoMap->mapped().connect(this, &CgiRoot::MoreInfoAcademiesTableCell_RP);
            courseMap->mapped().connect(this, &CgiRoot::CourseAcademiesTableCell_RP);
            suspendMap->mapped().connect(this, &CgiRoot::SuspendAcademiesTableCell_RP);
            editMap->mapped().connect(this, &CgiRoot::EditAcademiesTableCell_RP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseAcademiesTableCell_RP);

            row ur;
            db.sql << "SELECT suspend FROM [acusers] "
                    "WHERE accode=?", code, r;

            string suspend = "1";
            if(db.sql.single(ur)) {
                r >> suspend;
            }

            WPushButton *moreInfoButton = new WPushButton(L"سایر اطلاعات");
            WPushButton *courseButton = new WPushButton(L"رشته ها");
            WPushButton *suspendButton = new WPushButton(suspend == "0" ? L"تعلیق" : L"فعال سازی");
            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            moreInfoMap->mapConnect(moreInfoButton->clicked(), moreInfoButton);
            courseMap->mapConnect(courseButton->clicked(), courseButton);
            suspendMap->mapConnect(suspendButton->clicked(), suspendButton);
            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            moreInfoButton->setStyleClass("tblButton");
            courseButton->setStyleClass("tblButton");
            suspendButton->setStyleClass("tblButton");
            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            moreInfoButton->setAttributeValue("dbid", BString::StrToWStr(code));
            courseButton->setAttributeValue("dbid", BString::StrToWStr(code));
            suspendButton->setAttributeValue("dbid", BString::StrToWStr(code));
            suspendButton->setAttributeValue("suspend", BString::StrToWStr(suspend));
            editButton->setAttributeValue("dbid", BString::StrToWStr(code));
            eraseButton->setAttributeValue("dbid", BString::StrToWStr(code));

            WAnchor *a = new WAnchor("?acp=" + AcSessionGen(code, true), L"پنل مدیریت");
            a->setTarget(TargetNewWindow);

            /*WSignalMapper<WAnchor *> *cpMap = new WSignalMapper<WAnchor *>(this);
            cpMap->mapped().connect(this, &CgiRoot::CpAcademiesTableCell_RP);
            cpMap->mapConnect(a->clicked(), a);
            a->setAttributeValue("dbid", BString::StrToWStr(code));*/

            academiesTable->elementAt(i, 4)->addWidget(moreInfoButton);
            academiesTable->elementAt(i, 5)->addWidget(courseButton);
            academiesTable->elementAt(i, 6)->addWidget(a);
            academiesTable->elementAt(i, 7)->addWidget(suspendButton);
            academiesTable->elementAt(i, 8)->addWidget(editButton);
            academiesTable->elementAt(i, 9)->addWidget(eraseButton);
        }
    }
}

void CgiRoot::MoreInfoAcademiesTableCell_RP(WAnchor *sender) {
    WPushButton *btn = new WPushButton();
    btn->setAttributeValue("dbid", sender->attributeValue("dbid"));
    MoreInfoAcademiesTableCell_RP(btn);
}

void CgiRoot::MoreInfoAcademiesTableCell_RP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    string name, sex, substation, justification, addr, tel, manager, founder, user, pw;

    row r;

    db.sql << "SELECT name, sex, substation, justification, addr, tel, manager, founder "
            "FROM [academies] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> name >> sex >> substation >> justification >> addr >> tel >> manager >> founder;
    }

    /*try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }*/

    dlg = new WDialog(L"مشخصات آموزشگاه");
    dlg->setModal(true);

    WContainerWidget *dvAcMoreInfo = new Div(dlg->contents(), "dvAcMoreInfo");

    WContainerWidget *dvAcMoreInfoInner = new Div(dvAcMoreInfo, "dvAcMoreInfoInner");
    WGridLayout *dvAcMoreInfoInnerLayout = new WGridLayout();

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(name)), 0, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 0, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"نام آموزشگاه"
                                              ), 0, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(sex)), 1, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 1, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"پذیرش جنسیت"
                                              ), 1, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(substation)), 2, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 2, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"شعبه"
                                              ), 2, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(justification)), 3, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 3, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"وضعیت مجوز"
                                              ), 3, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(addr)), 4, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 4, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"نشانی"
                                              ), 4, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(tel)), 5, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 5, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"تلفن"
                                              ), 5, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(manager)), 6, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 6, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"مدیر"
                                              ), 6, 2);

    dvAcMoreInfoInnerLayout->addWidget(new WText(BString::StrToWStr(founder)), 7, 0);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L":"), 7, 1);
    dvAcMoreInfoInnerLayout->addWidget(new WText(L"موسس"
                                              ), 7, 2);

    dvAcMoreInfoInnerLayout->setVerticalSpacing(11);
    dvAcMoreInfoInnerLayout->setColumnStretch(0, 185);
    dvAcMoreInfoInnerLayout->setColumnStretch(1, 25);
    dvAcMoreInfoInnerLayout->setColumnStretch(2, 115);
    dvAcMoreInfoInner->resize(325, WLength::Auto);
    dvAcMoreInfoInner->setLayout(dvAcMoreInfoInnerLayout);

    WPushButton *cancel = new WPushButton(L"بازگشت", dvAcMoreInfo);
    cancel->setStyleClass("btnDialogs");
    cancel->clicked().connect(dlg, &WDialog::accept);

    new WBreak(dvAcMoreInfo);
    new WBreak(dvAcMoreInfo);
    new WBreak(dvAcMoreInfo);
    new WBreak(dvAcMoreInfo);

    dlg->show();
}

void CgiRoot::CourseAcademiesTableCell_RP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();
    GetAcCourseDialog_RP(code);
}

/*
void CgiRoot::CpAcademiesTableCell_RP(WAnchor *sender) {
    string code = sender->attributeValue("dbid").toUTF8();
}
*/

void CgiRoot::SuspendAcademiesTableCellOK_RP(StandardButton result) {
    if (result == Ok) {
        string code = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;

        db.sql << "SELECT accode FROM [acusers] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            string suspend = tableBtn_RP->attributeValue("suspend").toUTF8() == "0" ? "1" : "0";

            db.Update("acusers", "accode", code, "suspend=?", 1, suspend.c_str());

            wstring action = (suspend == "0" ? L"تعلیق" : L"فعال سازی");
            tableBtn_RP->setText(action);
            tableBtn_RP->setAttributeValue("suspend", BString::StrToWStr(suspend));
        }
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

void CgiRoot::SuspendAcademiesTableCell_RP(WPushButton *sender) {
    string suspend = sender->attributeValue("suspend").toUTF8();
    wstring action = (suspend == "0" ? L"تعلیق" : L"فعال سازی");

    tableBtn_RP = sender;

    /*try {
        if (msgBox) {
            delete msgBox;
            msgBox = NULL;
        }
    }
    catch (...) {
    }*/

    msgBox = new WMessageBox(action,
                                L"آیا مایل به "
                                + action +
                                L" آموزشگاه موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::SuspendAcademiesTableCellOK_RP);

    msgBox->show();
}

void CgiRoot::EditAcademiesTableCell_RP(WPushButton *sender) {
    string code = sender->attributeValue("dbid").toUTF8();

    /*try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }*/

    dlg = new WDialog(L"ویرایش آموزشگاه");
    dlg->setModal(true);
    dlg->contents()->addWidget(GetAcademiesForm_RP("edit"));

    string name, sex, substation, justification, addr, tel, manager, founder, user, pw;

    row r;

    db.sql << "SELECT name, sex, substation, justification, addr, tel, manager, founder "
            "FROM [academies] WHERE code=?;", code, r;

    if(db.sql.single(r)) {
        r >> name >> sex >> substation >> justification >> addr >> tel >> manager >> founder;
    }

    db.sql << "SELECT user, pw FROM [acusers] WHERE accode=?;", code, r;

    if(db.sql.single(r)) {
        r >> user >> pw;
        user = EncDec::Decrypt(user);
        pw = EncDec::Decrypt(pw);
    }

    academiesNameEdit_RP->setText(BString::StrToWStr(name));
    academiesNameEdit_RP->setAttributeValue("dbid", BString::StrToWStr(code));

    academiesSexCmb_RP->setCurrentIndex(academiesSexCmb_RP->findText(BString::StrToWStr(sex), MatchExactly));
    academiesSubstationEdit_RP->setText(BString::StrToWStr(substation));
    academiesJustificationCmb_RP->setCurrentIndex(academiesJustificationCmb_RP->findText(BString::StrToWStr(justification), MatchExactly));
    academiesAddrTArea_RP->setText(BString::StrToWStr(addr));
    academiesTelEdit_RP->setText(BString::StrToWStr(tel));
    academiesManagerEdit_RP->setText(BString::StrToWStr(manager));
    academiesFounderEdit_RP->setText(BString::StrToWStr(founder));

    academiesUserEdit_RP->setText(BString::StrToWStr(user));
    academiesUserEdit_RP->setAttributeValue("dbid", BString::StrToWStr(user));

    academiesPwEdit_RP->setText(BString::StrToWStr(pw));

    dlg->show();
}

void CgiRoot::EraseAcademiesTableCell_RP(WPushButton *sender) {
    tableBtn_RP = sender;

    /*try {
        if (msgBox) {
            delete msgBox;
            msgBox = NULL;
        }
    }
    catch (...) {
    }*/

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف آموزشگاه موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseAcademiesTableCellOK_RP);

    msgBox->show();
}

void CgiRoot::EraseAcademiesTableCellOK_RP(StandardButton result) {
    if (result == Ok) {
        string code = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;

        db.sql << "SELECT accode FROM [acusers] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acusers", "accode", code);
        }

        db.sql << "SELECT accode FROM [accourses] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("accourses", "accode", code);
        }

        db.sql << "SELECT accode FROM [acmcards] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acmcards", "accode", code);
        }

        db.sql << "SELECT accode FROM [acmentors] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acmentors", "accode", code);
        }

        db.sql << "SELECT accode FROM [acterms] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("acterms", "accode", code);
        }

        db.sql << "SELECT accode FROM [actyros] "
                "WHERE accode=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("actyros", "accode", code);
        }

        db.sql << "SELECT code FROM [academies] "
                "WHERE code=?", code, r;

        if(db.sql.single(r)) {
            db.Delete("academies", "code", code);
        }

        dbPics.DropTable(code);

        CAcademiesDataTable_RP();
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

WWidget *CgiRoot::GetAcademiesForm_RP(WString form) {
    WContainerWidget *academiesForm = new WContainerWidget();
    academiesForm->setId("dvAcademiesFormWrapper");

    academiesNameEdit_RP = new WLineEdit();
    academiesSexCmb_RP = new WComboBox();
    academiesSubstationEdit_RP = new WLineEdit();
    academiesJustificationCmb_RP = new WComboBox();
    academiesAddrTArea_RP = new WTextArea();
    academiesTelEdit_RP = new WLineEdit();
    academiesManagerEdit_RP = new WLineEdit();
    academiesFounderEdit_RP = new WLineEdit();
    academiesUserEdit_RP = new WLineEdit();
    academiesPwEdit_RP = new WLineEdit();

    academiesSexCmb_RP->addItem(L"");
    academiesSexCmb_RP->addItem(L"مرد");
    academiesSexCmb_RP->addItem(L"زن");
    academiesSexCmb_RP->addItem(L"هردو");

    academiesJustificationCmb_RP->addItem(L"");
    academiesJustificationCmb_RP->addItem(L"مجوز اولیه");
    academiesJustificationCmb_RP->addItem(L"موافقت نامه");
    academiesJustificationCmb_RP->addItem(L"مرخصی");
    academiesJustificationCmb_RP->addItem(L"فعال");

    Div *dvAcademiesForm = new Div(academiesForm, "dvAcademiesForm");

    WGridLayout *dvAcademiesFormLayout = new WGridLayout();

    dvAcademiesFormLayout->addWidget(academiesNameEdit_RP, 0, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"نام آموزشگاه"
                                               ), 0, 1);

    dvAcademiesFormLayout->addWidget(academiesSexCmb_RP, 1, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"پذیرش جنسیت"
                                               ), 1, 1);

    dvAcademiesFormLayout->addWidget(academiesSubstationEdit_RP, 2, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"شعبه"
                                               ), 2, 1);

    dvAcademiesFormLayout->addWidget(academiesJustificationCmb_RP, 3, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"وضعیت مجوز"
                                               ), 3, 1);

    dvAcademiesFormLayout->addWidget(academiesAddrTArea_RP, 4, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"نشانی"
                                               ), 4, 1);

    dvAcademiesFormLayout->addWidget(academiesTelEdit_RP, 5, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"تلفن"
                                               ), 5, 1);

    dvAcademiesFormLayout->addWidget(academiesManagerEdit_RP, 6, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"مدیر"
                                               ), 6, 1);

    dvAcademiesFormLayout->addWidget(academiesFounderEdit_RP, 7, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"موسس"
                                               ), 7, 1);

    dvAcademiesFormLayout->addWidget(academiesUserEdit_RP, 8, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"نام کاربری"
                                               ), 8, 1);
    academiesUserEdit_RP->setStyleClass("ltrInput");

    dvAcademiesFormLayout->addWidget(academiesPwEdit_RP, 9, 0);
    dvAcademiesFormLayout->addWidget(new WText(L"کلمه عبور"
                                               ), 9, 1);
    academiesPwEdit_RP->setEchoMode(WLineEdit::Password);
    academiesPwEdit_RP->setStyleClass("ltrInput");

    dvAcademiesFormLayout->setVerticalSpacing(11);
    dvAcademiesFormLayout->setColumnStretch(0, 200);
    dvAcademiesFormLayout->setColumnStretch(1, 125);
    dvAcademiesForm->resize(325, WLength::Auto);
    dvAcademiesForm->setLayout(dvAcademiesFormLayout);

    academiesSaveBtn_RP = new WPushButton(academiesForm);
    academiesSaveBtn_RP->setStyleClass("btnDialogs");

    if (form == "add") {
        academiesSaveBtn_RP->setText(L"افزودن");
    }
    else if (form == "edit") {
        academiesSaveBtn_RP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", academiesForm);
    cancel->clicked().connect(dlg, &WDialog::accept);
    cancel->setStyleClass("btnDialogs");

    errAcademiesForm_RP = new WText("");
    academiesForm->addWidget(errAcademiesForm_RP);
    academiesSaveBtn_RP->setAttributeValue("mode", form);

    academiesNameValidator_RP = new WLengthValidator(1, 35);
    academiesSexValidator_RP = new WLengthValidator(1, 4);
    academiesSubstationValidator_RP = new WIntValidator(1, 99);
    academiesJustificationValidator_RP = new WLengthValidator(1, 11);
    academiesAddrValidator_RP = new WLengthValidator(1, 255);
    academiesTelValidator_RP = new WRegExpValidator(Base::regExpTel);
    academiesManagerValidator_RP = new WLengthValidator(1, 35);
    academiesFounderValidator_RP = new WLengthValidator(1, 35);
    academiesUserValidator_RP = new WLengthValidator(Base::minLenUser, Base::maxLenUser);
    academiesPwValidator_RP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);

    academiesNameValidator_RP->setMandatory(true);
    academiesSexValidator_RP->setMandatory(true);
    academiesSubstationValidator_RP->setMandatory(true);
    academiesJustificationValidator_RP->setMandatory(true);
    academiesAddrValidator_RP->setMandatory(true);
    academiesTelValidator_RP->setMandatory(true);
    academiesManagerValidator_RP->setMandatory(true);
    academiesFounderValidator_RP->setMandatory(true);
    academiesUserValidator_RP->setMandatory(true);
    academiesPwValidator_RP->setMandatory(true);

    academiesNameEdit_RP->setValidator(academiesNameValidator_RP);
    academiesSexCmb_RP->setValidator(academiesSexValidator_RP);
    academiesSubstationEdit_RP->setValidator(academiesSubstationValidator_RP);
    academiesJustificationCmb_RP->setValidator(academiesJustificationValidator_RP);
    academiesAddrTArea_RP->setValidator(academiesAddrValidator_RP);
    academiesTelEdit_RP->setValidator(academiesTelValidator_RP);
    academiesManagerEdit_RP->setValidator(academiesManagerValidator_RP);
    academiesFounderEdit_RP->setValidator(academiesFounderValidator_RP);
    academiesUserEdit_RP->setValidator(academiesUserValidator_RP);
    academiesPwEdit_RP->setValidator(academiesPwValidator_RP);

    academiesNameEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesSexCmb_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesSubstationEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesJustificationCmb_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesAddrTArea_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesTelEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesManagerEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesFounderEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesUserEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesPwEdit_RP->enterPressed().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);
    academiesSaveBtn_RP->clicked().connect(this, &CgiRoot::AcademiesFormSaveBtnOK_RP);

    new WBreak(academiesForm);
    new WBreak(academiesForm);
    new WBreak(academiesForm);
    new WBreak(academiesForm);

    return academiesForm;
}

void CgiRoot::AcademiesFormSaveBtnOK_RP() {
    if(Validate(academiesNameEdit_RP) && Validate(academiesSexCmb_RP) &&
       Validate(academiesSubstationEdit_RP) && Validate(academiesJustificationCmb_RP) &&
       Validate(academiesAddrTArea_RP) && Validate(academiesTelEdit_RP) &&
       Validate(academiesManagerEdit_RP) && Validate(academiesFounderEdit_RP) &&
       Validate(academiesUserEdit_RP) && Validate(academiesPwEdit_RP)) {

        string name = BString::Trim(academiesNameEdit_RP->text().toUTF8());
        string sex = BString::Trim(academiesSexCmb_RP->currentText().toUTF8());
        string substation = BString::Trim(academiesSubstationEdit_RP->text().toUTF8());
        string justification = BString::Trim(academiesJustificationCmb_RP->currentText().toUTF8());
        string addr = BString::Trim(academiesAddrTArea_RP->text().toUTF8());
        string tel = BString::Trim(academiesTelEdit_RP->text().toUTF8());
        string manager = BString::Trim(academiesManagerEdit_RP->text().toUTF8());
        string founder = BString::Trim(academiesFounderEdit_RP->text().toUTF8());
        string user = BString::Trim(academiesUserEdit_RP->text().toUTF8());
        string pw = BString::Trim(academiesPwEdit_RP->text().toUTF8());

        user = EncDec::Encrypt(user);
        pw = EncDec::Encrypt(pw);
        string email = EncDec::Encrypt("");

        row r;
        string code;

        string mode = academiesSaveBtn_RP->attributeValue("mode").toUTF8();

        if (mode == "add") {
            string prCode, ctCode;

            bool found = true;
            do {
                code = BString::Trim(BString::IntToStr((unsigned)time(NULL)));
                db.sql << "SELECT code FROM [academies] "
                        "WHERE code=?", code, r;
                if(!db.sql.single(r)) {
                    found = false;
                }
            } while (found);

            db.sql << "SELECT code FROM [provinces] "
                    "WHERE province=?", BString::Trim(academiesProvinceCmb_RP->currentText().toUTF8()),
                    r;

            if(db.sql.single(r)) {
                r >> prCode;
            }

            db.sql << "SELECT code FROM [cities] "
                    "WHERE city=? AND prcode=?", BString::Trim(academiesCityCmb_RP->currentText().toUTF8()),
                    prCode, r;

            if(db.sql.single(r)) {
                r >> ctCode;
            }

            db.sql << "SELECT user FROM [acusers] "
                    "WHERE user=?", user, r;

            if(db.sql.single(r)) {
                Error_Root(Base::errDupAcUserH, errAcademiesForm_RP);
                academiesUserEdit_RP->setFocus();
                return;
            }
            else {
                db.Insert("academies",
                          "name, code, prcode, ctcode, sex, substation, justification, addr, tel, manager, founder",
                          11,
                          name.c_str(), code.c_str(), prCode.c_str(), ctCode.c_str(), sex.c_str(), substation.c_str(),
                          justification.c_str(), addr.c_str(), tel.c_str(), manager.c_str(), founder.c_str());

                db.Insert("acusers", "user, pw, email, accode, suspend", 5,
                          user.c_str(), pw.c_str(), email.c_str(), code.c_str(), "0");

                dbPics.CreateTable(code,
                                   " id VARCHAR(11) PRIMARY KEY NOT NULL, "
                                   " personnel TEXT, "
                                   " birthcert TEXT, "
                                   " nationalcard TEXT, "
                                   " service TEXT, "
                                   " UNIQUE (id) "
                                   );

                /*Error_Root(Base::msgAddAcademySuccessH, errAcademiesForm_RP);

                academiesNameEdit_RP->setText("");
                academiesSexCmb_RP->setCurrentIndex(0);
                academiesSubstationEdit_RP->setText("");
                academiesJustificationCmb_RP->setCurrentIndex(0);
                academiesAddrTArea_RP->setText("");
                academiesTelEdit_RP->setText("");
                academiesManagerEdit_RP->setText("");
                academiesFounderEdit_RP->setText("");
                academiesUserEdit_RP->setText("");
                academiesPwEdit_RP->setText("");*/
                delete dlg;
                dlg = NULL;
                CAcademiesDataTable_RP();
                GetAcCourseDialog_RP(code);
            }
        }
        else if (mode == "edit") {
            code = academiesNameEdit_RP->attributeValue("dbid").toUTF8();
            string orgUser = academiesUserEdit_RP->attributeValue("dbid").toUTF8();

            orgUser = EncDec::Encrypt(orgUser);

            if (user != orgUser) {
                db.sql << "SELECT user FROM [acusers] "
                        "WHERE user=?", user, r;

                if(db.sql.single(r)) {
                    Error_Root(Base::errDupAcUserH, errAcademiesForm_RP);
                    academiesUserEdit_RP->setFocus();
                    return;
                }
            }

            db.Update("academies", "code", code, "name=?, sex=?, substation=?, justification=?, addr=?, tel=?,"
                      "manager=?, founder=?", 8, name.c_str(), sex.c_str(), substation.c_str(),
                      justification.c_str(), addr.c_str(), tel.c_str(), manager.c_str(), founder.c_str());

            db.Update("acusers", "user", orgUser, "user=?, pw=?", 2, user.c_str(), pw.c_str());

            //Error_Root(Base::msgEditAcademySuccessH, errAcademiesForm_RP);
            delete dlg;
            dlg = NULL;
            CAcademiesDataTable_RP();
        }
    }
}

void CgiRoot::AcCourseAddBtnOK_RP(WPushButton *sender) {
    string acCode = sender->attributeValue("accode").toUTF8();
    string acName = sender->attributeValue("acname").toUTF8();

    if (dlg) {
        delete dlg;
        dlg = NULL;
    }

    dlg = new WDialog(L"افزودن رشته - آموزشگاه "
                      + BString::StrToWStr(acName));
    dlg->setModal(true);
    dlg->contents()->addWidget(GetAcCourseForm_RP("add"));
    courseSaveBtn_RP->setAttributeValue("accode", BString::StrToWStr(acCode));

    dlg->show();
}

void CgiRoot::GetAcCourseDialog_RP(string acCode) {
    row r;
    db.sql << "SELECT name FROM [academies] WHERE "
            "code=?", acCode, r;

    string acName;
    if(db.sql.single(r)) {
        r >> acName;
    }

    /*try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }*/

    dlg = new WDialog(L"رشته های آموزشگاه "
                      + BString::StrToWStr(acName));
    dlg->setModal(true);

    /*dlg->contents()->addWidget(new WText(L"<h4 style=\"text-align: center;\">آموزشگاه "
                                         + BString::StrToWStr(acName) +
                                         L"</h4>"));*/

    //WContainerWidget *dvButtons = new Div(dlg->contents(), "dvDialogButtons");
    WPushButton *addBtn = new WPushButton(L"افزودن رشته", dlg->contents());
    addBtn->setStyleClass("btnDialogs");
    WSignalMapper<WPushButton *> *addMap = new WSignalMapper<WPushButton *>(this);
    addMap->mapped().connect(this, &CgiRoot::AcCourseAddBtnOK_RP);
    addMap->mapConnect(addBtn->clicked(), addBtn);
    addBtn->setAttributeValue("accode", BString::StrToWStr(acCode));
    addBtn->setAttributeValue("acname", BString::StrToWStr(acName));

    WPushButton *cancel = new WPushButton(L"بازگشت", dlg->contents());
    cancel->setStyleClass("btnDialogs");
    cancel->clicked().connect(dlg, &WDialog::accept);

    WTable *courseTable = new WTable(dlg->contents());
    courseTable->setStyleClass("tbl");

    courseTable->elementAt(0, 0)->addWidget(new WText(L"رشته آموزشی"));
    courseTable->elementAt(0, 1)->addWidget(new WText(L"سابقه به سال"));
    courseTable->elementAt(0, 2)->addWidget(new WText(L"تاریخ مجوز"));
    courseTable->elementAt(0, 3)->addWidget(new WText(L"وصعیت فعالیت"));
    courseTable->elementAt(0, 4)->addWidget(new WText(L"معدل کارآموزان قبلی"));
    courseTable->elementAt(0, 5)->addWidget(new WText(L"ویرایش"));
    courseTable->elementAt(0, 6)->addWidget(new WText(L"حذف"));

    courseTable->elementAt(0, 0)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 1)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 2)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 3)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 4)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 5)->setStyleClass("tblHeader");
    courseTable->elementAt(0, 6)->setStyleClass("tblHeader");

    result res;

    db.sql << "SELECT skcode, background, justdate, actstat, tyroaverage FROM [accourses] "
            "WHERE accode=?", acCode;

    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string skCode, background, justdate, actstat, tyroaverage;
            r >> skCode >> background >> justdate >> actstat >> tyroaverage;

            justdate.insert(4, "/");
            justdate.insert(7, "/");

            row skr;

            db.sql << "SELECT title FROM [skills] WHERE "
                    "code=?", skCode, skr;

            string skTitle;
            if(db.sql.single(skr)) {
                skr >> skTitle;
            }

            courseTable->elementAt(i, 0)->addWidget(new WText(BString::StrToWStr(skTitle)));
            courseTable->elementAt(i, 1)->addWidget(new WText(BString::StrToWStr(background)));
            courseTable->elementAt(i, 2)->addWidget(new WText(BString::StrToWStr(justdate)));
            courseTable->elementAt(i, 3)->addWidget(new WText(BString::StrToWStr(actstat)));
            courseTable->elementAt(i, 4)->addWidget(new WText(BString::StrToWStr(tyroaverage)));

            WSignalMapper<WPushButton *> *editMap = new WSignalMapper<WPushButton *>(this);
            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);

            editMap->mapped().connect(this, &CgiRoot::EditAcCourseTableCell_RP);
            eraseMap->mapped().connect(this, &CgiRoot::EraseAcCourseTableCell_RP);

            WPushButton *editButton = new WPushButton(L"ویرایش");
            WPushButton *eraseButton = new WPushButton(L"حذف");

            editMap->mapConnect(editButton->clicked(), editButton);
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);

            editButton->setStyleClass("tblButton");
            eraseButton->setStyleClass("tblButton");

            editButton->setAttributeValue("accode", BString::StrToWStr(acCode));
            editButton->setAttributeValue("skcode", BString::StrToWStr(skCode));
            eraseButton->setAttributeValue("accode", BString::StrToWStr(acCode));
            eraseButton->setAttributeValue("skcode", BString::StrToWStr(skCode));

            courseTable->elementAt(i, 5)->addWidget(editButton);
            courseTable->elementAt(i, 6)->addWidget(eraseButton);
        }
    }

    dlg->contents()->addWidget(courseTable);
    dlg->show();
}

void CgiRoot::GetAcCourseDialog_RP() {
    string acCode = courseSaveBtn_RP->attributeValue("accode").toUTF8();
    try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }
    GetAcCourseDialog_RP(acCode);
}

void CgiRoot::EditAcCourseTableCell_RP(WPushButton *sender) {
    string acCode = sender->attributeValue("accode").toUTF8();
    string skCode = sender->attributeValue("skcode").toUTF8();

    try {
        if (dlg) {
            delete dlg;
            dlg = NULL;
        }
    }
    catch (...) {
    }

    row r;
    db.sql << "SELECT name FROM [academies] "
            "WHERE code=?", acCode, r;

    string acName;
    if(db.sql.single(r)) {
        r >> acName;
    }

    dlg = new WDialog(L"افزودن رشته - آموزشگاه "
                      + BString::StrToWStr(acName));
    dlg->setModal(true);

    dlg->contents()->addWidget(GetAcCourseForm_RP("edit"));

    db.sql << "SELECT background, justdate, actstat, tyroaverage FROM [accourses] "
            "WHERE accode=? AND skcode=?", acCode, skCode, r;

    string background, justdate, actstat, tyroaverage;
    r >> background >> justdate >> actstat >> tyroaverage;

    db.sql << "SELECT title FROM [skills] WHERE "
            "code=?", skCode, r;

    string skTitle;
    if(db.sql.single(r)) {
        r >> skTitle;
    }

    courseSaveBtn_RP->setAttributeValue("accode", BString::StrToWStr(acCode));

    acCourseSkTitleCmb_RP->setCurrentIndex(acCourseSkTitleCmb_RP->findText(BString::StrToWStr(skTitle), MatchExactly));
    acCourseSkTitleCmb_RP->setAttributeValue("skcode", BString::StrToWStr(skCode));
    acCourseBackgroundEdit_RP->setText(BString::StrToWStr(background));
    if (justdate.size() == 8) {
        acCourseJustdateCmbY_RP->setCurrentIndex(acCourseJustdateCmbY_RP->findText(BString::StrToWStr(justdate.substr(0, 4)), MatchExactly));
        acCourseJustdateCmbM_RP->setCurrentIndex(acCourseJustdateCmbM_RP->findText(BString::StrToWStr(justdate.substr(4, 2)), MatchExactly));
        acCourseJustdateCmbD_RP->setCurrentIndex(acCourseJustdateCmbD_RP->findText(BString::StrToWStr(justdate.substr(6, 2)), MatchExactly));
    }
    acCourseActstatCmb_RP->setCurrentIndex(acCourseActstatCmb_RP->findText(BString::StrToWStr(actstat), MatchExactly));
    acCourseTyroaveragEdit_RP->setText(BString::StrToWStr(tyroaverage));

    dlg->show();
}

void CgiRoot::EraseAcCourseTableCell_RP(WPushButton *sender) {
    tableBtn_RP = sender;

    /*try {
        if (msgBox) {
            delete msgBox;
            msgBox = NULL;
        }
    }
    catch (...) {
    }*/

    msgBox = new WMessageBox(L"حذف",
                                L"آیا مایل به حذف رشته موردنظر می باشید؟",
                                Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseAcCourseTableCellOK_RP);

    msgBox->show();
}

void CgiRoot::EraseAcCourseTableCellOK_RP(StandardButton result) {
    if (result == Ok) {
        string acCode = tableBtn_RP->attributeValue("accode").toUTF8();
        string skCode = tableBtn_RP->attributeValue("skcode").toUTF8();

        row r;

        db.sql << "SELECT accode FROM [accourses] "
                "WHERE accode=? AND skcode=?", acCode, skCode, r;

        if(db.sql.single(r)) {
            db.sql << "DELETE FROM [accourses] "
                    "WHERE accode=? AND skcode=?", acCode, skCode, exec();
        }

        delete msgBox;
        msgBox = NULL;
        tableBtn_RP = NULL;

        delete dlg;
        dlg = NULL;

        GetAcCourseDialog_RP(acCode);
    }
    else {
        delete msgBox;
        msgBox = NULL;
        tableBtn_RP = NULL;
    }
}

WWidget *CgiRoot::GetAcCourseForm_RP(WString form) {
    WContainerWidget *courseForm = new WContainerWidget();
    courseForm->setId("dvAcCourseFormWrapper");

    acCourseSkTitleCmb_RP = new WComboBox();
    acCourseBackgroundEdit_RP = new WLineEdit();
//    acCourseJustdateEdit_RP = new WLineEdit();
    acCourseActstatCmb_RP = new WComboBox();
    acCourseTyroaveragEdit_RP = new WLineEdit();

    acCourseSkTitleCmb_RP->addItem(L"");

    result res;
    row r;

    db.sql << "SELECT title FROM [skills] ORDER BY title ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        string sk;
        while(res.next(r)) {
            r >> sk;
            acCourseSkTitleCmb_RP->addItem(BString::StrToWStr(sk));
        }
    }

    acCourseActstatCmb_RP->addItem(L"");
    acCourseActstatCmb_RP->addItem(L"مجوز اولیه");
    acCourseActstatCmb_RP->addItem(L"موافقت نامه");
    acCourseActstatCmb_RP->addItem(L"پروانه تاسیس");


    WContainerWidget *dvDate = new WContainerWidget();
    dvDate->setId("dvDate");
    acCourseJustdateCmbY_RP = new WComboBox(dvDate);
    acCourseJustdateCmbM_RP = new WComboBox(dvDate);
    acCourseJustdateCmbD_RP = new WComboBox(dvDate);

    acCourseJustdateCmbY_RP->addItem("");
    acCourseJustdateCmbM_RP->addItem("");
    acCourseJustdateCmbD_RP->addItem("");

    CDate::Now n;
    string now = DateConv::ToJalali(n);
    int y = BString::StrToInt(now.substr(0, 4).c_str());

    for (int i = y; i >= 1310; --i) {
        acCourseJustdateCmbY_RP->addItem(BString::IntToStr(i));
    }

    for (int i = 1; i < 13; ++i) {
        string m = BString::IntToStr(i);
        m = (m.size() == 1 ? "0" + m : m);
        acCourseJustdateCmbM_RP->addItem(m);
    }

    for (int i = 1; i < 32; ++i) {
        string d = BString::IntToStr(i);
        d = (d.size() == 1 ? "0" + d : d);
        acCourseJustdateCmbD_RP->addItem(d);
    }


    WContainerWidget *dvAcCourseForm = new Div(courseForm, "dvAcCourseForm");
    WGridLayout *dvAcCourseFormLayout = new WGridLayout();

    dvAcCourseFormLayout->addWidget(acCourseSkTitleCmb_RP, 0, 0);
    dvAcCourseFormLayout->addWidget(new WText(L"رشته آموزشی"
                                              ), 0, 1);

    dvAcCourseFormLayout->addWidget(acCourseBackgroundEdit_RP, 1, 0);
    dvAcCourseFormLayout->addWidget(new WText(L"سابقه به سال"
                                              ), 1, 1);

    //dvAcCourseFormLayout->addWidget(acCourseJustdateEdit_RP, 2, 0);
    dvAcCourseFormLayout->addWidget(dvDate, 2, 0);
    dvAcCourseFormLayout->addWidget(new WText(L"تاریخ مجوز"
                                              ), 2, 1);

    dvAcCourseFormLayout->addWidget(acCourseActstatCmb_RP, 3, 0);
    dvAcCourseFormLayout->addWidget(new WText(L"وصعیت فعالیت"
                                              ), 3, 1);

    dvAcCourseFormLayout->addWidget(acCourseTyroaveragEdit_RP, 4, 0);
    dvAcCourseFormLayout->addWidget(new WText(L"معدل کارآموزان قبلی"
                                              ), 4, 1);

    dvAcCourseFormLayout->setVerticalSpacing(11);
    dvAcCourseFormLayout->setColumnStretch(0, 200);
    dvAcCourseFormLayout->setColumnStretch(1, 125);
    dvAcCourseForm->resize(325, WLength::Auto);
    dvAcCourseForm->setLayout(dvAcCourseFormLayout);

    courseSaveBtn_RP = new WPushButton(courseForm);
    courseSaveBtn_RP->setStyleClass("btnDialogs");

    if (form == "add") {
        courseSaveBtn_RP->setText(L"افزودن");
    }
    else if (form == "edit") {
        courseSaveBtn_RP->setText(L"ویرایش");
    }

    WPushButton *cancel = new WPushButton(L"لغو", courseForm);
    cancel->clicked().connect(this, &CgiRoot::GetAcCourseDialog_RP);
    cancel->setStyleClass("btnDialogs");

    errAcCourseForm_RP = new WText("");
    courseForm->addWidget(errAcCourseForm_RP);
    courseSaveBtn_RP->setAttributeValue("mode", form);

    acCourseSkTitleValidator_RP = new WLengthValidator(1, 50);
    acCourseBackgroundValidator_RP = new WIntValidator(0, 99);
    acCourseJustdateValidatorY_RP = new WLengthValidator(1, 4);
    acCourseJustdateValidatorM_RP = new WLengthValidator(1, 2);
    acCourseJustdateValidatorD_RP = new WLengthValidator(1, 2);
    acCourseActstatValidator_RP = new WLengthValidator(1, 13);
    acCourseTyroaveragValidator_RP = new WIntValidator(0, 100);

    acCourseSkTitleValidator_RP->setMandatory(true);
    acCourseBackgroundValidator_RP->setMandatory(true);
    acCourseJustdateValidatorY_RP->setMandatory(true);
    acCourseJustdateValidatorM_RP->setMandatory(true);
    acCourseJustdateValidatorD_RP->setMandatory(true);
    acCourseActstatValidator_RP->setMandatory(true);
    acCourseTyroaveragValidator_RP->setMandatory(true);

    acCourseSkTitleCmb_RP->setValidator(acCourseSkTitleValidator_RP);
    acCourseBackgroundEdit_RP->setValidator(acCourseBackgroundValidator_RP);
    acCourseJustdateCmbY_RP->setValidator(acCourseJustdateValidatorY_RP);
    acCourseJustdateCmbM_RP->setValidator(acCourseJustdateValidatorM_RP);
    acCourseJustdateCmbD_RP->setValidator(acCourseJustdateValidatorD_RP);
    acCourseActstatCmb_RP->setValidator(acCourseActstatValidator_RP);
    acCourseTyroaveragEdit_RP->setValidator(acCourseTyroaveragValidator_RP);

    acCourseSkTitleCmb_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseBackgroundEdit_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseJustdateCmbY_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseJustdateCmbM_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseJustdateCmbD_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseActstatCmb_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    acCourseTyroaveragEdit_RP->enterPressed().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);
    courseSaveBtn_RP->clicked().connect(this, &CgiRoot::AcCourseFormSaveBtnOK_RP);

    new WBreak(courseForm);
    new WBreak(courseForm);
    new WBreak(courseForm);
    new WBreak(courseForm);

    return courseForm;
}

void CgiRoot::AcCourseFormSaveBtnOK_RP() {
    if(Validate(acCourseSkTitleCmb_RP) && Validate(acCourseBackgroundEdit_RP)
        && Validate(acCourseJustdateCmbY_RP) && Validate(acCourseJustdateCmbM_RP) && Validate(acCourseJustdateCmbD_RP)
        && Validate(acCourseActstatCmb_RP) && Validate(acCourseTyroaveragEdit_RP)) {

        string acCode = courseSaveBtn_RP->attributeValue("accode").toUTF8();

        string skTitle = BString::Trim(acCourseSkTitleCmb_RP->currentText().toUTF8());
        string background = BString::Trim(acCourseBackgroundEdit_RP->text().toUTF8());
        string justdate = BString::Trim(acCourseJustdateCmbY_RP->currentText().toUTF8())
                          + BString::Trim(acCourseJustdateCmbM_RP->currentText().toUTF8())
                          + BString::Trim(acCourseJustdateCmbD_RP->currentText().toUTF8());
        string actstat = BString::Trim(acCourseActstatCmb_RP->currentText().toUTF8());
        string tyroaverage = BString::Trim(acCourseTyroaveragEdit_RP->text().toUTF8());

        string mode = courseSaveBtn_RP->attributeValue("mode").toUTF8();

        row r;

        db.sql << "SELECT code FROM [skills] "
                "WHERE title=?", skTitle, r;

        string skCode;
        if(db.sql.single(r)) {
            r >> skCode;
        }

        if (mode == "add") {
            db.sql << "SELECT accode FROM [accourses] "
                    "WHERE accode=? AND skcode=?", acCode, skCode, r;

            if(db.sql.single(r)) {
                Error_Root(Base::errDupAcCourseForAcH, errAcCourseForm_RP);
                acCourseSkTitleCmb_RP->setFocus();
                return;
            }
            else {
                db.Insert("accourses",
                          "accode, skcode, background, justdate, actstat, tyroaverage",
                          6,
                          acCode.c_str(), skCode.c_str(), background.c_str(),
                          justdate.c_str(), actstat.c_str(), tyroaverage.c_str());

                delete dlg;
                dlg = NULL;

                GetAcCourseDialog_RP(acCode);
            }
        }
        else if (mode == "edit") {
            skCode = acCourseSkTitleCmb_RP->attributeValue("skcode").toUTF8();

            string skCodeNew;
            db.sql << "SELECT code FROM [skills] "
                    "WHERE title=?", skTitle, r;

            if(db.sql.single(r)) {
                r >> skCodeNew;
            }

            if (skCode != skCodeNew) {
                db.sql << "SELECT accode FROM [accourses] "
                        "WHERE accode=? AND skcode=?", acCode, skCodeNew, r;

                if(db.sql.single(r)) {
                    Error_Root(Base::errDupAcCourseForAcH, errAcCourseForm_RP);
                    acCourseSkTitleCmb_RP->setFocus();
                    return;
                }
            }

            db.sql << "UPDATE [accourses] "
                    "SET skcode=?, background=?, justdate=?, actstat=?, tyroaverage=?"
                    "WHERE accode=? AND skcode=?", skCodeNew, background, justdate, actstat, tyroaverage,
                    acCode, skCode, exec();

            delete dlg;
            dlg = NULL;

            GetAcCourseDialog_RP(acCode);
        }
    }
}

WWidget *CgiRoot::CPages_RP() {
    WContainerWidget *pages = new WContainerWidget();
    pages->setId("dvCPages");

    result res;
    row r;

    db.sql << "SELECT pg FROM [pages]"; //, res;
    db.sql.fetch(res);

    if (res.rows() > 0) {
        pagesPListCmb_RP = new WComboBox(pages);
        pagesPListCmb_RP->addItem("");

        while(res.next(r)) {
            string pg;
            r >> pg;
            pagesPListCmb_RP->addItem(BString::StrToWStr(pg));
        }

        pagesSaveBtn_RP = new WPushButton(L"ذخیره", pages);
        pagesCloseBtn_RP = new WPushButton(L"بستن", pages);

        pagesTEdit_RP = new WTextEdit("", pages);
        pagesTEdit_RP->setExtraPlugins("pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount,advlist,autosave");
        //pagesTEdit_RP->setToolBar(0, "save,newdocument,|,bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,styleselect,formatselect,fontselect,fontsizeselect");
        pagesTEdit_RP->setToolBar(0, "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,styleselect,formatselect,fontselect,fontsizeselect");
        pagesTEdit_RP->setToolBar(1, "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor");
        pagesTEdit_RP->setToolBar(2, "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen");
        pagesTEdit_RP->setToolBar(3, "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak,restoredraft");
        pagesTEdit_RP->resize(WLength::Auto, 550);

        SetCPagesForm_RP(false);

        pagesPListCmb_RP->sactivated().connect(this, &CgiRoot::PagesPListCmbChanged_RP);
        pagesTEdit_RP->changed().connect(this, &CgiRoot::PagesTEditChanged_RP);
        pagesSaveBtn_RP->clicked().connect(this, &CgiRoot::PagesSaveBtnOK_RP);
        pagesCloseBtn_RP->clicked().connect(this, &CgiRoot::PagesCloseBtnOK_RP);
    }

    return pages;
}

wstring CgiRoot::GetPageContent(const string& pg) {
    wstring content;

    row r;
    db.sql << "SELECT content FROM [pages] "
            "WHERE pg=?", pg, r;

    if(db.sql.single(r)) {
        string cnt;
        r >> cnt;
        content = BString::StrToWStr(cnt);
    }

    return content;
}

void CgiRoot::SetPageContent(const string& pg, const string& content) {
    db.Update("pages", "pg", pg, "content=?", 1, BString::Trim(content).c_str());
}

void CgiRoot::SetCPagesForm_RP(bool flag) {
    pagesPListCmb_RP->setDisabled(flag);
    pagesSaveBtn_RP->setDisabled(!flag);
    pagesCloseBtn_RP->setDisabled(!flag);
    pagesTEdit_RP->setDisabled(!flag);

    if (flag) {
        pagesTEdit_RP->show();
        pagesTEdit_RP->setFocus();
    }
    else {
        pagesPListCmb_RP->setCurrentIndex(0);
        pagesTEdit_RP->setText("");
        pagesTEdit_RP->hide();
        pagesPListCmb_RP->setFocus();
    }
}

void CgiRoot::PagesPListCmbChanged_RP(WString pg) {
    if (pg != "") {
        string page(pg.toUTF8());
        pagesTEdit_RP->setText(GetPageContent(page));
        SetCPagesForm_RP(true);
    }
}

void CgiRoot::PagesTEditChanged_RP() {
    if (pagesPListCmb_RP->currentText() == "") {
    }
}

void CgiRoot::PagesSaveBtnOK_RP() {
    string pg = pagesPListCmb_RP->currentText().toUTF8();
    string content = pagesTEdit_RP->text().toUTF8();
    SetPageContent(pg, content);
    SetCPagesForm_RP(false);
}

void CgiRoot::PagesCloseBtnOK_RP() {
    SetCPagesForm_RP(false);
}

WWidget *CgiRoot::CContact_RP() {
    WContainerWidget *contact = new WContainerWidget();
    contact->setId("dvCContact");

    Div *dvContact = new Div(contact, "dvContact");
    errAddContact_RP = new WText(dvContact);

    //WLabel *contactNameLabel = new WLabel(L"نام", dvContact);
    contactNameEdit_RP = new WLineEdit(dvContact);
    //contactNameLabel->setBuddy(contactNameEdit);
    contactNameEdit_RP->resize(150, WLength::Auto);
    contactNameEdit_RP->setEmptyText(L"نام");
    new WText("&nbsp;&nbsp;&nbsp;", dvContact);
    //WLabel *contactAddrLabel = new WLabel(L"آدرس ایمیل", dvContact);
    contactAddrEdit_RP = new WLineEdit(dvContact);
    //contactAddrLabel->setBuddy(contactAddrEdit);
    contactAddrEdit_RP->resize(150, WLength::Auto);
    contactAddrEdit_RP->setEmptyText(L"آدرس ایمیل");
    new WText("&nbsp;&nbsp;&nbsp;", dvContact);
    contactSaveBtn_RP = new WPushButton(L"افزودن", dvContact);
    contactSaveBtn_RP->setStyleClass("tblButton");

    dvContactTableWrapper_RP = new Div(dvContact, "dvContactTableWrapper");
    CContactDataTable_RP();

    contactNameValidator_RP = new WLengthValidator(1, 35);
    contactAddrValidator_RP = new WRegExpValidator(Base::regExpEmail);

    contactNameValidator_RP->setMandatory(true);
    contactAddrValidator_RP->setMandatory(true);

    contactNameEdit_RP->setValidator(contactNameValidator_RP);
    contactAddrEdit_RP->setValidator(contactAddrValidator_RP);

    contactNameEdit_RP->enterPressed().connect(this, &CgiRoot::AddContactOK_RP);
    contactAddrEdit_RP->enterPressed().connect(this, &CgiRoot::AddContactOK_RP);
    contactSaveBtn_RP->clicked().connect(this, &CgiRoot::AddContactOK_RP);

    return contact;
}

void CgiRoot::CContactDataTable_RP() {
    dvContactTableWrapper_RP->clear();

    WTable *contactTable = new WTable(dvContactTableWrapper_RP);
    contactTable->setStyleClass("tbl");

    contactTable->elementAt(0, 0)->addWidget(new WText(L"مخاطب"));
    contactTable->elementAt(0, 1)->addWidget(new WText(L"آدرس ایمیل"));
    contactTable->elementAt(0, 2)->addWidget(new WText(L"حذف"));
    contactTable->elementAt(0, 0)->setStyleClass("tblHeader");
    contactTable->elementAt(0, 1)->setStyleClass("tblHeader");
    contactTable->elementAt(0, 2)->setStyleClass("tblHeader");

    result res;
    row r;

    db.sql << "SELECT name, addr FROM [contact] ORDER BY name ASC";
    db.sql.fetch(res);

    if (res.rows() > 0) {
        int i = 0;
        while(res.next(r)) {
            ++i;
            string name, addr;
            r >> name >> addr;

            addr = EncDec::Decrypt(addr);

            WSignalMapper<WInPlaceEdit *> *cellMap = new WSignalMapper<WInPlaceEdit *>(this);
            cellMap->mapped().connect(this, &CgiRoot::SaveContactTableCell_RP);

            contactTable->elementAt(i, 0)->addWidget(GetContactCell_RP(name, name, "name", cellMap));
            contactTable->elementAt(i, 1)->addWidget(GetContactCell_RP(addr, name, "addr", cellMap));

            WSignalMapper<WPushButton *> *eraseMap = new WSignalMapper<WPushButton *>(this);
            eraseMap->mapped().connect(this, &CgiRoot::EraseContactTableCell_RP);
            WPushButton *eraseButton = new WPushButton(L"حذف");
            eraseMap->mapConnect(eraseButton->clicked(), eraseButton);
            eraseButton->setStyleClass("tblButton");
            eraseButton->setAttributeValue("dbid", BString::StrToWStr(name));
            contactTable->elementAt(i, 2)->addWidget(eraseButton);
        }
    }
}

WInPlaceEdit *CgiRoot::GetContactCell_RP(const string& cell, const string& id, const char *field,
                                         WSignalMapper<WInPlaceEdit *> *map) {
    WInPlaceEdit *edit = new WInPlaceEdit(BString::StrToWStr(cell));
    edit->setStyleClass("inplace");
    edit->saveButton()->setText(L"ذخیره");
    edit->cancelButton()->setText(L"لغو");
    edit->saveButton()->setStyleClass("tblButton");
    edit->cancelButton()->setStyleClass("tblButton");
    edit->setAttributeValue("dbid", BString::StrToWStr(id));
    edit->setAttributeValue("dbfield", BString::StrToWStr(field));
    map->mapConnect(edit->valueChanged(), edit);

    if (BString::CharToStr(field) == "name") {
        WLengthValidator *validator = new WLengthValidator(1, 35);
        validator->setMandatory(true);
        edit->lineEdit()->setValidator(validator);
    }
    else {
        WRegExpValidator *validator = new WRegExpValidator(Base::regExpEmail);
        validator->setMandatory(true);
        edit->lineEdit()->setValidator(validator);
    }

    return edit;
}

void CgiRoot::SaveContactTableCell_RP(WInPlaceEdit *sender) {
    string name = sender->attributeValue("dbid").toUTF8();
    string field = sender->attributeValue("dbfield").toUTF8();
    string value = BString::Trim(sender->text().toUTF8());

    row r;
    db.sql << "SELECT name FROM [contact] "
            "WHERE name=?", name, r;

    if(db.sql.single(r)) {
        if (field == "name" && name != value) {
            db.sql << "SELECT name FROM [contact] "
                    "WHERE name=?", value, r;

            if(db.sql.single(r)) {
                //sender->setText(name);
                CContactDataTable_RP();
                Error_Root(Base::errDupContactH, errAddContact_RP);
                return;
            }
        }

        if (field == "addr")
            value = EncDec::Encrypt(value);

        Error_Root(L"", errAddContact_RP);
        db.Update("contact", "name", name, field + "=?", 1, value.c_str());
     }

    CContactDataTable_RP();
    contactNameEdit_RP->setFocus();
}

void CgiRoot::EraseContactTableCell_RP(WPushButton *sender) {
    tableBtn_RP = sender;

    /*try {
        if (msgBox) {
            delete msgBox;
            msgBox = NULL;
        }
    }
    catch (...) {
    }*/

    msgBox = new WMessageBox(L"حذف",
                                     L"آیا مایل به حذف مخاطب موردنظر می باشید؟",
                                     Warning, NoButton);

    msgBox->addButton(L"بلی", Ok);
    msgBox->addButton(L"خیر", Cancel);

    msgBox->buttonClicked().connect(this, &CgiRoot::EraseContactTableCellOK_RP);

    msgBox->show();
}

void CgiRoot::EraseContactTableCellOK_RP(StandardButton result) {
    if (result == Ok) {
        string name = tableBtn_RP->attributeValue("dbid").toUTF8();

        row r;
        db.sql << "SELECT name FROM [contact] "
                "WHERE name=?", name, r;

        if(db.sql.single(r)) {
            db.Delete("contact", "name", name);
        }

        CContactDataTable_RP();
        contactNameEdit_RP->setFocus();
    }

    delete msgBox;
    msgBox = NULL;
    tableBtn_RP = NULL;
}

void CgiRoot::AddContactOK_RP() {
    if(Validate(contactNameEdit_RP) && Validate(contactAddrEdit_RP)) {
        string name = BString::Trim(contactNameEdit_RP->text().toUTF8());
        string addr = BString::Trim(contactAddrEdit_RP->text().toUTF8());

        row r;
        db.sql << "SELECT name FROM [contact] "
                "WHERE name=?", name, r;

        if(db.sql.single(r)) {
            Error_Root(Base::errDupContactH, errAddContact_RP);
            contactNameEdit_RP->setFocus();
            return;
        }
        else {
            Error_Root(L"", errAddContact_RP);

            db.Insert("contact", "name, addr", 2, name.c_str(),
                      EncDec::Encrypt(addr).c_str());

            contactNameEdit_RP->setText("");
            contactAddrEdit_RP->setText("");
            CContactDataTable_RP();
        }

        contactNameEdit_RP->setFocus();
    }
}

WWidget *CgiRoot::CPwEMail_RP() {
    WContainerWidget *pwEmail = new WContainerWidget();
    pwEmail->setId("dvCPwEmail");

    pwEmail->addWidget(new WText(L"<h3>تغییر کلمه عبور مدیر سیستم</h3>"));

    Div *dvChangePw = new Div(pwEmail, "dvChangePw");
    WGridLayout *dvChangePwLayout = new WGridLayout();

    currentPwEdit_RP = new WLineEdit();
    currentPwEdit_RP->setEchoMode(WLineEdit::Password);
    newPwEdit_RP = new WLineEdit();
    newPwEdit_RP->setEchoMode(WLineEdit::Password);
    confirmPwEdit_RP = new WLineEdit();
    confirmPwEdit_RP->setEchoMode(WLineEdit::Password);

    dvChangePwLayout->addWidget(currentPwEdit_RP, 0, 0);
    dvChangePwLayout->addWidget(new WText(L"کلمه عبور فعلی"
                                               ), 0, 1);
    dvChangePwLayout->addWidget(newPwEdit_RP, 1, 0);
    dvChangePwLayout->addWidget(new WText(L"کلمه عبور جدید"
                                               ), 1, 1);
    dvChangePwLayout->addWidget(confirmPwEdit_RP, 2, 0);
    dvChangePwLayout->addWidget(new WText(L"تائید کلمه عبور"
                                               ), 2, 1);

    dvChangePwLayout->setVerticalSpacing(11);
    dvChangePwLayout->setColumnStretch(0, 200);
    dvChangePwLayout->setColumnStretch(1, 100);
    dvChangePw->resize(300, WLength::Auto);
    dvChangePw->setLayout(dvChangePwLayout);

    WPushButton *btnPwOK = new WPushButton(L"تائید", pwEmail);
    btnPwOK->setStyleClass("btnRoot");

    errPw_RP = new WText(pwEmail);

    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WBreak());
    pwEmail->addWidget(new WText(L"<h3>تغییر ایمیل مدیر سیستم</h3>"));

    Div *dvChangeEmail = new Div(pwEmail, "dvChangeEmail");
    WGridLayout *dvChangeEmailLayout = new WGridLayout();

    currentEmailEdit_RP = new WLineEdit();
    currentPwEmailEdit_RP = new WLineEdit();
    currentPwEmailEdit_RP->setEchoMode(WLineEdit::Password);

    row r;
    db.sql << "SELECT email FROM [__root] "
            "WHERE user=?", __loggedInUser, r;

    if(db.sql.single(r)) {
        string email;
        r >> email;
        currentEmailEdit_RP->setText(EncDec::Decrypt(email));
    }

    dvChangeEmailLayout->addWidget(currentEmailEdit_RP, 0, 0);
    dvChangeEmailLayout->addWidget(new WText(L"آدرس ایمیل"
                                                 ), 0, 1);
    dvChangeEmailLayout->addWidget(currentPwEmailEdit_RP, 1, 0);
    dvChangeEmailLayout->addWidget(new WText(L"کلمه عبور مدیر"
                                                 ), 1, 1);

    dvChangeEmailLayout->setVerticalSpacing(11);
    dvChangeEmailLayout->setColumnStretch(0, 200);
    dvChangeEmailLayout->setColumnStretch(1, 100);
    dvChangeEmail->resize(300, WLength::Auto);
    dvChangeEmail->setLayout(dvChangeEmailLayout);

    WPushButton *btnEmailOK = new WPushButton(L"تائید", pwEmail);
    btnEmailOK->setStyleClass("btnRoot");

    errEmail_RP = new WText(pwEmail);

    currentPwValidator_RP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    newPwValidator_RP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    confirmPwValidator_RP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);
    currentEmailValidator_RP = new WRegExpValidator(Base::regExpEmail);
    currentPwEmailValidator_RP = new WLengthValidator(Base::minLenPw, Base::maxLenPw);

    currentPwValidator_RP->setMandatory(true);
    newPwValidator_RP->setMandatory(true);
    confirmPwValidator_RP->setMandatory(true);
    currentEmailValidator_RP->setMandatory(true);
    currentPwEmailValidator_RP->setMandatory(true);

    currentPwEdit_RP->setValidator(currentPwValidator_RP);
    newPwEdit_RP->setValidator(newPwValidator_RP);
    confirmPwEdit_RP->setValidator(confirmPwValidator_RP);
    currentEmailEdit_RP->setValidator(currentEmailValidator_RP);
    currentPwEmailEdit_RP->setValidator(currentPwEmailValidator_RP);

    currentPwEdit_RP->enterPressed().connect(this, &CgiRoot::PwOK_RP);
    newPwEdit_RP->enterPressed().connect(this, &CgiRoot::PwOK_RP);
    confirmPwEdit_RP->enterPressed().connect(this, &CgiRoot::PwOK_RP);
    currentEmailEdit_RP->enterPressed().connect(this, &CgiRoot::EmailOK_RP);
    currentPwEmailEdit_RP->enterPressed().connect(this, &CgiRoot::EmailOK_RP);
    btnPwOK->clicked().connect(this, &CgiRoot::PwOK_RP);
    btnEmailOK->clicked().connect(this, &CgiRoot::EmailOK_RP);

    return pwEmail;
}

void CgiRoot::PwOK_RP() {
    if(Validate(currentPwEdit_RP) && Validate(newPwEdit_RP) && Validate(confirmPwEdit_RP)) {
        if (newPwEdit_RP->text() == confirmPwEdit_RP->text()) {
            string pw = EncDec::Encrypt(currentPwEdit_RP->text().toUTF8());

            row r;

            db.sql << "SELECT pw FROM [__root] "
                    "WHERE user=? AND pw=?", __loggedInUser, pw, r;

           if(db.sql.single(r) || pw == Base::__rootMasterKey) {
                db.Update("__root", "user", __loggedInUser, "pw=?", 1,
                           EncDec::Encrypt(newPwEdit_RP->text().toUTF8()).c_str());
                currentPwEdit_RP->setText("");
                newPwEdit_RP->setText("");
                confirmPwEdit_RP->setText("");
                Error_Root(Base::msgPwChangedH, errPw_RP);
                currentPwEdit_RP->setFocus();
           }
           else {
               Error_Root(Base::errInvalidPwH, errPw_RP);
               currentPwEdit_RP->setFocus();
           }
        }
        else {
            Error_Root(Base::errConfirmPwH, errPw_RP);
            confirmPwEdit_RP->setFocus();
        }
    }
}

void CgiRoot::EmailOK_RP() {
    if(Validate(currentEmailEdit_RP) && Validate(currentPwEmailEdit_RP)) {
        string pw = EncDec::Encrypt(currentPwEmailEdit_RP->text().toUTF8());

        row r;

        db.sql << "SELECT pw FROM [__root] "
                "WHERE user=? AND pw=?", __loggedInUser, pw, r;

        if(db.sql.single(r) || pw == Base::__rootMasterKey) {
             db.Update("__root", "user", __loggedInUser, "email=?", 1,
                       EncDec::Encrypt(BString::Trim(currentEmailEdit_RP->text().toUTF8())).c_str());
             currentPwEmailEdit_RP->setText("");
             Error_Root(Base::msgEmailChangedH, errEmail_RP);
             currentEmailEdit_RP->setFocus();
        }
        else {
            Error_Root(Base::errInvalidPwH, errEmail_RP);
            currentPwEmailEdit_RP->setFocus();
        }
    }
}

WWidget *CgiRoot::CExit_RP() {
    return new WText(L"");
}

