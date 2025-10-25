// Automatically generated C++ file on Sat Sep 27 16:49:16 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD foc_model.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265359
#endif

// Constants
#define const_2pi_3     2.09439510239    // 2*PI/3
#define const_sqrt3     1.73205080757    // sqrt(3)

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
#undef i_U
#undef i_V
#undef i_W
#undef omega
#undef theta
#undef V_th
#undef V_mag
#undef Sync
#undef id_ref
#undef iq_ref
#undef Vdc
#undef dbg

struct sFOC_MODEL
{
   bool sync_p;
   double it_d;
   double it_q;
   double bkt_d;
   double bkt_q;
};

extern "C" __declspec(dllexport) void foc_model(struct sFOC_MODEL **opaque, double t, union uData *data)
{
   double  i_U    = data[ 0].d; // input
   double  i_V    = data[ 1].d; // input
   double  i_W    = data[ 2].d; // input
   double  omega  = data[ 3].d; // input
   double  theta  = data[ 4].d; // input
   bool    Sync   = data[ 5].b; // input
   double  id_ref = data[ 6].d; // input
   double  iq_ref = data[ 7].d; // input
   double  Vdc    = data[ 8].d; // input
   double  Psi    = data[ 9].d; // input parameter
   double  Ld     = data[10].d; // input parameter
   double  Lq     = data[11].d; // input parameter
   int     pp     = data[12].i; // input parameter
   double  Kp     = data[13].d; // input parameter
   double  Ki     = data[14].d; // input parameter
   double  Ts     = data[15].d; // input parameter
   double &V_th   = data[16].d; // output
   double &V_mag  = data[17].d; // output

   double cos_U, cos_V, cos_W;
   double sin_U, sin_V, sin_W;
   double id, iq;
   double error_d, error_q;
   double vd_ref, vq_ref;
   double V_mag_nosat;
   double angle;

   if(!*opaque)
   {
      *opaque = (struct sFOC_MODEL *) malloc(sizeof(struct sFOC_MODEL));
      bzero(*opaque, sizeof(struct sFOC_MODEL));
   }
   struct sFOC_MODEL *inst = *opaque;

// Implement module evaluation code here:

   // Detect rising edge to trigger calculation
   if(!inst->sync_p && Sync){

      // Transform current waveforms into dq coordinates
      cos_U = cos(theta*double(pp));
      cos_V = cos(theta*double(pp)-const_2pi_3);
      cos_W = cos(theta*double(pp)+const_2pi_3);
      sin_U = sin(theta*double(pp));
      sin_V = sin(theta*double(pp)-const_2pi_3);
      sin_W = sin(theta*double(pp)+const_2pi_3);
      id = 2.0/3.0*(cos_U*i_U +cos_V*i_V +cos_W*i_W);
      iq = -2.0/3.0*(sin_U*i_U +sin_V*i_V +sin_W*i_W);

      // Current controller
      // DQ current error
      error_d = id_ref - id;
      error_q = iq_ref - iq;
      // PI controller with backtracking
      inst->it_d = inst->it_d + Ts*error_d - 0.1*Ts*inst->bkt_d;
      inst->it_q = inst->it_q + Ts*error_q - 0.1*Ts*inst->bkt_q;
      vd_ref = Kp*error_d +Ki*inst->it_d;
      vq_ref = Kp*error_q +Ki*inst->it_q;
      // Feedforward compensation
      vd_ref = vd_ref - omega*double(pp)*Lq*iq_ref;
      vq_ref = vq_ref + omega*double(pp)*(Ld*id_ref + Psi);
      // Compute reference vector magnitude and saturate
      V_mag_nosat = sqrt(vd_ref*vd_ref + vq_ref*vq_ref);
      V_mag = V_mag_nosat/(Vdc/const_sqrt3);
      if(V_mag > 0.995){
         V_mag = 0.995;
      }
      // Compute terms for backtracking
      inst->bkt_d = vd_ref - vd_ref*(Vdc/const_sqrt3)*V_mag/V_mag_nosat;
      inst->bkt_q = vq_ref - vq_ref*(Vdc/const_sqrt3)*V_mag/V_mag_nosat;
      // Compute relative reference vector angle
      V_th = atan2(vq_ref/V_mag_nosat, vd_ref/V_mag_nosat +1e-12);
      V_th = V_th + 1.5*Ts*omega*double(pp);
      // Compute absolute reference vector angle
      angle = V_th + theta*double(pp);
      if(angle > 2*PI){
         angle = angle -2*PI*int((angle/(2*PI)));
      }
      V_th = angle;
   }
   inst->sync_p = Sync;
}

extern "C" __declspec(dllexport) void Destroy(struct sFOC_MODEL *inst)
{
   free(inst);
}
