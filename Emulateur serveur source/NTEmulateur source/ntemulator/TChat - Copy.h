#pragma once

//CHAT_TYPES
#define NORMAL_CHAT 0
#define PET_CHAT 2
#define GROUP_CHAT 3
#define WHISPER_CHAT 5
#define FAMILY_CHAT 6
#define TIMESPACE_CHAT 7
#define SYSTEM_CHAT 10
#define EXPIRED_CHAT 11
#define ITEM_CHAT 12
#define SPEAKER_CHAT 13
#define UNKNOWN_CHAT 15
//--------------------------

#define SAY_DISTANCE 20


#ifndef _H_CHAT
#define _H_CHAT

//Typedefinitionen
class TSocket;
class TChat;
extern TChat Chat;
//---------------------------

class TChat
{
public:
	TChat(void); //Konstruktor
	~TChat(void); //Dekonstruktor

	void Say_Handle(TSocket *Client, std::vector<std::string> packet); //Text Nachricht verarbeiten
	void Whisper_Handle(TSocket *Client, std::vector<std::string> packet); //Flüster Nachricht verarbeiten
	void Group_Handle(TSocket *Client, std::vector<std::string> packet); //Gruppennachricht
	void ChatPromotion(TSocket *Client, int count, ...); //Nostale Werbebanner unten im Chat
	void ChatMessage(TSocket *Client, int type, std::string message); //System Chat Nachricht
	void LoadSystemMessages(TSocket *Client); //System Nachrichten , Promotions etc laden
};

#endif _H_CHAT