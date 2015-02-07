#include "SC_PlugIn.h"

static InterfaceTable *ft;

struct DynEnv : public Unit
{
  double t;
  double current_value;
  double target_value;
  double target_duration;
  float dx;
};

static void DynEnv_next(DynEnv *unit, int inNumSamples);
static void DynEnv_Ctor(DynEnv* unit);

void DynEnv_Ctor(DynEnv* unit)
{
  SETCALC(DynEnv_next);
  
  unit->target_value = IN0(0);
  unit->current_value = IN0(0);
  unit->t = 0.0;
  unit->dx = 0.0;
  
  DynEnv_next(unit, 1);
}

void DynEnv_next(DynEnv *unit, int inNumSamples)
{
  float *out = OUT(0);
  float target_value = unit->target_value;
  float new_target_value = IN0(0);
  float v = unit->current_value;

  float base_t = unit->t;
  float dx;
  
  if(new_target_value == target_value){
    dx = unit->dx;
    if(dx == 0.0){      
      std::fill(out, out + inNumSamples, target_value);
      return;
    }
  }else{
    base_t = 0.0;
    unit->target_value = new_target_value;
    unit->target_duration = IN0(1);
    dx = (new_target_value - v) / unit->target_duration * SAMPLEDUR;    
  }

  float target_duration = unit->target_duration;
  
  float t = base_t;
  for (int i=0; i < inNumSamples; ++i)
  {
     t += SAMPLEDUR;
     if(t < target_duration){
       v += dx;
     }else{
       v = target_value;
       dx = 0.0;
     }
     out[i] = v;
  }

  unit->t = t;
  unit->current_value = v;
  unit->dx = dx;
}

PluginLoad(DynEnv)
{
  ft = inTable;
  DefineSimpleUnit(DynEnv);
}
