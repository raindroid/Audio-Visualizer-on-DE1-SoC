
#ifndef _FFT_H__
#define __FFT_H__
typedef struct Complex  {
	int r, i ;
} Complex;

 void setReal ( Complex *c,  int x );
 Complex add (Complex c,  Complex rhs );
 Complex sub (Complex c,  Complex rhs );
 Complex multiply (Complex c,  Complex rhs );
 Complex divideByInt (Complex c,  int x );
 Complex conjg (Complex c);
void initOmega (Complex *omega, Complex *omegaInverse,   int n );
 void transform ( Complex *a,  int n,  Complex* omega );
 void dft ( Complex *a,  int n ,  Complex* omega);
 void idft ( Complex *a,  int n,  Complex* omegaInverse );
 void FastFourierTransform (Complex *a,  int n );
 void setReal ( Complex *c,  int x );
 int getReal ( Complex c );
void idftMag (unsigned * result, Complex *a,  int n,  Complex* omegaInverse );


 #endif // !_FFT_H__