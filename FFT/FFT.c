#include <math.h>
#include <complex.h>
complex a;
struct Complex  {
	double r, i ;
	Complex ( )  {	}
	Complex ( double r, double i ) : r ( r ), i ( i )  {	}
	inline void real ( const double& x )  {  r = x ;  }
	inline double real ( )  {  return r ;  }
	inline Complex operator + ( const Complex& rhs )  const  {
		return Complex ( r + rhs.r, i + rhs.i ) ;
	}
	inline Complex operator - ( const Complex& rhs )  const  {
		return Complex ( r - rhs.r, i - rhs.i ) ;
	}
	inline Complex operator * ( const Complex& rhs )  const  {
		return Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r ) ;
	}
	inline void operator /= ( const double& x )   {
		r /= x, i /= x ;
	}
	inline void operator *= ( const Complex& rhs )   {
		*this = Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r ) ;
	}
	inline void operator += ( const Complex& rhs )   {
		r += rhs.r, i += rhs.i ;
	}
	inline Complex conj ( )  {
		return Complex ( r, -i ) ;
	}
} ;

struct FastFourierTransform  {
    Complex omega [N], omegaInverse [N] ;

    void init ( const int& n )  {
        for ( int i = 0 ; i < n ; ++ i )  {
            omega [i] = Complex ( cos ( 2 * PI / n * i), sin ( 2 * PI / n * i ) ) ;
            omegaInverse [i] = omega [i].conj ( ) ;
        }
    }

    void transform ( Complex *a, const int& n, const Complex* omega ) {
        for ( int i = 0, j = 0 ; i < n ; ++ i )  {
		if ( i > j )  std :: swap ( a [i], a [j] ) ;
		for( int l = n >> 1 ; ( j ^= l ) < l ; l >>= 1 ) ;
	}

        for ( int l = 2 ; l <= n ; l <<= 1 )  {
            int m = l / 2;
            for ( Complex *p = a ; p != a + n ; p += l )  {
                for ( int i = 0 ; i < m ; ++ i )  {
                    Complex t = omega [n / l * i] * p [m + i] ;
                    p [m + i] = p [i] - t ;
                    p [i] += t ;
                }
            }
        }
    }

    void dft ( Complex *a, const int& n )  {
        transform ( a, n, omega ) ;
    }

    void idft ( Complex *a, const int& n )  {
        transform ( a, n, omegaInverse ) ;
        for ( int i = 0 ; i < n ; ++ i ) a [i] /= n ;
    }
} fft ;