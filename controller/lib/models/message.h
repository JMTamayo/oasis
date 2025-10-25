#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>

namespace models {

class Message {
private:
  String command;
  String payload;

public:
  Message(String command, String payload);

  ~Message();

  String getCommand();

  String getPayload();
};

} // namespace models

#endif // MESSAGE_H