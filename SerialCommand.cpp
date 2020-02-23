/**
 * SerialCommand - A Wiring/Arduino library to tokenize and parse commands
 * received over a serial port.
 *
 * Copyright (C) 2019 Christopher De Pasquale
 * Copyright (C) 2012 Stefan Rado
 * Copyright (C) 2011 Steven Cogswell <steven.cogswell@gmail.com>
 *                    http://husks.wordpress.com
 * 
 * Version 20120522
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
#include "SerialCommand.h"
#include "SerialCommandBuilder.h"

SerialCommand::SerialCommand() 
    : recvStream(Serial),
      delim(SERIALCOMMAND_DEFAULT_DELIM), 
      term(SERIALCOMMAND_DEFAULT_TERM),
      printableOnly(SERIALCOMMAND_DEFAULT_PRINTABLEONLY) {
    
    clearBuffer();
}

SerialCommand::SerialCommand(Stream& stream) 
    : recvStream(stream), 
      delim(SERIALCOMMAND_DEFAULT_DELIM),
      term(SERIALCOMMAND_DEFAULT_TERM),
      printableOnly(SERIALCOMMAND_DEFAULT_PRINTABLEONLY) {
    
    clearBuffer();
}

SerialCommand::SerialCommand(Stream& stream, const char *delimiter, char terminator) 
    : recvStream(stream), 
      delim(delimiter), 
      term(terminator),
      printableOnly(SERIALCOMMAND_DEFAULT_PRINTABLEONLY) {

    clearBuffer();
}

SerialCommand::SerialCommand(SerialCommandBuilder& builder) : 
    recvStream(builder.stream),
    printableOnly(builder.printableOnly),
    delim(builder.delim),
    term(builder.term),

}

void SerialCommand::addCommand(const char *cmd, CmdRecvFn_t handler) {
  
#ifdef SERIALCOMMAND_DEBUG
    Serial.print("Registering command '");
    Serial.print(cmd);
    Serial.print("' ");
    Serial.print(cmdCount);
    Serial.println(" commands currently registered")
#endif

    cmdList = (RegisteredCmd*) realloc(cmdList, (cmdCount + 1) * sizeof(RegisteredCmd));
    strncpy(cmdList[cmdCount].cmd, cmd, SERIALCOMMAND_MAXCOMMANDLENGTH);
    cmdList[cmdCount].handler = handler;
    cmdCount++;
}

void SerialCommand::setDefaultHandler(CmdRecvDefaultFn_t handler) {
    defaultHandler = handler;
}

void SerialCommand::readSerial() {
    while (recvStream.available() > 0) {
        char inChar = recvStream.read();   // Read single available character, there may be more waiting
        #ifdef SERIALCOMMAND_DEBUG
            Serial.print(inChar);   // Echo back to serial stream
        #endif

        if (inChar == term) {     // Check for the terminator (default '\r') meaning end of command
            #ifdef SERIALCOMMAND_DEBUG
                Serial.print("Received: ");
                Serial.println(cmdBuf);
            #endif

            char *command = strtok_r(cmdBuf, delim, &last);   // Search for command at start of buffer
            if (command != NULL) {
                boolean matched = false;
                for (int i = 0; i < cmdCount; i++) {
                    #ifdef SERIALCOMMAND_DEBUG
                        Serial.print("Comparing [");
                        Serial.print(command);
                        Serial.print("] to [");
                        Serial.print(cmdList[i].cmd);
                        Serial.println("]");
                    #endif

                    // Compare the found command against the list of known commands for a match
                    if (strncmp(command, cmdList[i].cmd, SERIALCOMMAND_MAXCOMMANDLENGTH) == 0) {
                        #ifdef SERIALCOMMAND_DEBUG
                            Serial.print("Matched Command: ");
                            Serial.println(command);
                        #endif

                        // Execute the stored handler function for the command
                        (*cmdList[i].handler)(this);
                        matched = true;
                        break;
                    }
                }
                if (!matched && (defaultHandler != NULL)) {
                    (*defaultHandler)(command);
                }
            }
            clearBuffer();
        }
        else if (isprint(inChar)) {     // Only printable characters into the buffer
            if (bufPos < SERIALCOMMAND_BUFFER) {
              cmdBuf[bufPos++] = inChar;  // Put character into buffer
              cmdBuf[bufPos] = '\0';      // Null terminate
            } else {
                #ifdef SERIALCOMMAND_DEBUG
                    Serial.println("Line buffer is full - increase SERIALCOMMAND_BUFFER");
                #endif
            }
        }
    }
}

void SerialCommand::clearBuffer() {
    cmdBuf[0] = bufPos = 0;
}

char *SerialCommand::next() {
    return strtok_r(NULL, delim, &last);
}
