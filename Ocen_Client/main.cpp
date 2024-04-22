#include <iostream>
#include <thread>
#include <functional>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

bool isExit(string& reply);
void recvReply(shared_ptr<tcp::socket> sock, bool& isQuit);
void sendCommand(shared_ptr<tcp::socket> sock);

int main() 
{
    bool isQuit = false;    // 用于记录服务器是否退出的布尔值
    string command;         // 保存用户输入的语句

    // 启动TCP服务，连接服务器
    io_service io;
    auto sock = make_shared<tcp::socket>(io);
    sock->connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 8888));

    // 通过子线程获取服务器的消息
    thread(bind(recvReply, sock, ref(isQuit))).detach();

    // 控制输入和输出的顺序
    this_thread::sleep_for(std::chrono::milliseconds(100));

    // 通过子线程获取用户输入并发送给服务器
    thread(bind(sendCommand, sock)).detach();

    // 等待服务器通知退出
    while (true) {
        if (isQuit)
            break;
    }

    // 关闭TCP服务
    sock->close();

    return 0;
}

/**
 * @brief 通过socket接收服务器的消息
 * @param sock 与服务器通信的socket
 * @param isQuit 保存服务器退出信息的布尔值
*/
void recvReply(shared_ptr<tcp::socket> sock, bool& isQuit)
{
    string reply;
    boost::asio::streambuf buffer;
    buffer.prepare(1024);
    
    // 持续接收服务器的消息
    while (true) {
        // 约定以 # 号作为消息的分隔符
        read_until(*sock, buffer, '#');
        reply = buffer_cast<const char*>(buffer.data());

        // 从消息中去除分隔符
        reply = reply.substr(0, reply.find('#'));

        // 接收服务器的消息并输出
        if (reply.size() != 0) {
            cout.flush();
            if (isExit(reply)) {
                cout << "Server: " << reply << endl;
                isQuit = true;
                break;
            }
            else cout << "Server: " << reply << endl;
        }

        // 清空缓冲区和消息
        buffer.consume(buffer.size());
        reply = "";
    }
}


/**
 * @brief 从标准输入获取用户的命令并发送给服务器
 * @param sock 与服务器通信的socket
*/
void sendCommand(shared_ptr<tcp::socket> sock)
{
    string command;

    // 持续接收用户命令
    while (true) {
        cout << "User: ";

        cin >> command;

        // 将接收到的用户命令发送给服务器
        write(*sock, buffer(command + '#'));

        // 控制输入和输出的顺序
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/**
 * @brief 判断服务器的消息是否包含退出通知
 * @param reply 待处理的服务器消息
 * @return 若包含退出通知，则返回true；否则，返回false
*/
bool isExit(string& reply)
{
    bool isExit = false;   // 用于消息是否包含退出通知

    size_t found = reply.find("EXIT");

    // 若包含退出通知，则在消息中去除退出通知并返回退出标志
    if (found != string::npos) {
        isExit = true;

        reply.erase(found, 4);
    }

    return isExit;
}