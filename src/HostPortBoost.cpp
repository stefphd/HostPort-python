/*
 * Boost.Python definition for the Python binding
 */

#include "HostPort.h"
#include "Serial.h"


#ifdef __USEBOOST__

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

using namespace boost::python;

//begin wrappers
bool (HostPort::*begin2args)(unsigned int, unsigned int) = &HostPort::begin;
bool (HostPort::*begin4args)(unsigned int, unsigned int, unsigned int, unsigned int) = &HostPort::begin;
bool (HostPort::*begin5args)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int) = &HostPort::begin;


// python module
BOOST_PYTHON_MODULE(hostport)
{   

    //HostPort class
    class_<HostPort>("HostPort")
        .def("begin", begin2args)
        .def("begin", begin4args)
        .def("begin", begin5args)
        .def("restart", &HostPort::restart)
        .def("flush", &HostPort::flush)
        .def("close", &HostPort::close)
        .def("read", &HostPort::readpy)
        .def("write", &HostPort::writepy)
        .def("isInit", &HostPort::isInit)
        .def("getPort", &HostPort::getPort)
        .def("getBaud", &HostPort::getBaud)
        .def("getHeader", &HostPort::getHeader)
        .def("getTerminator", &HostPort::getTerminator)
        .def("getTimeout", &HostPort::getTimeout)
        .def("setPort", &HostPort::setPort)
        .def("setBaud", &HostPort::setBaud)
        .def("setHeader", &HostPort::setHeader)
        .def("setTerminator", &HostPort::setTerminator)
        .def("setTimeout", &HostPort::setTimeout)
        .def(self_ns::str(self)) //operator <<
        .def_readonly("header", &HostPort::HEADER)
        .def_readonly("terminator", &HostPort::TERMINATOR)
        .def_readonly("timeout", &HostPort::TIMEOUT)
        ;
    ;
};

#endif