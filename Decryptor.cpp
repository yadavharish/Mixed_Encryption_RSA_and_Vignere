#include <iostream>
#include <fstream>
#include <gmp.h>

#include "./Includes/Vigenere.h"
#include "./Includes/RSA.h"
#include "./Includes/StringOperations.h"
#include "./Includes/FilePaths.h"
#include "./Includes/DefinedSizes.h"
#include "./Includes/DigitalSignatureHandler.h"

using namespace std;

#define RECIPIENT_USER_ID "user2"
#define SENDER_USER_ID "user1"


int main()
{
//(c', k'') <- D(c, k', skB), i.e. Decrypt the cipher text got from the sender using skB (secret key of recipient)
    //get skB
    cout << "Acquiring recipient's (" << RECIPIENT_USER_ID << ") secret key..." << endl;
    string skB;
    char *RecipientSecretKeyD = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *RecipientSecretKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    ifstream finSKB;
    mpz_t skBd, skBn;
    mpz_inits(skBd, skBn, NULL);
    string skBFilePath = SECRET_KEYS_FILE_PATH;
    skBFilePath.append(RECIPIENT_USER_ID);
    skBFilePath.append(TEXT_FILE_EXTENSION);
    finSKB.open(skBFilePath);
    finSKB >> RecipientSecretKeyD;
    finSKB >> RecipientSecretKeyN;
    finSKB.close();
    mpz_set_str(skBd, RecipientSecretKeyD, 10);
    mpz_set_str(skBn, RecipientSecretKeyN, 10);
    cout << "Recipient's (" << RECIPIENT_USER_ID << ") secret key acquired" << endl;

    //get c, k'
    cout << "Acquiring cipher text c..." << endl;
    string c = "", k_ = "";
    char ch;
    ifstream finC;
    finC.open(CIPHER_TEXT_FILE_PATH);
    finC.get(ch);
    while (ch != '\n')
    {
        c += ch;
        finC.get(ch);
    }
    finC.get(ch);
    while (ch != '\n')
    {
        k_ += ch;
        finC.get(ch);
    }
    cout << "Cipher text c acquired" << endl;

    //compute c' <- D(c, skB) and k'' <- D(k', skB)
    cout << "Computing c' <- D(c, skB)..." << endl;
    string c_ = RSA_Decryption(c, skBd, skBn);
    cout << "c' computed" << endl;
    cout << "Computing k'' <- D(k', skB)..." << endl;
    string k__ = RSA_Decryption(k_, skBd, skBn);
    cout << "k'' computed" << endl;

//(Cs, k) <- E(c', k'', pkA), i.e. Decrypt the c' and k'' using pkA (public key of sender), (for authenticity)
    //get pkA
    cout << "Acquiring sender's (" << SENDER_USER_ID << ") public key..." << endl;
    string pkA;
    char *SenderPublicKeyD = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *SenderPublicKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    ifstream finPKA;
    mpz_t pkAe, pkAn;
    mpz_inits(pkAe, pkAn, NULL);
    string pkAFilePath = PUBLIC_KEYS_FILE_PATH;
    pkAFilePath.append(SENDER_USER_ID);
    pkAFilePath.append(TEXT_FILE_EXTENSION);
    finPKA.open(pkAFilePath);
    finPKA >> SenderPublicKeyD;
    finPKA >> SenderPublicKeyN;
    mpz_set_str(pkAe, SenderPublicKeyD, 10);
    mpz_set_str(pkAn, SenderPublicKeyN, 10);
    string SenderPublicKey = SenderPublicKeyD;
    SenderPublicKey.append("#");
    SenderPublicKey.append(SenderPublicKeyN);
    cout << "Sender's (" << SENDER_USER_ID << ") public key acquired" << endl;

    
    //get signature
    cout << "Acquring sender's (" << SENDER_USER_ID << ") public key signature..." << endl;
    char chSign;
    string pkASign;
    finPKA.get(chSign);
    finPKA.get(chSign);
    while(chSign != '\n')
    {   
        pkASign += chSign;
        finPKA.get(chSign);
    }
    finPKA.close();
    cout << "Sender's (" << SENDER_USER_ID << ") public key signature acquried" << endl;

    //verify pkA using digital signature of CA
    cout << "Verifying sender's (" << SENDER_USER_ID << ") public key..." << endl;
    verify(pkASign, SenderPublicKey);
    cout << "Sender's (" << SENDER_USER_ID << ") public key verified" << endl;

    //compute Cs <- D(c', pkA) and k <- D(k'', pkA)
    cout << "Computing Cs <- D(c', skA)..." << endl;
    string Cs = RSA_Decryption(c_, pkAe, pkAn);
    cout << "Cs computed" << endl;
    cout << "Computing k <- D(k'', pkA)..." << endl;
    string k = RSA_Decryption(k__, pkAe, pkAn);
    cout << "k computed" << endl;

//m <- D(Cs, k)
    cout << "Computing message m <- D(Cs, k) using Vigenere Decryption..." << endl;
    string m = Vignere_Decryption(Cs, k);
    cout << "Message m computed" << endl;

// save the decrypted plain text in PLAIN_TEXT_FILE_PATH
    cout << "Saving decrypted message m..." << endl;
    ofstream foutPT;
    foutPT.open(PLAIN_TEXT_FILE_PATH);
    foutPT << m;
    foutPT.close();
    cout << "Decrypted message m saved" << endl;

    return 0;
}