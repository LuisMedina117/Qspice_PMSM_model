// Automatically generated C++ file on Sun Aug 17 19:22:51 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD pmsm_model_dll.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265359
#endif

extern "C" __declspec(dllexport) void (*bzero)(void *ptr, unsigned int count)   = 0;

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef u_vW
#undef u_vV
#undef u_vU
#undef y_iU
#undef y_iV
#undef y_iW
#undef y_th
#undef y_w
#undef y_tq
#undef y_vd
#undef y_vq
#undef u_id
#undef u_iq

struct sPMSM_MODEL_DLL
{
   // declare the structure here
   double x_th_n1;    // Previous theta
   double x_w_n1;     // Previous omega
   double y_tq_n1;    // Previous torque
   double t_n1;       // Previous time
   bool initialized;  // Initialization flag
};

extern "C" __declspec(dllexport) void pmsm_model_dll(struct sPMSM_MODEL_DLL **opaque, double t, union uData *data)
{
   double  u_vW  = data[ 0].d; // input
   double  u_vV  = data[ 1].d; // input
   double  u_vU  = data[ 2].d; // input
   double  u_id  = data[ 3].d; // input
   double  u_iq  = data[ 4].d; // input
   double  pp    = data[ 5].d; // input parameter
   double  J     = data[ 6].d; // input parameter
   double  B     = data[ 7].d; // input parameter
   double  ROUT  = data[ 8].d; // input parameter
   double  Ld    = data[ 9].d; // input parameter
   double  Lq    = data[10].d; // input parameter
   double  Psi   = data[11].d; // input parameter
   double  w0    = data[12].d; // input parameter
   double  Tload = data[13].d; // input parameter
   double &y_iU  = data[14].d; // output
   double &y_iV  = data[15].d; // output
   double &y_iW  = data[16].d; // output
   double &y_th  = data[17].d; // output
   double &y_w   = data[18].d; // output
   double &y_tq  = data[19].d; // output
   double &y_vd  = data[20].d; // output
   double &y_vq  = data[21].d; // output

   double cos_U, cos_V, cos_W;
   double sin_U, sin_V, sin_W;

   double k1_w,k2_w,k3_w,k4_w;
   double k1_t,k2_t,k3_t,k4_t;

   if(!*opaque)
   {
      *opaque = (struct sPMSM_MODEL_DLL *) malloc(sizeof(struct sPMSM_MODEL_DLL));
      bzero(*opaque, sizeof(struct sPMSM_MODEL_DLL));
   }
   struct sPMSM_MODEL_DLL *inst = *opaque;

// Implement module evaluation code here:
   // Apply initialization
   if (inst->initialized == 0){
      inst->x_w_n1 = w0;
      inst->initialized = 1;
   }
   // Time step
   double t_step = t-inst->t_n1;
   // Torque calculation
   y_tq = 3.0/2.0*pp*(u_iq*(u_id*Ld+Psi)-u_id*u_iq*Lq);
/*    // Integration using trapezoidal rule
   // State theta
   y_th = t_step/2.0*(y_w +inst->x_w_n1) +inst->x_th_n1;
   // State omega
   y_w = (t_step/2.0*(-B*inst->x_w_n1 +y_tq +inst->y_tq_n1 -2.0*Tload) +J*inst->x_w_n1)/(J+B*t_step/2.0); */
   // Integrate using RK4
   k1_w = (-B*inst->x_w_n1 +inst->y_tq_n1 -Tload)/J;
   k1_t = inst->x_w_n1;
   k2_w = (-B*(inst->x_w_n1+k1_w*t_step/2.0) +inst->y_tq_n1 -Tload)/J;
   k2_t = inst->x_w_n1+k1_w*t_step/2.0;
   k3_w = (-B*(inst->x_w_n1+k2_w*t_step/2.0) +inst->y_tq_n1 -Tload)/J;
   k3_t = inst->x_w_n1+k2_w*t_step/2.0;
   k3_w = (-B*(inst->x_w_n1+k3_w*t_step) +inst->y_tq_n1 -Tload)/J;
   k3_t = inst->x_w_n1+k3_w*t_step;
   y_th = inst->x_th_n1 + t_step*(k1_t+2.0*k2_t+2.0*k3_t+k4_t)/6.0;
   y_w =  inst->x_w_n1+ t_step*(k1_w+2.0*k2_w+2.0*k3_w+k4_w)/6.0;

   // Wrap to 2*pi
   if(y_th > 2.0*PI){
      y_th = y_th - 2.0*PI;
   }
   // Update previous values
   inst->t_n1 = t;
   inst->x_w_n1 = y_w;
   inst->x_th_n1 = y_th;
   inst->y_tq_n1 = y_tq;
   // Wrap theta to 0 when > 2PI
   if(inst->x_th_n1 > 2*PI){
      inst->x_th_n1 = inst->x_th_n1 - 2*PI;
   }

   cos_U = cos(y_th*pp);
   cos_V = cos(y_th*pp-2.0*PI/3.0);
   cos_W = cos(y_th*pp+2.0*PI/3.0);
   sin_U = sin(y_th*pp);
   sin_V = sin(y_th*pp-2.0*PI/3.0);
   sin_W = sin(y_th*pp+2.0*PI/3.0);

   // Voltage transformation
   y_vd = 2.0/3.0*(cos_U*u_vU +cos_V*u_vV +cos_W*u_vW);
   y_vq = -2.0/3.0*(sin_U*u_vU +sin_V*u_vV +sin_W*u_vW);

   // Current transformation
   y_iU = cos_U*u_id -sin_U*u_iq;
   y_iV = cos_V*u_id -sin_V*u_iq;
   y_iW = cos_W*u_id -sin_W*u_iq;

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sPMSM_MODEL_DLL *inst, double t)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sPMSM_MODEL_DLL
}

extern "C" __declspec(dllexport) void Destroy(struct sPMSM_MODEL_DLL *inst)
{
   free(inst);
}
