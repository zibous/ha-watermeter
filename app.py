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

from lib import logger, calc
from config import *


# register the application logger
log = logger.Log(__name__, LOG_LEVEL, LOG_DIR)


def WatermeterData():
    """Main Application
    """
    def on_connect(client, userdata, flags, rc):
        # The callback for when the client receives a CONNACK response from the server.
        if rc == 0:
            log.debug(f"✔︎ {MQTT_CLIENTID} connected OK Returned code= {rc}")
        else:
            log.error(f"✔︎ {MQTT_CLIENTID} Bad connection Returned code= {rc}")

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe(MQTT_TOPIC)

    def on_publish(client, userdata, mid):
        log.debug("✔︎ Message " + str(mid) + " published.")

    def on_message(client, userdata, msg):
        # The on_message callback is called for each message received ...
        log.info(f"✔︎ MQTT Message received topic {msg.topic}")
        # make all calculations
        if(msg.topic == MQTT_TOPIC):
           clc = calc.Calculator(client, msg.payload)

    def on_disconnect(client, userdata, rc):
        # he on_disconnect() callback is called when the client disconnects from the broker.
        client.publish(MQTT_AVAILABILITY_TOPIC, "Offline", 0, False)
        appLogger.info(f"✔︎ Watermeter MQTT Client {MQTT_CLIENTID} {MQTT_AVAILABILITY_TOPIC} got disconnected")


    log.debug(f"✔︎ Watermeter MQTT Client {MQTT_CLIENTID}")
    mqttclient = mqtt.Client(MQTT_CLIENTID)

    # Assign event callbacks
    mqttclient.on_connect = on_connect
    mqttclient.on_publish = on_publish
    mqttclient.on_message = on_message
    mqttclient.on_disconnect = on_disconnect

    ## enable /disable logging
    if MQTT_ENABLE_LOGGING:
        appLogger.info(f"✔︎ Enable Logging for {MQTT_HOST}")
        mqttclient.enable_logger(appLogger.logger)
    
    # Connect
    log.info(f"✔︎ Watermeter connect to MQTT Client {MQTT_HOST}, {MQTT_AVAILABILITY_TOPIC}")

    # To connect with a username and password, call username_pw_set() before connecting:
    mqttclient.username_pw_set(MQTT_APPID, MQTT_PASSWORD)

    # To connect with a will, you must call will_set() before connecting to the broker
    mqttclient.will_set(MQTT_AVAILABILITY_TOPIC, 'Online', 0, False)

    mqttclient.connect(MQTT_HOST, MQTT_PORT, keepalive=MQTT_KEEPALIVE)
    
    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    # Other loop*() functions are available that give a threaded interface and a
    # manual interface.
    # mqttclient.loop_forever()
    while True:
        try:
            mqttclient.loop_forever()
        except KeyboardInterrupt:
            mqttclient.disconnect()
            exit(0)
        except:
            raise

log.info("✔︎ Watermeter data application start")
WatermeterData()
