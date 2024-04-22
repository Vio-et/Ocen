#include "Interpreter.h"

void interpreter(Parser* parser, shared_ptr<tcp::socket> sock)
{
	bool isGetInput = false, isSilence = false;		// 记录是否获取到客户端的命令、记录客户端是否沉默
	StateId currentStateId = parser->getEntry();	// 记录当前解释执行的状态的状态标识
	State state = parser->states[currentStateId];	// 获取当前解释执行的状态
			
	// 获取客户端的用户名，获取对应的用户信息
	sendAnswer("请输入您的用户名#", sock);
	string reply;
	recvCommand(sock, reply, isGetInput);
	UserData* user = initUserData(reply);
	isGetInput = false;

	// 解释执行语法分析树
	while (true) {
		Answer sentence, command;	// 保存要发送给客户端的消息、保存从客户端接收的命令

		// 将当前状态的输出全部整合发送给客户端
		while (!state.isWriteEmpty()) {
			string  word = state.getWrite();

			if (word[0] == '{') 
				word = user->varmap[word.substr(1, word.size() - 2)];
			sentence += word;
		}

		// 如果当前状态不为退出状态，为发往客户端的消息添加分隔符
		if(!state.getExit())
			sentence += '#';

		// 向客户端发送消息
		sendAnswer(sentence, sock);

		// 如果当前状态为退出状态，为发往客户端的消息添加结束标识
		if (state.getExit()) {
			sentence = "\nServer: 谢谢您对Ocen的支持，期待与您的再次相遇，再见! ";
			sendAnswer(sentence, sock);

			sentence = "EXIT#";
			sendAnswer(sentence, sock);

			// 主线程沉睡一段时间，提供退出冗余时间
			this_thread::sleep_for(std::chrono::seconds(20));

			break;
		}
		
		// 通过子线程倒计时判断客户端是否为沉默状态
		thread(bind(timerThread, state.getRead(), ref(isSilence))).detach();

		// 通过子线程获取客户端的命令
		thread(bind(recvCommand, sock, ref(command), ref(isGetInput))).detach();

		// 等待成功获取客户端命令或等待超时
		while (!isSilence && !isGetInput);
		
		// 若成功获取客户端命令，则解析命令并更新状态标识；否则赋值为沉默状态标识
		if (isGetInput) {
			command = analyseCommand(command, state);
			currentStateId = state.getRouse(command);
		}
		else 
			currentStateId = state.getSilence();		

		// 若未能解析出状态标识，则赋值为默认状态标识
		if (currentStateId == "")
			currentStateId = state.getDefaults();

		// 更新状态和标识
		state = parser->states[currentStateId];
		isGetInput = false;
		isSilence = false;
	}

	// 解释执行结束，关闭TCP服务
	sock->close();
}

/**
 * @brief 通过socket向客户端发送回复
 * @param sentence 待发送的回复
 * @param socket 与客户端通信的socket
*/
void sendAnswer(Answer sentence, shared_ptr<tcp::socket> socket)
{
	// 将服务器的回复发送给客户端
	write(*socket, buffer(sentence, sentence.size()));
}

/**
 * @brief  通过socket接收客户端的命令
 * @param sock  与客户端通信的socket
 * @param command  保存客户端命令的字符串
 * @param isReady  保存是否收到客户端命令的布尔值；若收到命令，则为ture；否则，为false；
 */
void recvCommand(shared_ptr<tcp::socket> sock, string& command, bool& isGetInput)
{
	boost::asio::streambuf buffer;
	buffer.prepare(1024);

	// 约定以 # 号作为消息的分隔符
	read_until(*sock, buffer, '#');
	command = buffer_cast<const char*>(buffer.data());

	// 从消息中去除分隔符
	command = command.substr(0, command.size() - 1);

	// 更新命令获取标识为真
	isGetInput = true;
}

/**
 * @brief 解析客户端的命令
 * @param command 待解析的客户端命令
 * @param state 解析时语法分析树的当前状态
 * @return 返回客户端命令中对应的调用关系表的关键词
*/
string analyseCommand(const string& command,State state)
{
	string commandKey = "";
	vector<string> rouseKeys = state.getRouseKeys();

	for (const auto& key : rouseKeys) {
		size_t found = command.find(key);

		if (found != std::string::npos) {
			commandKey = key;
			break;
		}
	}

	return commandKey;
}

/**
* @brief 倒计时函数
* @param timer  倒计时的时间，单位为秒
* @param isSilence 保存倒计时是否结束的布尔值；若倒计时结束，为真；否则，为假
*/
void timerThread(int timer, bool& isSilence)
{
	// 当前线程沉睡指定的时间
	this_thread::sleep_for(std::chrono::seconds(timer));

	// 倒计时结束后，更新结束标识
	isSilence = true;
}