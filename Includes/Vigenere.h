#ifndef INCLUDED_V_H
#define INCLUDED_V_H

#include <string>

#include "./DefinedSizes.h"
#include "./CharacterIndex.h"

using namespace std;

//encrypts the msg using key using Vignere cipher
string Vignere_Encryption(string msg, string key)
{
    string cipher;
    int mi, ki;
    for (int i = 0; i < msg.length(); ++i)
    {
        mi = getIndexFromCharacter(msg[i]);
        ki = getIndexFromCharacter(key[i % key.length()]);
        cipher += getCharacterFromIndex((mi + ki) % NO_OF_CHARACTERS);
    }
    return cipher;
}

//decrypts the cipher using key using Vignere cipher
string Vignere_Decryption(string cipher, string key)
{
    string msg;
    int ci, ki;
    for (int i = 0; i < cipher.length(); ++i)
    {
        ci = getIndexFromCharacter(cipher[i]);
        ki = getIndexFromCharacter(key[i % key.length()]);
        msg += getCharacterFromIndex((ci - ki) % NO_OF_CHARACTERS + ((ci < ki) ? NO_OF_CHARACTERS : 0));
    }
    return msg;
}

#endif