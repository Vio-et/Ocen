#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// 记号类型枚举类型
enum Sign {
    KEYWORD,
    IDENTIFY,
    NUMBER,
    STRING,
    IO,
    STATE,
    PROCESS,
};

/// 抽象记号的类Token
class Token {
    int mark;           /// 记号的类型
    string attribute;   /// 记号的属性
public:
    explicit Token(int mark = -1, string attribute = " ");
    Token(const Token& token);
    ~Token();
    int getMark() const;
    string getAttribute();
    void setMark(int i);
    void setAttribute(string attribute);
    void output();
};

/// 抽象记号流的类TokenStream
class TokenStream {
    int size;           /// 记号流的最大空间
    int length;         /// 记号流的有效长度
    Token* tokens;      /// 保存记号流的记号数组
public:
    explicit TokenStream(int size = 50);
    TokenStream(TokenStream& tokenStream);
    ~TokenStream();
    int getLength() const;
    Token getToken(int index);
    void append(int mark, string attribute);
    void output();
};

/**
 * @brief 词法分析函数，处理脚本并转化为记号流
 * @rslPath 保存机器人语言脚本路径的字符串
 * @return 保存脚本对应的记号流
*/
TokenStream* lexical(string rslPath);

/**
 * @brief 检查字符串是否全部由字母组成
 * @param uncheck 待检查的字符串
 * @return 若待查字符串全由字母组成，返回true；否则，返回false；
*/
bool isAlpha(string uncheck);

/** 
* @brief  检查字符串是否全部由数字组成
* @param uncheck   待检查的字符串
* @return 若待查字符串全由数字组成，返回true；否则，返回false；
*/
bool isNumber(string uncheck);
