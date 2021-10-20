#ifndef INCLUDED_PG_H
#define INCLUDED_PG_H

#include <gmp.h>

#include "./DefinedSizes.h"

void generateLargePrime(mpz_t p, unsigned int n, unsigned int size) //assigns a large prime to p of bit size KEY_BIT_SIZE
{
    //generate a random integer in range 2^(size) to 2^(size + 1) - 1
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, n);
    mpz_rrandomb(p, state, size + 1);
    gmp_randclear(state);

    //compute the next prime number greater than the computed random integer
    mpz_nextprime(p, p);
}

void generateStrongPrime(mpz_t sP, int n1, int n2) //using Gordon's Algorithm
{
    //1. Generate two large random primes s and t of roughly equal bitlength
    mpz_t s, t;
    mpz_inits(s, t, NULL);
    generateLargePrime(s, n1, STRONG_PRIME_BIT_SIZE / 2);
    generateLargePrime(t, n2, STRONG_PRIME_BIT_SIZE / 2 - 1);

    //2. Select an integer i0 and i = i0, i0+1, i0+2,... such that 2it + 1 (say r)
    unsigned int i = 2;
    mpz_t r;
    mpz_init(r);
    mpz_mul_ui(r, t, 2 * i);
    mpz_add_ui(r, r, 1);
    while (mpz_probab_prime_p(r, NO_OF_REPS_FOR_PROBABILISTIC_PRIMALITY_TEST) != 1)
    {
        i++;
        mpz_mul_ui(r, t, 2 * i);
        mpz_add_ui(r, r, 1);
    }

    //3. Compute p0 = (2*s^(r-2) mod r)s - 1
    mpz_t p0, temp1, temp2;
    mpz_inits(p0, temp1, temp2, NULL);
    mpz_sub_ui(temp1, r, 2);
    mpz_powm(temp2, s, temp1, r);
    mpz_mul(temp2, temp2, s);
    mpz_mul_ui(temp2, temp2, 2);
    mpz_sub_ui(p0, temp2, 1);

    //4. Select an integer j0 and j = j0, j0+1, j0+2,... such that p0 + 2jrs (say p)
    unsigned int j = 2;
    mpz_t p, rs_product, temp;
    mpz_inits(p, rs_product, temp, NULL);
    mpz_mul(rs_product, r, s);
    mpz_mul_ui(temp, rs_product, 2 * j);
    mpz_add(p, p0, temp);
    while (mpz_probab_prime_p(p, NO_OF_REPS_FOR_PROBABILISTIC_PRIMALITY_TEST) != 1)
    {
        j++;
        mpz_mul_ui(temp, rs_product, 2 * j);
        mpz_add(p, p0, temp);
    }

    //5. return p
    mpz_set(sP, p);
}

#endif