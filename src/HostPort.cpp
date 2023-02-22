/*
 * HostPort class to handle serial communication - source file
 */

#include "HostPort.h"

//Begins
bool HostPort::begin(unsigned int port, unsigned int baud) {
    _terminator = TERMINATOR;
    _header = HEADER;
    _timeout = TIMEOUT;
    _port = port;
    _baud = baud;
    if (init(port, baud, _timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

bool HostPort::begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator) {
    _terminator = terminator;
    _header = header;
    _timeout = TIMEOUT;
    _port = port;
    _baud = baud;
    if (init(port, baud, _timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

bool HostPort::begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator, unsigned int timeout) {
    _terminator = terminator;
    _header = header;
    _port = port;
    _baud = baud;
    _timeout = timeout;
    if (init(port, baud, timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

HostPort::~HostPort() {
    serial.~Serial();
}

HostPort::HostPort() {
    //empty
}

//set/get funs
bool HostPort::isInit(void) {
    return (bool)serial;
}
bool HostPort::setPort(unsigned int port) {
    //if (!serial) {
        _port = port;
        return true;
    //}
    //return false;
}
bool HostPort::setBaud(unsigned int baud) {
   // if (!serial) {
        _baud = baud;
        return true;
    //}
    //return false;
}
bool HostPort::setHeader(unsigned int header) {
    //if (!serial) {
        _header = header;
        return true;
    //}
    //return false;
}
bool HostPort::setTerminator(unsigned int terminator) {
    //if (!serial) {
        _terminator = terminator;
        return true;
    //}
    //return false;
}
bool HostPort::setTimeout(unsigned int timeout) {
    //if (!serial) {
        _timeout = timeout;
        return true;
    //}
    //return false;
}
unsigned int HostPort::getPort(void) {
    return _port;
}
unsigned int HostPort::getBaud(void) {
    return _baud;
}
unsigned int HostPort::getHeader(void) {
    return _header;
}
unsigned int HostPort::getTerminator(void) {
    return _terminator;
}
unsigned int HostPort::getTimeout(void) {
    return _timeout;
}

//reset & close fun
bool HostPort::close(void) {
    return serial.end();
}
bool HostPort::restart(void) {
    serial.end();
    return init(_port, _baud, _timeout);
}
bool HostPort::flush(void) {
    if (isInit()) {
        serial.flush();
        return true;
    }
    return false;
}

//write
bool HostPort::write(unsigned char* packetPtr, unsigned int size) {

    if (!serial) {
        return false;
    }

    /*if (size > TX_BUF_SIZE) {
        return false;
    }*/

    size_t c = 0; //counter for sent size
    unsigned char* _tx_buf = (unsigned char*) malloc(size+8);

    //put header
    _tx_buf[c++] = _header & MASK;
    _tx_buf[c++] = (_header >> 8) & MASK;
    _tx_buf[c++] = (_header >> 16) & MASK;
    _tx_buf[c++] = (_header >> 24) & MASK;

    //add data
    for (int i = 0; i < size; ++i) _tx_buf[c++] = *(packetPtr + i);

    //put terminator
    _tx_buf[c++] = _terminator & MASK;
    _tx_buf[c++] = (_terminator >> 8) & MASK;
    _tx_buf[c++] = (_terminator >> 16) & MASK;
    _tx_buf[c++] = (_terminator >> 24) & MASK;

    //put start bytes in buf
    serial.write(_tx_buf, c);

    return true;

}

//read
bool HostPort::read(unsigned char* packetPtr, unsigned int size) {

    if (!serial) {
        memset(packetPtr,0,size);
        return false;
    }

    if (_isFirstRead) {
        serial.flush();
        _isFirstRead = false;
    }

    unsigned int c = 0;
    unsigned char b;
    while (1) {
        if (c < 4) { //check header
            if (serial.read(&b, 1)>0) {
                if (b == ((_header >> (8 * c)) & MASK)) {
                    c++;
                    continue;
                }
            } else { 
                return false; 
            }
        } else if (c==4) { //read data
            if (serial.read(packetPtr, size) == 0) {
                return false; 
            };
            c++;
            continue;
        } else {
            if (serial.read(&b, 1) > 0) {
                if (b == ((_terminator >> (8 * (c - 5))) & MASK)) {
                    c++;
                    if (c == 9) { //all ok
                        return true;
                    }
                    continue;
                }
            } else { 
                break;
            }
        }
        break;
    }
    return false; //is ok???
}

//init
bool HostPort::init(unsigned int port, unsigned int baud, unsigned int timeout) {
    _isInit = true;
    return serial.begin(port, baud, timeout);
}


#ifdef __USEBOOST__

//readpy
bool HostPort::readpy (boost::python::object obj) {
    PyObject* pobj = obj.ptr();
    Py_buffer pybuf;
    if(PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE)!=-1){
        bool exit = read((unsigned char*) pybuf.buf, pybuf.len);
        PyBuffer_Release(&pybuf);
        return exit;
    }
    return false;
}

//writepy
bool HostPort::writepy (boost::python::object obj) {
    PyObject* pobj = obj.ptr();
    Py_buffer pybuf;
    if(PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE)!=-1){
        bool exit = write((unsigned char*) pybuf.buf, pybuf.len);
        PyBuffer_Release(&pybuf);
        return exit;
    }
    return false;
}

#endif