#!/usr/bin/python3

from pylibftdi import BitBangDevice
import argparse
from time import sleep

parser = argparse.ArgumentParser()
parser.add_argument("-c", default=1, help="Impulses count")
parser.add_argument("-t", default=1, help="Impulse time (ms)")
parser.add_argument("-i", default=1, help="Interval (ms)")


args = parser.parse_args()

count = int(args.c)
time = float(args.t) / 1000
interval = float(args.i) / 1000

with BitBangDevice() as bb:
    bb.direction = (1 << 0)
    while count:
        bb.port |= (1 << 0)
        sleep(time)
        bb.port &= ~(1 << 0)
        sleep(interval)
        count = count - 1
