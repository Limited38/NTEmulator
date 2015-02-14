#pragma once

#ifndef _H_TRADE
#define _H_TRADE

#define ACTION_REQUEST 1
#define ACTION_ACCEPT 2
#define ACTION_FINISH 3
#define ACTION_CANCEL 4
#define ACTION_DECLINE 5

//Typedefintionen
class TTrade;
extern TTrade Trade;
//----------------------------

//Strukturen

//-----------------------------

class TTrade
{
public:
	TTrade(void); //Konstruktor
	~TTrade(void); //Destruktor
	
	void Trade_Handle(TSocket *Client, int action, int partnerid); //Allg. Handle der Actions (req_exc)
	void Request_Handle(TSocket *Client, int partnerid); //Request wird geschickt
	void Request_Decline(TSocket *Client, int partnerid); //Request wird abgelehnt
	void Request_Accept(TSocket *Client, int partnerid); //Request wird akzeptiert
	bool Request_Check(TSocket *Client, int id); //Kontrolle ob bereits ein Trade am laufen ist
	bool Requests_Delete(TSocket *Client, TSocket *Partner); //Requests löschen

	void Trade_Cancel(TSocket *Client); //Tausch abbrechen
	void Execute_Handle(TSocket *Client, std::vector<std::string> packet); //exc_list Packets Handle
	void Trade_Finish(TSocket *Client); //Tausch beenden
};

#endif _H_TRADE
