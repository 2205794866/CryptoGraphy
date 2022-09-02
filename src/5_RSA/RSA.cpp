#include <stdio.h>
#include <time.h>
#include <gmp.h>
// delete it if you submit the answer
#define LOCAL
#define DEBUG

// 素性检测
bool isPrime(mpz_t n)
{
    int k = 0;
    mpz_t m, a, b, temp;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    mpz_init(m);
    mpz_init(a);
    mpz_init(b);
    mpz_init(temp);
    mpz_set(temp, n);          // temp = n
    mpz_sub_ui(temp, temp, 1); // temp = n - 1
    mpz_mod_ui(m, temp, 2);    // m = temp % 2
    while (mpz_cmp_ui(m, 0) == 0)
    {                              // while m == 0
        mpz_div_ui(temp, temp, 2); // temp = temp / 2
        mpz_mod_ui(m, temp, 2);    // m = temp % 2
        k++;                       // k++
    }
    mpz_sub_ui(m, n ,1); // m = n - 1
    if (k == 0)              // k == 0 => n - 1是奇数， n 为偶数
    {                
        return false;       // 返回false
    }
    mpz_sub_ui(temp, n, 1);         // temp = n - 1
    for (int i = 0; i < 300; i++)
    {
        mpz_urandomm(a, state, n);   // a = random(0, n)
        if(mpz_cmp_ui(a, 1) == 0 || mpz_cmp_ui(a, 0) == 0)    // a == 1
        {
            continue;                // continue
        }
        mpz_powm(b, a, m, n);        // b = a^m % n
        if (mpz_cmp_ui(b, 1) == 0)  // if b == 1
        { 
            return true;
        }
        for (int j = 0; j < k; j++)
        {
            if (mpz_cmp(b, temp) == 0)  // if b == temp
            { 
                return true;
            }
            mpz_powm_ui(b, b, 2, n); // b = b^2 % n
        }
    }
    return false;
}

// 求解公因数
void get_gcd(mpz_t res, mpz_t a, mpz_t b)
{
    mpz_t temp;                     // 余数
    mpz_t temp_a;
    mpz_t temp_b;
    mpz_init(temp);                 // 初始化
    mpz_init_set(temp_a, a);        // temp_a = a
    mpz_init_set(temp_b, b);        // temp_b = b
    mpz_mod(temp, temp_a, temp_b);  // temp = a % b
    while (mpz_cmp_ui(temp, 0) != 0)   // temp != 0
    {
        mpz_set(temp_a, temp_b);     // temp_a = temp_b
        mpz_set(temp_b, temp);       // temp_b = temp
        mpz_mod(temp, temp_a, temp_b); // temp = temp_a % temp_b
    }
    mpz_set(res, temp_b);
    return;
}

// 求解模逆
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
    if(mpz_cmp_ui(temp_b, 1) != 0)
    {
        return false;
    }
    if(mpz_cmp_ui(t, 0) < 0)
    {
        mpz_add(t, t, a);
    }
    mpz_set(res, t);
    return true;
}

bool test(mpz_t e, mpz_t p, mpz_t q)
{
    mpz_t phi, temp, temp1, temp2;
    mpz_init(phi);
    mpz_init(temp);
    mpz_init(temp1);
    mpz_init(temp2);
    // e 过小
    if(mpz_cmp_ui(e, 10) < 0)
    {
        // printf("1 :");
        return false;
    }
    // p , q不是素数
    if(!isPrime(p) || !isPrime(q))
    {
        // printf("2 :");
        return false;
    }
    // p, q差值过小
    mpz_sub(temp, p, q);
    mpz_abs(temp, temp);    // temp = |p - q|
    mpz_div_ui(temp1, p, 10); // temp1 = p / 10
    if(mpz_cmp(temp, temp1) < 0)    // |p - q| < p / 10
    {
        // printf("3 :");
        return false;
    }
    // p-1 和 q-1 公因数过大
    mpz_sub_ui(temp, p, 1);
    mpz_sub_ui(temp1, q, 1);
    get_gcd(temp2, temp, temp1);
    if(mpz_cmp_ui(temp2, 100000) > 0)
    {
        // printf("4 :");
        return false;
    }
    // e和phi不互质
    mpz_mul(phi, temp, temp1);
    get_gcd(temp2, e, phi);
    if(mpz_cmp_ui(temp2, 1) != 0)
    {
        return false;
    }
    return true;
}




void calD(mpz_t e, mpz_t p, mpz_t q)
{
    if(test(e, p, q))
    {
        mpz_t phi, d;
        mpz_t temp, temp1, temp2;
        mpz_init(phi);
        mpz_init(temp);
        mpz_init(temp1);
        mpz_init(temp2);
        mpz_sub_ui(temp, p, 1);
        mpz_sub_ui(temp1, q, 1);
        mpz_mul(phi, temp, temp1);
        get_mod_inverse(d, phi, e);
        gmp_printf("%Zd\n", d);
    }
    else
    {
        gmp_printf("ERROR\n");
    }
    return;
}



int main()
{
// LOCAL func
#ifdef LOCAL
    freopen("test/1.in", "r", stdin);
#endif
    // Your code here

    int n;
    scanf("%d", &n);
    mpz_t p, q, e;
    mpz_init(e);
    mpz_init(p);
    mpz_init(q);
    for (int i = 0; i < n; i++)
    {
        gmp_scanf("%Zd%Zd%Zd", e, p, q);
        calD(e, p, q);
    }
    // gmp_scanf("%Zd", p);
    // printf("%d\n", isPrime(p));
    return 0;
}