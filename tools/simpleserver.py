#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys


try:
    import json
    import json
    import socket
    import time
    from threading import Thread

except Exception as e:
    print(f"Configuration error {str(e)}")
    sys.exit(1)


def getESBHomeData(url: str = "", platform: str = "sensor", id: str = "", timeout: int = 5) -> dict:
    """get the data from the defined platform and device id"""
    if url:
        strURL = "{}/{}/{}".format(url, platform, id)
        # default data
        dictData = {
            "url": strURL,
            "field": id,
            "value": -1,
            "state": "loading",
            "responsecode": 400,
            "responsemessage": "not found!",
            "encoding": "none"
        }
        response = getRequests(url=strURL, errormessage=True)
        if (response.status_code == 200):
            data = json.loads(response.text)
            if (data):
                dictData["field"] = data["id"]
                dictData["value"] = data["value"]
                dictData["state"] = "Ready"
                dictData["responsecode"] = response.status_code
                dictData["responsemessage"] = response.status_code
                return dictData
        else:
            dictData["state"] = "Error"
            dictData["value"] = float(0.00)
            dictData["responsecode"] = response.status_code
        return dictData


def runUDPServer():
    # https://github.com/ninedraft/python-udp/blob/master/client.py
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) # UDP
    client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    # Enable broadcasting mode
    client.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    client.bind(("zeusus.siebler.home", 7227))

    while True:
        # Thanks @seym45 for a fix
        data, addr = client.recvfrom(1024)
        if data:
           print("received message: %s"%data)

def runRESTServer():
    interval = 60
    next = time.time() + interval
    url = "http://water-meter-izar.local"

    while True:
        if time.time() >= next :
            data = getESBHomeData(url=url,platform="textsensor", id="watermeterdata" )
            if(data): 
              print("received message: %s"%data)
            next = next + interval
        time.sleep(next - time.time())

if __name__ == "__main__":
    print("Start Server")
    t1 = Thread(target = runUDPServer)
    t2 = Thread(target = runRESTServer)
    t1.setDaemon(True)
    t2.setDaemon(True)
    t1.start()
    t2.start()
    while True:
        pass