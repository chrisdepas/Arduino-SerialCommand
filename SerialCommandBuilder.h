/**
 * SerialCommand - A Wiring/Arduino library to tokenize and parse commands
 * received over a serial port.
 * 
 * Copyright (C) 2019 Christopher De Pasquale
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

#ifndef __SERIALCOMMANDBUILDER_H__
#define __SERIALCOMMANDBUILDER_H__
/*
#if defined(WIRING) && WIRING >= 100
    #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif
#include <string.h>*/
#include <Stream.h>
#include "SerialCommand.h"

class SerialCommand;


class SerialCommandBuilder {

friend class SerialCommand;

protected:
    Stream& stream     = Serial;
    char* delim        = SERIALCOMMAND_DEFAULT_DELIM;
    char term          = SERIALCOMMAND_DEFAULT_TERM;
    bool printableOnly = SERIALCOMMAND_DEFAULT_PRINTABLEONLY;

public:
    /** Set receiving stream, to be read from for commands */
    SerialCommandBuilder* setStream(Stream& stream);
    
    SerialCommandBuilder* setDelim(const char* delim);
    
    /** Set command/arg string terminator character */
    SerialCommandBuilder* setTerm(char term);

    /**
     * If set to false, non-printable characters will be parsed.
     * Otherwise, all non-printable characters will be ignored (default behaviour)
     */
    SerialCommandBuilder* setPrintableCharsOnly(bool printableOnly);

    /**
     * Create SerialCommand instance using builder options 
     * Uses new(), so must be free()'d
     * Can be called more than once, e.g.:
     *  cmd1 = cmdBuilder.build();
     *  cmdBuilder.setStream(Stream3);
     *  cmd2 = cmdBuilder.build();
     */
    SerialCommand* build();
};

#endif