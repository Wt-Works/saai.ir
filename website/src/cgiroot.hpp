#ifndef CGIROOT_HPP
#define CGIROOT_HPP


#include <string>

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WButtonGroup>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WEnvironment>
#include <Wt/WFileUpload>
#include <Wt/WFormWidget>
#include <Wt/WImage>
#include <Wt/WInPlaceEdit>
#include <Wt/WIntValidator>
#include <Wt/WLineEdit>
#include <Wt/WLengthValidator>
#include <Wt/WMenuItem>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WSignalMapper>
#include <Wt/WString>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTextEdit>
#include <Wt/WWidget>

#include "clientinfo.hpp"
#include "db.hpp"
#include "serverinfo.hpp"

namespace SAAIIR {
    class CgiRoot;
}

class SAAIIR::CgiRoot : public Wt::WApplication {
public:
    CgiRoot(const Wt::WEnvironment& env);
    virtual ~CgiRoot();

private:
    ClientInfo *clientInfo;
    ServerInfo *serverInfo;


    std::string __loggedInUser;
    std::string __lastIP;
    std::string __lastCCode;
    std::string __lastCName;
    std::string __lastDate;
    std::string __lastTime;

    std::string __acLoggedInUser;
    std::string __acLoggedInCode;
    std::string __acLastIP;
    std::string __acLastCCode;
    std::string __acLastCName;
    std::string __acLastDate;
    std::string __acLastTime;

    int __capResult;
    std::string __capImage;
    void GenCap();


    void InitEnv(const Wt::WEnvironment& env);
    bool IsReqRoot(const Wt::WEnvironment& env);
    std::string IsReqACP(const Wt::WEnvironment& env);

    DB db;
    DB dbPics;

    bool Validate(Wt::WFormWidget *widget);
    bool ValidatePic(const std::string& file);
    void ValidatePicClose(Wt::WDialog *sender);
    bool ValidatePicFlag;
    std::string ValidatePicPostAction;
    std::string ValidatePicPostArgs;

    /************* Home *************/

    Wt::WWidget *Initialize_Home();
    Wt::WContainerWidget *dvMainWrapper;
    Wt::WContainerWidget *dvRegister;

    Wt::WWidget *CMainMenu();
    Wt::WWidget *CHowTo();
    Wt::WWidget *CLinks();
    Wt::WWidget *CContact();
    Wt::WWidget *CAbout();

    Wt::WMenu *mainMenu;

    Wt::WContainerWidget *dvMainVote;

    Wt::WLineEdit *followEdit;
    Wt::WLineEdit *followCapEdit;
    Wt::WLineEdit *academyUserEdit;
    Wt::WLineEdit *academyPwEdit;
    Wt::WLineEdit *academyCapEdit;
    Wt::WLineEdit *voteCapEdit;
    Wt::WLineEdit *regTyroCapEdit;

    Wt::WImage *followCaptcha;
    Wt::WImage *academyCaptcha;
    Wt::WImage *voteCaptcha;
    Wt::WImage *regTyroCaptcha;

    Wt::WIntValidator *followCapValidator;
    Wt::WIntValidator *academyCapValidator;
    Wt::WIntValidator *voteCapValidator;
    Wt::WIntValidator *regTyroCapValidator;

    Wt::WText *errFollowing;
    Wt::WText *errACLogin;
    Wt::WText *errVote;
    Wt::WButtonGroup *dvMainVoteRadioGroup;

    bool IsForgetFormShownAcademy;
    Wt::WContainerWidget *dvForgetFormWrapperAcademy;
    Wt::WText *errForgetAcademy;
    Wt::WLineEdit *forgetEmailEditAcademy;
    Wt::WLineEdit *forgetCapEditAcademy;
    Wt::WIntValidator *forgetCapValidatorAcademy;
    Wt::WRegExpValidator *forgetEmailValidatorAcademy;
    void ForgetOKAcademy();

    void ReGenCap();

    void AcademyForgetForm();

    void FollowingOK();
    void AcademiesOK();
    void VoteOK();

    void VoteChart();

    Wt::WText *errContactForm;
    Wt::WComboBox *contactToCmb;
    Wt::WLineEdit *contactFromEdit;
    Wt::WLineEdit *contactEmailEdit;
    Wt::WLineEdit *contactUrlEdit;
    Wt::WLineEdit *contactSubjectEdit;
    Wt::WTextArea *contactBodyTArea;
    Wt::WPushButton *contactSendBtn;
    Wt::WPushButton *contactClearBtn;

    Wt::WLineEdit *contactCapEdit;
    Wt::WImage *contactCaptcha;

    Wt::WLengthValidator *contactFromValidator;
    Wt::WRegExpValidator *contactEmailValidator;
    Wt::WRegExpValidator *contactUrlValidator;
    Wt::WLengthValidator *contactSubjectValidator;
    Wt::WLengthValidator *contactBodyValidator;
    Wt::WIntValidator *contactCapValidator;

    void SetCContactForm(bool flag);
    void ContactToCmbChanged(Wt::WString to);
    void ContactClearOK_RP();
    void SendMessageOK();

    Wt::WDialog *dlg;
    Wt::WMessageBox *msgBox;

    Wt::WComboBox *regProvinceCmb;
    Wt::WComboBox *regCityCmb;
    Wt::WLineEdit *regTermTitleEdit;
    Wt::WComboBox *regTermBegdateCmbY;
    Wt::WComboBox *regTermBegdateCmbM;
    Wt::WComboBox *regTermBegdateCmbD;
    Wt::WComboBox *regTermEnddateCmbY;
    Wt::WComboBox *regTermEnddateCmbM;
    Wt::WComboBox *regTermEnddateCmbD;
    Wt::WComboBox *regTermDaypartCmb;


    Wt::WLineEdit *tyroFNameEdit;
    Wt::WLineEdit *tyroLNameEdit;
    Wt::WComboBox *tyroSexCmb ;
    Wt::WLineEdit *tyroFatherSNameEdit;
    Wt::WLineEdit *tyroNationalCodeEdit;
    Wt::WLineEdit *tyroBirthIdEdit;
    Wt::WComboBox *tyroBirthDateCmbY;
    Wt::WComboBox *tyroBirthDateCmbM;
    Wt::WComboBox *tyroBirthDateCmbD;
    Wt::WComboBox *tyroBirthlocCmb;
    Wt::WComboBox *tyroBirthEmissionLocCmb;
    Wt::WComboBox *tyroGraduateCertCmb;
    Wt::WLineEdit *tyroGraduateCourseEdit;
    Wt::WComboBox *tyroJobCmb;
    Wt::WTextArea *tyroAddrTArea;
    Wt::WLineEdit *tyroTelEdit;
    Wt::WLineEdit *tyroMobileEdit;
    Wt::WLineEdit *tyroEmailEdit;


    Wt::WLengthValidator *tyroFNameValidator;
    Wt::WLengthValidator *tyroLNameValidator;
    Wt::WLengthValidator *tyroSexValidator;
    Wt::WLengthValidator *tyroFatherSNameValidator;
    Wt::WRegExpValidator *tyroNationalCodeValidator;
    Wt::WRegExpValidator *tyroBirthIdValidator;
    Wt::WLengthValidator *tyroBirthDateYValidator;
    Wt::WLengthValidator *tyroBirthDateMValidator;
    Wt::WLengthValidator *tyroBirthDateDValidator;
    Wt::WLengthValidator *tyroBirthlocValidator;
    Wt::WLengthValidator *tyroBirthEmissionLocValidator;
    Wt::WLengthValidator *tyroGraduateCertValidator;
    Wt::WLengthValidator *tyroGraduateCourseValidator;
    Wt::WLengthValidator *tyroJobValidator;
    Wt::WLengthValidator *tyroAddrValidator;
    Wt::WRegExpValidator *tyroTelValidator;
    Wt::WRegExpValidator *tyroMobileValidator;
    Wt::WRegExpValidator *tyroEmailValidator;


    Wt::WText *errTyroForm;

    Wt::WFileUpload *tyroPicBirthCertFUP;
    Wt::WFileUpload *tyroPicNationalCardFUP;
    Wt::WFileUpload *tyroPicPersonnelFUP;
    Wt::WFileUpload *tyroPicServiceFUP;

    Wt::WPushButton *tyroPicBirthCertBtn;
    Wt::WPushButton *tyroPicNationalCardBtn;
    Wt::WPushButton *tyroPicPersonnelBtn;
    Wt::WPushButton *tyroPicServiceBtn;

    Wt::WPushButton *tyroRegFinishBtn;

    std::string regTermAcCode,
    regTermActCode,
    regTermStTitle,
    regTermBegdate,
    regTermEnddate,
    regTermDaypart,
    regTermBegtime,
    regTermEndtime,
    regTermLoc,
    regTermAcmName;

    std::string regTyroFName,
    regTyroLName,
    regTyroSex,
    regTyroFatherSName,
    regTyroNationalCode,
    regTyroBirthId,
    regTyroBirthDateY,
    regTyroBirthDateM,
    regTyroBirthDateD,
    regTyroBirthloc,
    regTyroBirthEmissionLoc,
    regTyroGraduateCert,
    regTyroGraduateCourse,
    regTyroJob,
    regTyroAddr,
    regTyroTel,
    regTyroMobile,
    regTyroEmail;

    void RegProvinceCmbChanged(Wt::WString pr);
    void ReturnHome();
    void RegStep_2();
    void RegStep_3(Wt::WPushButton *sender);
    void RegStep_4();
    void RegStep_5();
    void RegStep_5_1();
    void RegStep_5_2();
    void RegStep_5_3();
    void RegStep_5_4();
    void RegStep_6();

    /************* End Home *************/



    /************* Root Login *************/

    Wt::WContainerWidget *dvForgetFormWrapper_Root;
    bool IsForgetFormShown_Root;

    Wt::WImage *loginCaptcha_Root;
    Wt::WIntValidator *loginCapValidator_Root;
    Wt::WIntValidator *forgetCapValidator_Root;
    Wt::WLengthValidator *loginUserValidator_Root;
    Wt::WLengthValidator *loginPwValidator_Root;
    Wt::WRegExpValidator *forgetEmailValidator_Root;

    Wt::WText *errLogin_Root;
    Wt::WText *errForget_Root;
    void Error_Root(const std::wstring& err, Wt::WText *txt);

    Wt::WLineEdit *loginUserEdit_Root;
    Wt::WLineEdit *loginPwEdit_Root;
    Wt::WLineEdit *loginCapEdit_Root;
    Wt::WLineEdit *forgetEmailEdit_Root;
    Wt::WLineEdit *forgetCapEdit_Root;

    Wt::WWidget *Initialize_Root();
    void InitializeTables_Root();
    Wt::WWidget *RootLoginForm();
    Wt::WWidget *RootRegisterForm();
    void RootForgetForm();

    void ReGenCap_Root();
    void LoginOK_Root();
    void ForgetOK_Root();


    /************* End Root Login *************/



    /************* Root Panel *************/

    Wt::WMenu *mainMenu_RP;

    void ExitRootPanel(Wt::WMenuItem *mItem);

    /*Wt::WWidget *CHome_RP();
    Wt::WWidget *CBase_RP();*/
    Wt::WWidget *CAcademies_RP();
    Wt::WWidget *CPages_RP();
    Wt::WWidget *CContact_RP();
    Wt::WWidget *CPwEMail_RP();
    Wt::WWidget *CExit_RP();

    void Initialize_RP();

    Wt::WComboBox *pagesPListCmb_RP;
    Wt::WTextEdit *pagesTEdit_RP;
    Wt::WPushButton *pagesSaveBtn_RP;
    Wt::WPushButton *pagesCloseBtn_RP;

    std::wstring GetPageContent(const std::string& pg);
    void SetPageContent(const std::string& pg, const std::string& content);

    void SetCPagesForm_RP(bool flag);

    void PagesPListCmbChanged_RP(Wt::WString pg);
    void PagesTEditChanged_RP();
    void PagesSaveBtnOK_RP();
    void PagesCloseBtnOK_RP();

    Wt::WLineEdit *currentPwEdit_RP;
    Wt::WLineEdit *newPwEdit_RP;
    Wt::WLineEdit *confirmPwEdit_RP;
    Wt::WLineEdit *currentEmailEdit_RP;
    Wt::WLineEdit *currentPwEmailEdit_RP;

    Wt::WLengthValidator *currentPwValidator_RP;
    Wt::WLengthValidator *newPwValidator_RP;
    Wt::WLengthValidator *confirmPwValidator_RP;
    Wt::WRegExpValidator *currentEmailValidator_RP;
    Wt::WLengthValidator *currentPwEmailValidator_RP;

    Wt::WText *errPw_RP;
    Wt::WText *errEmail_RP;

    void PwOK_RP();
    void EmailOK_RP();

    Wt::WText *errAddContact_RP;
    Wt::WLengthValidator *contactNameValidator_RP;
    Wt::WRegExpValidator *contactAddrValidator_RP;

    Wt::WLineEdit *contactNameEdit_RP;
    Wt::WLineEdit *contactAddrEdit_RP;
    Wt::WPushButton *contactSaveBtn_RP;

    Wt::WContainerWidget *dvContactTableWrapper_RP;
    Wt::WInPlaceEdit *GetContactCell_RP(const std::string& cell, const std::string& id, const char *field,
                                        Wt::WSignalMapper<Wt::WInPlaceEdit *> *map);

    Wt::WPushButton *tableBtn_RP;

    void CContactDataTable_RP();
    void SaveContactTableCell_RP(Wt::WInPlaceEdit *sender);
    void EraseContactTableCell_RP(Wt::WPushButton *sender);
    void EraseContactTableCellOK_RP(Wt::StandardButton result);
    void AddContactOK_RP();

    Wt::WComboBox *academiesProvinceCmb_RP;
    Wt::WComboBox *academiesCityCmb_RP;

    void SetCAcademiesForm_RP(int flag);

    void AcademiesProvinceCmbChanged_RP(Wt::WString pr);
    void AcademiesCityCmbChanged_RP(Wt::WString pr);
    void AcademiesAddBtnOK_RP();
    //void AcademiesReturnBtnOK_RP();

    Wt::WContainerWidget *dvAcademiesWrapper_RP;
    Wt::WPushButton *academiesAddBtn_RP;
    //Wt::WPushButton *academiesReturnBtn_RP;

    void CAcademiesDataTable_RP();
    void CourseAcademiesTableCell_RP(Wt::WPushButton *sender);
    void MoreInfoAcademiesTableCell_RP(Wt::WAnchor *sender);
    void MoreInfoAcademiesTableCell_RP(Wt::WPushButton *sender);
    //void CpAcademiesTableCell_RP(Wt::WAnchor *sender);
    void SuspendAcademiesTableCell_RP(Wt::WPushButton *sender);
    void SuspendAcademiesTableCellOK_RP(Wt::StandardButton result);
    void EditAcademiesTableCell_RP(Wt::WPushButton *sender);
    void EraseAcademiesTableCell_RP(Wt::WPushButton *sender);
    void EraseAcademiesTableCellOK_RP(Wt::StandardButton result);

    Wt::WLineEdit *academiesNameEdit_RP;
    Wt::WComboBox *academiesSexCmb_RP;
    Wt::WLineEdit *academiesSubstationEdit_RP;
    Wt::WComboBox *academiesJustificationCmb_RP;
    Wt::WTextArea *academiesAddrTArea_RP;
    Wt::WLineEdit *academiesTelEdit_RP;
    Wt::WLineEdit *academiesManagerEdit_RP;
    Wt::WLineEdit *academiesFounderEdit_RP;
    Wt::WLineEdit *academiesUserEdit_RP;
    Wt::WLineEdit *academiesPwEdit_RP;
    Wt::WPushButton *academiesSaveBtn_RP;

    /*Wt::WPushButton *eraseAcademiesBtn_RP;
    Wt::WPushButton *suspendAcademiesBtn_RP;*/

    Wt::WLengthValidator *academiesNameValidator_RP;
    Wt::WLengthValidator *academiesSexValidator_RP;
    Wt::WIntValidator *academiesSubstationValidator_RP;
    Wt::WLengthValidator *academiesJustificationValidator_RP;
    Wt::WLengthValidator *academiesAddrValidator_RP;
    Wt::WRegExpValidator *academiesTelValidator_RP;
    Wt::WLengthValidator *academiesManagerValidator_RP;
    Wt::WLengthValidator *academiesFounderValidator_RP;
    Wt::WLengthValidator *academiesUserValidator_RP;
    Wt::WLengthValidator *academiesPwValidator_RP;

    Wt::WText *errAcademiesForm_RP;

    Wt::WWidget *GetAcademiesForm_RP(Wt::WString form);

    void AcademiesFormSaveBtnOK_RP();

    void AcCourseAddBtnOK_RP(Wt::WPushButton *sender);

    void GetAcCourseDialog_RP(std::string acCode);
    void GetAcCourseDialog_RP();
    void EditAcCourseTableCell_RP(Wt::WPushButton *sender);
    void EraseAcCourseTableCell_RP(Wt::WPushButton *sender);
    void EraseAcCourseTableCellOK_RP(Wt::StandardButton result);

    Wt::WComboBox *acCourseSkTitleCmb_RP;
    Wt::WLineEdit *acCourseBackgroundEdit_RP;
    Wt::WComboBox *acCourseJustdateCmbY_RP;
    Wt::WComboBox *acCourseJustdateCmbM_RP;
    Wt::WComboBox *acCourseJustdateCmbD_RP;
    Wt::WComboBox *acCourseActstatCmb_RP;
    Wt::WLineEdit *acCourseTyroaveragEdit_RP;
    Wt::WPushButton *courseSaveBtn_RP;

    Wt::WPushButton *eraseAcCourseBtn_RP;
    Wt::WPushButton *suspendAcCourseBtn_RP;

    Wt::WLengthValidator *acCourseSkTitleValidator_RP;
    Wt::WIntValidator *acCourseBackgroundValidator_RP;
    Wt::WLengthValidator *acCourseJustdateValidatorY_RP;
    Wt::WLengthValidator *acCourseJustdateValidatorM_RP;
    Wt::WLengthValidator *acCourseJustdateValidatorD_RP;
    Wt::WLengthValidator *acCourseActstatValidator_RP;
    Wt::WIntValidator *acCourseTyroaveragValidator_RP;

    Wt::WText *errAcCourseForm_RP;

    Wt::WWidget *GetAcCourseForm_RP(Wt::WString form);

    void AcCourseFormSaveBtnOK_RP();


    /************* End Root Panel *************/


    /************* Academy Panel *************/

    void Initialize_ACP();
    void ClearLoginForm_ACP();

    Wt::WContainerWidget *academyPage;

    Wt::WWidget *FirstLogin_ACP();

    Wt::WText *errConfirmFL_ACP;
    Wt::WLineEdit *newEmailEditFL_ACP;
    Wt::WLineEdit *confirmEmailEditFL_ACP;
    Wt::WLineEdit *newPwEditFL_ACP;
    Wt::WLineEdit *confirmPwEditFL_ACP;
    Wt::WLineEdit *currentPwEditFL_ACP;
    Wt::WPushButton *btnConfirmOK;

    Wt::WRegExpValidator *newEmailValidatorFL_ACP;
    Wt::WRegExpValidator *confirmEmailValidatorFL_ACP;
    Wt::WLengthValidator *newPwValidatorFL_ACP;
    Wt::WLengthValidator *confirmPwValidatorFL_ACP;
    Wt::WLengthValidator *currentPwValidatorFL_ACP;

    void ConfirmOKFL_ACP();
    std::string AcSessionGen(const std::string& acCode, bool isRoot);
    bool AcSessionValidate(const std::string& session);
    void Go_ACPanel(const std::string& acCode);
    void GoAway_ACP();
    void Initialize_ACPanel();

    Wt::WMenu *mainMenu_ACP;
    void ExitACPanel(Wt::WMenuItem *mItem);
    Wt::WWidget *CMentors_ACP();
    Wt::WWidget *CTyros_ACP();
    Wt::WWidget *CTerms_ACP();
    Wt::WWidget *CPwEMail_ACP();
    Wt::WWidget *CExit_ACP();

    Wt::WLineEdit *currentPwEdit_ACP;
    Wt::WLineEdit *newPwEdit_ACP;
    Wt::WLineEdit *confirmPwEdit_ACP;
    Wt::WLineEdit *currentEmailEdit_ACP;
    Wt::WLineEdit *currentPwEmailEdit_ACP;

    Wt::WLengthValidator *currentPwValidator_ACP;
    Wt::WLengthValidator *newPwValidator_ACP;
    Wt::WLengthValidator *confirmPwValidator_ACP;
    Wt::WRegExpValidator *currentEmailValidator_ACP;
    Wt::WLengthValidator *currentPwEmailValidator_ACP;

    Wt::WText *errPw_ACP;
    Wt::WText *errEmail_ACP;

    void PwOK_ACP();
    void EmailOK_ACP();

    void MentorsAddBtnOK_ACP();

    Wt::WContainerWidget *dvMentorsWrapper_ACP;

    void CMentorsDataTable_ACP();
    void MoreInfoMentorsTableCell_ACP(Wt::WAnchor *sender);
    void MoreInfoMentorsTableCell_ACP(Wt::WPushButton *sender);
    void CardMentorsTableCell_ACP(Wt::WPushButton *sender);
    void EditMentorsTableCell_ACP(Wt::WPushButton *sender);
    void EraseMentorsTableCell_ACP(Wt::WPushButton *sender);
    void EraseMentorsTableCellOK_ACP(Wt::StandardButton result);

    Wt::WLineEdit *mentorsFNameEdit_ACP;
    Wt::WLineEdit *mentorsLNameEdit_ACP;
    Wt::WComboBox *mentorsSexCmb_ACP;
    Wt::WLineEdit *mentorsNationalCodeEdit_ACP;
    Wt::WLineEdit *mentorsBirthIdEdit_ACP;
    Wt::WComboBox *mentorsBirthDateCmbY_ACP;
    Wt::WComboBox *mentorsBirthDateCmbM_ACP;
    Wt::WComboBox *mentorsBirthDateCmbD_ACP;
    Wt::WComboBox *mentorsBirthlocCmb_ACP;
    Wt::WComboBox *mentorsBirthEmissionLocCmb_ACP;
    Wt::WComboBox *mentorsGraduateCertCmb_ACP;
    Wt::WLineEdit *mentorsGraduateCourseEdit_ACP;
    Wt::WTextArea *mentorsAddrTArea_ACP;
    Wt::WLineEdit *mentorsTelEdit_ACP;
    Wt::WLineEdit *mentorsMobileEdit_ACP;
    Wt::WLineEdit *mentorsEmailEdit_ACP;
    Wt::WFileUpload *mentorsPicFUP_ACP;
    Wt::WPushButton *mentorsSaveBtn_ACP;

    Wt::WLengthValidator *mentorsFNameValidator_ACP;
    Wt::WLengthValidator *mentorsLNameValidator_ACP;
    Wt::WLengthValidator *mentorsSexValidator_ACP;
    Wt::WRegExpValidator *mentorsNationalCodeValidator_ACP;
    Wt::WRegExpValidator *mentorsBirthIdValidator_ACP;
    Wt::WLengthValidator *mentorsBirthDateYValidator_ACP;
    Wt::WLengthValidator *mentorsBirthDateMValidator_ACP;
    Wt::WLengthValidator *mentorsBirthDateDValidator_ACP;
    Wt::WLengthValidator *mentorsBirthlocValidator_ACP;
    Wt::WLengthValidator *mentorsBirthEmissionLocValidator_ACP;
    Wt::WLengthValidator *mentorsGraduateCertValidator_ACP;
    Wt::WLengthValidator *mentorsGraduateCourseValidator_ACP;
    Wt::WLengthValidator *mentorsAddrValidator_ACP;
    Wt::WRegExpValidator *mentorsTelValidator_ACP;
    Wt::WRegExpValidator *mentorsMobileValidator_ACP;
    Wt::WRegExpValidator *mentorsEmailValidator_ACP;

    Wt::WText *errMentorsForm_ACP;

    Wt::WWidget *GetMentorsForm_ACP(Wt::WString form);

    void MentorsFormFileUploaded_ACP();
    void MentorsFormFileTooLarge_ACP();
    void MentorsFormSaveBtnOK_ACP();

    void MCardAddBtnOK_ACP(Wt::WPushButton *sender);

    void GetMCardDialog_ACP(std::string acmCode);
    void GetMCardDialog_ACP();
    void EditMCardTableCell_ACP(Wt::WPushButton *sender);
    void EraseMCardTableCell_ACP(Wt::WPushButton *sender);
    void EraseMCardTableCellOK_ACP(Wt::StandardButton result);

    Wt::WComboBox *mCardStTitleCmb_ACP;
    Wt::WComboBox *mCardDateCmbY_ACP;
    Wt::WComboBox *mCardDateCmbM_ACP;
    Wt::WComboBox *mCardDateCmbD_ACP;
    Wt::WLineEdit *mCardPercentEdit_ACP;
    Wt::WPushButton *cardSaveBtn_ACP;

    Wt::WPushButton *eraseCardBtn_ACP;
    Wt::WPushButton *suspendCardBtn_ACP;

    Wt::WLengthValidator *mCardStTitleValidator_ACP;
    Wt::WLengthValidator *mCardDateValidatorY_ACP;
    Wt::WLengthValidator *mCardDateValidatorM_ACP;
    Wt::WLengthValidator *mCardDateValidatorD_ACP;
    Wt::WIntValidator *mCardPercentValidator_ACP;

    Wt::WText *errMCardForm_ACP;

    Wt::WWidget *GetMCardForm_ACP(Wt::WString form);

    void MCardFormSaveBtnOK_ACP();

    void TermsAddBtnOK_ACP();

    Wt::WContainerWidget *dvTermsWrapper_ACP;

    void CTermsDataTable_ACP();
    void NoticeTermsTableCell_ACP(Wt::WPushButton *sender);
    void EditTermsTableCell_ACP(Wt::WPushButton *sender);
    void EraseTermsTableCell_ACP(Wt::WPushButton *sender);
    void EraseTermsTableCellOK_ACP(Wt::StandardButton result);

    Wt::WComboBox *termsStTitleCmb_ACP;
    Wt::WComboBox *termsBegdateCmbY_ACP;
    Wt::WComboBox *termsBegdateCmbM_ACP;
    Wt::WComboBox *termsBegdateCmbD_ACP;
    Wt::WComboBox *termsEnddateCmbY_ACP;
    Wt::WComboBox *termsEnddateCmbM_ACP;
    Wt::WComboBox *termsEnddateCmbD_ACP;
    Wt::WComboBox *termsDaypartCmb_ACP;
    Wt::WComboBox *termsBegtimeCmbH_ACP;
    Wt::WComboBox *termsBegtimeCmbM_ACP;
    Wt::WComboBox *termsEndtimeCmbH_ACP;
    Wt::WComboBox *termsEndtimeCmbM_ACP;
    Wt::WComboBox *termsLocCmb_ACP;
    Wt::WComboBox *termsMentorCmb_ACP;
    Wt::WPushButton *termsSaveBtn_ACP;

    Wt::WLengthValidator *termsStTitleValidator_ACP;
    Wt::WLengthValidator *termsBegdateValidatorY_ACP;
    Wt::WLengthValidator *termsBegdateValidatorM_ACP;
    Wt::WLengthValidator *termsBegdateValidatorD_ACP;
    Wt::WLengthValidator *termsEnddateValidatorY_ACP;
    Wt::WLengthValidator *termsEnddateValidatorM_ACP;
    Wt::WLengthValidator *termsEnddateValidatorD_ACP;
    Wt::WLengthValidator *termsDaypartValidator_ACP;
    Wt::WLengthValidator *termsBegtimeValidatorH_ACP;
    Wt::WLengthValidator *termsBegtimeValidatorM_ACP;
    Wt::WLengthValidator *termsEndtimeValidatorH_ACP;
    Wt::WLengthValidator *termsEndtimeValidatorM_ACP;
    Wt::WLengthValidator *termsLocValidator_ACP;
    Wt::WLengthValidator *termsMentorValidator_ACP;

    Wt::WText *errTermsForm_ACP;

    Wt::WWidget *GetTermsForm_ACP(Wt::WString form);

    void TermsFormSaveBtnOK_ACP();

    Wt::WContainerWidget *dvTyrosWrapper_ACP;

    void CTyrosDataTable_ACP();
    void MoreInfoTyrosTableCell_ACP(Wt::WPushButton *sender);
    void ShowTyroPic_ACP(Wt::WAnchor *sender);
    void EditPicsTyrosTableCell_ACP(Wt::WPushButton *sender);
    void EditPicsTyrosTableCellOK_ACP(Wt::WFileUpload *sender);
    void EditTyrosTableCell_ACP(Wt::WPushButton *sender);
    void EraseTyrosTableCell_ACP(Wt::WPushButton *sender);
    void EraseTyrosTableCellOK_ACP(Wt::StandardButton result);

    Wt::WLineEdit *tyrosFNameEdit_ACP;
    Wt::WLineEdit *tyrosLNameEdit_ACP;
    Wt::WComboBox *tyrosSexCmb_ACP;
    Wt::WLineEdit *tyrosFatherSNameEdit_ACP;
    Wt::WLineEdit *tyrosNationalCodeEdit_ACP;
    Wt::WLineEdit *tyrosBirthIdEdit_ACP;
    Wt::WComboBox *tyrosBirthDateCmbY_ACP;
    Wt::WComboBox *tyrosBirthDateCmbM_ACP;
    Wt::WComboBox *tyrosBirthDateCmbD_ACP;
    Wt::WComboBox *tyrosBirthlocCmb_ACP;
    Wt::WComboBox *tyrosBirthEmissionLocCmb_ACP;
    Wt::WComboBox *tyrosGraduateCertCmb_ACP;
    Wt::WLineEdit *tyrosGraduateCourseEdit_ACP;
    Wt::WComboBox *tyrosJobCmb_ACP;
    Wt::WTextArea *tyrosAddrTArea_ACP;
    Wt::WLineEdit *tyrosTelEdit_ACP;
    Wt::WLineEdit *tyrosMobileEdit_ACP;
    Wt::WLineEdit *tyrosEmailEdit_ACP;
    Wt::WPushButton *tyrosSaveBtn_ACP;

    Wt::WLengthValidator *tyrosFNameValidator_ACP;
    Wt::WLengthValidator *tyrosLNameValidator_ACP;
    Wt::WLengthValidator *tyrosSexValidator_ACP;
    Wt::WLengthValidator *tyrosFatherSNameValidator_ACP;
    Wt::WRegExpValidator *tyrosNationalCodeValidator_ACP;
    Wt::WRegExpValidator *tyrosBirthIdValidator_ACP;
    Wt::WLengthValidator *tyrosBirthDateYValidator_ACP;
    Wt::WLengthValidator *tyrosBirthDateMValidator_ACP;
    Wt::WLengthValidator *tyrosBirthDateDValidator_ACP;
    Wt::WLengthValidator *tyrosBirthlocValidator_ACP;
    Wt::WLengthValidator *tyrosBirthEmissionLocValidator_ACP;
    Wt::WLengthValidator *tyrosGraduateCertValidator_ACP;
    Wt::WLengthValidator *tyrosGraduateCourseValidator_ACP;
    Wt::WLengthValidator *tyrosJobValidator_ACP;
    Wt::WLengthValidator *tyrosAddrValidator_ACP;
    Wt::WRegExpValidator *tyrosTelValidator_ACP;
    Wt::WRegExpValidator *tyrosMobileValidator_ACP;
    Wt::WRegExpValidator *tyrosEmailValidator_ACP;

    Wt::WText *errTyrosForm_ACP;

    Wt::WWidget *GetTyrosForm_ACP(Wt::WString form);

    void TyrosFormSaveBtnOK_ACP();

    Wt::WFileUpload *tyrosPicBirthCertFUP_ACP;
    Wt::WFileUpload *tyrosPicNationalCardFUP_ACP;
    Wt::WFileUpload *tyrosPicPersonnelFUP_ACP;
    Wt::WFileUpload *tyrosPicServiceFUP_ACP;


    /************* End Academy Panel *************/
};

#endif /* CGIROOT_HPP */
