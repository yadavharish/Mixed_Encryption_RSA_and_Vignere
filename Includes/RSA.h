#ifndef INCLUDED_RSA_H
#define INCLUDED_RSA_H

#include <gmp.h>
#include <math.h>
#include <string.h>

#include "StringOperations.h"
#include "./CharacterIndex.h"
#include "./DefinedSizes.h"

using namespace std;

//evaluates the block size such that noOfCharacters^blockSize <= n and noOfCharacters^(blockSize+1) > n
int RSA_GetBlockSize(mpz_t n)
{
    int blockSize = 0;

    mpz_t temp;
    mpz_init(temp);

    do
    {
        blockSize++;
        mpz_ui_pow_ui(temp, NO_OF_CHARACTERS, blockSize);
    } while (mpz_cmp(temp, n) != 1);

    return blockSize - 1;
}

//encrypts the message msg using key (e, n) by dividing msg into blocks of size blockSize using RSA
string RSA_Encryption(string msg, mpz_t e, mpz_t n)
{
    //compute the effective block size wrt n
    int blockSize = RSA_GetBlockSize(n);

    //compute number of blocks in cipher given
    int noOfBlocks = (msg.length() / blockSize) + ((msg.length() % blockSize != 0) ? 1 : 0);

    int i, j, Mi, index;
    string cipherBlock, cipher = "";
    mpz_t M, C, Ci, temp, temp2, temp3;
    mpz_inits(M, C, Ci, temp, temp2, temp3, NULL);

    //append spaces to msg if length of msg is not a multiple of blockSize
    if (msg.length() % blockSize != 0)
    {
        int spacesToAppend = blockSize - (msg.length() % blockSize);
        msg = appendSpaces(msg, spacesToAppend);
    }
        
    //for each block, compute the value of M, then encrypt cipher using formula C = M^e(mod n) and transform C to sequence of characters
    for (i = 0; i < noOfBlocks; i++)
    {
        cipherBlock = "";
        mpz_set_si(M, 0);
        mpz_ui_pow_ui(temp, NO_OF_CHARACTERS, blockSize - 1);

        //computing M for block i
        for (j = 0; j < blockSize; ++j)
        {
            Mi = getIndexFromCharacter(msg[i * blockSize + j]);
            mpz_mul_ui(temp3, temp, Mi);
            mpz_add(M, M, temp3);
            mpz_tdiv_q_ui(temp, temp, NO_OF_CHARACTERS);
        }

        //computing C for block i
        mpz_powm(C, M, e, n);

        //transforming C to sequence of characters
        mpz_ui_pow_ui(temp, NO_OF_CHARACTERS, blockSize);
        for (j = 0; j <= blockSize; ++j)
        {
            mpz_tdiv_q(Ci, C, temp);
            index = mpz_get_ui(Ci);
            cipherBlock = getCharacterFromIndex(index) + cipherBlock;
            mpz_tdiv_r(C, C, temp);
            mpz_tdiv_q_ui(temp, temp, NO_OF_CHARACTERS);
        }
        cipher += cipherBlock;
    }
    return cipher;
}

//decrypts the cipher using key (d, n) by dividing cipher into blocks of size blockSize using RSA
string RSA_Decryption(string cipher, mpz_t d, mpz_t n)
{
    //compute the effective block size wrt n
    int blockSize = RSA_GetBlockSize(n);

    //compute number of blocks in cipher given
    int noOfBlocks = (cipher.length() / (blockSize + 1));

    int i, j, Ci, index;
    string msg = "";
    mpz_t C, M, Mi, temp, temp2, temp3;
    mpz_inits(C, M, Mi, temp, temp2, temp3, NULL);

    //for each block, compute the value of C, then decrypt cipher using formula M = C^d(mod n) and transform M to sequence of characters
    for (i = 0; i < noOfBlocks; i++)
    {
        mpz_set_si(C, 0);
        mpz_set_ui(temp, 1);

        //computing C for block i
        for (j = 0; j <= blockSize; ++j)
        {
            Ci = getIndexFromCharacter(cipher[i * (blockSize + 1) + j]);
            mpz_mul_ui(temp3, temp, Ci);
            mpz_add(C, C, temp3);
            mpz_mul_ui(temp, temp, NO_OF_CHARACTERS);
        }

        //computing M for block i
        mpz_powm(M, C, d, n);

        //transforming M to sequence of characters
        mpz_ui_pow_ui(temp, NO_OF_CHARACTERS, blockSize - 1);
        for (j = 0; j < blockSize; ++j)
        {
            mpz_tdiv_q(Mi, M, temp);
            index = mpz_get_ui(Mi);
            msg += getCharacterFromIndex(index);
            mpz_tdiv_r(M, M, temp);
            mpz_tdiv_q_ui(temp, temp, NO_OF_CHARACTERS);
        }
    }

    // remove spaces from the end of string
    msg = removeSpacesFromEnd(msg);

    return msg;
}

#endif