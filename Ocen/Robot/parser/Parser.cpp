#include "Parser.h"
using namespace std;

Parser* parser(TokenStream* tokens)
{
	bool isGrammer = true;					// 记录是否存在词法错误
	Parser* parser = new Parser(*tokens);	// 保存通过记号流生成的语法分析树
	Token token;							// 当前处理的记号
	int index;								// 当前处理的记号在记号流中的索引
	
	// 若处理过程无词法错误，则生成语法分析树；否则，进行相应报错
	while (parser->getIndex() < (*tokens).getLength() && isGrammer) {
		// 获取当前处理的索引和对应记号
		index = parser->getIndex();
		token = parser->getToken();

		// 获取记号属性
		string attr = token.getAttribute();

		// 根据记号的属性调用对应的语法分析函数
		if (attr == "State")
			isGrammer = parser->dealState();
		else if (attr == "Write")
			isGrammer = parser->dealWrite();
		else if (attr == "Read")
			isGrammer = parser->dealRead();
		else if (attr == "Rouse")
			isGrammer = parser->dealRouse();
		else if (attr == "Silence")
			isGrammer = parser->dealSilecne();
		else if (attr == "Default")
			isGrammer = parser->dealDefaults();
		else if (attr == "Exit")
			parser->dealExit();
		else {
			// 若出现语法错误，更新语法错误标识并输出报错信息
			isGrammer = false;
			cout << "There is no this keywrod: " << attr << endl;
		}

		// 更新索引
		parser->updateIndex();
	}

	// 返回生成的语法分析树
	return parser;
}

/**
 * @brief 语法分析树类Parser的构造函数
 * @param tokens 语法分析树待处理的记号流
*/
Parser::Parser(TokenStream tokens)
{
	Token token;

	this->index = 0;
	for(int i = 0; i < tokens.getLength(); i++){
		token = tokens.getToken(i);
		this->tokens.append(token.getMark(), token.getAttribute());
	}
}

/**
 * @brief 语法分析树类Parser的析构函数
*/
Parser::~Parser(){}

/**
 * @brief 获取语法分析树当前处理的记号的索引
 * @return 返回当前处理的记号的索引
*/
int Parser::getIndex()
{
	return this->index;
}

/**
 * @brief 更新索引，使索引增加1
*/
void Parser::updateIndex()
{
	this->index++;
}

/**
 * @brief 回滚索引，使索引减少1
*/
void Parser::rollbackIndex()
{
	this->index--;
}

/**
 * @brief 获取当前索引对应的记号
 * @return 返回获取的记号
*/
Token Parser::getToken()
{
	return this->tokens.getToken(this->index);
}

/**
 * @brief 设置语法分析树的入口状态
 * @param entry 入口状态的状态标识
*/
void Parser::setEntry(StateId entry)
{
	this->entry = entry;
}

/**
 * @brief 获取语法分析树入口状态的状态标识
 * @return 返回入口状态的状态标识
*/
StateId Parser::getEntry()
{
	return this->entry;
}

/**
 * @brief 语法分析树处理状态类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealState()
{
	bool isState = true;

	this->updateIndex();
	Token token = this->getToken();
	StateId id = token.getAttribute();

	if (token.getMark() == STATE) {
		if (this->entry == "")
			this->entry = token.getAttribute();

		State state(token.getAttribute());

		this->currentState = token.getAttribute();
		this->states.insert(make_pair(id, state));
	}
	else {
		isState = false;
		cout << "State must have state name end with : signal" << endl;
	}

	return isState;
}

/**
 * @brief 语法分析树处理输出类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealWrite()
{
	bool isWrite = true;
	
	this->updateIndex();
	Token token = this->getToken();
	
	if (token.getMark() == IO && token.getAttribute() == "OUT") {
		this->updateIndex();
		token = this->getToken();

		while (token.getMark() == STRING || token.getMark() == IDENTIFY) {
			if (token.getMark() == IDENTIFY)
				this->varname.push_back(token.getAttribute());
			this->states[this->currentState].setWrite(token.getAttribute());
			this->updateIndex();
			token = this->getToken();
		}

		this->rollbackIndex();
	}
	else { 
		cout << "There must have a << signal after Write." << endl; 
		isWrite = false; 
	}

	return isWrite;
}

/**
 * @brief 语法分析树处理输入类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealRead()
{
	bool isRead = true;

	this->updateIndex();
	Token token = this->getToken();

	if (token.getMark() == IO && token.getAttribute() == "IN") {
		this->updateIndex();
		token = this->getToken();

		if(token.getMark() == NUMBER)
			this->states[this->currentState].setRead(stoi(token.getAttribute()));
		else {
			isRead = false;
			cout << "There must have a read time number" << endl;
		}
	}
	else {
		isRead = false;
		cout << "There must have a >> signal after Read." << endl;
	}

	return isRead;
}

/**
 * @brief 语法分析树处理调用类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealRouse()
{
	bool isRouse = true;

	this->updateIndex();
	Token token = this->getToken();

	if (token.getMark() == STRING) {
		string key = token.getAttribute();
		StateId value;

		this->updateIndex();
		token = this->getToken();

		if (token.getMark() == PROCESS)
			value = token.getAttribute();
		else {
			isRouse = false;
			cout << "Rouse must have a process function" << endl;
		}

		this->states[this->currentState].insertRouse(key, value);
	}
	else {
		isRouse = false;
		cout << "Rouse must have a key to rouse process function" << endl;
	}

	return isRouse;
}

/**
 * @brief 语法分析树处理沉默类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealSilecne()
{
	bool isSilence = true;

	this->updateIndex();
	Token token = this->getToken();

	if (token.getMark() == PROCESS) 
		this->states[this->currentState].setSilence(token.getAttribute());
	else {
		isSilence = false; 
		cout << "Silence must have a  process function" << endl;
	}

	return isSilence;
}

/**
 * @brief 语法分析树处理默认类记号的函数
 * @return 若处理过程中有语法错误，返回ture；否则，返回false
*/
bool Parser::dealDefaults()
{
	bool isDefaults = true;

	this->updateIndex();
	Token token = this->getToken();

	if (token.getMark() == PROCESS) 
		this->states[this->currentState].setDefaults(token.getAttribute());
	else {
		isDefaults = false;
		cout << "Defaults must have a  process function" << endl;
	}

	return isDefaults;
}

/**
 * @brief 语法分析树处理退出类记号的函数
*/
void Parser::dealExit()
{
	this->states[this->currentState].setExit(true);
}

/**
 * @brief 格式化输出语法分析树
*/
void Parser::output()
{
	cout << "Entry: " << this->entry << endl;
	cout << "Varname:	" << endl;

	for (auto& item : this->varname)
		cout << item << " ";
	cout << endl;

	for (auto it = this->states.begin(); it != this->states.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << endl;
		it->second.output();
	}
}

/**
 * @brief 状态类State的构造函数
 * @param id 状态的标识
*/
State::State(StateId id)
{
	this->id = id;
	this->read = 0;
	this->exit = false;
}

/**
 * @brief 状态类State的拷贝构造函数
 * @param state 被拷贝的状态
*/
State::State(const State& state)
{
	this->id = state.id;

	for (auto& sentence : state.write)
		this->write.push_back(sentence);
	this->read = state.read;

	for (auto& pair : state.rouse)
		this->rouse.insert({ pair.first, pair.second });

	this->silence = state.silence;
	this->defaults = state.defaults;
	this->exit = state.exit;
}

/**
 * @brief 状态类State的析构函数
*/
State::~State() {}

/**
 * @brief 添加状态的输出
 * @param sentence 待添加的输出字符串
*/
void State::setWrite(string sentence)
{
	this->write.push_back(sentence);
}

/**
 * @brief 获取状态的输出字符串
 * @return 返回状态的输出字符串
*/
string State::getWrite()
{
	string answer = this->write.front();
	this->write.erase(this->write.begin());

	return answer;
}

/**
 * @brief 判断状态的输出是否为空
 * @return 若状态的输出字符串已空，则返回true；否则，返回false
*/
bool State::isWriteEmpty() 
{
	return this->write.empty();
}

/**
 * @brief 设置状态读取输入的时间长度
 * @param period 状态输入的时间，单位为秒
*/
void State::setRead(int period)
{
	this->read = period;
}

/**
 * @brief 获取状态读取输入的时间长度
 * @return 返回读取输入的时间长度
*/
int State::getRead()
{
	return this->read;
}

/**
 * @brief 向状态的调用关系表中添加调用关系
 * @param key 保存调用关系的关键词的字符串
 * @param value 保存调用关系对应的状态标识
*/
void State::insertRouse(string key, StateId value)
{
	this->rouse.insert({ key, value });
}

/**
 * @brief 通过调用关系的关键词获取对应的状态标识
 * @param key 调用关系的关键词
 * @return 返回对应的状态标识
*/
StateId State::getRouse(StateId key)
{
	return this->rouse[key];
}

/**
 * @brief 获取状态的调用关系中的所有关键词组成的向量
 * @return 返回状态调用关系中的所有关键词组成的向量
*/
vector<string> State::getRouseKeys()
{
	vector<string> rouseKeys;
	
	for (const auto& key : this->rouse)
		rouseKeys.push_back(key.first);

	return rouseKeys;
}

/**
 * @brief 设置状态中沉默处理时对应的状态标识
 * @param silence 保存沉默处理对应状态的状态标识
*/
void State::setSilence(StateId silence)
{
	this->silence = silence;
}

/**
 * @brief 获取状态中沉默处理状态的状态标识
 * @return 返回默认沉默状态的状态标识
*/
StateId State::getSilence()
{
	return this->silence;
}

/**
 * @brief 设置状态中默认处理时对应的状态标识
 * @param defaults 保存默认处理对应状态的状态标识
*/
void State::setDefaults(StateId defaults)
{
	this->defaults = defaults;
}

/**
 * @brief 获取状态中默认处理状态的状态标识
 * @return 返回默认处理状态的状态标识
*/
StateId State::getDefaults()
{
	return this->defaults;
}

/**
 * @brief 设置状态中的退出标识
 * @param exit 保存退出标识的布尔值
*/
void State::setExit(bool exit)
{
	this->exit = exit;
}

/**
 * @brief 获取状态的退出标识
 * @return 返回状态的退出标识
*/
bool State::getExit()
{
	return this->exit;
}

/**
 * @brief 格式化输出状态信息
*/
void State::output()
{
	cout << "StateId: " << this->id << endl;
	cout << "	Write: ";

	for (const auto& str : this->write)
		cout << str << " ";

	cout << endl << "	Read: " << this->read << endl;
	cout << "	Rouse: " << endl;

	for (const auto& rouse : this->rouse)
		cout << "	" << rouse.first << " " << rouse.second << endl;

	cout << "	Silence: " << this->silence << endl;
	cout << "	Default: " << this->defaults << endl;
	cout << "	Exit: " << this->exit << endl;
}