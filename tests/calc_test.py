#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os.path
import sys
import json

from lib import logger, calc
from config import *

log = logger.Log(__name__, LOG_LEVEL, LOG_DIR)

client = None

payload = {
    "media": "water",
    "meter": "izar",
    "name": "watermeter",
    "id": "43430778",
    "total_m3": 168.369,
    "last_month_total_m3": 162.614,
    "last_month_measure_date": "2020-07-01",
    "remaining_battery_life_y": 13,
    "current_alarms": "no_alarm",
    "previous_alarms": "no_alarm",
    "timestamp": "2020-07-15T09:03:33Z"
}

clc = calc.Calculator(client, json.dumps(payload))
