#!/bin/sh
rm -f ./mod_sts.la
rm -f ./mod_sts.lo
rm -f ./mod_sts.slo
rm -f /etc/apache2/mods-available/sts.load
rm -f /etc/apache2/mods-enabled/sts.load
rm -f /usr/lib/apache2/modules/mod_sts.so
rm -f /usr/lib/apache2/modules/mod_sts.la
#apxs -i -a -c -o mod_sts.so -c mod_sts.c -lcurl
#systemctl restart apache2