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
  UINT8                 unitsIdToIdx[N_UNITS_TOTAL];
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
float _Classifier_GetBestPos    (Classifier_t *c, UINT8 idx);

static int _CompareFunc         (const void *p1, const void *p2);
static int _CompareFuncUint8    (const void *p1, const void *p2);


// Private variables
//==============================================================================

Classifier_t _classifier = 
{
  .unitArray      = NULL
 ,.nUnits         = 0
 ,.unitsIdToIdx   = {0}
 ,.curPos         = {0}
 ,.optPos         = {0}
 ,.margin         = 0.0f
};

const ClassifierInterface_t _classifier_if = 
{
  .ctx          = (void *)                      &_classifier
 ,.Init         = (ClassifierInit_fct)          &_Classifier_Init
 ,.UpdateValues = (ClassifierUpdateValues_fct)  &_Classifier_UpdateValues
 ,.ResetValues  = (ClassifierResetValues_fct)   &_Classifier_ResetValues
 ,.Classify     = (ClassifierClassify_fct)      &_Classifier_Classify
 ,.Release      = (ClassifierRelease_fct)       &_Classifier_Release
 ,.GetBestPos   = (ClassifierGetBestPos_fct)    &_Classifier_GetBestPos
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
    c->unitsIdToIdx[c->unitArray->GetUnitId(c->unitArray->ctx, i)] = i;
  }
  return 0;
}


float _Classifier_GetBestPos (Classifier_t *c, UINT8 idx)
{
  return c->optPos[c->unitsIdToIdx[idx]].curPos;
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
    c->optPos[c->unitsIdToIdx[idx[i]]].curPos = c->curPos[c->unitsIdToIdx[idx[i]]].curPos;
    c->optPos[c->unitsIdToIdx[idx[i]]].curFitness = c->curPos[c->unitsIdToIdx[idx[i]]].curFitness;
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
  
  if (nIdx <= 3)  // Only one group
  {
    memcpy(groups, idx, nIdx);
    lengths[0] = nIdx;
    nGroups = 1;
    return nGroups;
  }
  
  for (i = 0; i < nIdx; i++)
  {
    optPosSorted[i] = c->optPos[c->unitsIdToIdx[idx[i]]].curPos;
  }
  
  memcpy(optPos, optPosSorted, 4*nIdx);
  
  qsort( (void *) optPosSorted, (size_t) nIdx, sizeof(float), &_CompareFunc);
  
#define IDX_USED (N_UNITS_TOTAL+1)
  
  for (i = 0; i < nIdx; i++)
  {
    for (j = 0; j < nIdx; j++)
    {
      if (optPosSorted[i] == optPos[j])
      {
        idxSorted[i] = c->unitsIdToIdx[idx[j]];
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
    if ( (optPosSorted[i] - optPosSorted[iData]) <= c->margin)
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
          tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[i];
          lengths[iGroup - 1]--;
        }
        else
        {
          tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[i];
        }
      }
      else
      {
        tmpGroups[iGroup][lengths[iGroup]++] = idxSorted[i];
      }
      iData += lengths[iGroup - 1];
    }
  }
  
  tmpIdx = 0;
  for (i = 0; i < nGroups; i++)
  {
    memcpy(&groups[tmpIdx], &tmpGroups[i][0], lengths[i]);
    tmpIdx += lengths[i];
  }
  
  if (nGroups > 1)
  {
    // Merge groups with less than 3 units
    iGroup = 0;
    tmpIdx = 0;
    for (iGroup = 0; iGroup < nGroups; iGroup++)
    {
      if (lengths[iGroup] < 3)
      {
        if ( (iGroup > 0) && (iGroup < (nGroups - 1)) )   // Not first or last group
        {
          if (    (optPosSorted[tmpIdx] - optPosSorted[tmpIdx -1]) 
                < (optPosSorted[tmpIdx+lengths[iGroup]] - optPosSorted[tmpIdx+lengths[iGroup] - 1]) 
//          if (    (optPosSorted[groups[tmpIdx]] - optPosSorted[groups[tmpIdx -1]]) 
//                < (optPosSorted[groups[tmpIdx+lengths[iGroup]]] - optPosSorted[groups[tmpIdx+lengths[iGroup] - 1]]) 
             )
          {
            lengths[iGroup-1] += lengths[iGroup];
            for (i = iGroup; i < nGroups - 1; i++)
            {
              lengths[i] = lengths[i + 1];
            }
            nGroups--;
            iGroup--;   // To test this new group
          }
          else
          {
            lengths[iGroup] += lengths[iGroup + 1];
            for (i = iGroup + 1; i < nGroups; i++)
            {
              lengths[i] = lengths[i + 1];
            }
            nGroups--;
            iGroup--;   // To test this new group
          }
        }
        else if (iGroup == 0)
        {
          lengths[iGroup] += lengths[iGroup + 1];
          for (i = iGroup + 1; i < nGroups; i++)
          {
            lengths[i] = lengths[i + 1];
          }
          nGroups--;
          iGroup--;   // To test this new group
        }
        else  // iGroup == (nGroups - 1)
        {
          lengths[iGroup-1] += lengths[iGroup];
          for (i = iGroup; i < nGroups - 1; i++)
          {
            lengths[i] = lengths[i + 1];
          }
          nGroups--;
          iGroup--;   // To test this new group
        }
      }
      else
      {
        tmpIdx += lengths[iGroup];
      }
    }
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
    c->unitsIdToIdx[i] = 0;
  }
}


static int _CompareFuncUint8 (const void *p1, const void *p2)
{
  if ( *(UINT8 *) p1  > *(UINT8 *) p2 ) return  1;
  if ( *(UINT8 *) p1 == *(UINT8 *) p2 ) return  0;
  if ( *(UINT8 *) p1  < *(UINT8 *) p2 ) return -1;
}


static int _CompareFunc (const void *p1, const void *p2)
{
  if ( *(float *) p1  > *(float *) p2 ) return  1;
  if ( *(float *) p1 == *(float *) p2 ) return  0;
  if ( *(float *) p1  < *(float *) p2 ) return -1;
}


// Public functions
//==============================================================================

const ClassifierInterface_t * ClassifierInterface (void)
{
  return &_classifier_if;
}