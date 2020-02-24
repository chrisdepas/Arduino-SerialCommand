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
/*
#include "SerialCommandBuilder.h"

SerialCommandBuilder& SerialCommandBuilder::setStream(Stream& stream) {
    this->stream = stream;
    return *this;
}

SerialCommandBuilder& SerialCommandBuilder::setDelim(const char* delim) {
    this->delim = delim;
    return *this;
}

SerialCommandBuilder& SerialCommandBuilder::setTerm(char term) {
    this->term = term;
    return *this;
}

SerialCommandBuilder& SerialCommandBuilder::setPrintableCharsOnly(bool printableOnly) {
    this->printableOnly = printableOnly;
    return *this;
}

SerialCommand* SerialCommandBuilder::build() {
    return new SerialCommand(this);
}*/