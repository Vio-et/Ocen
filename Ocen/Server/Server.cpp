#include "Server.h"

void server(Parser* parser) {
    // 启动TCP服务，提供服务器功能
    io_service io;
    tcp::endpoint endpoint(tcp::v4(), 8888);
    tcp::acceptor acptr(io, endpoint);

    // 循环等待客户端连接请求
    while (true) {
        // 与客户端建立连接
        auto sock = make_shared<tcp::socket>(io);
        acptr.accept(*sock);

        // 为每个客户端建立子线程解释执行RSL
        thread(bind(interpreter, parser, sock)).detach();
    }
}