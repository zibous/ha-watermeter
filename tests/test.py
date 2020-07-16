#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import time
from datetime import datetime
now = datetime.now()


## 2020	2020/01	2020-01-01	08:00	123,00	5,27

prev_timestamp = "2020-07-15T07:34:17Z"
ts = datetime.strptime(prev_timestamp ,'%Y-%m-%dT%H:%M:%SZ')
current_dts  = datetime.strptime("2020-07-15T07:40:17Z" ,'%Y-%m-%dT%H:%M:%SZ')

print("Hour: ",current_dts.strftime('%H'))
print("Day: ",current_dts.strftime('%Y-%m-%d'))
print("Month: ",current_dts.strftime('%Y-%m'))
print("Year: ",current_dts.strftime('%Y'))


elapsed_time =  current_dts - ts
print("elapsed_time:" + str(elapsed_time))


data = {
    "name": "Wasserstand",
    "device": "watermeter",
    "deviceid": "test",
    "m3": {
        "current": 0.000,
        "hour": 0.000,
        "day": 0.000,
        "month": 0.000,
        "year": 0.000
    },
    "liter": {
        "current": 0.000,
        "hour": 0.000,
        "day": 0.000,
        "month": 0.000,
        "year": 0.000
    },
    "total_m3": 0.00,
    "last_month_total_m3": 0.00,
    "last_month_measure_date": "",
    "alarm": "No alarm",
    "last_alarm": "No alarm",
    "periode": now.strftime('%Y-%m-%d'),
    "month": now.strftime('%Y-%m'),
    "year": now.strftime('%Y'),
    "timestamp": now.strftime('%Y-%m-%d %H:%M:%S.%f'),
    "last_update": now.strftime('%Y-%m-%d %H:%M:%S.%f'),
    "icon": "mdi:billboard",
    "unit_of_measurement": u"\u33A5",
    "version": "1.0.0",
    "attribution": "Data provided by Peter Siebler"
}

with open("43430778.json", "r") as f:
    _prevdat = f.read()
    prevData = json.loads(_prevdat)

if(prevData):
    data['m3']['current'] = round(data['total_m3'] - prevData['total_m3'],3)