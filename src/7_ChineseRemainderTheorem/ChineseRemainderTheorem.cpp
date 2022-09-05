#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gmp.h>
#include<time.h>
// delete it if you submit the answer
#define LOCAL



char str[1000];
// 求解模逆
// res = b^(-1) mod a
bool get_mod_inverse(mpz_t res, mpz_t a, mpz_t b)
{
    mpz_t temp;
    mpz_t temp_a;
    mpz_t temp_b;
    mpz_t t0;
    mpz_t t;
    mpz_t q;
    mpz_t r;
    mpz_init(temp);
    mpz_init(temp_a);
    mpz_init(temp_b);
    mpz_init(t0);
    mpz_init(t);
    mpz_init(q);
    mpz_init(r);
    mpz_set(temp_a, a);
    mpz_set(temp_b, b);
    mpz_div(q, temp_a, temp_b); // q = a / b
    mpz_mod(r, temp_a, temp_b); // r = a % b
    mpz_set_ui(t0, 0);
    mpz_set_ui(t, 1);
    while (mpz_cmp_ui(r, 0) != 0)
    {
        mpz_mul(temp, q, t); // temp = q * t
        mpz_sub(temp, t0, temp); // temp = t0 - q * t
        mpz_set(t0, t); // t0 = t
        mpz_set(t, temp); // t = temp
        mpz_set(temp_a, temp_b); // a = b
        mpz_set(temp_b, r); // b = r
        mpz_div(q, temp_a, temp_b); // q = a / b
        mpz_mod(r, temp_a, temp_b); // r = a % b
    }
    // if(mpz_cmp_ui(temp_b, 1) != 0)
    // {
    //     return false;
    // }
     if(mpz_cmp_ui(t, 0) < 0)
    {
        mpz_add(t, t, a);
    }
    mpz_set(res, t);
    return true;
}


// 计算 res = a^e mod n
inline void expmod(mpz_t res, mpz_t a, mpz_t e, mpz_t n)
{
    int i = 0;
    mpz_t n_temp, a_temp, b;
    mpz_init(n_temp);
    mpz_init(a_temp);
    mpz_init(b);
    mpz_get_str(str, 2, e);
    i = strlen(str) - 1;
    mpz_set(n_temp, n);     // n_temp = n
    mpz_set_ui(a_temp, 1);  // a_temp = 1
    mpz_set(b, a);          // b = a
    while(i >= 0)
    {
        if(str[i] == '1')
        {
            mpz_mul(a_temp, a_temp, b);
            mpz_mod(a_temp, a_temp, n_temp);
        }
        mpz_pow_ui(b, b, 2);
        mpz_mod(b, b, n_temp);
        i--;
    }
    mpz_set(res, a_temp);
}


// void calD(mpz_t d, mpz_t e, mpz_t p, mpz_t q)
// {
//     mpz_t phi;
//     mpz_t temp, temp1;
//     mpz_init(phi);
//     mpz_init(temp);
//     mpz_init(temp1);
//     mpz_sub_ui(temp, p, 1);
//     mpz_sub_ui(temp1, q, 1);
//     mpz_mul(phi, temp, temp1);
//     get_mod_inverse(d, phi, e);
// }


// 中国剩余定理
// res = c ^ d mod (pq)
// void ChineseRemainderTheorem(mpz_t res, mpz_t c, mpz_t d, mpz_t p, mpz_t q)
// {
//     mpz_t M, temp1, temp2, temp3;
//     mpz_init(M);
//     mpz_init(temp1);
//     mpz_init(temp2);
//     mpz_init(temp3);

//     mpz_set_ui(res, 0);     // res = 0
//     mpz_mul(M, p, q);       // M = p * q

//     expmod(temp1, c, d, p); // temp1 = c ^ d mod p
//     mpz_div(temp2, M, p);   // temp2 = M / p
//     get_mod_inverse(temp3, p, temp2); // temp3 = temp2 ^ (-1) mod p
//     mpz_mul(temp1, temp1, temp2); // temp1 = temp1 * temp2
//     mpz_mul(temp1, temp1, temp3); // temp1 = temp1 * temp3
//     mpz_mod(temp1, temp1, M); // temp1 = temp1 mod M
//     mpz_add(res, res, temp1); // res = res + temp1

//     expmod(temp1, c, d, q); // temp1 = c ^ d mod q
//     mpz_div(temp2, M, q);   // temp2 = M / q
//     get_mod_inverse(temp3, q, temp2); // temp3 = temp2 ^ (-1) mod q
//     mpz_mul(temp1, temp1, temp2); // temp1 = temp1 * temp2
//     mpz_mul(temp1, temp1, temp3); // temp1 = temp1 * temp3
//     mpz_mod(temp1, temp1, M); // temp1 = temp1 mod M
//     mpz_add(res, res, temp1); // res = res + temp1
//     mpz_mod(res, res, M);   // res = res mod M
// }



int main()
{
    #ifdef LOCAL
    freopen("test/10.in", "r", stdin);
    clock_t start_time = clock();
    #endif
    // Your code here
    int n;
    mpz_t p, q, e;
    mpz_t c, d, res, d1, d2, _p, _q;
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(c);
    mpz_init(d);
    mpz_init(d1);
    mpz_init(d2);
    mpz_init(_p);
    mpz_init(_q);
    mpz_init(res);
    gmp_scanf("%d%Zd%Zd%Zd",&n, p, q, e);   
    // calD(d, e, p, q);

    mpz_t M, temp1, temp2, temp3, temp4, temp5, temp6, phi;
    mpz_init(M);
    mpz_init(phi);
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);
    mpz_init(temp4);
    mpz_init(temp5);
    mpz_init(temp6);

    mpz_sub_ui(_p, p, 1);
    mpz_sub_ui(_q, q, 1);

    mpz_mul(phi, _p, _q);
    get_mod_inverse(d, phi, e);
    mpz_mod(d1, d, _p);
    mpz_mod(d2, d, _q);

    mpz_mul(M, p, q);       // M = p * q
    mpz_div(temp2, M, p);   // temp2 = M / p
    get_mod_inverse(temp3, p, temp2); // temp3 = temp2 ^ (-1) mod p

    mpz_mul(temp2, temp2, temp3); // temp2 = temp2 * temp3
    mpz_mod(temp2, temp2, M); // temp2 = temp2 mod M

    mpz_div(temp5, M, q);   // temp5 = M / q
    get_mod_inverse(temp6, q, temp5); // temp6 = temp5 ^ (-1) mod q
    mpz_mul(temp5, temp5, temp6); // temp5 = temp5 * temp6

    mpz_mod(temp2, temp2, M); // temp1 = temp1 mod M
    for(int i = 0; i<n; i++)
    {
        gmp_scanf("%Zd", c);
        // ChineseRemainderTheorem(res, c, d, p, q);
        // mpz_set_ui(res, 0);     // res = 0

        expmod(temp1, c, d1, p); // temp1 = c ^ d mod p
        mpz_mul(temp1, temp1, temp2); // temp1 = temp1 * temp2
        mpz_mod(temp1, temp1, M); // temp1 = temp1 mod M

        expmod(temp4, c, d2, q); // temp1 = c ^ d mod q
        mpz_mul(temp4, temp4, temp5); // temp4 = temp4 * temp5
        mpz_mod(temp4, temp4, M); // temp4 = temp4 mod M

        mpz_add(res, temp1, temp4); // res = res + temp4

        mpz_mod(res, res, M);   // res = res mod M
        gmp_printf("%Zd\n", res);
    }

    #ifdef LOCAL
    printf("Time used = %.2fms\n", (double)(clock() - start_time)); 
    #endif


    return 0;
}
