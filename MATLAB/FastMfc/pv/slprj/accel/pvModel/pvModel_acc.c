#include "__cf_pvModel.h"
#include <math.h>
#include "pvModel_acc.h"
#include "pvModel_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
void fnplb0kurc ( SimStruct * const S ) { pcplow3pin * _rtB ; cpxksz1lxi *
_rtDW ; _rtDW = ( ( cpxksz1lxi * ) ssGetRootDWork ( S ) ) ; _rtB = ( (
pcplow3pin * ) _ssGetBlockIO ( S ) ) ; ssCallAccelRunBlock ( S , 0 , 0 ,
SS_CALL_MDL_INITIALIZE_CONDITIONS ) ; ssCallAccelRunBlock ( S , 1 , 1 ,
SS_CALL_MDL_INITIALIZE_CONDITIONS ) ; } void bzpiibtxxj ( SimStruct * const S
) { pcplow3pin * _rtB ; _rtB = ( ( pcplow3pin * ) _ssGetBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_RTW_GENERATED_ENABLE ) ; } void
m13kbqcs0g ( SimStruct * const S ) { ssCallAccelRunBlock ( S , 3 , 6 ,
SS_CALL_MDL_OUTPUTS ) ; } void oekp0dakeh ( SimStruct * const S ) {
pcplow3pin * _rtB ; cpxksz1lxi * _rtDW ; _rtDW = ( ( cpxksz1lxi * )
ssGetRootDWork ( S ) ) ; _rtB = ( ( pcplow3pin * ) _ssGetBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 1 , 1 , SS_CALL_MDL_UPDATE ) ; } void mx1fiirz0r (
SimStruct * const S ) { pcplow3pin * _rtB ; cpxksz1lxi * _rtDW ; _rtDW = ( (
cpxksz1lxi * ) ssGetRootDWork ( S ) ) ; _rtB = ( ( pcplow3pin * )
_ssGetBlockIO ( S ) ) ; ssCallAccelRunBlock ( S , 1 , 1 ,
SS_CALL_MDL_DERIVATIVES ) ; } void d3tq5ywgdg ( SimStruct * const S ) {
pcplow3pin * _rtB ; cpxksz1lxi * _rtDW ; _rtDW = ( ( cpxksz1lxi * )
ssGetRootDWork ( S ) ) ; _rtB = ( ( pcplow3pin * ) _ssGetBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 1 , 1 , SS_CALL_MDL_ZERO_CROSSINGS ) ; } void
kjlnbl0dyn ( SimStruct * const S ) { pcplow3pin * _rtB ; cpxksz1lxi * _rtDW ;
_rtDW = ( ( cpxksz1lxi * ) ssGetRootDWork ( S ) ) ; _rtB = ( ( pcplow3pin * )
_ssGetBlockIO ( S ) ) ; { SimStruct * rts = ssGetSFunction ( S , 0 ) ;
sfcnTerminate ( rts ) ; } { SimStruct * rts = ssGetSFunction ( S , 1 ) ;
sfcnTerminate ( rts ) ; } } static void mdlOutputs ( SimStruct * S , int_T
tid ) { pcplow3pin * _rtB ; jqfqggqe3x * _rtP ; iqlqog0tug * _rtX ;
cpxksz1lxi * _rtDW ; _rtDW = ( ( cpxksz1lxi * ) ssGetRootDWork ( S ) ) ; _rtX
= ( ( iqlqog0tug * ) ssGetContStates ( S ) ) ; _rtP = ( ( jqfqggqe3x * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( pcplow3pin * ) _ssGetBlockIO ( S ) ) ;
if ( ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> k4v00e2pja = _rtP -> P_2 ; _rtB
-> islktryvmw = _rtP -> P_3 ; _rtB -> dye0nw1i2j = _rtP -> P_4 ; _rtB ->
itxua2kdtj = _rtP -> P_7 ; } _rtB -> esev1eddky = _rtP -> P_6 * _rtX ->
oq2pg1irxm / _rtB -> itxua2kdtj ; m13kbqcs0g ( S ) ; _rtB -> dz3jsyraye =
_rtP -> P_8 * _rtB -> cbvn1px1bm [ 4 ] * ( _rtP -> P_9 * _rtB -> cbvn1px1bm [
3 ] ) ; ssCallAccelRunBlock ( S , 3 , 10 , SS_CALL_MDL_OUTPUTS ) ; if (
ssIsSampleHit ( S , 1 , 0 ) ) { _rtB -> l431luycds = _rtP -> P_10 ; } if (
ssIsMajorTimeStep ( S ) ) { _rtDW -> mmbcdnllhu = ( _rtB -> fxkyldpplo >=
_rtP -> P_11 ) ; } if ( _rtDW -> mmbcdnllhu ) { _rtB -> cjitexnrou = _rtP ->
P_1 * _rtB -> cbvn1px1bm [ 0 ] ; _rtB -> gnjrd5c1e1 = _rtB -> cjitexnrou ; }
else { _rtB -> gnjrd5c1e1 = _rtB -> l431luycds ; } if ( ssIsMajorTimeStep ( S
) ) { _rtDW -> fm4relyo2q = _rtB -> gnjrd5c1e1 >= _rtP -> P_12 ? 1 : _rtB ->
gnjrd5c1e1 > _rtP -> P_13 ? 0 : - 1 ; } _rtB -> mrbdavhyus = _rtP -> P_14 *
_rtB -> cbvn1px1bm [ 2 ] ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { oekp0dakeh ( S ) ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { pcplow3pin * _rtB ; jqfqggqe3x
* _rtP ; iqlqog0tug * _rtX ; ll2ikxcwvh * _rtXdot ; _rtXdot = ( ( ll2ikxcwvh
* ) ssGetdX ( S ) ) ; _rtX = ( ( iqlqog0tug * ) ssGetContStates ( S ) ) ;
_rtP = ( ( jqfqggqe3x * ) ssGetDefaultParam ( S ) ) ; _rtB = ( ( pcplow3pin *
) _ssGetBlockIO ( S ) ) ; _rtXdot -> oq2pg1irxm = 0.0 ; _rtXdot -> oq2pg1irxm
+= _rtP -> P_5 * _rtX -> oq2pg1irxm ; _rtXdot -> oq2pg1irxm += _rtB ->
mrbdavhyus ; mx1fiirz0r ( S ) ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { pcplow3pin * _rtB ;
jqfqggqe3x * _rtP ; i1o5rciocw * _rtZCSV ; _rtZCSV = ( ( i1o5rciocw * )
ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( ( jqfqggqe3x * )
ssGetDefaultParam ( S ) ) ; _rtB = ( ( pcplow3pin * ) _ssGetBlockIO ( S ) ) ;
d3tq5ywgdg ( S ) ; _rtZCSV -> pd3ianx0dw = _rtB -> fxkyldpplo - _rtP -> P_11
; _rtZCSV -> gek12koagh = _rtB -> gnjrd5c1e1 - _rtP -> P_12 ; _rtZCSV ->
cf0vtuc5ze = _rtB -> gnjrd5c1e1 - _rtP -> P_13 ; } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 2473713854U
) ; ssSetChecksumVal ( S , 1 , 3519078068U ) ; ssSetChecksumVal ( S , 2 ,
840861347U ) ; ssSetChecksumVal ( S , 3 , 532783562U ) ; { mxArray *
slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" ) ;
char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1
, & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField
( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; }
else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "8.5" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( cpxksz1lxi ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( pcplow3pin ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
jqfqggqe3x ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetDefaultParam ( S , (
real_T * ) & cigmirzl4e ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; ( (
jqfqggqe3x * ) ssGetDefaultParam ( S ) ) -> P_12 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } } static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
