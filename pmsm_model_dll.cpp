// Automatically generated C++ file on Tue Aug 26 21:46:54 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD svpwm_model.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265
#endif

// Maximum rise/dead time
#define dt_max 10e-9

// Constants
#define const_2_sqrt3 1.154700538 // 2/sqrt(3)

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
#undef HS_U
#undef LS_U
#undef HS_V
#undef LS_V
#undef HS_W
#undef LS_W
#undef Theta
#undef Amplitude

struct sSVPWM_MODEL
{
   // declare the structure here
   double delta_t;
   double t_prev;
   double t_nextE;      // Time of next event
   double t_mU_up;      // Time of U phase HIGH state
   double t_mV_up;      // Time of V phase HIGH state
   double t_mW_up;      // Time of W phase HIGH state
   double t_mU_down;    // Time of U phase LOW state
   double t_mV_down;    // Time of V phase LOW state
   double t_mW_down;    // Time of W phase LOW state
   double t_next_cycle;
   bool trigger;
   bool s_U;
   bool s_V;
   bool s_W;
   int sector;
   double m_U;
   double m_V;
   double m_W;
   long int ticks;
};

extern "C" __declspec(dllexport) void svpwm_model(struct sSVPWM_MODEL **opaque, double t, union uData *data)
{
   double Theta     = data[0].d; // input
   double Amplitude = data[1].d; // input
   double Tpwm      = data[2].d; // input parameter
   bool  &U         = data[3].b; // output
   bool  &V         = data[4].b; // output
   bool  &W         = data[5].b; // output

   double a = 0;
   double b = 0;


   if(!*opaque)
   {
      *opaque = (struct sSVPWM_MODEL *) malloc(sizeof(struct sSVPWM_MODEL));
      bzero(*opaque, sizeof(struct sSVPWM_MODEL));
   }
   struct sSVPWM_MODEL *inst = *opaque;

// Implement module evaluation code here:

    // By default, set max time step according to next event
    inst->delta_t = inst->t_nextE - t;

    // Verifies if an event has been reached
    if(t >= inst->t_nextE){

        // Trigger update if a cycle has been completed
        if(t >= inst->t_next_cycle)
            inst->trigger = 1;

        // Set next time step to defined rise/fall time
        inst->delta_t = dt_max;

        // Change corresponding output
        if(not(inst->s_U))   // Phase U
            if(t >= inst->t_mU_up){
            inst->s_U = 1;
            U = 1;
            }
        else
            if(t >= inst->t_mU_down){
            inst->s_U = 0;
            U = 0;
            }
        if(not(inst->s_V))   // Phase V
            if(t >= inst->t_mV_up){
            inst->s_V = 1;
            V = 1;
            }
        else
            if(t >= inst->t_mV_down){
            inst->s_V = 0;
            V = 0;
            }
        if(not(inst->s_W))   // Phase W
            if(t >= inst->t_mW_up){
            inst->s_W = 1;
            W = 1;
            }
        else
            if(t >= inst->t_mW_down){
            inst->s_W = 0;
            W = 0;
            }
        
        // Update next event
        update_next_event(inst, t);
    }


    // Update event times after a PWM cycle is completed
    if(inst->trigger){
         
         // Find sector
         inst->sector = int(6*Theta/(2*PI))+1;
         // Calculate coeficients
         a = const_2_sqrt3;
         b = const_2_sqrt3;
         switch (inst->sector){
            case 1:
               /* code */
               break;
            default:
               break;
         }
         // Next PWM cycle start
         inst->t_next_cycle = (inst->ticks+1) * Tpwm;
         

         // Update tick count 
         inst->ticks++;
         // Reset trigger
         inst->trigger = 0;
    }
}

// Update time of the next event
void update_next_event(sSVPWM_MODEL *inst, const double t){
   // Find next event 
   inst->t_nextE = fmin(fmax(inst->t_next_cycle,t), 
      fmin(fmax(inst->t_mU_down,t), 
         fmin(fmax(inst->t_mV_down,t), 
            fmin(fmax(inst->t_mW_down,t), 
               fmin(fmax(inst->t_mU_up,t), 
                  fmin(fmax(inst->t_mV_up,t), fmax(inst->t_mW_up,t)))))));
   return;
}

// Find minimum
double fmin(double a, double b){
   if(a <= b)
      return a;
   return b;
}

// Find maximum
double fmax(double a, double b){
   if(a >= b)
      return a;
   return b;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSVPWM_MODEL *inst, double t)
{
   //return 1e308; // implement a good choice of max timestep size that depends on struct sSVPWM_MODEL
   if (inst->delta_t <= 0) return 1e308;
   return inst->delta_t;
}

extern "C" __declspec(dllexport) void Destroy(struct sSVPWM_MODEL *inst)
{
   free(inst);
}
