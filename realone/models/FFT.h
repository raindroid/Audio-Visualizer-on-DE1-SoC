
#ifndef _FFT_H__
#define __FFT_H__
typedef struct Complex  {
	float r, i ;
} Complex;

 Complex add (Complex c,  Complex rhs );
 Complex sub (Complex c,  Complex rhs );
 Complex multiply (Complex c,  Complex rhs );
 Complex divideByInt (Complex c,  float x );
 Complex conjg (Complex c);
 int magnitude (Complex c) ;
void init(int size, Complex *omegaInverse);
void transform ( Complex *a,  int n,  Complex* omega );
void dft ( Complex *a,  int n ,  Complex* omega);
void idft ( Complex *a,  int n,  Complex* omegaInverse );
void FastFourierTransform (Complex *a,  int n );
void idftMag (unsigned * result, Complex *a,  int n,  Complex* omegaInverse );
void sdft(Complex *x, int n, Complex * omegaInverse, Complex *result);
void initExps(int size, Complex omegaInverse[][size]);
void fdft(Complex *x, int n, Complex omegaExp[][n], Complex *result);
void _fft(Complex buf[], Complex out[], int n, int step);
void fft(Complex buf[], int n);



 #endif // !_FFT_H__