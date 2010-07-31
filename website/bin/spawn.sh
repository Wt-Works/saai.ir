#/usr/local/bin/bash

ROOT_BIN=/srv/saai.ir/www/root.fcgi
CAPTCHA_BIN=/srv/saai.ir/www/captcha.fcgi
SHOWPICS_BIN=/srv/saai.ir/www/showpics.fcgi

cd /srv/saai.ir/bin
/etc/init.d/nginx stop
#kill -TERM "$(pidof $ROOT_BIN)"
#kill -TERM "$(pidof $CAPTCHA_BIN)"
#kill -TERM "$(pidof $SHOWPICS_BIN)"
killall -9 root.fcgi
killall -9 captcha.fcgi
killall -9 showpics.fcgi
/etc/init.d/nginx start

while true
do
	if [ ! "$(pidof $SHOWPICS_BIN)" ]
	then
		spawn-fcgi -n -f $SHOWPICS_BIN -a 0.0.0.0 -p 9003 &
	fi
	if [ ! "$(pidof $CAPTCHA_BIN)" ]
	then
		spawn-fcgi -n -f $CAPTCHA_BIN -a 0.0.0.0 -p 9002 &
	fi
	if [ ! "$(pidof $ROOT_BIN)" ]
	then
		spawn-fcgi -n -f $ROOT_BIN -a 0.0.0.0 -p 9001 &
	fi
done


