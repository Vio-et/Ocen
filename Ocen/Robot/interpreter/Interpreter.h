#pragma once

#include "../parser/Parser.h"
#include "../../Server/UserData.h"
#include <boost/asio.hpp>
#include <thread>
#include <chrono>
#include <functional>

using namespace std;
using namespace boost::asio;
using ip::tcp;

/**
 * @brief 用语法分析树和执行环境进行解释执行函数
 * @param parser 语法分析树
 * @param sock 与客户端通信的socket
*/
void interpreter(Parser* parser, shared_ptr<tcp::socket> sock);

/**
 * @brief 通过socket向客户端发送回复
 * @param sentence 待发送的回复
 * @param socket 与客户端通信的socket
*/
void sendAnswer(Answer sentence, shared_ptr<tcp::socket> socket);

/**
 * @brief  通过socket接收客户端的命令
 * @param sock  与客户端通信的socket
 * @param command  保存客户端命令的字符串
 * @param isReady  保存是否收到客户端命令的布尔值；若收到命令，则为ture；否则，为false；
 */
void recvCommand(shared_ptr<tcp::socket> sock, string& command, bool& isReady);

/**
 * @brief 解析客户端的命令
 * @param command 待解析的客户端命令
 * @param state 解析时语法分析树的当前状态
 * @return 返回客户端命令中对应的调用关系表的关键词
*/
string analyseCommand(const string& command, State state);

/** 
* @brief 倒计时函数
* @param timer  倒计时的时间，单位为秒
* @param isSilence 保存倒计时是否结束的布尔值；若倒计时结束，为真；否则，为假
*/
void timerThread(int timer, bool& isSilence);
