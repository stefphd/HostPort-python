# HostPort-python

Python binding of the HostPort  class for communication from and to the microcontroller.

This is the (Python) host-side implementation of the Host Port class. The class is internally implemented in C++ and interfaced to Pythn using  [Boost.Python](https://github.com/boostorg/python) to generate a dynamic library importable in Python as a module. The Python module is thus just a wrapper around a C++ class.

Tested on Windows 10 and Linux (ArchLinux) with Python 3.10.

## Contens

* `./src`: C/C++ source files
* `./include`: C/C++ include files
* `./.vscode`: setting folder for Visual Studio Code
* `Makefile`: makefile to compile and buid the dynamic library
* `testing.py`: testing Python script for the Host Port class
* `README.md`: this file

## Quick start

Usage for the Host Port class is (requires `numpy`):

```python
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
```

To check the connection status:

```python
# Check initialization
if not h.isInit(): #or hostport.isInit or hostport.begin(port, baud)
    print('Unable to connect')

# Check read or write
if not flag:
    print('Unable to read or write')
```

To show all properties:

```python
# Show all properties
print(h)
```

To restart and flush the port

```python
# Restart
h.restart()

# Flush
h.flush()
```

Finally, to close and clear

```python
# Close
h.close()
```

In addition, a number of set & get methods is provided (use `restart` after set to make the change effective):

```python
# Set methods
h.setPort(3)
h.setBaud(9600)
h.setHeader(0xABDE1234)
h.setTerminator(0x1234ABCD)
h.setTerminator(200)
h.restart() # Must use restart to make the change effective

# Get methods
print(h.getPort())
print(h.getBaud())
print(h.isInit())
print(hex(h.getHeader()))
print(hex(h.getTerminator()))
print(hex(h.getTimeout()))
```

## Building

Pre-requisites for building are

* *Python3*
* *Boost.Python*
* *Make*

For example, on ArchLinux you can install via pacman using

```shell
pacman -S python3 boost make
```

Compilation is performed using the *Makefile*

```shell
make
```

Before using the *Makefile*, change library and include so as to match your Python version (3.10 is used at the moment). For building in Windows you may use [Mingw-w64](https://www.mingw-w64.org/) with [MSYS2](https://www.msys2.org/) (requires *mingw-w64-x86_64-boost*).

Other make commands are: 

* `make clean` to clean
* `make remake` to clean and make
* `make test` to run the `testing.py` script for testing
* `make all` to make and test

## TODO list

* Implement boolean operator in Python binding
