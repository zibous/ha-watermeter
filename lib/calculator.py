#!/usr/bin/env python3
# -*- coding: utf-8 -*-


from operator import truediv
import sys

if sys.version_info[0] < 3:
    raise Exception("Python 3 is required to run")

try:
    import json
    import time
    import paho.mqtt.publish as publish
    from datetime import datetime

    from lib import logger
    from config import *

except Exception as e:
    print('Import error {}, check requirements.txt'.format(e))


log = logger.Log(__name__, LOG_LEVEL, LOG_DIR)


class Calculator:
    """Calculator
    """

    version = "1.0.4"

    def __init__(self, mqttClient):
        """Constructor Calculator
        """
        log.logger.debug('✔︎ Init calculator')
        self.mqttClient = mqttClient
        self.payload = ''
        self.cd = WATERMETER_DATA
        self.pd = self.cd
        self.cdfilename = DATAFILE
        self.reportdatafile = REPORTFILE
        self.dayreportfile = DAYREPORTFILE
        self.elapsed_time = -1
        self.state = "waiting"
        self.firstRun = True

    def getResult(self, payload: str = ''):
        self.payload = json.loads(payload)
        if self.payload:
            self.__doCalc__()
            self.state = "ready"
        else:
            log.logger.error(f"No payload present!")
            return None
        return now.strftime(DATEFORMAT_CURRENT)

    def __checkPayload__(self, key, default):
        if key in self.payload:
            return self.payload[key]
        else:
            log.logger.debug(f'Missing key {key} - used default value {default}')
            return default

    def __checkFirstRun__(self, meterM3:float=0.00):
        """Init values for the first run
        """
        try:
          if(self.firstRun==False):
              return

          if (self.pd['last_total']['hour_m3']==0.00):
            self.pd['last_total']['hour_m3'] = meterM3

          if (self.pd['last_total']['day_m3']==0.00):
            self.pd['last_total']['day_m3'] = meterM3

          if (self.pd['last_total']['month_m3']==0.00):
            self.pd['last_total']['month_m3'] = self.cd['last_total']['month_m3']


        except BaseException as e:
          log.logger.error(f"😡 Error on  {__name__},  line: {e.__traceback__.tb_lineno}, message: {str(e)}")

    def __doCalc__(self):
        """Calculate all data based on the current payload
        """

        log.logger.debug('✔︎ Load previous data')
        self.__loaddata__()
        now = datetime.now()

        try:
            log.logger.info(' ✔︎ Start calculation')
            current_dts = datetime.strptime(self.__checkPayload__('timestamp', now.strftime(DATEFORMAT_TIMESTAMP)), DATEFORMAT_TIMESTAMP)
            previous_dts = datetime.strptime(self.pd['timestamp'] ,DATEFORMAT_TIMESTAMP)
            lastmonth_dts = datetime.strptime(self.__checkPayload__('last_month_measure_date', now.strftime(DATEFORMAT_DAY)), DATEFORMAT_DAY)

            if (DEBUG_DATA == True):
                log.logger.warning("✔︎ Date current:{}, prevoius:{},last month:{}, last year:{} ".format(current_dts,previous_dts,lastmonth_dts, previous_dts.strftime(DATEFORMAT_YEAR)))

            self.elapsed_time = current_dts - previous_dts
            if (DEBUG_DATA == True):
                log.logger.warning("✔︎ Elapsed Time:{} ".format(self.elapsed_time))

            # watermeter display data
            self.cd['date'] = current_dts.strftime(DATEFORMAT_DAY)
            self.cd['time'] = current_dts.strftime(TIME_FORMAT)

            # current water meter counter value
            self.cd['total_m3'] = float(self.__checkPayload__('total_m3', 0.00))
            # total m3
            meterM3 = float(round(self.cd['total_m3'], 3))

            # the last periode data from the meter payload
            self.cd['last_total']['month'] = lastmonth_dts.strftime(DATEFORMAT_MONTH)
            self.cd['last_total']['month_m3'] = float(self.__checkPayload__('last_month_total_m3', 0.00))

            # current / prev m3
            if(self.cd['last_total']['month_m3'] > 0.00):
                self.cd['ratio']=round(float(meterM3 / self.cd['last_total']['month_m3']), 2)

            # calculate the m3
            if(self.pd):

                log.logger.info(' ✔︎ new data calculation m3, liter')
                self.cd['m3']['current']=round(meterM3 - float(self.pd['total_m3']), 3)

                if(self.firstRun==True):
                    log.logger.info(' ✔︎ Init first run, try to adjust the prevoius values.')

                self.__checkFirstRun__(meterM3)

                self.cd['m3']['hour']=round(meterM3 - float(self.pd['last_total']['hour_m3']), 3)
                self.cd['m3']['day']=round(meterM3 - float(self.pd['last_total']['day_m3']), 3)
                self.cd['m3']['month']=round(meterM3 - float(self.pd['last_total']['month_m3']), 3)
                self.cd['m3']['year']=round(meterM3 - float(self.pd['last_total']['year_m3']), 3)

                # caclulate the liter
                self.cd['liter']['current']=round(float(self.cd['m3']['current']) * 1000.00, 2)
                self.cd['liter']['hour']=round(float(self.cd['m3']['hour']) * 1000.00, 2)
                self.cd['liter']['day']=round(float(self.cd['m3']['day']) * 1000.00, 2)
                self.cd['liter']['month']=round(float(self.cd['m3']['month']) * 1000.00, 2)
                self.cd['liter']['year']=round(float(self.cd['m3']['year']) * 1000.00, 2)


                if (DEBUG_DATA == True):
                    log.logger.warning("⁍ DATA: result = data[current,hour,day,month,year] - previous[current,hour,day,month,year]")
                    log.logger.warning("⁍ DATA: current m3   : {}={}-{}".format(self.cd['m3']['current'],meterM3, self.pd['total_m3']))
                    log.logger.warning("⁍ DATA: current hour : {}={}-{}".format(self.cd['m3']['hour'],meterM3, self.pd['last_total']['hour_m3']))
                    log.logger.warning("⁍ DATA: current day  : {}={}-{}".format(self.cd['m3']['day'],meterM3, self.pd['last_total']['day_m3']))
                    log.logger.warning("⁍ DATA: current month: {}={}-{}".format(self.cd['m3']['month'],meterM3, self.pd['last_total']['month_m3']))
                    log.logger.warning("⁍ DATA: current year : {}={}-{}".format(self.cd['m3']['year'],meterM3, self.pd['last_total']['year_m3']))

                # check update last total for hour
                if (DEBUG_DATA == True):
                    log.logger.warning(f"⁍ DATA: Check update last total hour: {current_dts.strftime(DATEFORMAT_HOUR)} prev: {previous_dts.strftime(DATEFORMAT_HOUR)}")

                if(current_dts.strftime(DATEFORMAT_HOUR) != previous_dts.strftime(DATEFORMAT_HOUR)):
                    # new hour
                    log.logger.debug(f"✔︎ Update last total hour: {current_dts.strftime(DATEFORMAT_HOUR)} prev: {previous_dts.strftime(DATEFORMAT_HOUR)}")
                    self.cd['last_total']['hour']=current_dts.strftime(DATEFORMAT_HOUR)
                    self.cd['last_total']['hour_m3']=meterM3
                    # new hour, store the previous data to
                    self.__printLogData()
                else:
                    self.cd['last_total']['hour']=self.pd['last_total']['hour']
                    self.cd['last_total']['hour_m3']=self.pd['last_total']['hour_m3']

                # check update last total for day
                if (DEBUG_DATA == True):
                    log.logger.warning(f"⁍ DATA: Check last total day update: {current_dts.strftime(DATEFORMAT_DAY)} prev: {previous_dts.strftime(DATEFORMAT_DAY)}")

                if(current_dts.strftime(DATEFORMAT_DAY) != previous_dts.strftime(DATEFORMAT_DAY)):
                    # new day
                    log.logger.debug(f"✔︎ Update last total day: {current_dts.strftime(DATEFORMAT_DAY)} prev: {previous_dts.strftime(DATEFORMAT_DAY)}")
                    self.cd['last_total']['day']=current_dts.strftime(DATEFORMAT_DAY)
                    self.cd['last_total']['day_m3']=meterM3
                    self.__savedayreport__()
                else:
                    self.cd['last_total']['day']=self.pd['last_total']['day']
                    self.cd['last_total']['day_m3']=self.pd['last_total']['day_m3']

                # check update last total for month
                if (DEBUG_DATA == True):
                    log.logger.warning(f"⁍ DATA: Check last total month update: {current_dts.strftime(DATEFORMAT_MONTH)} prev: {previous_dts.strftime(DATEFORMAT_MONTH)}")

                if(current_dts.strftime(DATEFORMAT_MONTH) != previous_dts.strftime(DATEFORMAT_MONTH)):
                    # new month
                    log.logger.debug(f"✔︎ Update last total month: {current_dts.strftime(DATEFORMAT_MONTH)} prev: {previous_dts.strftime(DATEFORMAT_MONTH)}")
                    self.cd['last_total']['month']=current_dts.strftime(DATEFORMAT_MONTH)
                    self.cd['last_total']['month_m3']=meterM3
                    # new month, store the reportdata
                    self.__savereportdata__()
                else:
                    self.cd['last_total']['month']=self.pd['last_total']['month']
                    self.cd['last_total']['month_m3']=self.pd['last_total']['month_m3']

                # check update last total for year
                if (DEBUG_DATA == True):
                    log.logger.warning(f"⁍ DATA: Check last year update: {current_dts.strftime(DATEFORMAT_YEAR)} prev: {previous_dts.strftime(DATEFORMAT_YEAR)}")

                if(current_dts.strftime(DATEFORMAT_YEAR) != previous_dts.strftime(DATEFORMAT_YEAR)):
                    # new year
                    log.logger.debug(f"✔︎ Update last total year: {current_dts.strftime(DATEFORMAT_YEAR)} prev: {previous_dts.strftime(DATEFORMAT_YEAR)}")
                    self.cd['last_total']['year']=current_dts.strftime(DATEFORMAT_YEAR)
                    self.cd['last_total']['year_m3']=meterM3
                else:
                    self.cd['last_total']['year']=self.pd['last_total']['year']
                    self.cd['last_total']['year_m3']=self.pd['last_total']['year_m3']

            self.cd['wmbusmeter']=self.payload

            # add periodes and timestamps
            log.logger.debug('✔︎ update periodes and timestamps')
            self.cd['timestamp']=self.__checkPayload__('timestamp', now.strftime(DATEFORMAT_TIMESTAMP))
            self.cd['periode']=now.strftime(DATEFORMAT_DAY)
            self.cd['month']=now.strftime(DATEFORMAT_MONTH)
            self.cd['year']=now.strftime(DATEFORMAT_YEAR)
            self.cd['elapsed_time']=str(self.elapsed_time)
            self.cd['last_update']=now.strftime(DATEFORMAT_CURRENT)
            self.cd['first_run']=self.firstRun
            self.cd['data_provider']=APP_STATEINFO['hostname']

            # water leak status
            log.logger.debug('✔︎ update water leak status')
            self.cd['alarm']=self.__checkPayload__('current_alarms', 'no alarm')
            self.cd['last_alarm']=self.__checkPayload__('previous_alarms', 'no alarm')

            # publish the new data
            log.logger.info(f' ✔︎ Publish the new data {MQTT_PUBLISH_TOPIC}')
            self.mqttClient.publish(MQTT_PUBLISH_TOPIC, json.dumps(self.cd))

            # save logging data
            self.__savedata__()

            return True

        except BaseException as e:
            log.logger.error(f"😡 Error on calculation: {__name__},  line: {e.__traceback__.tb_lineno}, message: {str(e)}")
            return False

    def __loaddata__(self):
        log.logger.info(f" ✔︎ get previous data from {self.cdfilename}")
        try:
            if (self.cdfilename):
                if os.path.isfile(self.cdfilename):
                    with open(self.cdfilename, "r") as f:
                        _prevdat=f.read()
                        log.logger.debug(f"✔︎ previous data loaded {self.cdfilename}")
                        self.pd=json.loads(_prevdat)
                        self.firstRun = False
                    return True
                else:
                    log.logger.debug(f"✔︎ init previous data, datafile not found!")
                    self.pd=self.cd
                    self.firstRun = True
                    return True
        except BaseException:
            log.logger.error(f"😡 Error reading file {self.cdfilename}")
        return None

    def __savedata__(self):
        """saves the serial data to definied json file

        Returns:
            boolean: {True}: File saved -- {False}: Error
        """
        if(self.cdfilename):
            try:
                with open(self.cdfilename, 'w') as f:
                    f.write(json.dumps(self.cd))
                log.logger.debug(f'✔︎ saved current data to {self.cdfilename}')
                return True
            except BaseException:
                log.logger.error(f"😡 Error save history data to file {self.cdfilename}")
                return False
        return False

    def __savedayreport__(self):
        if(self.dayreportfile):
            try:
                datalist=(
                    self.pd['device'],
                    self.pd['deviceid'],
                    self.pd['month'],
                    self.pd['timestamp'],
                    self.pd['total_m3'],
                    self.pd['m3']['day'],
                    self.pd['m3']['month'],
                    self.pd['m3']['year'],
                    self.pd['last_total']['day_m3'],
                    self.pd['last_total']['month_m3'],
                    self.pd['last_total']['year_m3'],
                    self.pd['alarm'],
                    self.cd['last_alarm']
                )
                output = ",".join(map(str, datalist))
                with open(self.dayreportfile, 'a+') as f:
                    f.write(output + '\n')
                log.logger.debug(f'✔︎ saved report data to {self.reportdatafile}')
            except BaseException:
                log.logger.error(f"😡 Error save report data to file {self.reportdatafile}")
                return False
        return True

    def __savereportdata__(self):
        if(self.reportdatafile):
            try:
                datalist = (
                    self.pd['device'],
                    self.pd['deviceid'],
                    self.pd['date'],
                    self.pd['time'],
                    self.pd['timestamp'],
                    self.pd['total_m3'],
                    self.pd['last_total']['month_m3'],
                    self.pd['last_total']['year_m3'],
                    self.pd['alarm'],
                    self.cd['last_alarm']
                )
                output = ",".join(map(str, datalist))
                with open(self.reportdatafile, 'a+') as f:
                    f.write(output + '\n')
                log.logger.debug(f'✔︎ saved report data to {self.reportdatafile}')
            except BaseException:
                log.logger.error(f"😡 Error save report data to file {self.reportdatafile}")
                return False
        return True

    def __printLogData(self):
        if(DATALOG_ENABLED or DATALOG_ENABLED):
            dlist = (
                self.pd['device'],
                self.pd['deviceid'],
                self.pd['date'],
                self.pd['time'],
                self.pd['timestamp'],
                self.pd['total_m3'],
                self.pd['last_total']['month_m3'],
                self.pd['last_total']['year_m3'],
                self.pd['alarm'],
                self.cd['last_alarm']
            )
            csvdata = ",".join(map(str, dlist))

            if DATALOG_ENABLED:
                print(csvdata, end="")

            if(DATALOG_FILENAME):
                with open(DATALOG_FILENAME, 'a+') as f:
                    f.write(csvdata + '\n')
