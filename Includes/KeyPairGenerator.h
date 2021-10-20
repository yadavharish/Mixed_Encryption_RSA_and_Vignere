#ifndef INCLUDED_KPG_H
#define INCLUDED_KPG_H

#include <gmp.h>
#include <time.h>
#include <stdlib.h>

#include "PrimeGenerator.h"
#include "./DefinedSizes.h"

void GenerateKeyPair(mpz_t e, mpz_t d, mpz_t n)
{
    //generate 2 random strong primes
    srand((unsigned int)time(NULL));
    mpz_t p, q;
    mpz_inits(p, q, NULL);
    generateStrongPrime(p, rand(), rand());
    generateStrongPrime(q, rand(), rand());

    //compute n = p*q
    mpz_mul(n, p, q);

    //compute phi(n)
    mpz_t phi_n, ptemp, qtemp;
    mpz_inits(phi_n, ptemp, qtemp, NULL);
    mpz_sub_ui(ptemp, p, 1);
    mpz_sub_ui(qtemp, q, 1);
    mpz_mul(phi_n, ptemp, qtemp);

    //compute e
    mpz_t g;
    mpz_inits(e, g, NULL);
    mpz_set_si(e, INITIAL_EXPONENT_FOR_RSA);
    mpz_gcd(g, phi_n, e);
    while (mpz_cmp_si(g, 1) != 0)
    {
        mpz_add_ui(e, e, 2);
        mpz_gcd(g, phi_n, e);
    }

    //compute d
    mpz_invert(d, e, phi_n);
}

#endif