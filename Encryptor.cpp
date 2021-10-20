#include <iostream>
#include <fstream>
#include <gmp.h>

#include "./Includes/Vigenere.h"
#include "./Includes/StringOperations.h"
#include "./Includes/DigitalSignatureHandler.h"
#include "./Includes/RSA.h"
#include "./Includes/FilePaths.h"
#include "./Includes/DefinedSizes.h"

using namespace std;

#define SENDER_USER_ID "user1"
#define RECIPIENT_USER_ID "user2"

int main()
{
//Cs <- E(m, k)
    //get k
    cout << "Acquiring Vigenere Symmetric key k..." << endl;
    string k;
    ifstream finK;
    finK.open(VIGENERE_CIPHER_KEY_FILE_PATH);
    finK >> k;

    //get m
    string word, m = "";
    ifstream finM;
    finM.open(MESSAGE_FILE_PATH);
    finM >> word;
    m.append(word);
    while (finM >> word)
    {
        m.append(" ");
        m.append(word);
    }
    cout << "Vigenere Symmetric key k acquired" << endl;

    //Encrypt using Vigenere cipher
    cout << "Computing cipher text Cs <- E(m, k) using Vigenere Encryption..." << endl;
    string Cs = Vignere_Encryption(m, k);
    cout << "Cipher text Cs computed" << endl;

//(c', k'') <- D(Cs, k, skA), Encrypting Cs and k using skA (secret key of sender)
    //get skA
    cout << "Acquiring sender's (" << SENDER_USER_ID << ") secret key..." << endl;
    char *SenderSecretKeyE = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *SenderSecretKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    mpz_t skAe, skAn;
    mpz_inits(skAe, skAn, NULL);
    ifstream finSKA;
    string skAFilePath = SECRET_KEYS_FILE_PATH;
    skAFilePath.append(SENDER_USER_ID);
    skAFilePath.append(TEXT_FILE_EXTENSION);
    finSKA.open(skAFilePath);
    finSKA >> SenderSecretKeyE;
    finSKA >> SenderSecretKeyN;
    finSKA.close();
    mpz_set_str(skAe, SenderSecretKeyE, 10);
    mpz_set_str(skAn, SenderSecretKeyN, 10);
    cout << "Sender's (" << SENDER_USER_ID << ") secret key acquired" << endl;

    //compute c' <- D(Cs, skA) and k'' <- D(k, skA)
    cout << "Computing c' <- E(Cs, skA)..." << endl;
    string c_ = RSA_Encryption(Cs, skAe, skAn);
    cout << "c' computed" << endl;
    cout << "Computing k'' <- E(k, skA)..." << endl;
    string k__ = RSA_Encryption(k, skAe, skAn);
    cout << "k'' computed" << endl;

//(c, k') <- E(c', k'', pkB), i.e. Encrypt the c_ and k__ using pkB (public key of recipient)
    //get pkB
    cout << "Acquiring recipient's (" << RECIPIENT_USER_ID << ") public key..." << endl;
    char *RecipientPublicKeyE = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *RecipientPublicKeyN = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    ifstream finPKB;
    mpz_t pkBe, pkBn;
    mpz_inits(pkBe, pkBn, NULL);
    string pkBFilePath = PUBLIC_KEYS_FILE_PATH;
    pkBFilePath.append(RECIPIENT_USER_ID);
    pkBFilePath.append(TEXT_FILE_EXTENSION);
    finPKB.open(pkBFilePath);
    finPKB >> RecipientPublicKeyE;
    finPKB >> RecipientPublicKeyN;
    mpz_set_str(pkBe, RecipientPublicKeyE, 10);
    mpz_set_str(pkBn, RecipientPublicKeyN, 10);
    
    string RecipientPublicKey = RecipientPublicKeyE;
    RecipientPublicKey.append("#");
    RecipientPublicKey.append(RecipientPublicKeyN);
    cout << "Recipient's (" << RECIPIENT_USER_ID << ") public key acquired" << endl;

    //get signature
    cout << "Acquiring recipient's (" << RECIPIENT_USER_ID << ") public key signature..." << endl;
    char ch;
    string pkBSign;
    finPKB.get(ch);
    finPKB.get(ch);
    while (ch != '\n')
    {
        pkBSign += ch;
        finPKB.get(ch);
    }
    finPKB.close();
    cout << "Recipient's (" << RECIPIENT_USER_ID << ") public key signature acquired" << endl;

    //verify pkB using digital signature of CA
    cout << "Verifying recipient's (" << RECIPIENT_USER_ID << ") public key..." << endl;
    verify(pkBSign, RecipientPublicKey);
    cout << "Recipient's (" << RECIPIENT_USER_ID << ") public key verified" << endl;

    //compute c <- E(c', pkB) and k' <- E(k'', pkB)
    cout << "Computing c <- E(c', pkB)..." << endl;
    string c = RSA_Encryption(c_, pkBe, pkBn);
    cout << "c computed" << endl;
    cout << "Computing k' <- E(k'', pkB)..." << endl;
    string k_ = RSA_Encryption(k__, pkBe, pkBn);
    cout << "k' computed" << endl;

//send the encrypted text, i.e. saving the encrypted text in CIPHER_TEXT_FILE_PATH
    cout << "Saving encrypted text (c and k')..." << endl;
    ofstream foutCT;
    foutCT.open(CIPHER_TEXT_FILE_PATH);
    foutCT << c << "\n" << k_ << "\n";
    foutCT.close();
    cout << "Encrypted text (c and k') saved" << endl;

    return 0;
}