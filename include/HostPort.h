
#ifndef HOSTPORT_H
#define HOSTPORT_H

#include "Serial.h"
#include <memory> 
#include <iostream>

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

/*! \brief A class for host communication via serial port.
    \details Class for host communication via serial port with a specified communication protocol.
    Data packet consists of
    - 4-bytes header
    - data (with specified length)
    - 4-bytes terminator
    No check sum is performed.
    \author Stefano Lovato
    \date 2022
*/
class HostPort {
public:

    /*! \brief Constructor.
		\details Default constructor. 
	*/
    HostPort();

    /*! \brief Desctructor.
		\details Default destructor. It automatically destroy the serial object.
	*/
    ~HostPort();

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with default header, terminator, and timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud);

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with given header and terminator and default timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \param header The header to use (as a HEX number of 4-bytes).
        \param terminator The terminator to use (as a HEX number of 4-bytes).
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator);

    /*! \brief Begin the host port.
		\details Function to begin the host port communication with given header, terminator and timeout.
        \param port The serial port to use.
        \param baud The baudrate of the serial communication.
        \param header The header to use (as a HEX number of 4-bytes).
        \param terminator The terminator to use (as a HEX number of 4-bytes).
        \param timeout The timeout of the serial communcication.
        \return true if success, false otherwise.
	*/
    bool begin(unsigned int port, unsigned int baud, unsigned int header, unsigned int terminator, unsigned int timeout);
    
    /*! \brief Read data buffer.
		\details Function to read a data buffer with given length from the host port.
        Host port must be opened before.
        \param packetPtr The pointer to the data buffer.
        \param size The size of the data buffer.
        \return true if success, false otherwise.
	*/
    bool read(unsigned char* packetPtr, unsigned int size);

    /*! \brief Write data buffer.
		\details Function to write a data buffer with given length to the host port.
        Host port must be opened before.
        \param packetPtr The pointer to the data buffer.
        \param size The size of the data buffer.
        \return true if success, false otherwise.
	*/
    bool write(unsigned char* packetPtr, unsigned int size); 

    /*! \brief Close the host port.
		\details Function to close the host port.
        Host port must be opened before.
        \return true if success, false otherwise.
	*/
    bool close(void); 

    /*! \brief Restart the host port.
		\details Function to restart the host port.
        Host port must be opened before.
        \return true if success, false otherwise.
	*/
    bool restart(void);

    /*! \brief Set the serial port.
		\details Function to set the serial port. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setPort(unsigned int port);

    /*! \brief Set the baudrate.
		\details Function to set the baudrate of the serial communiation. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setBaud(unsigned int baud); 

    /*! \brief Set the header.
		\details Function to set the header of the host communication. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setHeader(unsigned int header); //set the header

    /*! \brief Set the terminator.
		\details Function to set the terminator of the host communication. The restart function must be called to make the change effective.
        \return true if success, false otherwise.
	*/
    bool setTerminator(unsigned int terminator); 

    /*! \brief Get the serial port.
		\details Function to get the serial port used for the host communication. 
        \return The serial port number.
	*/
    unsigned int getPort(void);

    /*! \brief Get the baudate.
		\details Function to get the baudrate used for the serial communication. 
        \return The baudrate.
	*/
    unsigned int getBaud(void);
    
    /*! \brief Get the header.
		\details Function to get the header used for the host communication. 
        \return The header.
	*/
    unsigned int getHeader(void); 

    /*! \brief Get the terminator.
		\details Function to get the terminator used for the host communication. 
        \return The terminator.
	*/
    unsigned int getTerminator(void); 

     /*! \brief Check if host port is open.
        \details Function to check if the host port has been open successfully.
        \return true if the host port is open, false otherwise.
    */
    bool isInit(void); //check if is init
    
    /*! \brief Check if host port is open.
        \details Boolean operator to check if the host port has been open successfully.
        \return true if the host port is open, false otherwise.
    */
    operator bool() { return isInit(); }
    
    static constexpr unsigned int HEADER = 0xFF812345; //!< Default header.
    static constexpr unsigned int TERMINATOR = 0xFF8CABDE; //!< Default terminator.
    static constexpr unsigned int TIMEOUT = 100; //!< Default timeout.
    static constexpr unsigned int TX_BUF_SIZE = 1024; //!< Default timeout.

    /*! \brief Print details on the object.
        \details Print operator to show the details on the HostPort object.
        \param os The stram object.
        \param hp The HostPort object.
        \return The stream object.
    */
    friend std::ostream& operator<<(std::ostream& os, HostPort hp) {
        os << "HostPort at:\t0x" << std::hex << (uint64_t) &hp << std::dec << std::endl;
        if (!hp._isInit) {
            os << "Port:\t\t" << "Undefined" << std::endl;
            os << "Baudrate:\t" << "Undefined" << std::endl;
            os << "Header:\t\t" << "Undefined" << std::endl;
            os << "Terminator:\t" << "Undefined" << std::endl;
            os << "Timeout:\t" << "Undefined" << std::endl;
        } else {
            os << "Port:\t\t" << hp._port << std::endl;
            os << "Baudrate:\t" << hp._baud << std::endl;
            os << "Header:\t\t0x" << std::hex << hp._header << std::dec << std::endl;
            os << "Terminator:\t0x" << std::hex << hp._terminator << std::dec << std::endl;
            os << "Timeout:\t" << hp._timeout << std::endl;
        }
        if (hp.isInit()) os << "isInit:\t\t" << "True"  << std::endl;
        else os << "isInit:\t\t" << "False" << std::endl;
        return os;
    }

    /*! \brief Read data for Pythoin.
        \details Wrapper function to read data from Python binding.
        \param obj The Python object, created using numpy.empty(SIZE, dtype=numpy.DATATYPE)
        \return true if success, false otherwise.
    */
    bool readpy(boost::python::object obj);

    /*! \brief Write data for Pythoin.
        \details Wrapper function to write data from Python binding.
        \param obj The Python object, created using numpy.empty(SIZE, dtype=numpy.DATATYPE)
        \return true if success, false otherwise.
    */
    bool writepy(boost::python::object obj);

private:
    Serial serial; //!< Serial object.
    unsigned int _port = 0; //!< Serial port.
    unsigned int _baud = 0; //!< Baudrate.
    unsigned int _terminator = 0; //!< Terminator bytes.
    unsigned int _header = 0; //!< Header bytes.
    unsigned int _timeout = 0; //!< Timeout.
    unsigned char _tx_buf[TX_BUF_SIZE]; //!< Tx buffer.
    bool _isFirstRead = false; //!< True if firt packer read.
    bool _isInit; //!< True if begin (but maybe not isInit() returns false...).
    bool init(unsigned int port, unsigned int baud, unsigned int timeout); //!< Private initialization function
    static constexpr unsigned int MASK = 0xFF; //!< Mask for parsing.
};

#endif