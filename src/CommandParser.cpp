/**
 * This file is part of flex.
 * 
 * This code is derived from the software projects owned by Luis Hsu.
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

#include <CommandParser.hpp>

#include <queue>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include <Util.hpp>
#include <color.hpp>
#include <exceptions.hpp>

static void help(std::filesystem::path program, std::string desc,
    std::vector<std::variant<CommandParser::Fixed, CommandParser::Optional, CommandParser::Message>> options
){
    // Usage
    std::cout << "Usage: " << program.filename().string() << " [OPTIONS] [FILE]..." << std::endl;
    // Description
    std::cout << desc << std::endl;
    // Calculate option length
    size_t max_len = 14;
    for(auto& option : options){
        if(std::holds_alternative<CommandParser::Optional>(option)){
            CommandParser::Optional& opt = std::get<CommandParser::Optional>(option);
            std::string ss;
            if(!opt.alias.empty()){
                ss = opt.alias + ", ";
            }
            ss += opt.name;
            if(opt.number == 1){
                ss += "=NAME";
            }
            if(ss.size() > max_len){
                max_len = ss.size();
            }
        }
    }
    // Options
    for(auto& option : options){
        std::visit(overloaded {
            [](CommandParser::Fixed&){},
            [&](CommandParser::Optional& opt){
                std::string out;
                if(!opt.alias.empty()){
                    out = opt.alias + ", ";
                }
                out += opt.name;
                if(opt.number == 1){
                    out += "=NAME";
                }
                std::cout << "  " << out << std::string(max_len - out.size() + 1, ' ') << opt.desc << std::endl;
            },
            [&](CommandParser::Message& msg){
                std::cout << std::endl << msg.message << std::endl;
            }
        }, option);
    }
    std::cout << "  -?, -h, --help" << std::string(max_len - 13, ' ') << "produce this help message" << std::endl;
    std::cout << std::endl;
}

CommandParser::CommandParser(int argc, char* argv[],
    std::initializer_list<std::variant<Fixed, Optional, Message>> options, std::string desc
) : program(argv[0]){

    std::queue<Fixed> fixed;
    std::unordered_map<std::string, Optional> optional;
    std::unordered_map<std::string, std::string> aliases;
    for(auto option : options){
        std::visit(overloaded {
            [&](Fixed& opt){
                Fixed& arg = fixed.emplace(opt);
                if(arg.number == 0){
                    arg.number = 1;
                }
            },
            [&](Optional& opt){
                optional[opt.name] = opt;
                if(!opt.alias.empty()){
                    aliases[opt.alias] = opt.name;
                }
            },
            [](Message&){}
        }, option);
    }

    int argi = 1;
    while(argi < argc){
        std::string arg(argv[argi]);
        if(arg == "--help" || arg == "-h" || arg == "-?"){
            help(program, desc, options);
            std::exit(0);
        }else if(optional.contains(arg) || aliases.contains(arg)){
            if(aliases.contains(arg)){
                arg = aliases[arg];
            }
            Arg& value = args[arg];
            argi += 1;
            switch(optional[arg].number){
                case 0:
                    value.emplace<std::monostate>();
                break;
                case 1:
                    value.emplace<std::string>(argv[argi]);
                    argi += 1;
                break;
                default:{
                    std::vector<std::string>& values = value.emplace<std::vector<std::string>>();
                    for(unsigned remain = optional[arg].number; remain > 0 && argi < argc; --remain, ++argi){
                        std::string argstr(argv[argi]);
                        if(optional.contains(argstr) || aliases.contains(argstr)){
                            break;
                        }else if(argstr == "--help" || argstr == "-h" || argstr == "-?"){
                            help(program, desc, options);
                            std::exit(0);
                        }
                        values.emplace_back(argstr);
                    }
                }break;
            }
        }else if(!fixed.empty()){
            Fixed& option = fixed.front();
            Arg& value = args[option.name];
            bool do_pop = true;
            switch(option.number){
                case 1:
                    value.emplace<std::string>(arg);
                    argi += 1;
                break;
                default:{
                    std::vector<std::string>& values = std::holds_alternative<std::vector<std::string>>(value) ? std::get<std::vector<std::string>>(value) : value.emplace<std::vector<std::string>>();
                    for(unsigned remain = option.number; remain > 0 && argi < argc; --remain, ++argi){
                        std::string argstr(argv[argi]);
                        if(optional.contains(argstr) || aliases.contains(argstr)){
                            do_pop = false;
                            break;
                        }else if(argstr == "--help" || argstr == "-h" || argstr == "-?"){
                            help(program, desc, options);
                            std::exit(0);
                        }
                        values.emplace_back(argstr);
                    }
                }break;
            }
            if(do_pop){
                fixed.pop();
            }
        }else{
            // Too many arguments
            help(program, desc, options);
            throw Flex::Error("too many arguments");
        }
    }
}

std::optional<CommandParser::Arg> CommandParser::operator[](const std::string key){
    if(args.contains(key)){
        return args[key];
    }else if(args.contains(std::string("--") + key)){
        return args[std::string("--") + key];
    }
    return std::nullopt;
}