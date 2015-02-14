#pragma once

#ifndef _H_PACKETCYPHER
#define _H_PACKETCYPHER

//Typedefinitionen

class TPacketCypher;
extern TPacketCypher PacketCypher;

//--------------------------------

class TPacketCypher
{
private:
	std::string DecryptGamePacketSecond(unsigned char str[]); //Zweiter Teil Packetdecryption
public:
	TPacketCypher(); //Konstruktor
	~TPacketCypher(); //Destruktor

	std::vector<std::vector<std::string>> DecryptGamePacket(int session_id, unsigned char *str, int length); //Gamepacket Decryption
	std::vector<std::string> DecryptLoginPacket(const char *packetbuffer, int packetlength); //Loginpacket Decryption
	std::string EncryptLoginPacket(std::string packetbuffer); //Loginpacket Encryption
	std::string DecryptPasswordString(std::string passwordstring); //Password decryption
	std::string EncryptGamePacket(std::string str); //Gamepacket Encryption
	int DecryptSessionPacket(std::string str); //Sessionpacket Decryption
};

#endif _H_PACKETCYPHER