#include <iostream>
#include <gmp.h>
#include <fstream>

#include "./Includes/RSA.h"
#include "./Includes/KeyPairGenerator.h"
#include "./Includes/DefinedSizes.h"
#include "./Includes/DigitalSignatureHandler.h"
#include "./Includes/FilePaths.h"

#define USER_ID "user1"

using namespace std;

int main()
{
    //generating a key pair for the user 
    cout << "Generating a key pair for " << USER_ID << "..." << endl;
    mpz_t e, d, n;
    mpz_inits(e, d, n, NULL);
    GenerateKeyPair(e, d, n);
    cout << "Key pair generated for " << USER_ID << endl;

    //transforming keys into strings
    cout << "Transforming generated " << USER_ID << "'s key pair to strings..." << endl;
    char *eStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *dStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *nStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE * 2);
    mpz_get_str(eStr, 10, e);
    mpz_get_str(dStr, 10, d);
    mpz_get_str(nStr, 10, n);
    cout << "Generated " << USER_ID <<"'s key pair transformed to strings" << endl;

    //signing the public key (Issuing the certificate)
    cout << "Signing (Issuing certificate) for generated " << USER_ID << "'s public key..." << endl;
    string publicKeyForSignature = eStr;
    publicKeyForSignature.append("#");
    publicKeyForSignature.append(nStr);
    string signature = sign(publicKeyForSignature);
    cout << "Signed (Issued certificate) for generated " << USER_ID << "'s public key" << endl;
    
    //publishing the public key with signature (Registering the key pair)
    cout << "Publishing (Registering) " << USER_ID << "'s public key..." << endl;
    string publicKeyFilePath = PUBLIC_KEYS_FILE_PATH;
    publicKeyFilePath.append(USER_ID);
    publicKeyFilePath.append(TEXT_FILE_EXTENSION);
    ofstream foutP;
    foutP.open(publicKeyFilePath);
    foutP << eStr << "\n" << nStr << "\n" << signature << "\n";
    foutP.close();
    cout << "Published (Registered) " << USER_ID << "'s public key" << endl;

    //saving the secret key
    cout << "Saving " << USER_ID << "'s secret key..." << endl;
    string secretKeyFilePath = SECRET_KEYS_FILE_PATH;
    secretKeyFilePath.append(USER_ID);
    secretKeyFilePath.append(TEXT_FILE_EXTENSION);
    ofstream foutS;
    foutS.open(secretKeyFilePath);
    foutS << dStr << "\n" << nStr << "\n";
    foutS.close();
    cout << "Saved " << USER_ID << "'s secret key" << endl;

    return 0;
}