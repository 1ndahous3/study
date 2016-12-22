#include <iostream>
#include <pthread.h>
#include <mutex>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "../gen-cpp/Chat.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace chat;

struct timeval lastTimestamp;
string login;
mutex service_mutex;

void *listener(void *arg) {
    ChatClient &client = *((ChatClient *) arg);
    vector<Message> messages;
    TimeStamp ts;

    while (!sleep(1)) {
        try {
            messages.clear();
            ts.seconds = lastTimestamp.tv_sec;
            ts.milliseconds = lastTimestamp.tv_usec;
            {
               lock_guard<mutex> lock(service_mutex);
               client.get_messages(messages, login, ts);
            }
            for (auto &obj : messages) {
               cout << obj.login << ": " << obj.message << endl;
               lastTimestamp.tv_sec = obj.timestamp.seconds;
               lastTimestamp.tv_usec = obj.timestamp.milliseconds;
            }
        } catch (IsNotAuthorized ex) {
            cout << "You don't authorized" << endl;
            return NULL;
        }
    }

    return NULL;
}



int main() { 
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  ChatClient client(protocol);

  gettimeofday(&lastTimestamp, NULL);
  transport->open();

  while (true) {
     try {
         cout << "Login: ";
         cin >> login;
         lock_guard<mutex> lock(service_mutex);
         client.login(login);
         break;
      } catch (InvalidLogin ex) {
         cout << "Authorization error: " << ex.reason << endl;
      }
   }
   pthread_t listen_thread;
   pthread_create(&listen_thread, NULL, listener, (void *) &client);

   while (true) {
      string message;
      try {
         cin >> message;
         if (message == "!dc") {
            lock_guard<mutex> lock(service_mutex);
            client.disconnect(login);
         } else if (message == "!list") {
            set<string> users;
            {
               lock_guard<mutex> lock(service_mutex);
               client.users_list(users, login);
            }
            for (auto &obj : users) {
               cout << obj << " ";
            }
            cout << endl;
         } else {
            lock_guard<mutex> lock(service_mutex);
            client.broadcast(login, message);
         }
      } catch (IsNotAuthorized ex) {
         cout << "You're not authorized" << endl;
         return 0;
      }
   }
}
