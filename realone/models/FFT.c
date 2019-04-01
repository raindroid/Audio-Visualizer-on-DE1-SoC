#include "../helpers/math.c"

//note: in this file, all the double value are shifted to the left by 16 bits

typedef struct Complex  {
	int r, i ;
} Complex;

 void setReal ( Complex *c, const int x );
 Complex add (Complex c, const Complex rhs );
 Complex sub (Complex c, const Complex rhs );
 Complex multiply (Complex c, const Complex rhs );
 Complex divideByInt (Complex c, const int x );
 Complex conj (Complex c);
 void initOmega (Complex omega[],  const int n );
 void transform ( Complex *a, const int n, const Complex* omega );
 void dft ( Complex *a, const int n , const Complex* omega);

 void setReal ( Complex *c, const int x )  {  c->r = x ;  }
 int getReal ( Complex c )  {  return c.r ;  }
 Complex add (Complex c, const Complex rhs )   {
    Complex newC;
    newC.i += c.i;
    newC.r += c.r;
    return newC ;
}
  Complex sub (Complex c, const Complex rhs )   {
   Complex newC;
    newC.i -= c.i;
    newC.r -= c.r;
    return newC ;
}
 Complex multiply (Complex c, const Complex rhs )   {
    Complex newC;
    newC.i = (c.r>>8) * (rhs.i>>8) + (c.i>>8) * (rhs.r>>8);
    newC.r = (c.r>>8) * (rhs.r>>8) - (c.i>>8) * (rhs.i>>8);
    return newC ;
    //return Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r )
}

 Complex divideByInt (Complex c, const int x )   {
    Complex newC;
    newC.i /= x;
    newC.r /= x;
    return newC ;
}

 Complex conj (Complex c)  {
    Complex newC;
    newC.i = -c.i;
    newC.r = c.r;
    return newC ;
}

 void initOmega (Complex omega[],  const int n )  {
    for ( int i = 0 ; i < n ; ++ i )  {
        Complex c;
        c.i = VIS_FastSin_d16 ( 360 / n * i );
        c.r = VIS_FastCos_d16 ( 360 / n * i);
        omega [i] = c;
    }
}

void transform ( Complex *a, const int n, const Complex* omega ) {
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
        for ( Complex *p = a ; p != a + n ; p += l )  {
            for ( int i = 0 ; i < m ; ++ i )  {
                Complex t = multiply(omega [n / l * i] , p [m + i]);
                p [m + i] = sub (p [i] , t) ;
                p [i] = add(p [i], t) ;
            }
        }
    }
}

void dft ( Complex *a, const int n , const Complex* omega)  {
        transform ( a, n, omega ) ;
}

void FastFourierTransform (Complex *a, const int n ){
    Complex omega [n];
    initOmega (omega, n );
    dft ( a , n ,  omega);
}