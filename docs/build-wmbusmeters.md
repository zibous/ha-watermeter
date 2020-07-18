# Builds and runs rtl-wmbusmeters on GNU/Linux 

# wmbusmeters

The program receives and decodes C1,T1 or S1 telegrams (using the wireless mbus protocol) to acquire utility meter readings. The readings can then be published using MQTT, curled to a REST api, inserted into a database or stored in a log file.



 Using an  dongle, then make sure the binaries  exists and are executable:

- /usr/bin/rtl_sdr 

- /usr/bin/rtl_wmbus 

  

## Build wmbusmeters

```bash
$ git clone https://github.com/weetmuts/wmbusmeters.git
$ cd wmbusmeters
$ make && make test
$ make install

# example for testing
$ cd wmbusmeters/build_debug/
$ wmbusmeters --debug --t1 rtlwmbus watermeter1 izar 20eec43b NOKEY

```



### System configuration

`make install` installs the files:

```bash
## files
/usr/sbin/wmbusmetersd 
/etc/systemd/system/wmbusmeters.service 
/etc/udev/rules.d/99-wmbus-usb-serial.rules 
/etc/logrotate.d/wmbusmeters

## directories
/etc/wmbusmeters.d 
/var/log/wmbusmeters/meter_readings
```

You can trigger a reload of the config files with `sudo killall -HUP wmbusmetersd`



### Running without config files, good for experimentation and test.



```bash
$ cd wmbusmeters/build_debug/
$ wmbusmeters --debug --t1 rtlwmbus diehl izar 43410778 NOKEY

# with config files
$ wmbusmeters --debug --useconfig=/root/.config/wmbusmeters

$ cd /
wmbusmeters --debug --t1 rtlwmbus diehl izar 43410778 NOKEY
wmbusmeters --debug --useconfig=/root/.config/wmbusmeters

$ tail -f /var/log/syslog
$ tail -f /root/log/wmbusmeters/wmbusmeters.log
```

see: https://github.com/weetmuts/wmbusmeters#running-without-config-files-good-for-experimentation-and-test



## Configuration

Check the config file `nano /etc/wmbusmeters.conf` and edit the device to point to your dongle.

```ini
loglevel=normal
device=rtlwmbus:rtl_sdr -f 868.95M -s 1.6e6 - 2>/dev/null | rtl_wmbus
logtelegrams=true
format=json
meterfiles=/var/log/wmbusmeters/meter_readings
meterfilesaction=overwrite
meterfilesnaming=name
meterfilestimestamp=hour
logfile=/var/log/wmbusmeters/wmbusmeters.log
shell=/usr/bin/mosquitto_pub -h localhost -p 1883 -t tele/wasser/verbrauch -m "$METER_JSON"

```

Then add a meter file in `nano /etc/wmbusmeters.d/diehl`

```ini
name=watermeter
type=izar
id=FFFF0778
key=
```



## wmbusmeters.service

`nano /lib/systemd/system/wmbusmeters.service`

```bash
[Unit]
Description="wmbusmeters service (no udev trigger)"
Documentation=https://github.com/weetmuts/wmbusmeters
Documentation=man:wmbusmeters(1)
After=network.target

[Service]
Type=forking

# Run ExecStartPre with root-permissions
PermissionsStartOnly=true
ExecStartPre=-/bin/mkdir -p /var/log/wmbusmeters/meter_readings
ExecStartPre=/bin/chown -R wmbusmeters:wmbusmeters /var/log/wmbusmeters
ExecStartPre=-/bin/mkdir -p /run/wmbusmeters
ExecStartPre=/bin/chown -R wmbusmeters:wmbusmeters /run/wmbusmeters

ExecStart=/usr/sbin/wmbusmetersd /run/wmbusmeters/wmbusmeters.pid
PIDFile=/run/wmbusmeters/wmbusmeters.pid

[Install]
WantedBy=multi-user.target

```



`nano /lib/systemd/system/wmbusmeters@.service` 

```bash
[Unit]
Description="wmbusmeters service on %I"
Documentation=https://github.com/weetmuts/wmbusmeters
Documentation=man:wmbusmeters(1)
After=network.target

[Service]
Type=forking
ExecStart=/usr/sbin/wmbusmetersd --device='%I' /run/wmbusmeters/wmbusmeters-%i.pid
PIDFile=/run/wmbusmeters/wmbusmeters-%i.pid

[Install]
WantedBy=multi-user.target
```



```bash
$ sudo systemctl daemon-reload

$ systemctl start wmbusmeters
$ systemctl status wmbusmeters

$ systemctl start wmbusmeters@-dev-rtlsdr_5
$ systemctl status wmbusmeters@-dev-rtlsdr_5

$ systemctl enable wmbusmeters
$ systemctl enable wmbusmeters@-dev-rtlsdr_5

# $ systemctl stop wmbusmeters
# $ systemctl start wmbusmeters
# $ systemctl restart wmbusmeters
# $ systemctl status wmbusmeters

# $ systemctl stop wmbusmeters@-dev-rtlsdr_5
# $ systemctl start wmbusmeters@-dev-rtlsdr_5
# $ systemctl restart wmbusmeters@-dev-rtlsdr_5
# $ systemctl status wmbusmeters@-dev-rtlsdr_5

```



Wmbusmeters should start automatically, check with 

```bash
$ tail -f /var/log/syslog
$ tail -f /var/log/wmbusmeters/wmbusmeters.log
```

 If not you will see the error message `(rtlwmbus) error: when starting as daemon, wmbusmeters expects /usr/bin/rtl_sdr to exist!` and the daemon will refuse to start.)



https://github.com/weetmuts/wmbusmeters