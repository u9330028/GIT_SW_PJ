//
//	sf_bigint.h
//

#ifndef	__SF_BIGINT_H__
#define	__SF_BIGINT_H__

#include "sfdefine.h"

#if	defined (CPU_16BIT)
#define		BIGINT_MAX_SIZE				32			// 32 word = 1024 bit
#else
#define		BIGINT_MAX_SIZE				64			// 64 word = 2048 bit
#endif

#ifdef  __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
//
//	BIGINT_GetByteLength : get byte length of input data
//
//	return : byte_len		: byte length of input data
//
int far		BIGINT_GetByteLength	(SF_WORD far *input, int inputSize);

///////////////////////////////////////////////////////////////////////////////
//
//	BIGINT_GetWordLength : get word length of input data
//
//	return : word_len		: word length of input data
//
int far BIGINT_GetWordLength(SF_WORD far *input, int inputMaxSize);

///////////////////////////////////////////////////////////////////////////////
//
//	BIGINT_Comp : compare two big integer input
//
//	input  : inputA			: pointer to inputA data
//			 intputASize	: length of inputA
//			 inputB			: pointer to inputB data
//			 inputBSize		: length of inputB
//
//	return :  1				: if A > B
//			  0				: if A = B
//			 -1				: if A < B
//
int far BIGINT_Comp(SF_WORD far *inputA, int inputASize, SF_WORD far *inputB, int inputBSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_Add : calc A + B where A, B >= 0.
//
//	output : output			: pointer to output buffer
//	input  : inputA			: pointer to inputA data
//			 inputASize		: length of inputA
//			 inputB			: pointer to inputB data
//			 inputBSize		: length of inputB
//
//	return : outputSize		: length of output
//	
//	NOTE : output and inputA and inputB can be same pointer.
//
int far BIGINT_Add(SF_WORD far *output, SF_WORD far *inputA, int inputASize, SF_WORD far *inputB, int inputBSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_Sub : calc A - B where A >= B >= 0.
//
//	output : output			: pointer to output buffer
//	input  : inputA			: pointer to inputA data
//			 inputASize		: length of inputA
//			 inputB			: pointer to inputB data
//			 inputBSize		: length of inputB
//
//	return : outputSize		: length of output
//			 -1				: fail (A < B)
//
//	NOTE : output and inputA can be same pointer.
//
int far BIGINT_Sub(SF_WORD far *output, SF_WORD far *inputA, int inputASize, SF_WORD far *inputB, int inputBSize);

///////////////////////////////////////////////////////////////////////////////
//
//	BIGINT_Div : input = divider * quotient + remainder
//
//	input  : input			:
//			 divider		:
//	output : quotient		:
//			 remainder		:
//
//	NOTE : input value is changed after divie operation.
//
void far BIGINT_Div(SF_WORD far *quotient, SF_WORD far *remainder, SF_WORD far *input, int inputSize,
					SF_WORD far *divider, int dividerSize);


/////////////////////////////////////////////////////////////////////
//
//	BIGINT_Mult : calc A * B where A, B >= 0
//
//	output : output			: pointer to output buffer
//	input  : inputA			: pointer to inputA data
//			 inputASize		: length of inputA
//			 inputB			: pointer to inputB data
//			 inputBSize		: length of inputB
//
//	return : outputSize		: length of output
//
int far BIGINT_Mult(SF_WORD far *output, SF_WORD far *inputA, int inputASize, 
					SF_WORD far *inputB, int inputBSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_MultKaratsuba : Karatsuba Multiplication
//
//	output : output			: pointer to output buffer
//	input  : inputA			: pointer to inputA data
//			 inputB			: pointer to inputB data
//			 inputSize		: length of input
//
//	return : outputSize		: length of output
//
//	NOTE : the length of inputA and inputB should be equal
//
int far BIGINT_MultKaratsuba(SF_WORD far *output, SF_WORD far *inputA, SF_WORD far *inputB, int inputSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_Sqr : Ordinary Squaring
//
//	output : output			: pointer to output buffer
//	input  : input			: pointer to input data
//			 inputSize		: length of input
//
//	return : outputSize		: length of output
//
int far BIGINT_Sqr(SF_WORD far *output, SF_WORD far *input, int inputSize);

int far BIGINT_SqrKaratsuba(SF_WORD far *output, SF_WORD far *input, int inputSize);

SF_WORD far BIGINT_MontInit(SF_WORD far *constant, SF_WORD far *prime, int primeSize);

void far BIGINT_MontReduction(SF_WORD far *result, SF_WORD far *input, SF_WORD far *prime, SF_WORD montConst, int intSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_ModReduction :
//
//	output : output			: pointer to output buffer
//	input  : input			: pointer to input data
//			 inputSize		: length of input
//			 mod			: pointer to mod data
//			 modSize		: length of mod
//
//	return : outputSize		: length of output
//
int far BIGINT_ModReduction(SF_WORD far *output, SF_WORD far *input, int inputSize, 
							SF_WORD far *mod, int modSize);

/////////////////////////////////////////////////////////////////////
//
//	BIGINT_ModExp : Modulus exponentiation using montgomery method
//
//	output : result			: pointer to the result buffer
//	input  : input			: pointer to the input data
//			 inputSize		: length of input data
//			 exponent		: pointer to the exponent
//			 expSize		: length of exponent
//			 mod			: pointer to the modulus
//			 modSize		: length of modulus
//
//	return : resultSize		: length of result data
//
int far BIGINT_ModExp(SF_WORD far *result, SF_WORD far *input, int inputSize, 
					  SF_WORD far *exponent, int expSize, SF_WORD far *mod, int modSize);

#ifdef  __cplusplus
}
#endif

#endif
