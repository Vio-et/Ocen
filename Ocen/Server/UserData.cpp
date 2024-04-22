#include "UserData.h"
#include <fstream>

/**
 * @brief 初始化用户数据
 * @param username 保存用户名的字符串
 * @return 返回初始化后的用户数据
*/
UserData* initUserData(string username)
{
	UserData* user = new  UserData(username);

	if (getUserData(user))
		cout << username << "是一个注册用户" << endl;
	else cout << username << "是一个未注册用户" << endl;

	return user;
}

/**
* @brief  用户数据测试桩
* @param user 保存用户数据
* @return 若成功获取用户数据，返回ture；否则，返回false；
*/
bool getUserData(UserData* user)
{
	bool isUser = false;
	string username = user->getName();

	user->varmap["username"] = username;

	if (username == "Orlando" || username == "Violet") {
		user->varmap["product"] = "电脑 手机 平板 耳机";
		user->varmap["computer"] = "价格 6500 分辨率 2K 内存 1T 显卡 RTX3050 ";
		user->varmap["phone"] = "价格 4000 分辨率 2K 内存 256GB";
		user->varmap["pad"] = "价格 2500 分辨率 2K 内存 256GB 操作系统 OcenOS";
		user->varmap["pods"] = "价格 1000 续航 48h 蓝牙技术 Ocen3代";

		if (username == "Orlando") {
			user->varmap["order"] = "\n订单编号 2001\n        订单商品 Ocen电脑\n"
				"        订单金额 6500\n        下单时间 2023-1-15\n"
				"        售后服务时长 2年";
		}
		else if (username == "Violet") {
			user->varmap["order"] = "\n        订单编号 3001\n        订单商品 Ocen耳机\n"
				"        订单金额 1000\n        下单时间 2023-10-30\n"
				"        售后服务时长 1年";
		}
		isUser = true;
	}
	else if (username == "Sasuke") {
		user->varmap["flow"] = "您本月的流量套餐为20GB，已经使用12GB，剩余8GB";
		user->varmap["bill"] = "您当前的话费余额为23.5元";
		isUser = true;
	}
	else if (username == "Sakura") {
		user->varmap["haircuts"] = "毛寸 烫发 染发";
		user->varmap["buzzcut"] = "价格 15 理发时间 20分钟";
		user->varmap["perm"] = "价格 200 理发时间 90分钟";
		user->varmap["dye"] = "价格 300 理发时间 2小时";
		user->varmap["vip"] = "会员到期时间 2024年2月1日 账户余额 300元";
		isUser = true;
	}

	return isUser;
}

/**
 * @brief 用户数据类UserData的构造函数
 * @param username 用户名
*/
UserData::UserData(string username)
{
	this->username = username;
}

/**
 * @brief 设置用户数据的用户名
 * @param username 用户名
*/
void UserData::setName(string username)
{
	this->username = username;
}

/**
 * @brief 获取用户名
 * @return 返回用户名
*/
string UserData::getName()
{
	return this->username;
}

/**
 * @brief 格式化输出用户数据
*/
void UserData::output()
{
	cout << "username: " << this->username << endl;
	for (const auto& var : this->varmap)
		cout << "	" << var.first << " " << var.second << endl;
}
