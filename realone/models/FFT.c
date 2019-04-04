#include "../helpers/math.h"
#include "FFT.h"
#include <math.h>
#include <stdlib.h>

//note: in this file, all the float value are shifted to the left by 16 bits
 
 float PI = 3.1415926535897932384626;

 Complex add (Complex c,  Complex rhs )   {
	 Complex newC = { 0,0};
    newC.i = c.i + rhs.i;
    newC.r = c.r + rhs.r;
    return newC ;
}
  Complex sub (Complex c,  Complex rhs )   {
	  Complex newC = {0,0};
	  newC.i = c.i - rhs.i;
	  newC.r = c.r - rhs.r;
    return newC ;
}
 Complex multiply (Complex c,  Complex rhs )   {
    Complex newC;
    newC.i = (c.r) * (rhs.i) + (c.i) * (rhs.r);
    newC.r = (c.r) * (rhs.r) - (c.i) * (rhs.i);
    return newC ;
    //return Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r )
}

 Complex divideByInt (Complex c,  float x )   {
    Complex newC;
    newC.i /= x;
    newC.r /= x;
    return newC ;
}

 Complex conjg (Complex c)  {
    Complex newC;
    newC.i = -c.i;
    newC.r = c.r;
    return newC ;
}
 
  int magnitude (Complex c)  {
    return ((int) c.r)*((int) c.r)+((int) c.i)*((int) c.i) ;
}

void init(int size, Complex *omegaInverse) {
        for (int i = 0; i < size; ++i) {
            Complex c ={cosf(2 * PI *i / size), -sinf(2 * PI *i/ size)};
            omegaInverse[i] = c;
    }
}



// void sdft(Complex *x, int n, Complex * omegaInverse, Complex *result) {
// 		for (int om = 0; om < n; om++) {
// 			Complex c = {0,0};
// 			init(om,n,omegaInverse);
// 			for (int k = 0; k < n; k++) {
// 				c = add(c, multiply(x[k],omegaInverse[k]));
// 			}
// 			result[om] = c;
// 		}
// 	}

void initExps(int size, Complex omegaInverse[][size]) {
    //initialize first line
    for(int k =0; k < size; k++){
            Complex c ={1,0};
            omegaInverse[0][k] = c;
    }
    for (int index = 1; index < size; ++index) {
        for(int k =0; k < size; k++){
            Complex c ={cosf(2 * PI / index * k), -sinf(2 * PI / index * k)};
            omegaInverse[index][k] = c;
        }
    }
}

void fdft(Complex *x, int n, Complex omegaExp[][n], Complex *result) {
		for (int om = 0; om < n; om++) {
			Complex c = {0,0};
			for (int k = 0; k < n; k++) {
				c = add(c, multiply(x[k],omegaExp[om][k]));
			}
			result[om] = c;
		}
	}

void fdft2(Complex *x, int n, Complex omegaExp[][n], Complex *result) {

        for (int om = 0; om < n; om++) {
			Complex c = {0,0};
			for (int k = 0; k < n; k++) {
				c = add(c, multiply(x[k],omegaExp[om][k]));
			}
			result[om] = c;
		}
	}


void _fft(Complex buf[], Complex out[], int n, int step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
 
		for (int i = 0; i < n; i += 2 * step) {
			Complex expo = {cosf(PI * i / n), -sinf(PI * i / n)};
			Complex t =multiply(expo,out[i + step]);
			buf[i / 2]     = add(out[i],t);
			buf[(i + n)/2] = sub(out[i],t);
		}
	}
}
 
void fft(Complex buf[], int n)
{
	Complex out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];
 
	_fft(buf, out, n, 1);
}