#include<stdio.h>
#include<gmp.h>
// delete it if you submit the answer
// #define LOCAL



// 计算 res = a^e mod n
void expmod(mpz_t res, mpz_t a, mpz_t e, mpz_t n)
{
    mpz_t e_temp, n_temp, a_temp, b, temp;
    mpz_init(e_temp);
    mpz_init(n_temp);
    mpz_init(a_temp);
    mpz_init(b);
    mpz_init(temp);
    mpz_set(e_temp, e);     // e_temp = e
    mpz_set(n_temp, n);     // n_temp = n
    mpz_set_ui(a_temp, 1);  // a_temp = 1
    mpz_set(b, a);          // b = a
    while(mpz_cmp_ui(e_temp, 0) != 0)
    {
        mpz_mod_ui(temp, e_temp, 2);
        if(mpz_cmp_ui(temp, 1) == 0)
        {
            mpz_mul(a_temp, a_temp, b);
            mpz_mod(a_temp, a_temp, n_temp);
        }
        mpz_pow_ui(b, b, 2);
        mpz_mod(b, b, n_temp);
        mpz_div_ui(e_temp, e_temp, 2);
    }
    mpz_set(res, a_temp);
}


int main()
{
    #ifdef LOCAL
    freopen("test/6.in", "r", stdin);
    freopen("a.out", "w", stdout);
    #endif
    // Your code here
    int t;
    scanf("%d", &t);
    mpz_t m, e, p, q, n, res;
    mpz_init(m);
    mpz_init(e);
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(res);
    for(int i = 0; i<t; i++)
    {
        gmp_scanf("%Zd%Zd%Zd%Zd", e, m, p, q);
        mpz_mul(n, p, q);
        expmod(res, m, e, n);
        gmp_printf("%Zd\n", res);
    }
    return 0;
}