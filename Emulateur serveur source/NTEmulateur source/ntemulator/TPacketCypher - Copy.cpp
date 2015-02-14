#include "stdafx.h"
#include "TPacketCypher.h"

TPacketCypher PacketCypher;
//------------------------------------------------------------

TPacketCypher::TPacketCypher()
{
}

//------------------------------------------------------------

TPacketCypher::~TPacketCypher()
{
}

//------------------------------------------------------------

std::vector<std::string> TPacketCypher::DecryptLoginPacket( const char *packetbuffer, int packetlength )
{
	std::string decryptedpacket;

	//Packet decrypten
	for(auto i = 0; i < packetlength; i++)
	{
		decryptedpacket += (int)((int)(packetbuffer[i] - 0xF) ^ 0xC3);
	}

	//Packet splitten und zurückgeben
	return General.split(decryptedpacket, ' ');
}

//------------------------------------------------------------

std::string TPacketCypher::EncryptLoginPacket( std::string packetbuffer )
{
	std::string encryptedbuffer;
	auto length = packetbuffer.length();

	//Packet encrypten
	for(auto i = 0; i < length; i++)
	{
		encryptedbuffer += (int)(packetbuffer[i] + 0xF);
	}

	return (encryptedbuffer += 0x19);
}

//------------------------------------------------------------

std::string TPacketCypher::DecryptPasswordString(std::string passwordstring)
{
	std::stringstream ss;
	std::string temppasswordstring, restoredpassword;
	int hex, count = 1;

	//Wenn Password gerade dann erste 2 Chars löschen ansonsten 3
	if(passwordstring.length() %2 == 0)
	{
		passwordstring.erase(0, 2);
	} else
	{
		passwordstring.erase(0, 3);
	}

	//Jeden zweiten Buchstaben zusammenfügen und anschließend nen Leerzeichen setzen
	for(auto i = 1; i < passwordstring.length(); i+=2, count++)
	{
		temppasswordstring += passwordstring[i];

		if(count %2 == 0)
		{
			temppasswordstring += ' ';
		}
	}

	//Bytes in String umwandeln
	ss.str(temppasswordstring);

	while(ss >> std::hex >> hex)
	{
		restoredpassword.push_back(hex);
	}

	return restoredpassword;
}

//------------------------------------------------------------

std::string TPacketCypher::EncryptGamePacket(std::string str)
{
	std::string encrypted_string;
	int length = str.length();
	int secondlength = (length / 122);
	int zaehler = 0;

	//Packet encrypten
	for (int i = 0 ; i < length; i++)
	{
		if(i == (122 * zaehler))
		{
			if(secondlength == 0)
			{
				encrypted_string += abs((((length / 122) * 122) - length));
			} else
			{
				encrypted_string += 0x7A;
				secondlength--;
				zaehler++;
			}
		}

		encrypted_string +=	str[i] ^ 0xFF;
	}

	encrypted_string += (unsigned)0xFF;
	return encrypted_string;
}

//------------------------------------------------------------

std::vector<std::vector<std::string>> TPacketCypher::DecryptGamePacket( int session_id, unsigned char *str, int length )
{
	std::string encrypted_string = "";
	byte session_key = (session_id & 0xFF);
	unsigned char session_number = (session_id >> 6);
	session_number &= 0xFF;
	session_number &= 0x80000003;
	unsigned char firstbyte;
	unsigned char highbyte;

	//Packet Encrypten anhand des Keys das Entschlüsselungsverfahren bestimmen
	switch (session_number)
	{
	case 0:
		for (int i = 0; i < length; i++)
		{
			firstbyte = (session_key + 0x40);
			highbyte = (str[i] - firstbyte);
			encrypted_string += highbyte;
		}
		break;

	case 1:
		for (int i = 0; i < length; i++)
		{
			firstbyte = (session_key + 0x40);
			highbyte = (str[i] + firstbyte);
			encrypted_string += highbyte;
		}
		break;

	case 2:
		for (int i = 0; i < length; i++)
		{
			firstbyte = (session_key + 0x40);
			highbyte = ((str[i] - firstbyte) ^ 0xC3);
			encrypted_string += highbyte;
		}
		break;

	case 3:
		for (int i = 0; i < length; i++)
		{
			firstbyte = (session_key + 0x40);
			highbyte = ((str[i] + firstbyte) ^ 0xC3);
			encrypted_string += highbyte;
		}
		break;

	default:
		break;
	}

	std::vector<std::vector<std::string>> retn;
	std::vector<std::string> nextsplit;

	//Schauen ob nochmehr Packets in den Bytes vorhanden
	std::vector<std::string> temp = General.split(encrypted_string, 0xFF);
	int TempSize = temp.size();
	int NextSize;

	//Packets decrypten
	for (int i = 0; i < TempSize; i++)
	{
		retn.resize(i+1);
		nextsplit = General.split(DecryptGamePacketSecond((unsigned char*)temp[i].c_str()), ' ');

		NextSize = nextsplit.size();
		for (int x = 0; x < NextSize; x++)
		{
			retn[i].push_back(nextsplit[x]);
		}
	}

	return retn;
}

//------------------------------------------------------------

std::string TPacketCypher::DecryptGamePacketSecond(unsigned char str[])
{
	std::string decrypted_string;
	char table[] = {' ','-','.','0','1','2','3','4','5','6','7','8','9','n'};
	int count = 0;
	unsigned char len;
	unsigned char highbyte;
	unsigned char lowbyte;
	int str_length = strlen((char*)str);

	for (count = 0; count < str_length; )
	{
		if (str[count] <= 0x7A)
		{
			len = str[count];

			for (int i = 0; i < (int)len; i++)
			{
				count++;
				decrypted_string += (str[count] ^ 0xFF);
			}

			count++;
		} else
		{
			len = str[count];
			len &= 0x7F;

			for (int i = 0; i < (int)len;)
			{
				count++;

				highbyte = str[count];
				highbyte &= 0xF0;
				highbyte >>= 0x4;

				lowbyte = str[count];
				lowbyte &= 0x0F;

				if (highbyte != 0x0 && highbyte != 0xF)
				{
					decrypted_string += table[highbyte-1];
					i++;
				}

				if (lowbyte != 0x0 && lowbyte != 0xF)
				{
					decrypted_string += table[lowbyte-1];
					i++;
				}
			}
			count++;
		}
	}

	return decrypted_string;
}

//------------------------------------------------------------

int TPacketCypher::DecryptSessionPacket(std::string str)
{
	std::string	encrypted_string;
	unsigned char firstbyte;
	unsigned char secondbyte;
	std::vector<std::string> temp;
	int packet_len = str.length();


	for (auto i = 1; i < packet_len; i++)
	{
		if (str[i] == 0xE) 
		{ 
			temp = General.split(encrypted_string, ' ');
			return atoi(temp[1].c_str());
		}

		firstbyte = (str[i] - 0xF);
		secondbyte = firstbyte;
		secondbyte &= 0xF0;
		firstbyte =	(firstbyte - secondbyte);
		secondbyte >>=	0x4;

		switch (secondbyte)
		{
		case 0:
			encrypted_string +=	' ';
			break;

		case 1:
			encrypted_string +=	' ';
			break;

		case 2:
			encrypted_string +=	'-';
			break;

		case 3:
			encrypted_string +=	'.';
			break;

		default:
			secondbyte += 0x2C;
			encrypted_string +=	secondbyte;
			break;
		}

		switch (firstbyte)
		{
		case 0:
			encrypted_string +=	' ';
			break;

		case 1:
			encrypted_string += ' ';
			break;

		case 2:
			encrypted_string += '-';
			break;

		case 3:
			encrypted_string += '.';
			break;

		default:
			firstbyte += 0x2C;
			encrypted_string +=	firstbyte;
			break;
		}
	}

	temp = General.split(encrypted_string, ' ');
	return atoi(temp[1].c_str());
}

//------------------------------------------------------------
