#!/usr/bin/env python3

#-------------------------------------------------------------#
# Testing script for the Python binding of the HostPort class #
#-------------------------------------------------------------#

# Import the python modules
import hostport
import numpy as np

# Variables
port = 3 #serial port
baud = 115200 #baudrate
len = 64 #size of the buffer to read
data = np.array([0, 1, 2, 3, 4, 5], dtype=np.single) #buffer to write

# Instantiate a new object
h = hostport.HostPort()

# Start the communication
flag = h.begin(port, baud)
# flag = h.begin(port, baud, header, terminator) #set also header and terminator
# flag = h.begin(port, baud, header, terminator, timeout) #set also timeout

# read 'len' elements of single, return true if success
data_read = np.empty(len, dtype=np.single) #buffer must be defined as a numpy array
flag = h.read(data_read)

# write buffer (must be a numpy array)
flag = h.write(data)

# Check initialization
if not h.isInit(): #or hostport.isInit or hostport.begin(port, baud)
    print('Unable to connect')

# Check read or write
if not flag:
    print('Unable to read or write')

# Show all properties
print(h)

# Set methods
h.setPort(3)
h.setBaud(9600)
h.setHeader(0xABDE1234)
h.setTerminator(0x1234ABCD)

# Get methods
print(h.getPort())
print(h.getBaud())
print(h.isInit())
print(hex(h.getHeader()))
print(hex(h.getTerminator()))

# Restart
h.restart()

# Close
h.close()
