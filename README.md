xfce4-defaultoutput-plugin
==========================

Monitor and change default sound output device on xfce4 panel.

Usage
-----

Run ```make && make install``` to compile and install the plugin.

Platforms
---------

Currently this only works on FreeBSD.

Method
------

##### FreeBSD

Basically it reads and writes sysctl hw.snd.default_unit.
It cycles through different devices one by one.
Because that sysctl must be changed with superuser permission,
this program uses sudo to run the command. Make sure that you
can sudo without password.


