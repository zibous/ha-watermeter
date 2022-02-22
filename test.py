#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from datetime import datetime

t = "2011-01-21 02:37:21"
t = "2022-02-22 07:35:30"

print(date2Local(t))

def dateToLocal(t):
   return datetime.fromisoformat(t) + (datetime.now() - datetime.utcnow())



