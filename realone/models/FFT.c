#include "../helpers/math.h"
#include "FFT.h"
#include <math.h>

//note: in this file, all the double value are shifted to the left by 16 bits

 void setReal ( Complex *c,  int x )  {  c->r = x ;  }
 int getReal ( Complex c )  {  return c.r ;  }
 Complex add (Complex c,  Complex rhs )   {
    Complex newC;
    newC.i += c.i;
    newC.r += c.r;
    return newC ;
}
  Complex sub (Complex c,  Complex rhs )   {
   Complex newC;
    newC.i -= c.i;
    newC.r -= c.r;
    return newC ;
}
 Complex multiply (Complex c,  Complex rhs )   {
    Complex newC;
    newC.i = (c.r>>8) * (rhs.i>>8) + (c.i>>8) * (rhs.r>>8);
    newC.r = (c.r>>8) * (rhs.r>>8) - (c.i>>8) * (rhs.i>>8);
    return newC ;
    //return Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r )
}

 Complex divideByInt (Complex c,  int x )   {
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
 
  unsigned magnitude (Complex c)  {
    return ((c.i>>8)*(c.i>>8) + (c.r>>8)*(c.r>>8));
}


 void initOmega (Complex *omega, Complex *omegaInverse,   int n )  {
    for ( int i = 0 ; i < n ; ++ i )  {
        Complex c;
        c.i = VIS_FastSin_d16 ( 360 / n * i );
        c.r = VIS_FastCos_d16 ( 360 / n * i);
        omega [i] = c;
        omegaInverse [i] = conjg ( omega [i] ) ;
    }
}

void transform ( Complex *a,  int n,  Complex* omega ) {
    for ( int i = 0, j = 0 ; i < n ; ++ i )  {
        if ( i > j ){
            //swap ( a [i], a [j] )
            Complex temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        } 
        for( int l = n >> 1 ; ( j ^= l ) < l ; l >>= 1 ) ;
    }
    
    for ( int l = 2 ; l <= n ; l <<= 1 )  {
        int m = l / 2;
        for (Complex * p = a ; p < a + n ; p += l )  {
            for ( int i = 0 ; i < m ; ++ i )  {
                Complex t = multiply(omega [n / l * i] , p [m + i]);
                p [m + i] = sub (p [i] , t) ;
                p [i] = add(p [i], t) ;
            }
        }
    }
}

void dft ( Complex *a,  int n ,  Complex* omega)  {
        transform ( a, n, omega ) ;
}

void idft ( Complex *a,  int n,  Complex* omegaInverse )  {
        transform ( a, n, omegaInverse ) ;
        for ( int i = 0 ; i < n ; ++ i ) a [i] = divideByInt(a[i],n) ;
    }
void FastFourierTransform (Complex *a,  int n ){
    Complex omega [n], omegaInverse[n];
    initOmega (omega, omegaInverse, n);
    dft ( a , n ,  omega);
}
void idftMag (unsigned * result, Complex *a,  int n,  Complex* omegaInverse )  {
        transform ( a, n, omegaInverse ) ;
        for ( int i = 0 ; i < n ; ++ i ) result [i] = (magnitude(a [i]))>>16 ;
    }

// int * separate_freq_bands(fftw_complex out[M / 2 + 1], int bars, int lcf[200],
// 			 int hcf[200], float k[200], int channel, double sens, double ignore) {
// 	int o,i;
// 	float peak[201];
// 	static int fl[200];
// 	static int fr[200];
// 	int y[M / 2 + 1];
// 	float temp;


// 	// process: separate frequency bands
// 	for (o = 0; o < bars; o++) {

// 		peak[o] = 0;

// 		// process: get peaks
// 		for (i = lcf[o]; i <= hcf[o]; i++) {

// 			//getting r of compex
// 			y[i] = hypot(out[i][0], out[i][1]);
// 			peak[o] += y[i]; //adding upp band
// 		}


// 		peak[o] = peak[o] / (hcf[o]-lcf[o]+1); //getting average
// 		temp = peak[o] * k[o] * sens; //multiplying with k and adjusting to sens settings
// 		if (temp <= ignore) temp = 0;
// 		if (channel == 1) fl[o] = temp;
// 		else fr[o] = temp;

// 	}

// 	if (channel == 1) return fl;
//  	else return fr;
// }