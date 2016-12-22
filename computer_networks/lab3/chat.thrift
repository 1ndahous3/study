namespace cpp chat


struct TimeStamp {
  1: i64 seconds;
  2: i64 milliseconds;
}

struct Message {
  1: TimeStamp timestamp,
  2: string login,
  3: string message
}

exception InvalidLogin {
  1: string reason
}

exception IsNotAuthorized{}

service Chat {
   void login(1: string name) throws (1:InvalidLogin ex),
   void broadcast(1: string login, 2: string text) throws (1:IsNotAuthorized ex),
   set<string> users_list(1: string login) throws (1:IsNotAuthorized ex),
   list<Message> get_messages(1: string login, 2: TimeStamp timestamp) throws (1:IsNotAuthorized ex);
   void disconnect(1: string login) throws (1:IsNotAuthorized ex);
}

