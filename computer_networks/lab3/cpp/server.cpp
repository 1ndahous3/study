#include <iostream>
#include <vector>
#include <mutex>

#include <sys/time.h>

#include "../gen-cpp/Chat.h"
#include "../gen-cpp/chat_types.h"
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::chat;

class ChatHandler : virtual public ChatIf {

 public:
  ChatHandler() {}

  void login(const std::string& name) {
    std::cout << "Сlient trying to login..." << std::endl;
    std::lock_guard<std::mutex> lock(containers_mutex);
    if (name.size() == 0) {
        InvalidLogin ex;
        ex.reason = "empty login";
        throw ex;
    } else if (users.count(name) != 0) {
        InvalidLogin ex;
        ex.reason = "login is already exist";
        std::cout << "Login is already exist: " << name << std::endl;
        throw ex;
    }
    std::cout << "Client logined: " << name << std::endl;
    users.insert(name);
  }

  void broadcast(const std::string& login, const std::string& text) {
    std::lock_guard<std::mutex> lock(containers_mutex);
    if (users.count(login) == 0) {
        IsNotAuthorized ex;
        throw ex;
    }
    Message msg;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    msg.timestamp.seconds = tv.tv_sec;
    msg.timestamp.milliseconds = tv.tv_usec;
    msg.login = login;
    msg.message = text;
    std::cout << msg.login << ": " << msg.message << std::endl;
    messages.push_back(msg);
  }

  void users_list(std::set<std::string> & _return, const std::string& login) {
    std::lock_guard<std::mutex> lock(containers_mutex);
    if (users.count(login) == 0) {
        IsNotAuthorized ex;
        throw ex;
    }
    _return = users;
  }

  void get_messages(std::vector<Message> & _return, const std::string& login, const TimeStamp& timestamp) {
    std::lock_guard<std::mutex> lock(containers_mutex);
    if (users.count(login) == 0) {
        IsNotAuthorized ex;
        throw ex;
    }
    struct timeval tv = { timestamp.seconds, timestamp.milliseconds };
    for (auto &obj : messages) {
        struct timeval tv_msg = { obj.timestamp.seconds, obj.timestamp.milliseconds };
	if (timercmp(&tv, &tv_msg, <) && obj.login != login) {
           std::cout << obj.message << std::endl;
           _return.push_back(obj);
        }
    }
  }

  void disconnect(const std::string& login) {
    std::lock_guard<std::mutex> lock(containers_mutex);
    if (users.count(login) == 0) {
        IsNotAuthorized ex;
        throw ex;
    }
    users.erase(login);
    std::cout << "Client disconnected: " << login << std::endl;
  }
protected:
  std::set<std::string> users;
  std::vector<Message> messages;
  std::mutex containers_mutex;
};



class ChatCloneFactory : virtual public ChatIfFactory {
public:

    virtual ChatIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
        std::cout << "Incoming connection" << std::endl;
        return HANDLER_INSTANCE;
    }

    virtual void releaseHandler(ChatIf* handler) {}

    virtual ~ChatCloneFactory() {}

private:
    ChatHandler* HANDLER_INSTANCE = new ChatHandler();
};




int main(int argc, char **argv) {
  int port = 9090;

  TThreadedServer server(
        boost::make_shared<ChatProcessorFactory>(boost::make_shared<ChatCloneFactory>()),
        boost::make_shared<TServerSocket>(port),
        boost::make_shared<TBufferedTransportFactory>(),
        boost::make_shared<TBinaryProtocolFactory>());
  server.serve();
  return 0;
}

