#pragma once

#include <map>
#include <vector>
#include "../lex/Lexical.h"

typedef string StateId;		// 定义String的别名为状态标识 StateId
typedef string Answer;		// 定义String的别名为答复	Answer

///	抽象状态的类State
class State {
	StateId id;					/// 状态的标识
	vector<string> write;		/// 状态中要输出的字符串
	int read;					/// 状态中要等待输入的时间长度，以秒为单位
	map<string, StateId> rouse;	/// 状态的调用关系，以关键词调用对应的状态标识
	StateId silence;			/// 状态中的沉默处理，即处理用户无输入的情况
	StateId defaults;			/// 状态中的默认处理，即处理用户输入无法解析的情况
	bool exit;					/// 标识是否跳转至结束状态；若为1，则状态结束后跳转至结束状态；否则，正常结束
public:
	State() = default;
	State(StateId id);
	State(const State& state);
	~State();
	void setWrite(string sentence);
	string getWrite();
	bool isWriteEmpty();
	void setRead(int period);
	int getRead();
	void insertRouse(string key, StateId value);
	StateId getRouse(StateId key);
	vector<string> getRouseKeys();
	void setSilence(StateId silence);
	StateId getSilence();
	void setDefaults(StateId defaults);
	StateId getDefaults();
	void setExit(bool exit);
	bool getExit();
	void output(void);
};

/// 抽象语法分析树的类Parser
class Parser {
	int index;						/// 语法分析树中当前处理的记号流中的记号的索引
	TokenStream tokens;				/// 语法分析树需要处理的记号流
	StateId entry;					/// 语法分析树的入口状态的状态标识
	StateId currentState;			/// 语法分析树当前处理的状态
public:
	map<StateId, State> states;		/// 语法分析树的状态对应表
	vector<string> varname;			/// 语法分析树的变量表
	Parser(TokenStream tokens);
	~Parser();
	int getIndex(void);
	void updateIndex(void);
	void rollbackIndex(void);
	void setEntry(StateId entry);
	StateId getEntry();
	Token getToken();
	bool dealState();
	bool dealWrite();
	bool dealRead();
	bool dealRouse();
	bool dealSilecne();
	bool dealDefaults();
	void dealExit();
	void output();
};

/**
 * @brief 语法分析函数，处理记号流并生成语法分析树
 * @param tokens 待处理的记号流
 * @return	报错记号流对应的语法分析树
*/
Parser* parser(TokenStream* tokens);