#include <stdio.h>
#include "openssl/evp.h"
#include <string.h>

#define MAX_LENGTH 16 // A global variable t define max-length

int main (int argc, const char *argv[])
{
	//INITIALISE VARIABLES
	//the hex 'd23a' would be written as {0xd2,0x3a} in C
	unsigned char ciphertxt[] = {0x20, 0x75, 0x38, 0x6b, 0x75, 0xee, 0xd8, 0xb4, 0xf2, 0xb4, 0xa9, 0xc9, 0xb7, 0x69, 0x67, 0xd0, 0x57, 0xb4, 0xa4, 0x41, 0xd3, 0x49, 0xc1, 0x5d, 0xd4, 0xb8, 0xbf, 0x4b, 0x87, 0x44, 0x5a, 0x9e};

	// This is the plaintext that we are encrypting. Do not change it as it may result in a different ciphertext
	char plaintext[]= "This is a top secret";

	// Pointer fp used to point to the English words file provided along.
	FILE *fp = fopen("words.txt", "r");

	// The initialization Vector is set to 0
	unsigned char iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	// Output buffer set to 1024 size
	unsigned char outbuf [1024];

	// Some other variables that are used in the program
	int outlen = 0, tmplen = 0;
	
	//flag
	int isCipherTextMatch = 1;

	EVP_CIPHER_CTX ctx;
	
	// Check file can be opened in 'r' permission
	if (fp == NULL)
	{
		printf("Dictionary file not found.\n");
		return 0;
	}

	char key [MAX_LENGTH]; //array to store the key getting from the dictionary

	//For each key word in the dictionary
	while (fgets(key, MAX_LENGTH, fp) != NULL)
	{

		//Add the buffer
		for(int i = 0; i < MAX_LENGTH; i++)
		{
			if (key[i] == '\n' || key[i] == 0 )
			{
				for (int j = i; j < strlen(key); j++)
				{
					key[j] = ' ';
				}
				break;
			}
		}

		// Use the EVP library to initialize cipher
		EVP_CIPHER_CTX_init (&ctx);

		// Use the EVP library to encrypt the cipher
		EVP_EncryptInit_ex (&ctx, EVP_aes_128_cbc(), NULL, key, iv);

		// Checking to see if EVP_EncryptUpdate is valid or not
		if (!EVP_EncryptUpdate (&ctx, outbuf, &outlen, plaintext, strlen(plaintext)))
		{
			printf("EVP_EncryptUpdate is not valid.\n");
			return 0;
		}

		// Buffer passed to EVP_EncryptFinal() must be after data just encrypted to avoid overwriting it

		// Checking to see if !EVP_EncryptFinal_ex is valid or not
		if (!EVP_EncryptFinal_ex (&ctx, outbuf + outlen, &tmplen))
		{
			printf("EVP_EncryptFinal_ex is not valid.\n");
			return 0;
		}

		outlen += tmplen;

		EVP_CIPHER_CTX_cleanup (&ctx);

		//print curret key and its corresponding ciphertext
		printf ("The key is: %s       The corresponding cipher text is:  ", key); 

		//Print out the buffer
		for(int j = 0; j < outlen; j++) 
		{
			printf("%x", outbuf[j]);
		}
		printf ("\n");

		//reset cipher flag
		isCipherTextMatch = 1;
		for(int q = 0; q < outlen; q++)
		{
			/* Judge whether the cipher text generated by this key is match for the provided one */
			if (outbuf[q] != ciphertxt[q])
			{
				isCipherTextMatch = 0;
				
				//breaking here to make the program faster :)
				break;
			}

			
			/* As the whole ciphertext cannot be matched at once, use this loop to match it bit by bit */
		}
			/* If the generated ciphertext matched with the one provided*/
		if (isCipherTextMatch == 1)
		{
			printf ("\n*****************************************************\n");

			/* Print the key used */
			printf ("The key is: %s\n", key);

			/* Print the text in the buffer */
			printf ("The buffer text is: ");

			//Print out the buffer
			for(int j = 0; j < outlen; j++) 
			{
				printf("%x", outbuf[j]);
			}
			printf ("\n");

			/* Print the provided ciphertext*/
			printf ("The ciphertext is:");

			//Print out the buffer
			for(int j = 0; j < outlen; j++) 
			{
				printf("%x", ciphertxt[j]);
			}
			printf ("\n");

			/* Print the length of the ciphertext */
			printf ("The length of the ciphertext is: %u\n", outlen);

			printf ("\n*****************************************************\n");
			return 0;
		}
	}
	fclose (fp);
	return 0;
}
