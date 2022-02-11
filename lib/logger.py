#!/usr/bin/python3
# -*- coding: utf-8 -*-
import logging
import sys
import traceback
from time import gmtime, strftime
from config import *


class CustomFormatter(logging.Formatter):
    """Logging colored formatter, adapted from https://stackoverflow.com/a/56944256/3638629"""

    grey = '\x1b[38;21m'
    blue = '\x1b[38;5;39m'
    yellow = '\x1b[38;5;226m'
    red = '\x1b[38;5;196m'
    bold_red = '\x1b[31;1m'
    reset = '\x1b[0m'

    def __init__(self, fmt):
        super().__init__()
        self.fmt = fmt
        self.FORMATS = {
            logging.DEBUG: self.grey + self.fmt + self.reset,
            logging.INFO: self.blue + self.fmt + self.reset,
            logging.WARNING: self.yellow + self.fmt + self.reset,
            logging.ERROR: self.red + self.fmt + self.reset,
            logging.CRITICAL: self.bold_red + self.fmt + self.reset
        }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        date_fmt = "%Y/%m/%d %H:%M:%S"
        formatter = logging.Formatter(log_fmt,date_fmt)
        formatter.default_msec_format = '%s.%03d'
        return formatter.format(record)

class Log(object):
    """Logging wrapper for better output
    """

    def __init__(self, name: str='applogger', level: int = logging.DEBUG, logDir: str = None):
        """Constructor application logger

        Args:
            name (str, optional): [description]. Defaults to 'applogger'.
            level (int, optional): [description]. Defaults to logging.DEBUG.
            enableLogFile (bool, optional): [description]. Defaults to False
        """
        self.logger = logging.getLogger(name)
        self.logger.setLevel(level)
        self.loglevel = level
        self.logDir = logDir

        if(logDir):
            # use log file
            fh = logging.FileHandler(self.logDir + '%s.log' % name, 'w')
            self.logger.addHandler(fh)

        fmt = "%(asctime)s.%(msecs)03d  - %(levelname)s: %(message)s (%(name)s: Line %(lineno)d)"

        sh = logging.StreamHandler()
        sh.setFormatter(CustomFormatter(fmt))
        self.logger.addHandler(sh)

        sys.excepthook = self.handle_excepthook

    def debug(self, msg):
        self.logger.debug(msg)

    def info(self, msg):
        self.logger.info(msg)

    def warning(self, msg):
        self.logger.warning(msg)

    def error(self, msg):
        self.logger.error(msg)

    def critical(self, msg):
        self.logger.critical(msg)

    def print(self, msg: str = ' ', end: str = '\r'):
        if(self.loglevel < 100):
            print(msg, end)

    def handle_excepthook_debug(self, type, message, stack):
        self.logger.critical(f'An unhandled exception occured: {message}. Traceback: {traceback.format_tb(stack)}')

    def handle_excepthook(self, type, message, stack):
        self.logger.error(f'An unhandled exception occured: {message}. Traceback: {traceback.extract_tb(stack,1)}')

