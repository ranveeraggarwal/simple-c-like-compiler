#!/bin/sh

sed -i '/include/a #include "Parserbase.h"' Scanner.ih;
sed -i '/insert preincludes/a #include "headers.cc"' Parserbase.h;
sed -i '/include "Parser.h"/a #include "definitions.cc"' Parser.ih;

sed -i '\|STYPE__     d_val__;| d 
' Parserbase.h

sed -i '\|typedef Meta__::SType STYPE__;| a \\tpublic: static STYPE__  d_val__;
' Parserbase.h 

sed -i '\|void Parser::print__()| i Meta__::SType ParserBase::d_val__;
' parse.cc



#Observe how the delimiter was changed from / to |
#-i for inplace update
# d for delete
# a for append (or insert)