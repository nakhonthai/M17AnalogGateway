#include "LMS.h"


LMSClass::LMSClass(int fSize, int muValue)
{
	filterSize = fSize; //how many input samples to track
	mu = muValue;
	input = new int[filterSize](); //array of input samples
	noise = new int[filterSize](); //noise to minimize
	coefficients = new int[filterSize](); //filter coefficients
}

LMSClass::~LMSClass()
{
	filterSize = 0; //how many input samples to track
	delete[] input;//array of input samples
	delete[] noise; //noise to minimize
	delete[] coefficients; //filter coefficients
}

void LMSClass::pushInput(int sample)
{
	for(int i = 0; i<(filterSize-1); i++)
	{
		input[i] = input[i+1];
	}
	input[filterSize-1] = sample;
}

void LMSClass::pushNoise(int sample)
{
	for(int i = 0; i<(filterSize-1); i++)
	{
		noise[i] = noise[i+1];
	}
	noise[filterSize-1] = sample;
}

int LMSClass::pullOutput()
{
	//compute LMS
	int e_k = input[0] - dotProduct(coefficients, noise, filterSize);
	for(int i = 0; i < filterSize; i++)
	{
		coefficients[i] += scalarMultiply(scalarMultiply(2 * mu, e_k), noise[i]);
	}
	//return error signal
	return e_k;
}

int LMSClass::computeLMS(int inputSamp, int noiseSamp)
{
	pushInput(inputSamp);
	pushNoise(noiseSamp);
	return pullOutput();
}

int LMSClass::dotProduct(int* vector1, int* vector2, int length) //vector multiply, scaled result.
{
	int result = 0;
	for(int i = 0; i < length; i++)
	{
		result += scalarMultiply(vector1[i], vector2[i]);
	}
	return result;
}

int LMSClass::scalarMultiply(int a, int b)
{
	 // precomputed value:
	#define Q 15
 	#define K   (1 << (Q-1))
 
	int result;
	long temp;
	temp = (long)a * (long)b; // result type is operand's type
	// Rounding; mid values are rounded up
	temp += K;
	// Correct by dividing by base
	result = temp >> Q;
	return result;
}