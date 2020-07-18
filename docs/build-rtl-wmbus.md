# Builds and runs rtl-wmbus on GNU/Linux 

The primary purpose of rtl-wmbus is experimenting with digital signal  processing and software radio. rtl-wmbus can be used on resource  constrained devices such Raspberry Pi Zero or Raspberry PI B+  overclocked to 1GHz.

rtl-wmbus is a software defined receiver for Wireless-M-Bus. It is written in plain C and uses RTL-SDR (https://github.com/osmocom/rtl-sdr) to interface with RTL2832-based hardware.

The Osmocom RTL-SDR library must be installed before you can build rtl-wmbus. See http://sdr.osmocom.org/trac/wiki/rtl-sdr for more information. 

To install rtl-wmbus, download, unpack the source code and go to the top level directory:

```bash
$ git clone https://github.com/xaelsouth/rtl-wmbus.git
$ cd rtl-wmbus
$ make
$ make debug    # (no optimization at all, with debug options)
$ make release  # (-O3 optimized version, without any debugging options
$ cp ~/rtl-wmbus/build/rtl_wmbus /usr/bin/rtl_wmbus

```

## Usage rtl-wmbus

```bash
$ ls -l /dev/rtlsdr*

$ rtl_sdr -f 868.9M -s 1.6e6 - | rtl_wmbus
$ rtl_sdr -f 868.9M -s 1600000 - 2 | rtl_wmbus
$ rtl_sdr -f 868.95M -s 1.6e6 - 2>/dev/null | rtl_wmbus

```



For more information see:

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



