#ifndef _MATH_H_VIS__
#define _MATH_H_VIS__

#define VIS_PI 3.1415926535897932384
#define VIS_PI_2 1.5707963267948966192
#define VIS_PI_4 0.7853981633974483096
#define VIS_2_PI 6.2831853071795864768
#define VIS_4_PI 12.5663706143591729536
#define VIS_D2R 0.017453292519943295768
#define VIS_R2D 57.29577951308232087794
#define VIS_R2D_16 14668

double VIS_FastSin_r(double rad);
double VIS_FastSin_d(double deg);
int VIS_FastSin_d16(int deg);
int VIS_FastSin_r16(int rad);

double VIS_FastCos_r(double rad);
double VIS_FastCos_d(double deg);
int VIS_FastCos_d16(int deg);
int VIS_FastCos_r16(int rad);


#endif