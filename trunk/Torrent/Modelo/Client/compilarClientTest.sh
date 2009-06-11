#! /usr/bin/bash
g++ *.cpp ../ProtocoloBitTorrent/protocoloBitTorrent.cpp ../HTTP/HttpRe*.cpp  ../ParserBencode/parserBencode.cpp ../SHA1/sha1.cpp -lpthread -g2 ${@}