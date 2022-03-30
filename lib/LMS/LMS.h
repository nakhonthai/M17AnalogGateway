/* Arduino LMS Algorithm */
/*
This library will implement a simple LMS Noise cancelling algorithm

W_k+1 = W_k + 2 * mu * e_k * X_k
e_k = d_k - W_k^T * X_k

*/


#ifndef _LMS_H_INCLUDED	//prevent DSP Shield library from being invoked twice and breaking the namespace
#define _LMS_H_INCLUDED

#define defualtFilterSize 8
#define defaultMu 6553 //3276
class LMSClass {
	public:
		int 	filterSize; //how many input samples to track
		int*	input; //array of input samples
		int*	noise; //noise to minimize
		int*	coefficients; //filter coefficients
		int 	mu; //weighting factor

		LMSClass(int fSize = defualtFilterSize, int muValue=defaultMu); //sets filter size, clears buffers
		~LMSClass(); //clears and deallocates buffers
		void pushInput(int sample); //add new input sample
		void pushNoise(int sample); //add new noise sample
		int pullOutput(); //returns e_k, the most recent sample output, updates weights
		int computeLMS(int inputSamp, int noiseSamp);
	//private:
		int dotProduct(int* vector1, int* vector2, int length); //vector multiply, scaled result.
		int scalarMultiply(int a, int b); //scalar multiply, scaled result
	};
#endif