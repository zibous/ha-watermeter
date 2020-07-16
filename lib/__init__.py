
if __name__ == '__main__' and __package__ is None:
    from os import sys, path
    PARENT_DIR = path.dirname(path.dirname(path.abspath(__file__)))
    sys.path.append(PARENT_DIR)

import lib.logger as logger
import lib.calculator as calc
name = 'lib'
