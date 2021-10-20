#ifndef INCLUDED_DSH_H
#define INCLUDED_DSH_H

#include <iostream>
#include <fstream>
#include <gmp.h>

#include "./DefinedSizes.h"
#include "./RSA.h"
#include "./FilePaths.h"

#define CA_SECRET_KEY_FILE_PATH "./Keys/SecretKeys/CA.txt"

using namespace std;

string sign(string msg)
{
    //get CA's secret key
    char *CASecretKeyE = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *CASecretKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    ifstream fin;
    fin.open(CA_SECRET_KEY_FILE_PATH);
    fin >> CASecretKeyE;
    fin >> CASecretKeyN;
    fin.close();

    //transforming string to mpz_t
    mpz_t e, n;
    mpz_inits(e, n, NULL);
    mpz_set_str(e, CASecretKeyE, 10);
    mpz_set_str(n, CASecretKeyN, 10);

    //return encrypted string using CA's secret key
    return RSA_Encryption(msg, e, n);
}

void verify(string sign, string msg)
{
    //get CA's public key
    char *CAPublicKeyD = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *CAPublicKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    ifstream finPKCA;
    string pkCAFilePath = CA_PUBLIC_KEY_FILE_PATH;
    finPKCA.open(pkCAFilePath);
    finPKCA >> CAPublicKeyD;
    finPKCA >> CAPublicKeyN;

    mpz_t pkCAd, pkCAn;
    mpz_inits(pkCAd, pkCAn, NULL);
    mpz_set_str(pkCAd, CAPublicKeyD, 10);
    mpz_set_str(pkCAn, CAPublicKeyN, 10);

    //decrypt the signature
    string decryptedSignature = RSA_Decryption(sign, pkCAd, pkCAn);

    string modifiedDecryptedSignature = removeSpacesFromEnd(decryptedSignature);

    if (!(compareStrings(modifiedDecryptedSignature, msg)))
    {
        cout << modifiedDecryptedSignature << endl;
        cout << msg << endl;
        cout << "Tampered Recipient Public Key (verified by CA Public Key)";
        exit(0);
    }
}

#endif