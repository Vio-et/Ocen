#include "Ocen.h"
using namespace std;

int main(int argc, char* argv[])
{
	// 用命令行指定的脚本文件生成记号流
	TokenStream* tokens = lexical(argv[1]);

	// 用记号流生成语法分析树
	Parser* syntax = parser(tokens);

	// 用语法分析树提供服务器功能
	server(syntax);

	// 释放内存
	delete tokens;
	delete syntax;

	return 0;
}
