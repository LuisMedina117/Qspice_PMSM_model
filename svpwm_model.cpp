// Automatically generated C++ file on Tue Aug 26 21:46:54 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD svpwm_model.cpp kernel32.lib

#include <malloc.h>
#include <cmath>

#ifndef PI
#define PI 3.14159265
#endif

// Maximum rise/fall time
#define dt_max 10e-9

// Constants
#define const_2_sqrt3   1.154700538    // =2/sqrt(3)
#define const_pi_6      0.523598775    // =PI/6
#define const_pi_3      1.047197551    // =PI/3

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
#undef Theta
#undef Amplitude

struct sSVPWM_MODEL
{
   double delta_t;      // Maximum time step of simulation
   double t_nextE;      // Time of next event
   double t_mU_up;      // Time of U phase HIGH state
   double t_mV_up;      // Time of V phase HIGH state
   double t_mW_up;      // Time of W phase HIGH state
   double t_mU_down;    // Time of U phase LOW state
   double t_mV_down;    // Time of V phase LOW state
   double t_mW_down;    // Time of W phase LOW state
   double t_next_cycle; // Time of next PWM cycle
   bool s_U;            // State of phase U
   bool s_V;            // State of phase V
   bool s_W;            // State of phase W
   long int ticks;      // Number of PWM cycles completed
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
   int sector = 0;
   bool slope = 0;
   double theta_rel = 0;

   double mU, mV, mW;

   if(!*opaque)
   {
      *opaque = (struct sSVPWM_MODEL *) malloc(sizeof(struct sSVPWM_MODEL));
      bzero(*opaque, sizeof(struct sSVPWM_MODEL));
      (*opaque)->t_nextE = Tpwm;
      (*opaque)->t_next_cycle = Tpwm;
      (*opaque)->ticks = 1;
   }
   struct sSVPWM_MODEL *inst = *opaque;

// Implement module evaluation code here:

   // By default, set max time step according to next event
   inst->delta_t = inst->t_nextE - t;

   // Verifies if an event has been reached
   if(t >= inst->t_nextE){

      // Update event times after a PWM cycle is completed
      if(t >= inst->t_next_cycle){
         // Find sector
         sector = int(6.0*Theta/(2*PI))+1;

         // Calculate relative angle inside sector
         theta_rel = Theta - (sector-1)*const_pi_3;

         // Auxiliary variables
         //a = const_2_sqrt3*Amplitude*cos(theta_rel+const_pi_6);
         //b = const_2_sqrt3*Amplitude*sin(theta_rel);
         a = Amplitude*cos(theta_rel+const_pi_6);
         b = Amplitude*sin(theta_rel);

         // Compute duty cycle of each phase
         switch (sector){
            case 1:
               mU = (1.0+a+b)/2.0;
               mV = (1.0-a+b)/2.0;
               mW = (1.0-a-b)/2.0;
               break;
            case 2:
               mU = (1.0+a-b)/2.0;
               mV = (1.0+a+b)/2.0;
               mW = (1.0-a-b)/2.0;
               break;
            case 3:
               mU = (1.0-a-b)/2.0;
               mV = (1.0+a+b)/2.0;
               mW = (1.0-a+b)/2.0;
               break;
            case 4:
               mU = (1.0-a-b)/2.0;
               mV = (1.0+a-b)/2.0;
               mW = (1.0+a+b)/2.0;
               break;
            case 5:
               mU = (1.0-a+b)/2.0;
               mV = (1.0-a-b)/2.0;
               mW = (1.0+a+b)/2.0;
               break;
            case 6:
               mU = (1.0+a+b)/2.0;
               mV = (1.0-a-b)/2.0;
               mW = (1.0+a-b)/2.0;
               break;
            default:
               break;
         }

         // Define switching times
         inst->t_mU_up = (inst->ticks+(1.0-mU)/2.0)*Tpwm;
         inst->t_mU_down = (inst->ticks+(1.0+mU)/2.0)*Tpwm;
         inst->t_mV_up = (inst->ticks+(1.0-mV)/2.0)*Tpwm;
         inst->t_mV_down = (inst->ticks+(1.0+mV)/2.0)*Tpwm;
         inst->t_mW_up = (inst->ticks+(1.0-mW)/2.0)*Tpwm;
         inst->t_mW_down = (inst->ticks+(1.0+mW)/2.0)*Tpwm;

         // Next PWM cycle start
         inst->t_next_cycle = (inst->ticks+1)*Tpwm;

         // Update tick count 
         inst->ticks += 1;
      }

      // Set next time step to defined rise/fall time
      inst->delta_t = dt_max;//

      // Check slope
      if(t < inst->t_next_cycle-0.5*Tpwm)
         slope = 1;  // positive
      else
         slope = 0;  // negative
      
      // Change corresponding output and update phase status
      if(not(inst->s_U)){   // Phase U
         if(t >= inst->t_mU_up && slope){
            inst->s_U = 1;
            U = 1;
         }
      }
      else{
         if(t >= inst->t_mU_down && !slope){
            inst->s_U = 0;
            U = 0;
         }
      }
      if(not(inst->s_V)){   // Phase V
         if(t >= inst->t_mV_up && slope){
            inst->s_V = 1;
            V = 1;
         }
      }
      else{
         if(t >= inst->t_mV_down && !slope){
            inst->s_V = 0;
            V = 0;
         }
      }
      if(not(inst->s_W)){   // Phase W
         if(t >= inst->t_mW_up && slope){
            inst->s_W = 1;
            W = 1;
         }
      }
      else{
         if(t >= inst->t_mW_down && !slope){
            inst->s_W = 0;
            W = 0;
         }
      }

      // Update next event      
      inst->t_nextE = t + fmin(f_eval(inst->t_next_cycle-t), 
         fmin(f_eval(inst->t_mU_down-t), 
            fmin(f_eval(inst->t_mV_down-t), 
               fmin(f_eval(inst->t_mW_down-t), 
                  fmin(f_eval(inst->t_mU_up-t), 
                     fmin(f_eval(inst->t_mV_up-t), f_eval(inst->t_mW_up-t)))))));

   }
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
