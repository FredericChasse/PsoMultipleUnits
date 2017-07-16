//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Parallel PSO
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Classifier.c
// Author  : Frederic Chasse
// Date    : 2017-07-15
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file provides the Classifier_t type.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Classifier.h"


// Private definitions
//==============================================================================

typedef struct
{
  UnitArrayInterface_t *unitArray;
  UINT8                 nUnits;
  Position_t            curPos[N_UNITS_TOTAL];
  Position_t            optPos[N_UNITS_TOTAL];
  float                 margin;
} Classifier_t;


// Private prototypes
//==============================================================================

INT8  _Classifier_Init          (Classifier_t *c, UnitArrayInterface_t *unitArray, float margin);
INT8  _Classifier_UpdateValues  (Classifier_t *c);
INT8  _Classifier_ResetValues   (Classifier_t *c, UINT8 *idx, UINT8 nIdx);
INT16 _Classifier_Classify      (Classifier_t *c, UINT8 *idx, UINT8 nIdx, UINT8 *groups, UINT8 *lengths);
void  _Classifier_Release       (Classifier_t *c);

static int _CompareFunc         (const void *p1, const void *p2);


// Private variables
//==============================================================================

Classifier_t _classifier = 
{
  .unitArray  = NULL
 ,.nUnits     = 0
 ,.curPos     = {0}
 ,.optPos     = {0}
 ,.margin     = 0.0f
};

const ClassifierInterface_t _classifier_if = 
{
  .ctx          = (void *)                      &_classifier
 ,.Init         = (ClassifierInit_fct)          &_Classifier_Init
 ,.UpdateValues = (ClassifierUpdateValues_fct)  &_Classifier_UpdateValues
 ,.ResetValues  = (ClassifierResetValues_fct)   &_Classifier_ResetValues
 ,.Classify     = (ClassifierClassify_fct)      &_Classifier_Classify
 ,.Release      = (ClassifierRelease_fct)       &_Classifier_Release
};


// Private functions
//==============================================================================

INT8 _Classifier_Init (Classifier_t *c, UnitArrayInterface_t *unitArray, float margin)
{
  UINT8 i;
  
  if ((unitArray == NULL) || (margin < 0))
  {
    return -1;
  }
  c->unitArray = unitArray;
  c->nUnits    = unitArray->GetNUnits(unitArray->ctx);
  c->margin    = margin;
  for (i = 0; i < c->nUnits; i++)
  {
    Position_Reset(&c->curPos[i]);
    Position_Reset(&c->optPos[i]);
  }
  return 0;
}


INT8 _Classifier_UpdateValues (Classifier_t *c)
{
  UINT8 i;
  Position_t   *curPos
              ,*optPos
              ;
  
  for (i = 0; i < c->nUnits; i++)
  {
    curPos = &c->curPos[i];
    optPos = &c->optPos[i];
    
    curPos->prevPos = curPos->curPos;
    curPos->prevFitness = curPos->curFitness;
    curPos->curPos = c->unitArray->GetPos(c->unitArray->ctx, i);
    curPos->curFitness = c->unitArray->GetPower(c->unitArray->ctx, i);
    
    if ((optPos->curPos == curPos->curPos) && (optPos->curFitness != curPos->curFitness))
    {
      optPos->curFitness = curPos->curFitness;
    }
    
    if (curPos->curFitness > optPos->curFitness)
    {
      optPos->prevFitness = optPos->curFitness;
      optPos->prevPos = optPos->curPos;
      optPos->curFitness = curPos->curFitness;
      optPos->curPos = curPos->curPos;
    }
  }
  
  return 0;
}


INT8 _Classifier_ResetValues (Classifier_t *c, UINT8 *idx, UINT8 nIdx)
{
  UINT8 i;
  
  if ((nIdx == 0) || (nIdx > c->nUnits))
  {
    return -1;
  }
  
  for (i = 0; i < nIdx; i++)
  {
    c->optPos[idx[i]].curPos = c->curPos[idx[i]].curPos;
    c->optPos[idx[i]].curFitness = c->curPos[idx[i]].curFitness;
  }
  
  return 0;
}


INT16 _Classifier_Classify (Classifier_t *c, UINT8 *idx, UINT8 nIdx, UINT8 *groups, UINT8 *lengths)
{
  if ((idx == NULL) || (nIdx == 0) || (groups == NULL) || (lengths == NULL))
  {
    return -1;
  }
  
  float optPos[N_UNITS_TOTAL];
  float optPosSorted[N_UNITS_TOTAL];
  UINT8 i, j, iData, nGroups, iGroup, tmpIdx;
  UINT8 idxSorted[N_UNITS_TOTAL];
  UINT8 tmpGroups[N_UNITS_TOTAL][N_UNITS_TOTAL];
  
  for (i = 0; i < nIdx; i++)
  {
    optPosSorted[i] = c->optPos[idx[i]].curPos;
  }
  
  memcpy(optPos, optPosSorted, 4*nIdx);
  
  qsort( (void *) optPosSorted, (size_t) nIdx, sizeof(UINT8), &_CompareFunc);
  
#define IDX_USED (N_UNITS_TOTAL+1)
  
  for (i = 0; i < nIdx; i++)
  {
    for (j = 0; j < nIdx; j++)
    {
      if (optPosSorted[i] == optPos[j])
      {
        idxSorted[i] = j;
        optPos[j] = IDX_USED;
        break;
      }
    }
  }
  
  iData = 0;
  nGroups = 1;
  iGroup = 0;
  
  lengths[iGroup] = 0;
  tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[iData];
  
  for (i = 1; i < nIdx; i++)
  {
    if ( (optPosSorted[i] - optPosSorted[tmpGroups[iGroup][0]]) <= c->margin)
    {
      tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[i];
    }
    else
    {
      iGroup++;
      nGroups++;
      lengths[iGroup] = 0;
      if (lengths[iGroup - 1] > 1)
      {
        if ( (optPosSorted[i] - optPosSorted[i - 1]) < (optPosSorted[i - 1] - optPosSorted[i - 2]))
        {
          tmpIdx = tmpGroups[iGroup - 1][lengths[iGroup - 1] - 1];
          tmpGroups[iGroup][lengths[iGroup]++] = tmpIdx;
          tmpGroups[iGroup][lengths[iGroup]++] = optPosSorted[i];
        }
        else
        {
          tmpGroups[iGroup][lengths[iGroup]++] = optPosSorted[i];
        }
      }
      else
      {
        tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[i];
      }
    }
  }
  
  tmpIdx = 0;
  for (i = 0; i < nGroups; i++)
  {
    memcpy(&groups[tmpIdx], &tmpGroups[i][0], lengths[i]);
    tmpIdx += lengths[i];
  }
  
  return nGroups;
}


void _Classifier_Release (Classifier_t *c)
{
  UINT8 i;
  c->unitArray = NULL;
  c->nUnits    = 0;
  c->margin    = 0;
  for (i = 0; i < N_UNITS_TOTAL; i++)
  {
    Position_Reset(&c->curPos[i]);
    Position_Reset(&c->optPos[i]);
  }
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(float *) p1  > *(float *) p2 ) return -1;
  if ( *(float *) p1 == *(float *) p2 ) return  0;
  if ( *(float *) p1  < *(float *) p2 ) return  1;
}


// Public functions
//==============================================================================

const ClassifierInterface_t * ClassifierInterface (void)
{
  return &_classifier_if;
}