#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

// 测试桩文件：测试用户数据

///	抽象用户数据的类UserData
class UserData{
	string username;				/// 用户名
public:
	map<string, string> varmap;		/// 用户对应的变量表
	UserData(string username);		
	void setName(string username);
	string getName();
	void output();
};

/**
 * @brief 初始化用户数据
 * @param username 保存用户名的字符串
 * @return 返回初始化后的用户数据
*/
UserData* initUserData(string username);

/** 
* @brief  用户数据测试桩
* @param user 保存用户数据
* @return 若成功获取用户数据，返回ture；否则，返回false；
*/
bool getUserData(UserData* user);