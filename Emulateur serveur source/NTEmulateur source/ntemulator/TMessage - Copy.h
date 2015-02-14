#pragma once

#ifndef _H_MESSAGE
#define _H_MESSAGE

//Typedefinitionen
class TMessage;
extern TMessage Message;
//-------------------------------

class TMessage
{
public:
	TMessage(void); //Konstruktor
	~TMessage(void); //Destruktor

	void Message_Info(TSocket *Client, std::string message); //Messagebox am Clienten aufrufen
	void Dialog(TSocket *Client, std::string acceptpacket, std::string cancelpacket, std::string message); //Dialog im Clienten aufrufen
};

#endif _H_MESSAGE