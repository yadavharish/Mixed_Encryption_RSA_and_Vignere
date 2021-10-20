#include <iostream>
#include <gmp.h>
#include <fstream>

#include "./Includes/KeyPairGenerator.h"
#include "./Includes/DefinedSizes.h"
#include "./Includes/FilePaths.h"

#define CA_SECRET_KEY_FILE_PATH "./Keys/SecretKeys/CA.txt"

using namespace std;

int main()
{
    //generating a key pair
    cout << "Generating a key pair for CA..." << endl;
    mpz_t e, d, n;
    mpz_inits(e, d, n, NULL);
    GenerateKeyPair(e, d, n);
    cout << "Key pair generated for CA" << endl;

    //transforming keys into strings
    cout << "Transforming generated CA's key pair to strings..." << endl;
    char *eStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *dStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE);
    char *nStr = (char *)malloc(sizeof(char) * KEY_BIT_SIZE * 2);
    mpz_get_str(eStr, 10, e);
    mpz_get_str(dStr, 10, d);
    mpz_get_str(nStr, 10, n);
    cout << "Generated CA's key pair transformed to strings" << endl;

    //publishing the public key
    cout << "Publishing CA's public key..." << endl;
    ofstream foutP;
    foutP.open(CA_PUBLIC_KEY_FILE_PATH);
    foutP << eStr << "\n" << nStr;
    foutP.close();
    cout << "CA's public key published" << endl;

    //saving the secret key
    cout << "Saving CA's secret key..." << endl;
    ofstream foutS;
    foutS.open(CA_SECRET_KEY_FILE_PATH);
    foutS << dStr << "\n" << nStr;
    foutS.close();
    cout << "CA's secret key saved" << endl;

    return 0;
}