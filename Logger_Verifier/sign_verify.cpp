#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>

#include <cstring>

using namespace std;
string privateKey = "";
string publicKey = "";

RSA* createPrivateRSA(std::string key) {
	RSA *rsa = NULL;
	const char* c_string = key.c_str();
	BIO * keybio = BIO_new_mem_buf((void*)c_string, -1);
	if (keybio==NULL) {
		return 0;
	}
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
	return rsa;
}
 
// generate private key
RSA* genPrivateRSA() {
	RSA *rsa = RSA_generate_key(2048, 3, NULL, NULL);
	// PRIVATE KEY
	BIO *bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
	int pem_pkey_size = BIO_pending(bio);
	char *pem_pkey = (char*) calloc((pem_pkey_size)+1, 1);
	BIO_read(bio, pem_pkey, pem_pkey_size);
	// global variable 
	privateKey = pem_pkey;
	return rsa;
}

char* genPubicRAS(RSA * rsa) {

	BIO	*bp_public = NULL;
   
	// PUB KEY to string
	bp_public = BIO_new(BIO_s_mem());
	PEM_write_bio_RSA_PUBKEY(bp_public, rsa) ;
	int pub_pkey_size = BIO_pending(bp_public);
	char *pub_pkey = (char*) calloc((pub_pkey_size)+1, 1);
	BIO_read(bp_public, pub_pkey, pub_pkey_size);

	return pub_pkey;
}


RSA* createPublicRSA(std::string key) {
	RSA *rsa = NULL;
	BIO *keybio;
	const char* c_string = key.c_str();
	keybio = BIO_new_mem_buf((void*)c_string, -1);
	if (keybio==NULL) {
		return 0;
	}
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
	return rsa;
}


/*
int main() {

	// Private keygen
	RSA * privateRSA = genPrivateRSA();
	publicKey = genPubicRAS(privateRSA);

	std::cout << "privateKey----" << std::endl << privateKey << std::endl;
	std::cout << "privateKey size(byte): " << privateKey.capacity() << std::endl;
	std::cout << "publicKey----" << std::endl << publicKey << std::endl;
	std::cout << "publicKey size(byte): " << publicKey.capacity() << std::endl;
	
	int pubKey_bufsize = publicKey.capacity();
	std::cout << "pubKey_bufsize: " << pubKey_bufsize << std::endl;
	
	char *pubKey_buffer = new char[pubKey_bufsize];
    strcpy(pubKey_buffer, publicKey.c_str());
	
}
*/
