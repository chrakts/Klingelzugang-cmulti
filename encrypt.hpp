/*
 * encrypt.hpp
 *
 * Created: 16.11.2015 21:08:01
 *  Author: Christof
 */


#ifndef ENCRYPT_H_
#define ENCRYPT_H_

enum Encrypt_mode {Encrypt_Undefined=0, Encrypt_KeyDefined=1, Encrypt_SubKeyDefined=2};

class Encrypt
{
	private:

	uint8_t key[16];
	uint8_t subkey[16];
	AES_t *Encrypt_aes;
	uint8_t status;

	protected:

	public:
	Encrypt(void);
	Encrypt(uint8_t *newkey);
	Encrypt(uint8_t *newkey,uint8_t dummy);
	uint8_t Encrypt_Data_Direct(uint8_t *data);
	uint8_t Encrypt_Data_Wait();
	uint8_t Encrypt_Data(uint8_t *data);
	uint8_t Decrypt_Data(uint8_t *data);
	void Get_Data(uint8_t *data);
	void Get_Key(uint8_t *keysave);
	void Output(uint8_t *data,Serial* file);
	void Output(char *data,Serial* file);
	void Output_Encrypt(Serial* file, const char node, const char function, const char job, uint8_t *code );
	void Output_Encrypt_Code(Serial* file, const char node, const char function, const char job, uint8_t *data, uint8_t *code, uint8_t length );
	void Get_Encrypt_Code(uint8_t *random, uint8_t *data, uint8_t *code, uint8_t length );
};

#endif /* ENCRYPT_H_ */
