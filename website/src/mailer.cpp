#include <cstdlib>
#include <ctime>

#include <jwsmtp/jwsmtp.h>

#include "mailer.hpp"
#include "base.hpp"
#include "bstring.hpp"
#include "cdate.hpp"
#include "clientinfo.hpp"
#include "errlogger.hpp"
#include "serverinfo.hpp"

using namespace std;
using namespace jwsmtp;
using namespace SAAIIR;
using namespace CDate;

void Mailer::SendMail(const string& from, const string& to, const string& subject, const string& body) {
    //mailer mail(from, to, subject, body, "localhost", mailer::SMTP_PORT, false);

    mailer mail;
    mail.addrecipient(to);
    mail.setsender(from);
    mail.setsubject(subject);
    mail.setmessageHTML(body);
    mail.setserver("localhost");

    //mail.addrecipient("whooton@somewhere.net", mailer::Bcc);
    //mail.attach("attach.png");

    /* Authentication
    mail.username("loginname");
    mail.password("secret");
    // To stop using authentication
    mail.username("");

    mail.authtype("mailer::PLAIN");
    // or
    mail.authtype("mailer::LOGIN");
    */

    /*
    instead of: mail.send();
    #include <boost/thread/thread.hpp>
    boost::thread thrd(mail);  // operator()() called implicitly
    thrd.join();  // optional
    // or:-
    mail.operator()();
    cout << mail.response() << "\n";
    */

    /*
    // A valid response will begin with the string "250"
    // see rfc 821 for response codes, section 4.2.2.>
    if(mail.response().substr(0,3) != "250") {
        // error
    }
    */

    mail.operator()();

    if(mail.response().substr(0, 3) != "250") {
        ErrLogger::LogError(("jwSMTP :  " + mail.response()).c_str());
        /*ErrLogger::LogError(("  *from    :  " + from).c_str());
        ErrLogger::LogError(("  *to      :  " + to).c_str());
        ErrLogger::LogError(("  *subject :  " + subject).c_str());
        ErrLogger::LogError(("  *body    :  " + body).c_str());)*/
    }

    //mail.reset();
}

void Mailer::SendLoginAlert(const string& to, const string& user, const Now& now,
                            const ServerInfo *serverInfo, const ClientInfo *clientInfo) {
    SendMail(serverInfo->noReply, to, "ALERT: " + serverInfo->host + " access from " + user,
             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Access From " + user + "</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->ip + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" +
             clientInfo->countryCode + "; " +
             clientInfo->countryName +
             "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->browser + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->referer + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" +
             DateConv::ToJalali(now) + " * " +
             BString::Trim(DateConv::RawLocalDateTime(now)) +
             "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}

void Mailer::SendLoginRecovery(const string& to, const string& user, const string& pw, const Now& now,
                               const ServerInfo *serverInfo, const ClientInfo *clientInfo) {
    SendMail(serverInfo->noReply, to, serverInfo->host + " password recovery request from " + user,
             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Password Recovery</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + user + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Password</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + pw + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             ""
             "<br /><h3>Requested By Client</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->ip + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" +
             clientInfo->countryCode + "; " +
             clientInfo->countryName +
             "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->browser + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->referer + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" +
             DateConv::ToJalali(now) + " * " +
             BString::Trim(DateConv::RawLocalDateTime(now)) +
             "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}

void Mailer::SendUserMessage(const string& to, const string& from,
                             const string& user, const string& uri,
                             const string& subject, const string& body,
                             const Now& now,
                             const ServerInfo *serverInfo, const ClientInfo *clientInfo) {

    string mailBodyPath("../tmp/mailbody");
    string mailSubjectPath("../tmp/mailsubject");
    srand((unsigned)time(NULL));
    string rnd = "-" + BString::IntToStr(rand()) + ".txt";
    mailBodyPath += rnd;
    mailSubjectPath += rnd;

    Base::FileWrite(mailSubjectPath.c_str(), ("user of " + serverInfo->host + " - " + subject).c_str());
    Base::FileWrite(mailBodyPath.c_str(),
                    ("<center><div style=\"position: relative; width: 89%; direction: rtl; text-align: justify; "
                    "font-family:Tahoma; font-size: 14px; line-height: 33px;\">"
                    "<p>"
                    "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                    "مشخصات فرستنده پيغام"
                    "</h6>"
                    "نام فرستنده:&nbsp;&nbsp;&nbsp;"
                    + user +
                    "<br />"
                    "آدرس ايميل:&nbsp;&nbsp;&nbsp;"
                    + from +
                    "<br />"
                    "وب سايت:&nbsp;&nbsp;&nbsp;"
                    + (uri != "" ? uri : " - ") +
                    "<br />"
                    "</p>"
                    "<br />"
                    "<p>"
                    "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                    "موضوع پيام"
                    "</h6>"
                    + subject +
                    "</p>"
                    "<br />"
                    "<p>"
                    "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                    "متن  پيام"
                    "</h6>"
                    + body +
                    "</p>"
                    "<br />"
                    "</div></center>"

                    "<br /><br />"

                    "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
                    "font-size: 11px; font-weight: bold; color: #203070; "
                    "letter-spacing: 1px; line-height: 17px;\">"
                    "<br /><h3>Sent By Client</h3>"
                    "<blockquote>"
                    "<table>"
                    ""
                    "<tr valign=\"top\">"
                    "<td width=\"85\">IP</td>"
                    "<td width=\"15\">-</td>"
                    "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->ip + "</span></td>"
                    "</tr>"
                    ""
                    "<tr valign=\"top\">"
                    "<td width=\"85\">Location</td>"
                    "<td width=\"15\">-</td>"
                    "<td><span style=\"color: #900; font-weight: normal;\">" +
                    clientInfo->countryCode + "; " +
                    clientInfo->countryName +
                    "</span></td>"
                    "</tr>"
                    ""
                    "<tr valign=\"top\">"
                    "<td width=\"85\">User Agent</td>"
                    "<td width=\"15\">-</td>"
                    "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->browser + "</span></td>"
                    "</tr>"
                    ""
                    "<tr valign=\"top\">"
                    "<td width=\"85\">Referer</td>"
                    "<td width=\"15\">-</td>"
                    "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->referer + "</span></td>"
                    "</tr>"
                    ""
                    "<tr valign=\"top\">"
                    "<td width=\"85\">Time</td>"
                    "<td width=\"15\">-</td>"
                    "<td><span style=\"color: #900; font-weight: normal;\">" +
                    DateConv::ToJalali(now) + " * " +
                    BString::Trim(DateConv::RawLocalDateTime(now)) +
                    "</span></td>"
                    "</tr>"
                    ""
                    "</table>"
                    "</blockquote>"
                    "<br /></div>").c_str());

    //Base::Exec(("cd ../bin/ && ./sendmail.pl " + rnd + " " + from + " " + to).c_str());
    Base::Exec(("cd ../bin/ && ./sendmail.pl " + rnd + " " + from + " " + to).c_str());

    //ErrLogger::LogError(("cd ../bin/ && ./sendmail.pl " + rnd + " " + from + " " + to).c_str());

    Base::FileErase(mailBodyPath.c_str());
    Base::FileErase(mailSubjectPath.c_str());
}

/*
void Mailer::SendUserMessage(const string& to, const string& from,
                             const string& user, const string& uri,
                             const string& subject, const string& body,
                             const Now& now) {
    SendMail(from, to, "user of " + serverInfo->host + " - " + subject,
             "<center><div style=\"position: relative; width: 89%; direction: rtl; text-align: justify; "
                     "font-family:Tahoma; font-size: 14px; line-height: 33px;\">"
                     "<p>"
                     "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                     "مشخصات فرستنده پيغام"
                     "</h6>"
                     "نام فرستنده:&nbsp;&nbsp;&nbsp;"
                     + user +
                     "<br />"
                     "آدرس ايميل:&nbsp;&nbsp;&nbsp;"
                     + from +
                     "<br />"
                     "وب سايت:&nbsp;&nbsp;&nbsp;"
                     + (uri != "" ? uri : " - ") +
                     "<br />"
                     "</p>"
                     "<br />"
                     "<p>"
                     "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                     "موضوع پيام"
                     "</h6>"
                     + subject +
                     "</p>"
                     "<br />"
                     "<p>"
                     "<h6 style=\"color: #0000FF; font-size: 11px;\">"
                     "متن  پيام"
                     "</h6>"
                     + body +
                     "</p>"
                     "<br />"
                     "</div></center>"

                     "<br /><br />"

                     "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
                     "font-size: 11px; font-weight: bold; color: #203070; "
                     "letter-spacing: 1px; line-height: 17px;\">"
                     "<br /><h3>Sent By Client</h3>"
                     "<blockquote>"
                     "<table>"
                     ""
                     "<tr valign=\"top\">"
                     "<td width=\"85\">IP</td>"
                     "<td width=\"15\">-</td>"
                     "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->ip + "</span></td>"
                     "</tr>"
                     ""
                     "<tr valign=\"top\">"
                     "<td width=\"85\">Location</td>"
                     "<td width=\"15\">-</td>"
                     "<td><span style=\"color: #900; font-weight: normal;\">" +
                     clientInfo->countryCode + "; " +
                     clientInfo->countryName +
                     "</span></td>"
                     "</tr>"
                     ""
                     "<tr valign=\"top\">"
                     "<td width=\"85\">User Agent</td>"
                     "<td width=\"15\">-</td>"
                     "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->browser + "</span></td>"
                     "</tr>"
                     ""
                     "<tr valign=\"top\">"
                     "<td width=\"85\">Referer</td>"
                     "<td width=\"15\">-</td>"
                     "<td><span style=\"color: #900; font-weight: normal;\">" + clientInfo->referer + "</span></td>"
                     "</tr>"
                     ""
                     "<tr valign=\"top\">"
                     "<td width=\"85\">Time</td>"
                     "<td width=\"15\">-</td>"
                     "<td><span style=\"color: #900; font-weight: normal;\">" +
                     DateConv::ToJalali(now) + " * " +
                     BString::Trim(DateConv::RawLocalDateTime(now)) +
                     "</span></td>"
                     "</tr>"
                     ""
                     "</table>"
                     "</blockquote>"
                     "<br /></div>"
             );
}
*/
