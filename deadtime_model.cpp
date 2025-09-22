// Automatically generated C++ file on Sun Sep 21 17:32:30 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD deadtime_model.cpp kernel32.lib

#include <malloc.h>

// Maximum rise/fall time
#define dt_max 10e-9

extern "C" __declspec(dllexport) void (*bzero)(void *ptr, unsigned int count)   = 0;

double fmin(double, double);
double f_eval(double);

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
#undef U
#undef V
#undef W
#undef HS_U
#undef LS_U
#undef HS_V
#undef LS_V
#undef HS_W
#undef LS_W

struct sDEADTIME_MODEL
{
   double delta_t;      // Maximum time step of simulation
   bool p_U;            // Previous U state
   bool p_V;            // Previous V state
   bool p_W;            // Previous W state
   bool HS_U_flag;      // Flag to enable HS U
   bool LS_U_flag;      // Flag to enable LS U
   bool HS_V_flag;      // Flag to enable HS V
   bool LS_V_flag;      // Flag to enable LS V
   bool HS_W_flag;      // Flag to enable HS W
   bool LS_W_flag;      // Flag to enable LS W
   double t_nextE;      // Time of next event
   double t_U;          // Time of next U enable
   double t_V;          // Time of next V enable
   double t_W;          // Time of next W enable
};

extern "C" __declspec(dllexport) void deadtime_model(struct sDEADTIME_MODEL **opaque, double t, union uData *data)
{
   bool   U     = data[0].b; // input
   bool   V     = data[1].b; // input
   bool   W     = data[2].b; // input
   double Tdead = data[3].d; // input parameter
   bool  &HS_U  = data[4].b; // output
   bool  &LS_U  = data[5].b; // output
   bool  &HS_V  = data[6].b; // output
   bool  &LS_V  = data[7].b; // output
   bool  &HS_W  = data[8].b; // output
   bool  &LS_W  = data[9].b; // output

   if(!*opaque)
   {
      *opaque = (struct sDEADTIME_MODEL *) malloc(sizeof(struct sDEADTIME_MODEL));
      bzero(*opaque, sizeof(struct sDEADTIME_MODEL));
      //(*opaque)->delta_t = 1e3;
   }
   struct sDEADTIME_MODEL *inst = *opaque;

// Implement module evaluation code here:

   // By default, set max time step according to next event
   inst->delta_t = inst->t_nextE - t;

   // Enable outputs if flags are set and next event is reached
   if(t >= inst->t_nextE){

      inst->delta_t = dt_max;
      
      if(t >= inst->t_U){
         if(inst->HS_U_flag){
            HS_U = 1;
            inst->HS_U_flag = 0;
         }
         if(inst->LS_U_flag){
            LS_U = 1;
            inst->LS_U_flag = 0;
         }
      }
      if(t >= inst->t_V){
         if(inst->HS_V_flag){
            HS_V = 1;
            inst->HS_V_flag = 0;
         }
         if(inst->LS_V_flag){
            LS_V = 1;
            inst->LS_V_flag = 0;
         }
      }
      if(t >= inst->t_W){
         if(inst->HS_W_flag){
            HS_W = 1;
            inst->HS_W_flag = 0;
         }
         if(inst->LS_W_flag){
            LS_W = 1;
            inst->LS_W_flag = 0;
         }   
      }
   }

   // Check if states have changed and set next max. simulation time step and next time of event
   if(U != inst->p_U){
      inst->delta_t = dt_max;
      inst->t_U = t+Tdead;
      if(U == 1){
         LS_U = 0;
         if(inst->LS_U_flag == 1)
            inst->LS_U_flag = 0;
         inst->HS_U_flag = 1;
      }
      else{
         HS_U = 0;
         if(inst->HS_U_flag == 1)
            inst->HS_U_flag = 0;
         inst->LS_U_flag = 1;
      }
   }
   if(V != inst->p_V){
      inst->delta_t = dt_max;
      inst->t_V = t+Tdead;
      if(V == 1){
         LS_V = 0;
         if(inst->LS_V_flag == 1)
            inst->LS_V_flag = 0;
         inst->HS_V_flag = 1;
      }
      else{
         HS_V = 0;
         if(inst->HS_V_flag == 1)
            inst->HS_V_flag = 0;
         inst->LS_V_flag = 1;
      }
   }
   if(W != inst->p_W){
      inst->delta_t = dt_max;
      inst->t_W = t+Tdead;
      if(W == 1){
         LS_W = 0;
         if(inst->LS_W_flag == 1)
            inst->LS_W_flag = 0;
         inst->HS_W_flag = 1;
      }
      else{
         HS_W = 0;
         if(inst->HS_W_flag == 1)
            inst->HS_W_flag = 0;
         inst->LS_W_flag = 1;
      }
   }

   // Update previous values
   inst->p_U = U;
   inst->p_V = V;
   inst->p_W = W;

   // Update next event      
   inst->t_nextE = t + fmin(f_eval(inst->t_U-t), 
      fmin(f_eval(inst->t_V-t),f_eval(inst->t_W-t)));

}

// Find minimum
double fmin(double a, double b){
   if(a <= b) return a;
   return b;
}

// Evaluate wether to ignore value from minimum calculation
double f_eval(double a){
   if(a <= 0) return 1e10;
   return a;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDEADTIME_MODEL *inst, double t)
{
   //return 1e308; // implement a good choice of max timestep size that depends on struct sSVPWM_MODEL
   if (inst->delta_t <= 0) return 1e308;
   return inst->delta_t;
}

extern "C" __declspec(dllexport) void Destroy(struct sDEADTIME_MODEL *inst)
{
   free(inst);
}
