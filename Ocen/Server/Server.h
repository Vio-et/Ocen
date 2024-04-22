#pragma once

#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../Robot/parser/Parser.h"
#include "../Robot/interpreter/Interpreter.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;

/**
 * @brief 服务器函数，通过脚本对应的语法分析树提供多客户端的服务器功能
 * @param parser 对应的语法分析树
*/
void server(Parser* parser);