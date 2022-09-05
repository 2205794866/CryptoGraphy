#include<stdio.h>
#include<string.h>
// openssl库
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/sm3.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sm2.h>
#include <openssl/pkcs7.h>

// delete it if you submit the answer
#define LOCAL
#define DEBUG


// 证书
const char cacert[] = "\
-----BEGIN CERTIFICATE----- \n\
MIIB/zCCAaagAwIBAgIJAKKa0PAt9M1FMAoGCCqBHM9VAYN1MFsxCzAJBgNVBAYT \n\
AkNOMQ4wDAYDVQQIDAVIdUJlaTEOMAwGA1UEBwwFV3VIYW4xDTALBgNVBAoMBEhV \n\
U1QxDDAKBgNVBAsMA0NTRTEPMA0GA1UEAwwGY2Fyb290MB4XDTIwMDkyMDIwNTkx \n\
OVoXDTMwMDkxODIwNTkxOVowWzELMAkGA1UEBhMCQ04xDjAMBgNVBAgMBUh1QmVp \n\
MQ4wDAYDVQQHDAVXdUhhbjENMAsGA1UECgwESFVTVDEMMAoGA1UECwwDQ1NFMQ8w \n\
DQYDVQQDDAZjYXJvb3QwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAASJ8mm28JJR \n\
bZKLr6DCo1+KWimpKEsiTfZM19Zi5ao7Au6YLosyN71256MWmjwkwXxJeLa0lCfm \n\
kF/YWCX6qGQ0o1MwUTAdBgNVHQ4EFgQUAL5hW3RUzqvsiTzIc1gUHeK5uzQwHwYD \n\
VR0jBBgwFoAUAL5hW3RUzqvsiTzIc1gUHeK5uzQwDwYDVR0TAQH/BAUwAwEB/zAK \n\
BggqgRzPVQGDdQNHADBEAiAaZMmvE5zzXHx/TBgdUhjtpRH3Jpd6OZ+SOAfMtKxD \n\
LAIgdKq/v2Jkmn37Y9U8FHYDfFqk5I0qlQOAmuvbVUi3yvM= \n\
-----END CERTIFICATE----- \n\
";

// 用户B的私钥，接收方
const char pkeyB[] = "\
-----BEGIN EC PARAMETERS----- \n\
BggqgRzPVQGCLQ== \n\
-----END EC PARAMETERS----- \n\
-----BEGIN EC PRIVATE KEY----- \n\
MHcCAQEEINQhCKslrI3tKt6cK4Kxkor/LBvM8PSv699Xea7kTXTToAoGCCqBHM9V \n\
AYItoUQDQgAEH7rLLiFASe3SWSsGbxFUtfPY//pXqLvgM6ROyiYhLkPxEulwrTe8 \n\
kv5R8/NA7kSSvcsGIQ9EPWhr6HnCULpklw== \n\
-----END EC PRIVATE KEY----- \n\
";

void init()
{
    //加载
	ERR_load_crypto_strings();
	//配置
	OpenSSL_add_all_algorithms();
}


// 将证书转换为X509结构体
X509 *load_cert(const char *cert)
{
    BIO *bio =  BIO_new(BIO_s_mem());
    BIO_puts(bio, cert);
    if (bio == NULL)
    {
        printf("ERROR\n");
        return NULL;
    }
    X509 *x509 = PEM_read_bio_X509(bio, NULL, NULL, NULL);
    if (x509 == NULL)
    {
        printf("ERROR\n");
        return NULL;
    }
    BIO_free(bio);
    return x509;
}

// 将私钥转换为EVP_PKEY结构体
EVP_PKEY *load_pkey(const char *pkey)
{
    BIO *bio =  BIO_new_mem_buf((char *)pkey, strlen(pkey));
    if (bio == NULL)
    {
        printf("ERROR\n");
        return NULL;
    }
    EVP_PKEY *pkey1 = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    if (pkey1 == NULL)
    {
        printf("ERROR\n");
        return NULL;
    }
    BIO_free(bio);
    return pkey1;
}

int x509cb(int ok, X509_STORE_CTX* ctx) {
    return ok;
}

void pkcs7_func()
{
    char result[1000] = {0};

    // 私钥转化为EVP_PKEY结构体
    EVP_PKEY *pkey = load_pkey(pkeyB);

    // 证书转化为X509结构体
    X509_STORE *store = X509_STORE_new();
    X509 *cert = load_cert(cacert);
    X509_STORE_CTX* rootCertStoreCTX = X509_STORE_CTX_new();
    X509_STORE_add_cert(store, cert);
    X509_STORE_set_verify_cb_func(store, x509cb);//自验证

    // 读入密文
    BIO *en_bio = NULL;
    PKCS7 *p7 = NULL;
    en_bio = BIO_new_fd(fileno(stdin), BIO_NOCLOSE);
    p7 = PEM_read_bio_PKCS7(en_bio, NULL, NULL, NULL);
    if(p7 == NULL)
    {
        printf("ERROR\n");
        return;
    }
    BIO* de_bio = NULL;
    de_bio = PKCS7_dataDecode(p7, pkey, NULL, NULL);
    int len = BIO_read(de_bio, result, sizeof(result));
    if(len <= 0)
    {
        #ifdef DEBUG
        printf("BIO_read: ");
        #endif
        printf("ERROR\n");
        return;
    }
    result[len] = '\0';

    STACK_OF(PKCS7_SIGNER_INFO)* stack = NULL;//用于存储签名信息
    PKCS7_SIGNER_INFO* signInfo;
    stack = PKCS7_get_signer_info(p7);//获得签名者信息
    if (stack != NULL) {
        int num = sk_PKCS7_SIGNER_INFO_num(stack);//获取签名者个数
        for (int i = 0; i < num; i++) {
            signInfo = sk_PKCS7_SIGNER_INFO_value(stack, i);//依次获得签名者的信息
            int num2 = PKCS7_dataVerify(store, rootCertStoreCTX, de_bio, p7, signInfo);
            if (num2 <= 0) {
                printf("ERROR\n");
                return 0;
            }
        }
    }
    else {
        printf("ERROR\n");
        return 0;
    }
    PKCS7_free(p7);
    BIO_free(en_bio);
    BIO_free(de_bio);
    
    printf("%s", out);
    return;
}


int main()
{
    #ifdef LOCAL
    freopen("test/1.in", "r", stdin);
    #endif
    // Your code here
    // 初始化
    init();
    // 解密
    pkcs7_func();
    return 0;
}
