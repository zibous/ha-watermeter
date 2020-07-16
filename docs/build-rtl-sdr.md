

# RTL-SDR: Inexpensive Software Defined Radio

## Driver Installation Instructions

All the installation instructions below were derived from the following links:

- http://sdr.osmocom.org/trac/wiki/rtl-sdr
- https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr
- http://www.rtlsdr.org

The Osmocom RTL-SDR library must be installed before you can build rtl-wmbus. See http://sdr.osmocom.org/trac/wiki/rtl-sdr for more information.

Here, we will provide you with binaries and basic installation that  will enable you to get tune to a frequency and get samples into Matlab or GNU Octave. If you are bold, you can go  to the aove websites and follow the installation instructions. There are instructions there to install additional software, such as  GNU-radio or SDR.  To install rtl-wmbus, download, unpack the source code and go to the top level directory. 

## Build rtl-sdr

```bash
>> sudo apt-get update
>> sudo apt install ncurses-dev apt install cmake librtlsdr-dev
>> sudo apt install git git-core cmake libusb-1.0-0-dev build-essential
>> sudo apt install libtool-bin automake makeinfo libc6-dev libgmp-dev libgmp3-dev gawk qpdf bison

>> git clone git://git.osmocom.org/rtl-sdr.git
>> cd /rtl-sdr
>> cd /rtl-sdr/build
>> cmake ../ -DINSTALL_UDEV_RULES=ON
>> sudo ldconfig

>> cp ./rtl-sdr/rtl-sdr.rules /etc/udev/rules.d/
>> ls -l /dev/rtlsdr*

>> rtl_test
>> rtl_eeprom
>> rtl_test -p
>> lsusb

```

Connect your USB dongle to your computer and run the rtl_test. You should get the following output:

```bash
>> rtl_test -t
```



### Using rtl_sdr to capture to a file

```bash
>> ls -l /dev/rtlsdr*
## lrwxrwxrwx 1 root root 15 Jul 14 10:49 /dev/rtlsdr_5 -> bus/usb/001/004

## To save an IQ-stream on disk and decode that off-line:
$ rtl_sdr samples.bin -f 868.95M -s 1600000
$ rtl_sdr /tmp/cap.bin -f 868.95M -s 1600000

```



### For more information see:

```txt
https://www.raspberry-pi-geek.de/ausgaben/rpg/2014/06/luftraum-ueberwachen-mit-dem-raspberry-pi/2/
https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/raspberry-headless-setup-rtl-sdr
```



