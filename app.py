#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright (c) 2020 Peter Siebler
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Distribution License
# which accompanies this distribution.
#
# Contributors:
#    Peter Siebler - initial implementation
#
# Copyright (c) 2020 Peter Siebler
# All rights reserved.

import os.path
import sys
import paho.mqtt.client as mqtt
import json
import threading
import time
from time import sleep
from uptime import uptime

from lib import logger, calc
from config import *

# register the application logger
appLogger = logger.Log(__name__, LOG_LEVEL, LOG_DIR)


def up_time():

    total_seconds = uptime()

    # Helper vars:
    MINUTE = 60
    HOUR = MINUTE * 60
    DAY = HOUR * 24

    # Get the days, hours, etc:
    days = int(total_seconds / DAY)
    hours = int((total_seconds % DAY) / HOUR)
    minutes = int((total_seconds % HOUR) / MINUTE)
    seconds = int(total_seconds % MINUTE)

    # Build up the pretty string (like this: "N days, N hours, N minutes, N seconds")
    string = ""
    if days > 0:
        string += str(days) + " " + (days == 1 and "day" or "days") + ", "
    if len(string) > 0 or hours > 0:
        string += str(hours) + " " + (hours == 1 and "hour" or "hours") + ", "
    if len(string) > 0 or minutes > 0:
        string += str(minutes) + " " + (minutes == 1 and "minute" or "minutes") + ", "
    string += str(seconds) + " " + (seconds == 1 and "second" or "seconds")

    return string


def WatermeterData():
    """Main Application
    """

    def on_connect(client, userdata, flags, rc):
        # The callback for when the client receives a CONNACK response from the server.
        if rc == mqtt.CONNACK_ACCEPTED:
            appLogger.debug(f"✔︎ {MQTT_HOST} connected OK, subscribe to topic {MQTT_TOPIC}")
            # Subscribing in on_connect() means that if we lose the connection and
            # reconnect then subscriptions will be renewed.
            client.subscribe(MQTT_TOPIC)
        else:
            appLogger.error(f"✔︎ {MQTT_HOST} Bad connection, returned code= {rc}")

    def on_publish(client, userdata, mid):
        appLogger.debug(f"✔︎ Message Id {str(mid)} published.")

    def on_message(client, userdata, msg):
        # The on_message callback is called for each message received ...
        appLogger.info(f"✔︎ MQTT Message received topic {msg.topic}")
        # application state
        if(APP_STATEINFO):
            APP_STATEINFO["timestamp"] = now.strftime(DATEFORMAT_CURRENT)
            APP_STATEINFO["time"] = time.ctime()
            APP_STATEINFO["uptime"] = up_time()
            # publish state
            appLogger.info(f"✔︎ MQTT Publish state topic {MQTT_STATE_TOPIC}")
            client.publish(MQTT_STATE_TOPIC, json.dumps(APP_STATEINFO))
            # reset state
            APP_STATEINFO["state"] = 'waiting'
            APP_STATEINFO["synctime"] = -1
        # make all calculations
        if(msg.topic == MQTT_TOPIC):
            # make the calculations
            calcModul = calc.Calculator(client)
            lastData = calcModul.getResult(msg.payload)
            # reset state
            APP_STATEINFO["lastdata"] = now.strftime(DATEFORMAT_CURRENT)
            APP_STATEINFO["synctime"] = str(calcModul.elapsed_time)
            APP_STATEINFO["state"] = calcModul.state

    def on_disconnect(client, userdata, rc):
        # he on_disconnect() callback is called when the client disconnects from the broker.
        if rc != 1:
            appLogger.info(f"✔︎ Watermeter MQTT Client {MQTT_HOST} {MQTT_AVAILABILITY_TOPIC} got disconnected (code: {rc}")

    def on_log(client, userdata, level, buf):
        appLogger.debug(f"❖ Mqtt Logmessage {level}:  {buf}")

    appLogger.debug(f"✔︎ Watermeter MQTT Client {MQTT_HOST}")
    mqttclient = mqtt.Client()

    # Assign event callbacks
    mqttclient.on_connect = on_connect
    mqttclient.on_publish = on_publish
    mqttclient.on_message = on_message
    mqttclient.on_disconnect = on_disconnect

    # enable /disable logging
    if MQTT_ENABLE_LOGGING:
        appLogger.info(f"✔︎ Enable Logging for {MQTT_HOST}")
        mqttclient.on_log = on_log
        mqttclient.enable_logger(appLogger.logger)

    # Connect to mqtt brocker
    appLogger.info(f"✔︎ Watermeter connect to MQTT Client {MQTT_HOST}, {MQTT_AVAILABILITY_TOPIC}")

    # To connect with a username and password, call username_pw_set() before connecting:
    mqttclient.username_pw_set(MQTT_APPID, MQTT_PASSWORD)

    # Set a Will to be sent by the broker in case the client disconnects unexpectedly.
    mqttclient.will_set(MQTT_AVAILABILITY_TOPIC, 'Offline', 2, True)

    # conect to the MQTT Broker
    mqttclient.connect(MQTT_HOST, MQTT_PORT, keepalive=MQTT_KEEPALIVE)

    # Wait until we've connected
    while not not mqttclient.is_connected():  # wait in loop
        appLogger.debug(f"✔︎ Waiting for connection {MQTT_HOST}")
        time.sleep(1)

    # publish online state
    mqttclient.publish(MQTT_AVAILABILITY_TOPIC, 'Online', 2, True)

    # The loop_forever call blocks the main thread and so it will never terminate.
    # Press crtl+c to exit
    mqttclient.loop_forever()


# Start main application
appLogger.info("✔︎ Watermeter data application start")
WatermeterData()
