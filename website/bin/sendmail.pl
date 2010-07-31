#!/usr/bin/perl
use strict;
use utf8;
use encoding 'utf-8';
use MIME::Lite;

my $rnd = "$ARGV[0]";
my $from = "$ARGV[1]";
my $to = "$ARGV[2]";

my $path = "../tmp/";
my $mailSubjectPath = $path . "mailsubject" . $rnd;
my $mailBodyPath = $path . "mailbody" . $rnd;

my $subject = ReadFile($mailSubjectPath);
my $body = ReadFile($mailBodyPath);

#my $subject =~ s/\r\n|\n|\r/<br \/>/;
#my $body =~ s/\r\n|\n|\r/<br \/>/;

my $message = MIME::Lite->new(
        From        => "<$from>",
        To          => "<$to>",
	'Reply-to'  => "<$from>",
        Subject     => "=?UTF-8?B?" .
                       Encode::encode("utf-8", "$subject") .
                       "?=",
        Type        => "text/html",
        Encoding    => '8bit',
        Charset     => 'utf-8',
        Data        => "$body",
        Filename    => 'mail.html',
);

$message->attr("content-type.charset" => "utf-8");

MIME::Lite->send('smtp', 'localhost', Timeout => 320);

$message->send;



sub ReadFile {
    my @lines;

    open (FILE, "<:utf8", $_[0]) || Error ();
    flock (FILE, 1);
    @lines = <FILE>;
    close(FILE);

    return join("<br />", @lines);
}

sub Error {
}


