#include <math.h>
#include <algorithm>
#include <fstream>
#include <istream>
#include <iostream>
using namespace std;


// struct Complex {
// 	double r, i;
// 	Complex() {	}
// 	Complex(double r, double i) : r(r), i(i) {	}
// 	inline void real(const double& x) { r = x; }
// 	inline double real() { return r; }
// 	inline Complex operator + (const Complex& rhs)  const {
// 		return Complex(r + rhs.r, i + rhs.i);
// 	}
// 	inline Complex operator - (const Complex& rhs)  const {
// 		return Complex(r - rhs.r, i - rhs.i);
// 	}
// 	inline Complex operator * (const Complex& rhs)  const {
// 		return Complex(r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r);
// 	}
// 	inline void operator /= (const double& x) {
// 		r /= x, i /= x;
// 	}
// 	inline void operator *= (const Complex& rhs) {
// 		*this = Complex(r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r);
// 	}
// 	inline void operator += (const Complex& rhs) {
// 		r += rhs.r, i += rhs.i;
// 	}
// 	inline Complex conj() {
// 		return Complex(r, -i);
// 	}
// };

// struct FastFourierTransform {
// 	Complex omega[1000], omegaInverse[1000];

// 	void init(const int& index, const int & size) {
// 		for (int i = 0; i < size; ++i) {
// 			omega[i] = Complex(cos(2 * PI / index * i), sin(2 * PI / index * i));
// 			omegaInverse[i] = omega[i].conj();
// 		}
// 	}

// 	void transform(Complex *a, const int& n, const Complex* omega) {
// 		for (int i = 0, j = 0; i < n; ++i) {
// 			if (i > j)  std::swap(a[i], a[j]);
// 			for (int l = n >> 1; (j ^= l) < l; l >>= 1);
// 		}

// 		for (int l = 2; l <= n; l <<= 1) {
// 			int m = l / 2;
// 			for (Complex *p = a; p < a + n; p += l) {
// 				for (int i = 0; i < m; ++i) {
// 					Complex t = omega[n / l * i] * p[m + i];
// 					p[m + i] = p[i] - t;
// 					p[i] += t;
// 				}
// 			}
// 		}
// 	}

// 	void fft2(Complex *x, const int& n, const Complex* omegaInverse, Complex *result) {
// 		for (int om = 1; om <= n; om++) {
// 			Complex c(0,0);
// 			init(om,n);
// 			for (int k = 0; k < n; k++) {

// 				c += x[k] * omegaInverse[k];
// 			}
// 			result[om-1] = c;
// 		}

// 	}

// 	void dft(Complex *a, const int& n) {
// 		transform(a, n, omega);
// 	}

// 	void idft(Complex *a, const int& n) {
// 		transform(a, n, omegaInverse);
// 		for (int i = 0; i < n; ++i) a[i] /= n;
// 	}
// } fft;

typedef struct Complex  {
	float r, i ;
	int mag;
} Complex;

 Complex add (Complex c,  Complex rhs );
 Complex sub (Complex c,  Complex rhs );
 Complex multiply (Complex c,  Complex rhs );
 Complex divideByInt (Complex c,  float x );
 Complex conjg (Complex c);
 int magnitude (Complex c) ;
void init(int index, int size, Complex *omegaInverse);
void sdft(Complex *x, int n, Complex  omegaInverse[], Complex *result) ;
void initExps(int size, Complex omegaInverse[][180]);
#include <math.h>

//note: in this file, all the double value are shifted to the left by 16 bits
 
 float PI = (float) 3.1415926535897932384626;

 Complex add (Complex c,  Complex rhs )   {
	 Complex newC = { 0,0,0 };
    newC.i = c.i + rhs.i;
    newC.r = c.r + rhs.r;
    return newC ;
}
  Complex sub (Complex c,  Complex rhs )   {
	  Complex newC = {0,0,0};
	  newC.i = c.i - rhs.i;
	  newC.r = c.r - rhs.r;
    return newC ;
}
 Complex multiply (Complex c,  Complex rhs )   {
	 Complex newC = { 0,0,0 };
    newC.i = (c.r) * (rhs.i) + (c.i) * (rhs.r);
    newC.r = (c.r) * (rhs.r) - (c.i) * (rhs.i);
    return newC ;
    //return Complex ( r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r )
}

 Complex divideByInt (Complex c,  float x )   {
	 Complex newC = { 0,0,0 };
    newC.i /= x;
    newC.r /= x;
    return newC ;
}

 Complex conjg (Complex c)  {
	 Complex newC = { 0,0,0 };
    newC.i = -c.i;
    newC.r = c.r;
    return newC ;
}
 
  int magnitude (Complex c)  {
    return floor(((c.i)*(c.i) + (c.r)*(c.r)));
}

void init(int index, int size, Complex *omegaInverse) {
    if(index == 0){
        for (int i = 0; i < size; ++i) {
            Complex c ={1,0,0};
            omegaInverse[i] = c;
        }
    }
    else{
        for (int i = 0; i < size; ++i) {
            Complex c ={cosf(2 * PI / index * i), -sinf(2 * PI / index * i),0};
            omegaInverse[i] = c;
        }
    }
}

void sdft(Complex *x, int n, Complex omegaInverse[], Complex *result) {
		for (int om = 0; om < n; om++) {
			Complex c = {0,0,0};
			init(om,n,omegaInverse);
			for (int k = 0; k < n; k++) {
				c = add(c, multiply(x[k],omegaInverse[k]));
			}
			result[om] = c;
            c.mag = magnitude(c);
		}
	}

void initExps(const int size, Complex omegaInverse[][180]) {
    //initialize first line
    for(int k =0; k < size; k++){
            Complex c ={1,0,0};
            omegaInverse[0][k] = c;
    }
    for (int index = 1; index < size; ++index) {
        for(int k =0; k < size; k++){
            Complex c ={cos(2 * PI / index * k), -sin(2 * PI / index * k),0};
            omegaInverse[index][k] = c;
        }
    }
}

void fdft(Complex *x, int n, Complex omegaExp[][180], Complex *result) {
		for (int om = 0; om < n; om++) {
			Complex c = {0,0,0};
			for (int k = 0; k < n; k++) {
				c = add(c, multiply(x[k],omegaExp[om][k]));
			}
			result[om] = c;
		}
	}
// int main() {
// 	int n = 500;
// 	struct FastFourierTransform fastft;
// 	fastft.init(n,n);

// 	double output[1000] = { 0, 0.125333233564304, 0.248689887164855, 0.368124552684678, 0.481753674101715, 0.587785252292473, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466020, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466020, 0.844327925502015, 0.770513242775789, 0.684547105928689, 0.587785252292473, 0.481753674101716, 0.368124552684678, 0.248689887164855, 0.125333233564305, 1.22464679914735e-16, -0.125333233564304, -0.248689887164855, -0.368124552684678, -0.481753674101715, -0.587785252292473, -0.684547105928689, -0.770513242775789, -0.844327925502015, -0.904827052466020, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466020, -0.844327925502016, -0.770513242775790, -0.684547105928689, -0.587785252292473, -0.481753674101715, -0.368124552684678, -0.248689887164855, -0.125333233564305, -2.44929359829471e-16, 0.125333233564304, 0.248689887164855, 0.368124552684678, 0.481753674101716, 0.587785252292474, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928688, 0.587785252292473, 0.481753674101716, 0.368124552684678, 0.248689887164856, 0.125333233564304, 3.67394039744206e-16, -0.125333233564303, -0.248689887164855, -0.368124552684677, -0.481753674101716, -0.587785252292473, -0.684547105928689, -0.770513242775789, -0.844327925502016, -0.904827052466019, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502015, -0.770513242775789, -0.684547105928689, -0.587785252292474, -0.481753674101716, -0.368124552684678, -0.248689887164856, -0.125333233564304, -4.89858719658941e-16, 0.125333233564303, 0.248689887164855, 0.368124552684677, 0.481753674101715, 0.587785252292473, 0.684547105928689, 0.770513242775789, 0.844327925502016, 0.904827052466020, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295153, 0.904827052466020, 0.844327925502016, 0.770513242775790, 0.684547105928690, 0.587785252292474, 0.481753674101716, 0.368124552684678, 0.248689887164856, 0.125333233564304, 6.12323399573677e-16, -0.125333233564303, -0.248689887164855, -0.368124552684679, -0.481753674101715, -0.587785252292473, -0.684547105928688, -0.770513242775790, -0.844327925502016, -0.904827052466020, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466020, -0.844327925502016, -0.770513242775791, -0.684547105928689, -0.587785252292474, -0.481753674101717, -0.368124552684680, -0.248689887164854, -0.125333233564304, -7.34788079488412e-16, 0.125333233564303, 0.248689887164853, 0.368124552684679, 0.481753674101715, 0.587785252292473, 0.684547105928688, 0.770513242775790, 0.844327925502015, 0.904827052466020, 0.951056516295153, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502016, 0.770513242775791, 0.684547105928689, 0.587785252292474, 0.481753674101717, 0.368124552684680, 0.248689887164854, 0.125333233564304, 8.57252759403147e-16, -0.125333233564303, -0.248689887164853, -0.368124552684679, -0.481753674101715, -0.587785252292473, -0.684547105928688, -0.770513242775790, -0.844327925502015, -0.904827052466020, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502016, -0.770513242775791, -0.684547105928689, -0.587785252292474, -0.481753674101717, -0.368124552684680, -0.248689887164854, -0.125333233564305, -9.79717439317883e-16, 0.125333233564306, 0.248689887164852, 0.368124552684678, 0.481753674101715, 0.587785252292470, 0.684547105928687, 0.770513242775788, 0.844327925502015, 0.904827052466018, 0.951056516295153, 0.982287250728688, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502016, 0.770513242775789, 0.684547105928689, 0.587785252292471, 0.481753674101717, 0.368124552684677, 0.248689887164854, 0.125333233564301, 1.10218211923262e-15, -0.125333233564306, -0.248689887164852, -0.368124552684678, -0.481753674101715, -0.587785252292475, -0.684547105928687, -0.770513242775788, -0.844327925502015, -0.904827052466018, -0.951056516295153, -0.982287250728688, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502017, -0.770513242775789, -0.684547105928689, -0.587785252292471, -0.481753674101717, -0.368124552684677, -0.248689887164855, -0.125333233564301, -1.22464679914735e-15, 0.125333233564306, 0.248689887164852, 0.368124552684678, 0.481753674101715, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428271, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928689, 0.587785252292474, 0.481753674101717, 0.368124552684674, 0.248689887164858, 0.125333233564301, 4.89982515786259e-15, -0.125333233564306, -0.248689887164856, -0.368124552684678, -0.481753674101715, -0.587785252292472, -0.684547105928687, -0.770513242775792, -0.844327925502013, -0.904827052466021, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502013, -0.770513242775787, -0.684547105928687, -0.587785252292471, -0.481753674101714, -0.368124552684677, -0.248689887164855, -0.125333233564305, -1.46957615897682e-15, 0.125333233564309, 0.248689887164859, 0.368124552684681, 0.481753674101712, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928684, 0.587785252292474, 0.481753674101717, 0.368124552684674, 0.248689887164852, 0.125333233564302, -1.96067283990894e-15, -0.125333233564305, -0.248689887164862, -0.368124552684678, -0.481753674101715, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775787, -0.684547105928687, -0.587785252292472, -0.481753674101714, -0.368124552684678, -0.248689887164848, -0.125333233564305, -1.71450551880629e-15, 0.125333233564309, 0.248689887164859, 0.368124552684681, 0.481753674101711, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775790, 0.684547105928684, 0.587785252292475, 0.481753674101718, 0.368124552684674, 0.248689887164852, 0.125333233564302, 5.38968387752153e-15, -0.125333233564305, -0.248689887164862, -0.368124552684678, -0.481753674101714, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775787, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684678, -0.248689887164848, -0.125333233564305, -1.95943487863577e-15, 0.125333233564302, 0.248689887164858, 0.368124552684681, 0.481753674101711, 0.587785252292469, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295155, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775794, 0.684547105928685, 0.587785252292475, 0.481753674101718, 0.368124552684675, 0.248689887164852, 0.125333233564302, -1.47081412025000e-15, -0.125333233564298, -0.248689887164862, -0.368124552684677, -0.481753674101714, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728688, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775792, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684671, -0.248689887164849, -0.125333233564306, -2.20436423846524e-15, 0.125333233564301, 0.248689887164858, 0.368124552684681, 0.481753674101711, 0.587785252292469, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295155, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775785, 0.684547105928685, 0.587785252292475, 0.481753674101718, 0.368124552684675, 0.248689887164852, 0.125333233564302, 5.87954259718047e-15, -0.125333233564298, -0.248689887164861, -0.368124552684677, -0.481753674101714, -0.587785252292471, -0.684547105928692, -0.770513242775791, -0.844327925502013, -0.904827052466017, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728688, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775788, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684672, -0.248689887164849, -0.125333233564306};
// 	Complex Carray[1000] = { Complex(0,0) };

// 	for (int i = 0; i < n; i++) {
// 		Carray[i] = Complex(output[i], 0);
// 	}
// 	Complex result[1000];
// 	fastft.fft2(Carray, n, fastft.omegaInverse,result);
// 	//fastft.idft(Carray, n);

// 	for (int i = 0; i < n; i++) {
// 		if (sqrt(pow(result[i].r, 2) + pow(result[i].i, 2))<30) continue;
// 		std::cout << "{\n  frequency" << i << ": " << sqrt(pow(result[i].r, 2) + pow(result[i].i, 2)) << endl << "  Real: " <<
// 			result[i].r << endl << "  Imag: " << result[i].i << endl<<"}"<<endl;
// 	}


// 	getchar();
// }

int main() {

	 int n = 180;
	 int fourierLength = n;

	Complex omegaInverse[500];

    //initExps( fourierLength, omegaInverse);
	
	double output[500] = { 0, 0.125333233564304, 0.248689887164855, 0.368124552684678, 0.481753674101715, 0.587785252292473, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466020, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466020, 0.844327925502015, 0.770513242775789, 0.684547105928689, 0.587785252292473, 0.481753674101716, 0.368124552684678, 0.248689887164855, 0.125333233564305, 1.22464679914735e-16, -0.125333233564304, -0.248689887164855, -0.368124552684678, -0.481753674101715, -0.587785252292473, -0.684547105928689, -0.770513242775789, -0.844327925502015, -0.904827052466020, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466020, -0.844327925502016, -0.770513242775790, -0.684547105928689, -0.587785252292473, -0.481753674101715, -0.368124552684678, -0.248689887164855, -0.125333233564305, -2.44929359829471e-16, 0.125333233564304, 0.248689887164855, 0.368124552684678, 0.481753674101716, 0.587785252292474, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928688, 0.587785252292473, 0.481753674101716, 0.368124552684678, 0.248689887164856, 0.125333233564304, 3.67394039744206e-16, -0.125333233564303, -0.248689887164855, -0.368124552684677, -0.481753674101716, -0.587785252292473, -0.684547105928689, -0.770513242775789, -0.844327925502016, -0.904827052466019, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502015, -0.770513242775789, -0.684547105928689, -0.587785252292474, -0.481753674101716, -0.368124552684678, -0.248689887164856, -0.125333233564304, -4.89858719658941e-16, 0.125333233564303, 0.248689887164855, 0.368124552684677, 0.481753674101715, 0.587785252292473, 0.684547105928689, 0.770513242775789, 0.844327925502016, 0.904827052466020, 0.951056516295154, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295153, 0.904827052466020, 0.844327925502016, 0.770513242775790, 0.684547105928690, 0.587785252292474, 0.481753674101716, 0.368124552684678, 0.248689887164856, 0.125333233564304, 6.12323399573677e-16, -0.125333233564303, -0.248689887164855, -0.368124552684679, -0.481753674101715, -0.587785252292473, -0.684547105928688, -0.770513242775790, -0.844327925502016, -0.904827052466020, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466020, -0.844327925502016, -0.770513242775791, -0.684547105928689, -0.587785252292474, -0.481753674101717, -0.368124552684680, -0.248689887164854, -0.125333233564304, -7.34788079488412e-16, 0.125333233564303, 0.248689887164853, 0.368124552684679, 0.481753674101715, 0.587785252292473, 0.684547105928688, 0.770513242775790, 0.844327925502015, 0.904827052466020, 0.951056516295153, 0.982287250728689, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502016, 0.770513242775791, 0.684547105928689, 0.587785252292474, 0.481753674101717, 0.368124552684680, 0.248689887164854, 0.125333233564304, 8.57252759403147e-16, -0.125333233564303, -0.248689887164853, -0.368124552684679, -0.481753674101715, -0.587785252292473, -0.684547105928688, -0.770513242775790, -0.844327925502015, -0.904827052466020, -0.951056516295153, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502016, -0.770513242775791, -0.684547105928689, -0.587785252292474, -0.481753674101717, -0.368124552684680, -0.248689887164854, -0.125333233564305, -9.79717439317883e-16, 0.125333233564306, 0.248689887164852, 0.368124552684678, 0.481753674101715, 0.587785252292470, 0.684547105928687, 0.770513242775788, 0.844327925502015, 0.904827052466018, 0.951056516295153, 0.982287250728688, 0.998026728428272, 0.998026728428272, 0.982287250728689, 0.951056516295154, 0.904827052466019, 0.844327925502016, 0.770513242775789, 0.684547105928689, 0.587785252292471, 0.481753674101717, 0.368124552684677, 0.248689887164854, 0.125333233564301, 1.10218211923262e-15, -0.125333233564306, -0.248689887164852, -0.368124552684678, -0.481753674101715, -0.587785252292475, -0.684547105928687, -0.770513242775788, -0.844327925502015, -0.904827052466018, -0.951056516295153, -0.982287250728688, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295154, -0.904827052466019, -0.844327925502017, -0.770513242775789, -0.684547105928689, -0.587785252292471, -0.481753674101717, -0.368124552684677, -0.248689887164855, -0.125333233564301, -1.22464679914735e-15, 0.125333233564306, 0.248689887164852, 0.368124552684678, 0.481753674101715, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428271, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928689, 0.587785252292474, 0.481753674101717, 0.368124552684674, 0.248689887164858, 0.125333233564301, 4.89982515786259e-15, -0.125333233564306, -0.248689887164856, -0.368124552684678, -0.481753674101715, -0.587785252292472, -0.684547105928687, -0.770513242775792, -0.844327925502013, -0.904827052466021, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502013, -0.770513242775787, -0.684547105928687, -0.587785252292471, -0.481753674101714, -0.368124552684677, -0.248689887164855, -0.125333233564305, -1.46957615897682e-15, 0.125333233564309, 0.248689887164859, 0.368124552684681, 0.481753674101712, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775789, 0.684547105928684, 0.587785252292474, 0.481753674101717, 0.368124552684674, 0.248689887164852, 0.125333233564302, -1.96067283990894e-15, -0.125333233564305, -0.248689887164862, -0.368124552684678, -0.481753674101715, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775787, -0.684547105928687, -0.587785252292472, -0.481753674101714, -0.368124552684678, -0.248689887164848, -0.125333233564305, -1.71450551880629e-15, 0.125333233564309, 0.248689887164859, 0.368124552684681, 0.481753674101711, 0.587785252292475, 0.684547105928690, 0.770513242775790, 0.844327925502015, 0.904827052466019, 0.951056516295153, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775790, 0.684547105928684, 0.587785252292475, 0.481753674101718, 0.368124552684674, 0.248689887164852, 0.125333233564302, 5.38968387752153e-15, -0.125333233564305, -0.248689887164862, -0.368124552684678, -0.481753674101714, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728689, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775787, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684678, -0.248689887164848, -0.125333233564305, -1.95943487863577e-15, 0.125333233564302, 0.248689887164858, 0.368124552684681, 0.481753674101711, 0.587785252292469, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295155, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775794, 0.684547105928685, 0.587785252292475, 0.481753674101718, 0.368124552684675, 0.248689887164852, 0.125333233564302, -1.47081412025000e-15, -0.125333233564298, -0.248689887164862, -0.368124552684677, -0.481753674101714, -0.587785252292472, -0.684547105928692, -0.770513242775792, -0.844327925502013, -0.904827052466018, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728688, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775792, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684671, -0.248689887164849, -0.125333233564306, -2.20436423846524e-15, 0.125333233564301, 0.248689887164858, 0.368124552684681, 0.481753674101711, 0.587785252292469, 0.684547105928689, 0.770513242775789, 0.844327925502015, 0.904827052466019, 0.951056516295155, 0.982287250728690, 0.998026728428271, 0.998026728428272, 0.982287250728688, 0.951056516295153, 0.904827052466019, 0.844327925502015, 0.770513242775785, 0.684547105928685, 0.587785252292475, 0.481753674101718, 0.368124552684675, 0.248689887164852, 0.125333233564302, 5.87954259718047e-15, -0.125333233564298, -0.248689887164861, -0.368124552684677, -0.481753674101714, -0.587785252292471, -0.684547105928692, -0.770513242775791, -0.844327925502013, -0.904827052466017, -0.951056516295154, -0.982287250728689, -0.998026728428272, -0.998026728428272, -0.982287250728688, -0.951056516295152, -0.904827052466021, -0.844327925502017, -0.770513242775788, -0.684547105928687, -0.587785252292472, -0.481753674101715, -0.368124552684672, -0.248689887164849, -0.125333233564306};
	Complex Carray[1000];
	Complex omegaExp[180][180];
    initExps( fourierLength, omegaExp);

	for (int i = 0; i < n; i++) {
		Complex c = {(float)output[i], 0,0};
		Carray[i] = c;
	}
	Complex result[1000];
	//sdft(Carray, fourierLength, omegaInverse, result);
	//fastft.idft(Carray, n);
	fdft(Carray, fourierLength, omegaExp, result);

	for (int i = 0; i < n; i++) {
		//if (sqrt(pow(result[i].r, 2) + pow(result[i].i, 2))<100) continue;
        if (magnitude(result[i])<1000) continue;
		std::cout << "{\n  frequency" << i << ": " << sqrt(pow(result[i].r, 2) + pow(result[i].i, 2)) << endl << "  Real: " <<
			result[i].r << endl << "  Imag: " << result[i].i << endl<<"}"<<endl;
	}

	getchar();
}