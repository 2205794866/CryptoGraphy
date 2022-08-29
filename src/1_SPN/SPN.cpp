#include <stdio.h>

// 明文，密文使用unsigned short 2字节
typedef unsigned short ushort;

// S盒， 0=为加密，1=为解密
ushort s[2][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, {14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5}};
ushort p[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

// 快速读取十六进制数
inline unsigned int read()
{
    unsigned int x = 0;
    char c = getchar();
    while (c != ' ' && c != '\n')
    {
        // 如果不是空格或换行，则继续读取
        x = x * 16 + ((c >= 'a') ? (c - 'a' + 10) : (c - '0'));
        c = getchar();
    }
    return x;
}

// 快速输出十六进制数
inline void print(ushort x)
{
    for (int i = 3; i >= 0; i--)
    {
        // 将十六进制数输出为四位十六进制数
        char c = x >> (i * 4) & 0xf;
        // 转换为字符
        c += c > 9 ? 'a' - 10 : '0';
        putchar(c);
    }
}

// 白化 Nr为轮数
ushort xor_func(unsigned int key, int Nr, ushort text)
{
    unsigned int k = key >> 4 * (4 - Nr) & 0xffff;
    return text ^ k;
}

// S盒 代换， mode=0为加密，mode=1为解密
ushort S_func(ushort text, int mode)
{
    text = s[mode][text & 0xf] | (s[mode][(text >> 4) & 0xf] << 4) | (s[mode][(text >> 8) & 0xf] << 8) | (s[mode][(text >> 12) & 0xf] << 12);
    return text;
}

// P盒 代换
ushort P_func(ushort text)
{
    ushort res = 0;
    for (int i = 0; i < 16; i++)
    {
        res += ((text >> (15 - i)) & 1) << (15 - p[i]);
    }
    return res;
}

// mode = 0 为加密，mode = 1 为解密
ushort SPN(unsigned int key, ushort text, int mode)
{
    ushort res = text;
    if (mode == 1)
    {
        res = xor_func(key, 4, res);
        res = S_func(res, 1);
        res = xor_func(key, 3, res);
    }

    for (int i = 0; i < 3; i++)
    {
        if (mode == 0)
        {
            res = xor_func(key, i, res);
            res = S_func(res, 0);
            res = P_func(res);
        }
        else
        {
            res = P_func(res);
            res = S_func(res, 1);
            res = xor_func(key, 2 - i, res);
        }
    }

    if (mode == 0)
    {
        res = xor_func(key, 3, res);
        res = S_func(res, 0);
        res = xor_func(key, 4, res);
    }
    return res;
}

int main()
{
    int n;
    unsigned int key;
    ushort plain_text, cipher_text, decrypted_text;
    scanf("%d", &n);
    getchar();
    for (int i = 0; i < n; i++)
    {
        key = read();
        plain_text = read();
        cipher_text = SPN(key, plain_text, 0);
        print(cipher_text);
        putchar(' ');
        
        // 对密文最后一位进行异或
        cipher_text = cipher_text ^ 0x1;
        decrypted_text = SPN(key, cipher_text, 1);
        print(decrypted_text);
        putchar('\n');
    }
    return 0;
}