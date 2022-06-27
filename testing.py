#!/usr/bin/env python

#-------------------------------------------------------------#
# Testing script for the Python binding of the HostPort class #
#-------------------------------------------------------------#

import hostport
import numpy as np

h = hostport.HostPort()

h.begin(2, 115200)
print(h)

h.begin(2, 115200, 0xABCD1122, 0x1122DEFA)
print(h)

h.begin(2, 115200, 0xABCD1122, 0x1122DEFA, 500)
print(h)

h.setPort(3)
h.setBaud(9600)
h.setHeader(0xABDE1234)
h.setTerminator(0x1234ABCD)
print(h.getPort())
print(h.getBaud())
print(h.isInit())
print(hex(h.getHeader()))
print(hex(h.getTerminator()))

h.close()
print(h)

h.restart()
print(h)

#HostPort::read(unsigned char* packetPtr, unsigned int size) 
buf = np.empty(64, dtype=np.single)
print(buf)
h.read(buf)
print(buf)
h.write(buf)