# ha-watermeter

A simple Linux python script to query arbitrary smartmeter water sensor devices and send the data to an MQTT broker, e.g., the famous Eclipse Mosquitto.  After data made the hop to the MQTT broker it can be used by home automation software, like openHAB or Home Assistant.

## Installation ha-watermeter Version 1.0.1

On a modern Linux system just a few steps are needed to get the daemon working. The following example shows the installation under Debian/Raspbian below the `/opt` directory:

```bash
$ git clone https://github.com/zibous/ha-watermeter.git /opt/ha-watermeter
$ cd /opt/ha-watermeter
$ sudo pip3 install -r requirements.txt

```

To match personal needs, all operation details can be configured using the file **config.py** The file needs to be created first:

```bash
$ cd /opt/ha-watermeter
$ cp config.py.dist config.py
$ nano config.py

$ chmod +x app.py
$ python3 app.py  ## or ./app.py
```

### Application log

![logging](docs/logging.png)



### Continuous Daemon/Service

You most probably want to execute the program **continuously in the background**. This can be done either by using the internal daemon or cron.

**Attention:** Daemon mode must be enabled in the configuration file (default).

1. Systemd service - on systemd powered systems the **recommended** option

   ```bash
   sudo cp /opt/ha-watermeter/service.template /etc/systemd/system/ha-watermeter.service

   sudo systemctl daemon-reload
   sudo systemctl start ha-watermeter.service
   sudo systemctl status ha-watermeter.service

   sudo systemctl enable ha-watermeter.service
   ```



## Results Homeassistant Card

![ha-card](docs/ha-card.png)


Sensor setting see: [smartmeter-watermeter.yaml](docs/homeassistant)
