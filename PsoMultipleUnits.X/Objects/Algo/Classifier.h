//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Classifier.h
// Author  : Frederic Chasse
// Date    : 2017-07-15
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file provides the Classifier type.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __CLASSIFIER_H__
#define __CLASSIFIER_H__

#include "Setup.h"
#include "UnitArray.h"
#include "Position.h"


// Public definitions
//==============================================================================

typedef INT8  (*ClassifierInit_fct)         (void *ctx, UnitArrayInterface_t *unitArray, float margin);
typedef INT8  (*ClassifierUpdateValues_fct) (void *ctx);
typedef INT8  (*ClassifierResetValues_fct)  (void *ctx, UINT8 *idx, UINT8 nIdx);
typedef INT16 (*ClassifierClassify_fct)     (void *ctx, UINT8 *idx, UINT8 nIdx, UINT8 *groups, UINT8 *lengths);
typedef void  (*ClassifierRelease_fct)      (void *ctx);
typedef float (*ClassifierGetBestPos_fct)   (void *ctx, UINT8 idx);

typedef struct
{
  void                       *ctx;
  ClassifierInit_fct          Init;
  ClassifierUpdateValues_fct  UpdateValues;
  ClassifierResetValues_fct   ResetValues;
  ClassifierClassify_fct      Classify;
  ClassifierRelease_fct       Release;
  ClassifierGetBestPos_fct    GetBestPos;
} ClassifierInterface_t;


// Public functions
//==============================================================================

const ClassifierInterface_t * ClassifierInterface(void);


#endif // __CLASSIFIER_H__