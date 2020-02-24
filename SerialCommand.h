/**
 * SerialCommand - A Wiring/Arduino library to tokenize and parse commands
 * received over a serial port.
 * 
 * Copyright (C) 2019 Christopher De Pasquale
 * Copyright (C) 2012 Stefan Rado
 * Copyright (C) 2011 Steven Cogswell <steven.cogswell@gmail.com>
 *                    http://husks.wordpress.com
 * 
 * Version 20191116
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __SERIALCOMMAND_H__
#define __SERIALCOMMAND_H__


// To print debug messages, uncomment the following line and
// ensure that Serial.begin(BAUDRATE) is called in setup()
//#define SERIALCOMMAND_DEBUG

// Max command length including args (cmd buffer size)
#define SERIALCOMMAND_BUFFER 32

#define SERIALCOMMAND_DEFAULT_BUFFER 32

// Max command length excluding null terminator and args
#define SERIALCOMMAND_MAXCOMMANDLENGTH 8

// Terminator character for a command
#define SERIALCOMMAND_DEFAULT_TERM '\n'

// Null-terminated delimiter between args/cmd (required by strtok_r)
#define SERIALCOMMAND_DEFAULT_DELIM " "

// Only allow printable characters by default
#define SERIALCOMMAND_DEFAULT_PRINTABLEONLY true

#if defined(WIRING) && WIRING >= 100
    #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif
#include <string.h>
#include <Stream.h>

class SerialCommandBuilder;

/**
 * Interface provided to handler functions. 
 * Includes next() func to get next arg
*/
class SerialCmdInstance {

public:

    /**
     * Retrieve the next token (word/arg seperated by delimiter) from the 
     * command buffer. Returns NULL if no more tokens exist.
     */
    char *next();

    /** Length of received data (i.e. excludes command str and space) */
    int dataLen();
};

// Registered command handler callback type
typedef void (*CmdRecvFn_t)(SerialCmdInstance*);

// Default (i.e. invalid) command handler callback type
typedef void (*CmdRecvDefaultFn_t)(const char*);

class SerialCommand : public SerialCmdInstance {

    // Represents a registered command & handler pair
    struct RegisteredCmd {
        char cmd[SERIALCOMMAND_MAXCOMMANDLENGTH + 1];
        CmdRecvFn_t handler;
    };


    // Stream from which commands are read
    const Stream& recvStream;

    RegisteredCmd *cmdList = NULL;
    byte cmdCount = 0;

    // Pointer to the default handler function
    CmdRecvDefaultFn_t defaultHandler = NULL;

    // Null-terminated delimiter string between args and cmd
    const char *delim;

    // Terminator char
    const char term = SERIALCOMMAND_DEFAULT_TERM;

    // Current command buffer & position
    char cmdBuf[SERIALCOMMAND_BUFFER + 1];
    byte bufPos;
    
    // State var used by strtok_r
    char *last = NULL;

    const bool printableOnly;

public:
	/*
    SerialCommand(SerialCommandBuilder& builder);
    */
    
    /**
     * stream           Stream which receives commands
     * delimiter        Null-terminated delimiter string between command/arguments
     * terminator       Character representing the end of a command and arguments
     * printableOnly    If true, only printable chars are added to buffer
     */
    SerialCommand(Stream& stream, const char *delimiter, char terminator, bool printableOnly);

    /**
     * stream       Stream which receives commands
     * delimiter    Null-terminated delimiter string between command/arguments
     * terminator   Character representing the end of a command and arguments
     */
    SerialCommand(Stream& stream, const char *delimiter, char terminator);
    SerialCommand(Stream& stream);
    SerialCommand();
    
    /** Register a new command */
    void addCommand(const char *cmd, CmdRecvFn_t function);
    
    /**
     * Handler to call when an invalid command is received.
     * Handler must take a const char* argument (the buffer for invalid cmd)
     */
    void setDefaultHandler(CmdRecvDefaultFn_t function);

    /**
     * 'Update' function, should be called each loop()
     * 
     * Checks input stream for characters, and assembles them into a buffer.
     * When the terminator character is seen, it starts parsing the buffer
     * for a prefix command, and calls matching command handler (registered 
     * with addCommand())
     */
    void readSerial();

    /** Clear all data */
    void clearBuffer();     

    /**
     * Retrieve the next token (word/arg seperated by delimiter) from the 
     * command buffer. Returns NULL if no more tokens exist.
     */
    char *next();
};

#endif
