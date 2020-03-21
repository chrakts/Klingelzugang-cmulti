/*
 * encrypt.cpp
 *
 * Created: 16.11.2015 21:07:36
 *  Author: Christof
 */

#include "Klingelzugang.h"

Encrypt::Encrypt(void)
{
	status = Encrypt_Undefined;
	Encrypt_aes = &AES;
}

Encrypt::Encrypt(uint8_t *newkey)
{
int i;
uint8_t dummy[16];
	for (i=0;i<16;i++)
	{
		key[i] = newkey[i];
	}
	status = Encrypt_KeyDefined;
	Encrypt_aes = &AES;
	Encrypt_Data(dummy);
}

Encrypt::Encrypt(uint8_t *newkey,uint8_t dummy)
{
	int i;
	for (i=0;i<16;i++)
	{
		key[i] = newkey[i];
	}
	status = Encrypt_KeyDefined;
	Encrypt_aes = &AES;
}

uint8_t Encrypt::Encrypt_Data_Direct(uint8_t *data)
{
int i;

/*	if ( (status&Encrypt_KeyDefined) )
	{*/
		Encrypt_aes->CTRL = AES_RESET_bm ; // setzt das AES-Modul zurück

		for (i=0;i<16;i++)
			Encrypt_aes->KEY=key[i];
		for (i=0;i<16;i++)
			Encrypt_aes->STATE=data[i];
		Encrypt_aes->CTRL = AES_START_bm ;
		return(0);
/*	}
	else
		return(-1);*/
}

uint8_t Encrypt::Encrypt_Data_Wait()
{
int i;

	while(!(Encrypt_aes->STATUS & AES_SRIF_bm))
		;
	for (i=0;i<16;i++)
		subkey[i] = Encrypt_aes->KEY;
	status |= Encrypt_SubKeyDefined;
	return(0);
}

uint8_t Encrypt::Encrypt_Data(uint8_t *data)
{
	if ( Encrypt_Data_Direct(data)==0 )
	{
		Encrypt_Data_Wait();
		return(0);
	}
	else
	{
		return(-1);
	}
}

uint8_t Encrypt::Decrypt_Data(uint8_t *data)
{
	int i;

	if (status&Encrypt_SubKeyDefined)
	{
		Encrypt_aes->CTRL = AES_RESET_bm  | AES_DECRYPT_bm; // setzt das AES-Modul zurück
		_delay_us(100);
		Encrypt_aes->CTRL = AES_DECRYPT_bm; // setzt das AES-Modul zurück

		for (i=0;i<16;i++)
			Encrypt_aes->KEY=subkey[i];
		for (i=0;i<16;i++)
			Encrypt_aes->STATE=data[i];
		Encrypt_aes->CTRL = AES_START_bm  | AES_DECRYPT_bm;

		while(!(Encrypt_aes->STATUS & AES_SRIF_bm))
			;
		//		status = Encrypt_SubKeyDefined;
		return(0);
	}
	else
		return(-1);
}


void Encrypt::Get_Data(uint8_t *data)
{
int i;
	for (i=0;i<16;i++)
		data[i] = Encrypt_aes->STATE;
}

void Encrypt::Get_Key(uint8_t *keysave)
{
	int i;
	for (i=0;i<16;i++)
	keysave[i] = Encrypt_aes->KEY;
}

void Encrypt::Output(uint8_t *data,Serial* file)
{
int i;
	file->print("Data:");
	for (i=0;i<16;i++)
	{
		file->pformat("%x",data[i]);
	}
	file->print("\n\r");
	file->print("Key:");
	for (i=0;i<16;i++)
	{
		file->pformat("%x",Encrypt_aes->KEY);
	}
	file->print("\n\r");

}

void Encrypt::Output(char *data,Serial* file)
{
	int i;
	file->print("Data:");
	for (i=0;i<16;i++)
	{
		file->transmit(data[i]);
	}
	file->print("\n\r");
	file->print("Key:");
	for (i=0;i<16;i++)
	{
		file->pformat("%x",Encrypt_aes->KEY);
	}
	file->print("\n\r");

}

void Encrypt::Output_Encrypt(Serial* file, const char node, const char function, const char job, uint8_t *data )
{
uint8_t i;
uint16_t res;
CRC_Calc cc;
uint8_t lokal_data[16];
char qput[50];

	Encrypt_Data_Direct(data);
	sprintf(qput,"\\>%c%c%c",node,function,job);
	cc.String(qput);
	Encrypt_Data_Wait();
	Get_Data(lokal_data);
	for(i=0;i<16;i++)
	{
		qput[5+i*2] = ( (lokal_data[i] &0xf0)>>4 )+65;
		cc.Data(qput[5+i*2]);
		qput[5+i*2+1] = ( lokal_data[i] &0x0f )+65;
		cc.Data(qput[5+i*2+1]);
	}
	res = cc.Get_CRC();
	for (i=0;i<4;i++)
	{
		qput[40-i] = (res & 0x000f)+ 65;
		res = res >> 4;
	}
	qput[41] = '<';
	qput[42] = '\\';
	qput[43] = 0;
	file->print(qput);
}

void Encrypt::Output_Encrypt_Code(Serial* file, const char node, const char function, const char job, uint8_t *data, uint8_t *code, uint8_t length )
{
uint8_t i;
uint8_t local[16];
	for (i=0;i<length;i++)
		local[i] = data[i] ^ code[i];
	for (i=length;i<16;i++)
		local[i] = data[i];
	Output_Encrypt( file, node, function, job, local );
}

void Encrypt::Get_Encrypt_Code(uint8_t *random, uint8_t *data, uint8_t *code, uint8_t length )
{
	uint8_t i;
	for (i=0;i<length;i++)
		code[i] = data[i] ^ random[i];
//	output_str(KNET,"\nCode:");
/*	for (i=0;i<length;i++)
		output(KNET,code[i]);*/
}
