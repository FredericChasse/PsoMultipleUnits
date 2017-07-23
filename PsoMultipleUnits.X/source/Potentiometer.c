//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Potentiometer.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the potentiometers AD8403.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\Potentiometer.h"


//==============================================================================
// Potentiometer variables
//==============================================================================
const float potRealValues[256] = { 50, 53.9216, 57.8431, 61.7647, 65.6863, 69.6078, 73.5294
        , 77.451, 81.3725, 85.2941, 89.2157, 93.1373, 97.0588, 100.9804, 104.902, 108.8235
        , 112.7451, 116.6667, 120.5882, 124.5098, 128.4314, 132.3529, 136.2745, 140.1961
        , 144.1176, 148.0392, 151.9608, 155.8824, 159.8039, 163.7255, 167.6471, 171.5686
        , 175.4902, 179.4118, 183.3333, 187.2549, 191.1765, 195.098, 199.0196, 202.9412
        , 206.8627, 210.7843, 214.7059, 218.6275, 222.549, 226.4706, 230.3922, 234.3137
        , 238.2353, 242.1569, 246.0784, 250, 253.9216, 257.8431, 261.7647, 265.6863, 269.6078
        , 273.5294, 277.451, 281.3725, 285.2941, 289.2157, 293.1373, 297.0588, 300.9804, 304.902
        , 308.8235, 312.7451, 316.6667, 320.5882, 324.5098, 328.4314, 332.3529, 336.2745, 340.1961
        , 344.1176, 348.0392, 351.9608, 355.8824, 359.8039, 363.7255, 367.6471, 371.5686, 375.4902
        , 379.4118, 383.3333, 387.2549, 391.1765, 395.098, 399.0196, 402.9412, 406.8627, 410.7843
        , 414.7059, 418.6275, 422.549, 426.4706, 430.3922, 434.3137, 438.2353, 442.1569, 446.0784
        , 450, 453.9216, 457.8431, 461.7647, 465.6863, 469.6078, 473.5294, 477.451, 481.3725
        , 485.2941, 489.2157, 493.1373, 497.0588, 500.9804, 504.902, 508.8235, 512.7451, 516.6667
        , 520.5882, 524.5098, 528.4314, 532.3529, 536.2745, 540.1961, 544.1176, 548.0392, 551.9608
        , 555.8824, 559.8039, 563.7255, 567.6471, 571.5686, 575.4902, 579.4118, 583.3333, 587.2549
        , 591.1765, 595.098, 599.0196, 602.9412, 606.8627, 610.7843, 614.7059, 618.6275, 622.549
        , 626.4706, 630.3922, 634.3137, 638.2353, 642.1569, 646.0784, 650, 653.9216, 657.8431
        , 661.7647, 665.6863, 669.6078, 673.5294, 677.451, 681.3725, 685.2941, 689.2157, 693.1373
        , 697.0588, 700.9804, 704.902, 708.8235, 712.7451, 716.6667, 720.5882, 724.5098, 728.4314
        , 732.3529, 736.2745, 740.1961, 744.1176, 748.0392, 751.9608, 755.8824, 759.8039, 763.7255
        , 767.6471, 771.5686, 775.4902, 779.4118, 783.3333, 787.2549, 791.1765, 795.098, 799.0196
        , 802.9412, 806.8627, 810.7843, 814.7059, 818.6275, 822.549, 826.4706, 830.3922, 834.3137
        , 838.2353, 842.1569, 846.0784, 850, 853.9216, 857.8431, 861.7647, 865.6863, 869.6078
        , 873.5294, 877.451, 881.3725, 885.2941, 889.2157, 893.1373, 897.0588, 900.9804, 904.902
        , 908.8235, 912.7451, 916.6667, 920.5882, 924.5098, 928.4314, 932.3529, 936.2745, 940.1961
        , 944.1176, 948.0392, 951.9608, 955.8824, 959.8039, 963.7255, 967.6471, 971.5686, 975.4902
        , 979.4118, 983.3333, 987.2549, 991.1765, 995.098, 999.0196, 1002.9412, 1006.8627, 1010.7843
        , 1014.7059, 1018.6275, 1022.549, 1026.4706, 1030.3922, 1034.3137, 1038.2353, 1042.1569, 1046.0784, 1050 };

const float potRealValuesInverse[256] = { 0.02, 0.018545, 0.017288, 0.01619, 0.015224, 0.014366, 0.0136, 0.012911
        , 0.012289, 0.011724, 0.011209, 0.010737, 0.010303, 0.0099029, 0.0095327, 0.0091892, 0.0088696, 0.0085714
        , 0.0082927, 0.0080315, 0.0077863, 0.0075556, 0.0073381, 0.0071329, 0.0069388, 0.006755, 0.0065806, 0.0064151
        , 0.0062577, 0.0061078, 0.0059649, 0.0058286, 0.0056983, 0.0055738, 0.0054545, 0.0053403, 0.0052308, 0.0051256
        , 0.0050246, 0.0049275, 0.0048341, 0.0047442, 0.0046575, 0.004574, 0.0044934, 0.0044156, 0.0043404, 0.0042678
        , 0.0041975, 0.0041296, 0.0040637, 0.004, 0.0039382, 0.0038783, 0.0038202, 0.0037638, 0.0037091, 0.0036559
        , 0.0036042, 0.003554, 0.0035052, 0.0034576, 0.0034114, 0.0033663, 0.0033225, 0.0032797, 0.0032381, 0.0031975
        , 0.0031579, 0.0031193, 0.0030816, 0.0030448, 0.0030088, 0.0029738, 0.0029395, 0.002906, 0.0028732, 0.0028412
        , 0.0028099, 0.0027793, 0.0027493, 0.00272, 0.0026913, 0.0026632, 0.0026357, 0.0026087, 0.0025823, 0.0025564
        , 0.002531, 0.0025061, 0.0024818, 0.0024578, 0.0024344, 0.0024113, 0.0023888, 0.0023666, 0.0023448, 0.0023235
        , 0.0023025, 0.0022819, 0.0022616, 0.0022418, 0.0022222, 0.002203, 0.0021842, 0.0021656, 0.0021474, 0.0021294
        , 0.0021118, 0.0020945, 0.0020774, 0.0020606, 0.0020441, 0.0020278, 0.0020118, 0.0019961, 0.0019806, 0.0019653
        , 0.0019503, 0.0019355, 0.0019209, 0.0019065, 0.0018924, 0.0018785, 0.0018647, 0.0018512, 0.0018378, 0.0018247
        , 0.0018117, 0.0017989, 0.0017863, 0.0017739, 0.0017617, 0.0017496, 0.0017376, 0.0017259, 0.0017143, 0.0017028
        , 0.0016915, 0.0016804, 0.0016694, 0.0016585, 0.0016478, 0.0016372, 0.0016268, 0.0016165, 0.0016063, 0.0015962
        , 0.0015863, 0.0015765, 0.0015668, 0.0015573, 0.0015478, 0.0015385, 0.0015292, 0.0015201, 0.0015111, 0.0015022
        , 0.0014934, 0.0014847, 0.0014761, 0.0014676, 0.0014592, 0.0014509, 0.0014427, 0.0014346, 0.0014266, 0.0014186
        , 0.0014108, 0.001403, 0.0013953, 0.0013878, 0.0013802, 0.0013728, 0.0013655, 0.0013582, 0.001351, 0.0013439
        , 0.0013368, 0.0013299, 0.001323, 0.0013161, 0.0013094, 0.0013027, 0.0012961, 0.0012895, 0.001283, 0.0012766
        , 0.0012702, 0.0012639, 0.0012577, 0.0012515, 0.0012454, 0.0012394, 0.0012334, 0.0012274, 0.0012216, 0.0012157
        , 0.00121, 0.0012043, 0.0011986, 0.001193, 0.0011874, 0.0011819, 0.0011765, 0.0011711, 0.0011657, 0.0011604
        , 0.0011552, 0.0011499, 0.0011448, 0.0011397, 0.0011346, 0.0011296, 0.0011246, 0.0011196, 0.0011148, 0.0011099
        , 0.0011051, 0.0011003, 0.0010956, 0.0010909, 0.0010863, 0.0010817, 0.0010771, 0.0010726, 0.0010681, 0.0010636
        , 0.0010592, 0.0010548, 0.0010505, 0.0010462, 0.0010419, 0.0010376, 0.0010334, 0.0010293, 0.0010251, 0.001021
        , 0.0010169, 0.0010129, 0.0010089, 0.0010049, 0.001001, 0.00099707, 0.00099318, 0.00098933, 0.00098551, 0.00098171
        , 0.00097795, 0.00097421, 0.0009705, 0.00096682, 0.00096317, 0.00095955, 0.00095595, 0.00095238 };


//==============================================================================
// Potentiometer private functions prototypes
//==============================================================================


//==============================================================================
// Potentiometers functions
//==============================================================================


/**************************************************************
 * Function name  : ShutdownPot
 * Purpose        : Shutdown a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ShutdownPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  Port.D.ClearBits(1 << (numPot + 8));
  return 0;
}


/**************************************************************
 * Function name  : TurnOnPot
 * Purpose        : Turn on a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 TurnOnPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  Port.D.SetBits(1 << (numPot + 8));
  return 0;
}


/**************************************************************
 * Function name  : ResetPot
 * Purpose        : Reset a pot at its mid-value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ResetPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  UINT8 i = 0;
  Port.E.ClearBits(1 << numPot);
  for (i = 0; i < 200; i++);
  Port.E.SetBits  (1 << numPot);
  return 0;
}


/**************************************************************
 * Function name  : ComputePotValueFloat2Dec
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : float desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
inline INT16 ComputePotValueFloat2Dec(float desiredValue)
{
  if ( !((desiredValue <= MAX_POT_VALUE) && (desiredValue >= WIPER_VALUE)) )
  {
    return -1;
  }
  return ((float) ((desiredValue - WIPER_VALUE) / (MAX_POT_VALUE - WIPER_VALUE)) * 255) + 0.5;
}


/**************************************************************
 * Function name  : ComputePotValueDec2Float
 * Purpose        : Converts a range of 0-255 to an ohmic value.
 * Arguments      : UINT8 decimal value [0 - 255]
 *                  float *realValue : address of the result
 * Returns        : None.
 *************************************************************/
inline void ComputePotValueDec2Float(UINT8 desiredValue, float *realValue)
{
  *realValue = (float) desiredValue * (MAX_POT_VALUE - WIPER_VALUE) / 255.0f + 50.0f;
}


/**************************************************************
 * Function name  : SetPotAllUnits
 * Purpose        : Set all units of a pot to the same value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3)
 *                  UINT8 index : pot index (0 - 3)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPotAllUnits (UINT8 numPot, UINT8 value)
{
  if (numPot > 3)
  {
    return -1;
  }
  
  SetPot(numPot << 2 | 0, value);
  SetPot(numPot << 2 | 1, value);
  SetPot(numPot << 2 | 2, value);
  SetPot(numPot << 2 | 3, value);

  return 0;
}


/**************************************************************
 * Function name  : SetPot
 * Purpose        : Set the value of a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-15)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPot (UINT8 numPot, UINT8 value)
{
  if (numPot > 15)
  {
    return -1;
  }
  
  UINT8 pot = numPot >> 2;
  UINT8 index = numPot & 0b11;
  
  while(Spi.IsSpiBusy(SPI3));
  Port.D.ClearBits(1 << (pot + 4));
  Spi.SendCharacter(SPI3, (index << 8) | value);
  while(Spi.IsSpiBusy(SPI3));
//  ret = Spi.BlockingTransfer(SPI3, (index << 8) | value);
  Port.D.SetBits(1 << (pot + 4));

  return 0;
}


///**************************************************************
// * Function name  : SetPot
// * Purpose        : Set the value of a potentiometer.
// * Arguments      : UINT8 numPot : the number of the pot (0-3)
// *                  UINT8 index : pot index (0 - 3)
// *                  UINT8 value : pot increment (0 - 255)
// * Returns        : 0 on success, -1 on failure.
// *************************************************************/
//inline INT8 SetPot (UINT8 numPot, UINT16 index, UINT8 value)
//{
//  if (numPot >= 4)
//  {
//    return -1;
//  }
//  if (index >= 4)
//  {
//    return -1;
//  }
//  
//  while(Spi.IsSpiBusy(SPI3));
//  Port.D.ClearBits(1 << (numPot + 4));
//  Spi.SendCharacter( SPI3, ( (index << 8) | value ) );
//  while(Spi.IsSpiBusy(SPI3));
//  Port.D.SetBits(1 << (numPot + 4));
//
//  return 0;
//}


/**************************************************************
 * Function name  : InitPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 InitPot (UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  
  TurnOnPot(numPot);
  
  ResetPot(numPot);

  return 0;
}