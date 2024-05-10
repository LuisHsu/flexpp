/**
 * This file is part of flex.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.
 */

#include <iostream>
#include <exceptions.hpp>
#include <color.hpp>
#include <CommandParser.hpp>

using namespace Flex;

int main(int argc, char *argv[]) {
    try{
        // Parse command-line arguments
        CommandParser args(argc, argv, {
            CommandParser::Message("Table Compression:"),
            CommandParser::Optional("--align", "trade off larger tables for better memory alignment", "-Ca"),
            CommandParser::Optional("--ecs", "construct equivalence classes", "-Ce"),
            CommandParser::Optional("--no-compress", "do not compress tables", "-Cf"),
            CommandParser::Optional("--meta-ecs", "construct meta-equivalence classes", "-Cm"),
            CommandParser::Optional("--read", "use read() instead of stdio for scanner input", "-Cr"),
            CommandParser::Optional("--full", "generate fast, large scanner", "-f"),
            CommandParser::Optional("--fast", "use alternate table representation", "-F"),
            CommandParser::Optional("--default", "default compression (same as --ecs --meta-ecs)", "-Cem"),
            CommandParser::Message("Debugging:"),
            CommandParser::Optional("--debug", "enable debug mode in scanner", "-d"),
            CommandParser::Optional("--backup", "write backing-up information to lex.backup", "-b"),
            CommandParser::Optional("--perf-report", "write performance report to stderr", "-p"),
            CommandParser::Optional("--nodefault", "suppress default rule to ECHO unmatched text", "-s"),
            CommandParser::Optional("--trace", "flex should run in trace mode", "-T"),
            CommandParser::Optional("--nowarn", "do not generate warnings", "-w"),
            CommandParser::Optional("--verbose", "write summary of scanner statistics to stdout", "-v"),
            CommandParser::Optional("--hex", "use hexadecimal numbers instead of octal in debug outputs", "-x"),
            CommandParser::Message("Files:"),
            CommandParser::Optional("--outfile", "enable debug mode in scanner", 1, "-o"),
            CommandParser::Optional("--skel", "write backing-up information to lex.backup", 1, "-S"),
            CommandParser::Optional("--stdout", "write scanner on stdout instead of lex.yy.cpp", "-t"),
            CommandParser::Optional("--yyclass", "suppress default rule to ECHO unmatched text", 1),
            CommandParser::Optional("--header-file", "flex should run in trace mode", 1),
            CommandParser::Optional("--tables-file", "do not generate warnings", 1),
            CommandParser::Message("Scanner behavior:"),
            CommandParser::Optional("--batch", "generate batch scanner (opposite of -I)", "-B"),
            CommandParser::Optional("--case-insensitive", "ignore case in patterns", "-i"),
            CommandParser::Optional("--interactive", "generate interactive scanner (opposite of -B)", "-I"),
            CommandParser::Optional("--yylineno", "track line count in yylineno", 0),
            CommandParser::Message("Generated code:"),
            CommandParser::Optional("--noline", "suppress #line directives in scanner", "-L"),
            CommandParser::Optional("--prefix", "use NAME as prefix instead of \"yy\"", 1, "-P"),
            CommandParser::Optional("--stdinit", "initialize yyin/yyout to stdin/stdout", 0),
            CommandParser::Message("Miscellaneous:"),
            CommandParser::Optional("--version", "report flex version", "-V"),
            CommandParser::Fixed("files", "input files", (unsigned int)-1)
        },
            "Generates programs that perform pattern-matching on text"
        );

    }catch(Fatal& ex){
        // Fatal error
        std::cerr << COLOR_Error ": fatal internal error, " << ex.what() << std::endl;
        return 1;
    }catch(Error& ex){
        // Error
        std::cerr << COLOR_Error ": " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}