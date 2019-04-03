#include "../helpers/math.h"
#include "FFT.h"
#include <math.h>

//note: in this file, all the double value are shifted to the left by 16 bits
 
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

void init(int index, int size, Complex *omegaInverse) {
    if(index == 0){
        for (int i = 0; i < size; ++i) {
            Complex c ={1,0};
            omegaInverse[i] = c;
        }
    }
    else{
        for (int i = 0; i < size; ++i) {
            Complex c ={cosf(2 * PI / index * i), -sinf(2 * PI / index * i)};
            omegaInverse[i] = c;
        }
    }
}


// void transform ( Complex *a,  int n,  Complex* omega ) {
//     for ( int i = 0, j = 0 ; i < n ; ++ i )  {
//         if ( i > j ){
//             //swap ( a [i], a [j] )
//             Complex temp = a[i];
//             a[i] = a[j];
//             a[j] = temp;
//         } 
//         for( int l = n >> 1 ; ( j ^= l ) < l ; l >>= 1 ) ;
//     }
    
//     for ( int l = 2 ; l <= n ; l <<= 1 )  {
//         int m = l / 2;
//         for (Complex * p = a ; p < a + n ; p += l )  {
//             for ( int i = 0 ; i < m ; ++ i )  {
//                 Complex t = multiply(omega [n / l * i] , p [m + i]);
//                 p [m + i] = sub (p [i] , t) ;
//                 p [i] = add(p [i], t) ;
//             }
//         }
//     }
// }

// void dft ( Complex *a,  int n ,  Complex* omega)  {
//         transform ( a, n, omega ) ;
// }

// void idft ( Complex *a,  int n,  Complex* omegaInverse )  {
//         transform ( a, n, omegaInverse ) ;
//         for ( int i = 0 ; i < n ; ++ i ) a [i] = divideByInt(a[i],n) ;
//     }
// void FastFourierTransform (Complex *a,  int n ){
//     Complex omega [n], omegaInverse[n];
//     initOmega (omega, omegaInverse, n);
//     dft ( a , n ,  omega);
// }
// void idftMag (unsigned * result, Complex *a,  int n,  Complex* omegaInverse )  {
//         transform ( a, n, omegaInverse ) ;
//         for ( int i = 0 ; i < n ; ++ i ) result [i] = (magnitude(a [i]))>>16 ;
//     }

// void sdft(Complex *x, int n, Complex omegaInverse[][n], Complex *result) {
//     for (int om = 0; om < n; om++) {
//         Complex c = {0,0,0};
//         for (int k = 0; k < n; k++) {
//             //c = c + x[k] * omegaInverse[k];
//             c = add(c, multiply(x[k],omegaInverse[om][k]));
//         }
//         c.mag = magnitude(c);
//         result[om] = c;
//     }
// }


void sdft(Complex *x, int n, Complex * omegaInverse, Complex *result) {
		for (int om = 0; om < n; om++) {
			Complex c = {0,0};
			init(om,n,omegaInverse);
			for (int k = 0; k < n; k++) {
				c = add(c, multiply(x[k],omegaInverse[k]));
			}
			result[om] = c;
		}
	}

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


// int FFT_remap(double * src,int size_n){

//     if(size_n==1)
//         return 0;
//     double * temp=(double *)malloc(sizeof(double)*size_n);
//     for(int i=0;i<size_n;i++)
//         if(i%2==0)
//             temp[i/2]=src[i];
//         else
//             temp[(size_n+i)/2]=src[i];
//     for(int i=0;i<size_n;i++)
//         src[i]=temp[i];
//     free(temp);
//     FFT_remap(src, size_n/2);
//     FFT_remap(src+size_n/2, size_n/2);
//     return 1;


// }

// void FFT(double * src,Complex * dst,int size_n){

//     FFT_remap(src, size_n);
//    // for(int i=0;i<size_n;i++)
//     //    printf("%lf\n",src[i]);
//     int k=size_n;
//     int z=0;
//     while (k/=2) {
//         z++;
//     }
//     k=z;
//     // if(size_n!=(1<<k)) //memory check
//     //     exit(0);
//     Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
//     // if(src_com==NULL) //memory check
//     //     exit(0);
//     for(int i=0;i<size_n;i++){
//         src_com[i].r=src[i];
//         src_com[i].i=0;
//     }
//     for(int i=0;i<k;i++){
//         z=0;
//         for(int j=0;j<size_n;j++){
//             if((j/(1<<i))%2==1){
//                 Complex wn;
//                 getWN(z, size_n, &wn);
//                 Multy_Complex(&src_com[j], &wn,&src_com[j]);
//                 z+=1<<(k-i-1);
//                 Complex temp;
//                 int neighbour=j-(1<<(i));
//                 temp.r=src_com[neighbour].r;
//                 temp.i=src_com[neighbour].i;
//                 Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
//                 Sub_Complex(&temp, &src_com[j], &src_com[j]);
//             }
//             else
//                 z=0;
//         }

//     }

// 	for(int i=0;i<size_n;i++){
// 		dst[i].i=src_com[i].i;
// 		dst[i].r=src_com[i].r;
// 	}

// }

// void getWN(double n,double size_n,Complex * dst, Complex *omegaExps){
//     double x=2.0*M_PI*n/size_n;
//     dst->i=-sin(x);
//     dst->r=cos(x);
// }