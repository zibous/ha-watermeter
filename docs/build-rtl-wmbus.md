# Builds and runs rtl-wmbus on GNU/Linux 

The primary purpose of rtl-wmbus is experimenting with digital signal  processing and software radio. rtl-wmbus can be used on resource  constrained devices such Raspberry Pi Zero or Raspberry PI B+  overclocked to 1GHz.

rtl-wmbus is a software defined receiver for Wireless-M-Bus. It is written in plain C and uses RTL-SDR (https://github.com/osmocom/rtl-sdr) to interface with RTL2832-based hardware.

The Osmocom RTL-SDR library must be installed before you can build rtl-wmbus. See http://sdr.osmocom.org/trac/wiki/rtl-sdr for more information. 

To install rtl-wmbus, download, unpack the source code and go to the top level directory:

```bash
# $ git clone https://github.com/xaelsouth/rtl-wmbus.git
$ git clone https://github.com/weetmuts/rtl-wmbus.git
$ cd rtl-wmbus
# $ make
# $ make debug    # (no optimization at all, with debug options)
$ make release  # (-O3 optimized version, without any debugging options
$ cp rtl-wmbus/build/rtl_wmbus /usr/bin/rtl_wmbus

```

## Test rtl-wmbus

```bash
$ ls -l /dev/rtlsdr*
>> lrwxrwxrwx 1 root root 15 Jul 19 05:59 /dev/rtlsdr_5 -> bus/usb/001/004

$ cd build
$ rtl_sdr -f 868.95M -s 1.6e6 - | rtl_wmbus
$ rtl_sdr -f 868.95M -s 1600000 - 2 | rtl_wmbus
$ rtl_sdr -f 868.95M -s 1.6e6 - 2>/dev/null | rtl_wmbus

>>> Results
Found 1 device(s):
  0:  Realtek, RTL2838UHIDIR, SN: 00000001

Using device 0: Generic RTL2832U OEM
Found Fitipower FC0012 tuner
Sampling at 1600000 S/s.
Tuned to 868950000 Hz.
Tuner gain set to automatic.
Reading samples in async mode...
Allocating 15 zero-copy buffers
T1;0;0;2020-07-19 05:28:57.000;46;95;43410778;0x1944a511780741434418ff411a0013109684c3a89f7017569381
T1;0;0;2020-07-19 05:28:59.000;53;107;FF430778;0x1944a511780743ffff18a2611a001312f3e9669a7e9d9c8381d2
T1;0;0;2020-07-19 05:29:00.000;61;60;FFFF0785;0x19ff24238507ffffff18a261ffffffff1cbe74ff32ffff72ffff
T1;0;0;2020-07-19 05:29:05.000;59;100;43FF0778;0x1944a5117807ff43ffffa251ff00131081abff03bea3ce8d8a83
T1;0;0;2020-07-19 05:29:08.000;69;77;43430778;0x1944a51178074343ffffa2711a001312e4c62f315f4e455898d0
T1;0;0;2020-07-19 05:29:09.000;34;66;FFFFFFFF;0x1944ff23ffffffffff18a2ffffffff11ffff3dffffff70a9ffff
T1;0;0;2020-07-19 05:29:14.000;51;84;43410778;0x1944a511780741434418a261ff001310b8da50fedcd7a4e0a184
T1;0;0;2020-07-19 05:29:17.000;46;179;43FF0778;0x1944a5117807ff4344ffa2011a001312810ad360ba754859d7dd
T1;0;0;2020-07-19 05:29:18.000;42;68;FF8907FF;0x19ffffffff0789ffff18ff01ffffff116b5dfffff664ffffffff
T1;0;0;2020-07-19 05:29:22.000;48;52;43FF0778;0x19ffa5117807ff434418a2711a001310aff51955fd04ffffff01
T1;0;0;2020-07-19 05:29:26.000;53;169;43430778;0x1944a511780743434418a2111a00131296259acb9bff9182cedf
T1;0;0;2020-07-19 05:29:27.000;45;61;FFFF07FF;0x19ff2423ff07ffff4418a2ffff0013117cffffbbd7b7ffffffff
T1;1;1;2020-07-19 05:29:31.000;57;167;43410778;0x1944a511780741434418a2011a001310ca39e504183f703af78b
T1;0;0;2020-07-19 05:29:34.000;67;135;43430778;0x1944a511780743434418a221ff001312af544036f9d2fbefe5d8
T1;0;0;2020-07-19 05:29:36.000;40;80;FFFF07FF;0x1944ffffff07ffffffffa2210600ffffffffffffffc3ffffffff
T1;0;0;2020-07-19 05:29:40.000;49;178;43410778;0x1944a511780741434418a211ffff1310dd16acaf39eca9e1ee89
```



## For more information see:

```txt
https://github.com/xaelsouth/rtl-wmbus

## Adjust length byte after DLL CRC:s are removed.
   When trimming away the crc bytes, update the first len byte.

   Previously wmbusmeters was not so picky, and it worked anyway,
   but now wmbusmeters actually check the internal payload crc:s and
   other lengths. So an incorrect len byte fails.

   The hardware dongles like im871a, amb8465 do update the 
   len byte as well.

   https://github.com/weetmuts/rtl-wmbus
```



