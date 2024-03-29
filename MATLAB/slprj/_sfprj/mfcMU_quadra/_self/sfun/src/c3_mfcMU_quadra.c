/* Include files */

#include <stddef.h>
#include "blas.h"
#include "mfcMU_quadra_sfun.h"
#include "c3_mfcMU_quadra.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "mfcMU_quadra_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c_with_debugger(S, sfGlobalDebugInstanceStruct);

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization);
static void chart_debug_initialize_data_addresses(SimStruct *S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c3_debug_family_names[14] = { "j1", "j2", "j3", "u1", "u2",
  "u3", "allo", "nargin", "nargout", "u", "J", "a", "uopt", "jopt" };

/* Function Declarations */
static void initialize_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void initialize_params_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void enable_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void disable_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void c3_update_debugger_state_c3_mfcMU_quadra
  (SFc3_mfcMU_quadraInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c3_mfcMU_quadra
  (SFc3_mfcMU_quadraInstanceStruct *chartInstance);
static void set_sim_state_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_st);
static void finalize_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void sf_gateway_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void mdl_start_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void initSimStructsc3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);
static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber, uint32_T c3_instanceNumber);
static const mxArray *c3_sf_marshallOut(void *chartInstanceVoid, void *c3_inData);
static real_T c3_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct *chartInstance,
  const mxArray *c3_b_jopt, const char_T *c3_identifier);
static real_T c3_b_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static const mxArray *c3_b_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static real_T c3_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a);
static void c3_eml_scalar_eg(SFc3_mfcMU_quadraInstanceStruct *chartInstance);
static void c3_dimagree(SFc3_mfcMU_quadraInstanceStruct *chartInstance);
static real_T c3_b_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a);
static real_T c3_c_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a);
static const mxArray *c3_c_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static int32_T c3_c_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static uint8_T c3_d_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_is_active_c3_mfcMU_quadra, const char_T
  *c3_identifier);
static uint8_T c3_e_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void init_dsm_address_info(SFc3_mfcMU_quadraInstanceStruct *chartInstance);
static void init_simulink_io_address(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  if (sf_is_first_init_cond(chartInstance->S)) {
    initSimStructsc3_mfcMU_quadra(chartInstance);
    chart_debug_initialize_data_addresses(chartInstance->S);
  }

  chartInstance->c3_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c3_is_active_c3_mfcMU_quadra = 0U;
}

static void initialize_params_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void enable_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c3_update_debugger_state_c3_mfcMU_quadra
  (SFc3_mfcMU_quadraInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c3_mfcMU_quadra
  (SFc3_mfcMU_quadraInstanceStruct *chartInstance)
{
  const mxArray *c3_st;
  const mxArray *c3_y = NULL;
  real_T c3_hoistedGlobal;
  real_T c3_b_u;
  const mxArray *c3_b_y = NULL;
  real_T c3_b_hoistedGlobal;
  real_T c3_c_u;
  const mxArray *c3_c_y = NULL;
  real_T c3_c_hoistedGlobal;
  real_T c3_d_u;
  const mxArray *c3_d_y = NULL;
  uint8_T c3_d_hoistedGlobal;
  uint8_T c3_e_u;
  const mxArray *c3_e_y = NULL;
  c3_st = NULL;
  c3_st = NULL;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_createcellmatrix(4, 1), false);
  c3_hoistedGlobal = *chartInstance->c3_a;
  c3_b_u = c3_hoistedGlobal;
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 0, c3_b_y);
  c3_b_hoistedGlobal = *chartInstance->c3_jopt;
  c3_c_u = c3_b_hoistedGlobal;
  c3_c_y = NULL;
  sf_mex_assign(&c3_c_y, sf_mex_create("y", &c3_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 1, c3_c_y);
  c3_c_hoistedGlobal = *chartInstance->c3_uopt;
  c3_d_u = c3_c_hoistedGlobal;
  c3_d_y = NULL;
  sf_mex_assign(&c3_d_y, sf_mex_create("y", &c3_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 2, c3_d_y);
  c3_d_hoistedGlobal = chartInstance->c3_is_active_c3_mfcMU_quadra;
  c3_e_u = c3_d_hoistedGlobal;
  c3_e_y = NULL;
  sf_mex_assign(&c3_e_y, sf_mex_create("y", &c3_e_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 3, c3_e_y);
  sf_mex_assign(&c3_st, c3_y, false);
  return c3_st;
}

static void set_sim_state_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_st)
{
  const mxArray *c3_b_u;
  chartInstance->c3_doneDoubleBufferReInit = true;
  c3_b_u = sf_mex_dup(c3_st);
  *chartInstance->c3_a = c3_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell("a", c3_b_u, 0)), "a");
  *chartInstance->c3_jopt = c3_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell("jopt", c3_b_u, 1)), "jopt");
  *chartInstance->c3_uopt = c3_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell("uopt", c3_b_u, 2)), "uopt");
  chartInstance->c3_is_active_c3_mfcMU_quadra = c3_d_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell("is_active_c3_mfcMU_quadra",
       c3_b_u, 3)), "is_active_c3_mfcMU_quadra");
  sf_mex_destroy(&c3_b_u);
  c3_update_debugger_state_c3_mfcMU_quadra(chartInstance);
  sf_mex_destroy(&c3_st);
}

static void finalize_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  int32_T c3_i0;
  int32_T c3_i1;
  int32_T c3_i2;
  real_T c3_b_u[3];
  int32_T c3_i3;
  real_T c3_b_J[3];
  uint32_T c3_debug_family_var_map[14];
  real_T c3_j1;
  real_T c3_j2;
  real_T c3_j3;
  real_T c3_u1;
  real_T c3_u2;
  real_T c3_u3;
  real_T c3_allo;
  real_T c3_nargin = 2.0;
  real_T c3_nargout = 3.0;
  real_T c3_b_a;
  real_T c3_b_uopt;
  real_T c3_b_jopt;
  real_T c3_A;
  real_T c3_B;
  real_T c3_x;
  real_T c3_y;
  real_T c3_b_x;
  real_T c3_b_y;
  real_T c3_c_x;
  real_T c3_c_y;
  real_T c3_b_A;
  real_T c3_b_B;
  real_T c3_d_x;
  real_T c3_d_y;
  real_T c3_e_x;
  real_T c3_e_y;
  real_T c3_f_x;
  real_T c3_f_y;
  real_T c3_c_A;
  real_T c3_c_B;
  real_T c3_g_x;
  real_T c3_g_y;
  real_T c3_h_x;
  real_T c3_h_y;
  real_T c3_i_x;
  real_T c3_i_y;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 2U, chartInstance->c3_sfEvent);
  for (c3_i0 = 0; c3_i0 < 3; c3_i0++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_J)[c3_i0], 1U, 1U, 0U,
                          chartInstance->c3_sfEvent, false);
  }

  for (c3_i1 = 0; c3_i1 < 3; c3_i1++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_u)[c3_i1], 0U, 1U, 0U,
                          chartInstance->c3_sfEvent, false);
  }

  chartInstance->c3_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 2U, chartInstance->c3_sfEvent);
  for (c3_i2 = 0; c3_i2 < 3; c3_i2++) {
    c3_b_u[c3_i2] = (*chartInstance->c3_u)[c3_i2];
  }

  for (c3_i3 = 0; c3_i3 < 3; c3_i3++) {
    c3_b_J[c3_i3] = (*chartInstance->c3_J)[c3_i3];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 14U, 14U, c3_debug_family_names,
    c3_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_j1, 0U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_j2, 1U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_j3, 2U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_u1, 3U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_u2, 4U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_u3, 5U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_allo, 6U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_nargin, 7U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_nargout, 8U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_u, 9U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_J, 10U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_a, 11U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_uopt, 12U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_jopt, 13U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 4);
  c3_j1 = c3_b_J[0];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 5);
  c3_j2 = c3_b_J[1];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 6);
  c3_j3 = c3_b_J[2];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 7);
  c3_u1 = c3_b_u[0];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 8);
  c3_u2 = c3_b_u[1];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 9);
  c3_u3 = c3_b_u[2];
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 11);
  c3_A = ((((c3_j1 * c3_u2 - c3_j2 * c3_u1) - c3_j1 * c3_u3) + c3_j3 * c3_u1) +
          c3_j2 * c3_u3) - c3_j3 * c3_u2;
  c3_B = (c3_u1 - c3_u2) * (((c3_u1 * c3_u2 - c3_u1 * c3_u3) - c3_u2 * c3_u3) +
    c3_mpower(chartInstance, c3_u3));
  c3_x = c3_A;
  c3_y = c3_B;
  c3_b_x = c3_x;
  c3_b_y = c3_y;
  c3_c_x = c3_b_x;
  c3_c_y = c3_b_y;
  c3_b_a = c3_c_x / c3_c_y;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 13);
  c3_b_A = ((((c3_j1 * c3_mpower(chartInstance, c3_u2) - c3_j2 * c3_mpower
               (chartInstance, c3_u1)) - c3_j1 * c3_mpower(chartInstance, c3_u3))
             + c3_j3 * c3_mpower(chartInstance, c3_u1)) + c3_j2 * c3_mpower
            (chartInstance, c3_u3)) - c3_j3 * c3_mpower(chartInstance, c3_u2);
  c3_b_B = 2.0 * (((((c3_j1 * c3_u2 - c3_j2 * c3_u1) - c3_j1 * c3_u3) + c3_j3 *
                    c3_u1) + c3_j2 * c3_u3) - c3_j3 * c3_u2);
  c3_d_x = c3_b_A;
  c3_d_y = c3_b_B;
  c3_e_x = c3_d_x;
  c3_e_y = c3_d_y;
  c3_f_x = c3_e_x;
  c3_f_y = c3_e_y;
  c3_b_uopt = c3_f_x / c3_f_y;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 15);
  c3_c_A = -(((((((((((((((((((((((((((((((((((((((((c3_mpower(chartInstance,
    c3_j1) * c3_b_mpower(chartInstance, c3_u2) - 4.0 * c3_mpower(chartInstance,
    c3_j1) * c3_c_mpower(chartInstance, c3_u2) * c3_u3) + 6.0 * c3_mpower
    (chartInstance, c3_j1) * c3_mpower(chartInstance, c3_u2) * c3_mpower
    (chartInstance, c3_u3)) - 4.0 * c3_mpower(chartInstance, c3_j1) * c3_u2 *
    c3_c_mpower(chartInstance, c3_u3)) + c3_mpower(chartInstance, c3_j1) *
    c3_b_mpower(chartInstance, c3_u3)) - 2.0 * c3_j1 * c3_j2 * c3_mpower
    (chartInstance, c3_u1) * c3_mpower(chartInstance, c3_u2)) + 4.0 * c3_j1 *
    c3_j2 * c3_mpower(chartInstance, c3_u1) * c3_u2 * c3_u3) - 2.0 * c3_j1 *
    c3_j2 * c3_mpower(chartInstance, c3_u1) * c3_mpower(chartInstance, c3_u3)) +
    4.0 * c3_j1 * c3_j2 * c3_u1 * c3_mpower(chartInstance, c3_u2) * c3_u3) - 8.0
    * c3_j1 * c3_j2 * c3_u1 * c3_u2 * c3_mpower(chartInstance, c3_u3)) + 4.0 *
    c3_j1 * c3_j2 * c3_u1 * c3_c_mpower(chartInstance, c3_u3)) - 2.0 * c3_j1 *
    c3_j2 * c3_mpower(chartInstance, c3_u2) * c3_mpower(chartInstance, c3_u3)) +
    4.0 * c3_j1 * c3_j2 * c3_u2 * c3_c_mpower(chartInstance, c3_u3)) - 2.0 *
    c3_j1 * c3_j2 * c3_b_mpower(chartInstance, c3_u3)) - 2.0 * c3_j1 * c3_j3 *
    c3_mpower(chartInstance, c3_u1) * c3_mpower(chartInstance, c3_u2)) + 4.0 *
    c3_j1 * c3_j3 * c3_mpower(chartInstance, c3_u1) * c3_u2 * c3_u3) - 2.0 *
    c3_j1 * c3_j3 * c3_mpower(chartInstance, c3_u1) * c3_mpower(chartInstance,
    c3_u3)) + 4.0 * c3_j1 * c3_j3 * c3_u1 * c3_c_mpower(chartInstance, c3_u2)) -
    8.0 * c3_j1 * c3_j3 * c3_u1 * c3_mpower(chartInstance, c3_u2) * c3_u3) + 4.0
    * c3_j1 * c3_j3 * c3_u1 * c3_u2 * c3_mpower(chartInstance, c3_u3)) - 2.0 *
    c3_j1 * c3_j3 * c3_b_mpower(chartInstance, c3_u2)) + 4.0 * c3_j1 * c3_j3 *
    c3_c_mpower(chartInstance, c3_u2) * c3_u3) - 2.0 * c3_j1 * c3_j3 * c3_mpower
    (chartInstance, c3_u2) * c3_mpower(chartInstance, c3_u3)) + c3_mpower
    (chartInstance, c3_j2) * c3_b_mpower(chartInstance, c3_u1)) - 4.0 *
    c3_mpower(chartInstance, c3_j2) * c3_c_mpower(chartInstance, c3_u1) * c3_u3)
    + 6.0 * c3_mpower(chartInstance, c3_j2) * c3_mpower(chartInstance, c3_u1) *
    c3_mpower(chartInstance, c3_u3)) - 4.0 * c3_mpower(chartInstance, c3_j2) *
    c3_u1 * c3_c_mpower(chartInstance, c3_u3)) + c3_mpower(chartInstance, c3_j2)
    * c3_b_mpower(chartInstance, c3_u3)) - 2.0 * c3_j2 * c3_j3 * c3_b_mpower
    (chartInstance, c3_u1)) + 4.0 * c3_j2 * c3_j3 * c3_c_mpower(chartInstance,
    c3_u1) * c3_u2) + 4.0 * c3_j2 * c3_j3 * c3_c_mpower(chartInstance, c3_u1) *
                        c3_u3) - 2.0 * c3_j2 * c3_j3 * c3_mpower(chartInstance,
    c3_u1) * c3_mpower(chartInstance, c3_u2)) - 8.0 * c3_j2 * c3_j3 * c3_mpower
                      (chartInstance, c3_u1) * c3_u2 * c3_u3) - 2.0 * c3_j2 *
                     c3_j3 * c3_mpower(chartInstance, c3_u1) * c3_mpower
                     (chartInstance, c3_u3)) + 4.0 * c3_j2 * c3_j3 * c3_u1 *
                    c3_mpower(chartInstance, c3_u2) * c3_u3) + 4.0 * c3_j2 *
                   c3_j3 * c3_u1 * c3_u2 * c3_mpower(chartInstance, c3_u3)) -
                  2.0 * c3_j2 * c3_j3 * c3_mpower(chartInstance, c3_u2) *
                  c3_mpower(chartInstance, c3_u3)) + c3_mpower(chartInstance,
    c3_j3) * c3_b_mpower(chartInstance, c3_u1)) - 4.0 * c3_mpower(chartInstance,
    c3_j3) * c3_c_mpower(chartInstance, c3_u1) * c3_u2) + 6.0 * c3_mpower
               (chartInstance, c3_j3) * c3_mpower(chartInstance, c3_u1) *
               c3_mpower(chartInstance, c3_u2)) - 4.0 * c3_mpower(chartInstance,
    c3_j3) * c3_u1 * c3_c_mpower(chartInstance, c3_u2)) + c3_mpower
             (chartInstance, c3_j3) * c3_b_mpower(chartInstance, c3_u2));
  c3_c_B = 4.0 * (c3_u1 - c3_u2) * (((c3_u1 * c3_u2 - c3_u1 * c3_u3) - c3_u2 *
    c3_u3) + c3_mpower(chartInstance, c3_u3)) * (((((c3_j1 * c3_u2 - c3_j2 *
    c3_u1) - c3_j1 * c3_u3) + c3_j3 * c3_u1) + c3_j2 * c3_u3) - c3_j3 * c3_u2);
  c3_g_x = c3_c_A;
  c3_g_y = c3_c_B;
  c3_h_x = c3_g_x;
  c3_h_y = c3_g_y;
  c3_i_x = c3_h_x;
  c3_i_y = c3_h_y;
  c3_b_jopt = c3_i_x / c3_i_y;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 17);
  c3_allo = 1.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, -17);
  _SFD_SYMBOL_SCOPE_POP();
  *chartInstance->c3_a = c3_b_a;
  *chartInstance->c3_uopt = c3_b_uopt;
  *chartInstance->c3_jopt = c3_b_jopt;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 2U, chartInstance->c3_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_mfcMU_quadraMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_a, 2U, 1U, 0U,
                        chartInstance->c3_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_uopt, 3U, 1U, 0U,
                        chartInstance->c3_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_jopt, 4U, 1U, 0U,
                        chartInstance->c3_sfEvent, false);
}

static void mdl_start_c3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void initSimStructsc3_mfcMU_quadra(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber, uint32_T c3_instanceNumber)
{
  (void)c3_machineNumber;
  (void)c3_chartNumber;
  (void)c3_instanceNumber;
}

static const mxArray *c3_sf_marshallOut(void *chartInstanceVoid, void *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  real_T c3_b_u;
  const mxArray *c3_y = NULL;
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_b_u = *(real_T *)c3_inData;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static real_T c3_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct *chartInstance,
  const mxArray *c3_b_jopt, const char_T *c3_identifier)
{
  real_T c3_y;
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_jopt), &c3_thisId);
  sf_mex_destroy(&c3_b_jopt);
  return c3_y;
}

static real_T c3_b_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  real_T c3_y;
  real_T c3_d0;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_d0, 1, 0, 0U, 0, 0U, 0);
  c3_y = c3_d0;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_b_jopt;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y;
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)chartInstanceVoid;
  c3_b_jopt = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_jopt), &c3_thisId);
  sf_mex_destroy(&c3_b_jopt);
  *(real_T *)c3_outData = c3_y;
  sf_mex_destroy(&c3_mxArrayInData);
}

static const mxArray *c3_b_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i4;
  real_T c3_b_u[3];
  const mxArray *c3_y = NULL;
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i4 = 0; c3_i4 < 3; c3_i4++) {
    c3_b_u[c3_i4] = (*(real_T (*)[3])c3_inData)[c3_i4];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 3, 1), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

const mxArray *sf_c3_mfcMU_quadra_get_eml_resolved_functions_info(void)
{
  const mxArray *c3_nameCaptureInfo = NULL;
  c3_nameCaptureInfo = NULL;
  sf_mex_assign(&c3_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c3_nameCaptureInfo;
}

static real_T c3_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a)
{
  real_T c3_c_a;
  real_T c3_d_a;
  real_T c3_ak;
  real_T c3_e_a;
  c3_c_a = c3_b_a;
  c3_d_a = c3_c_a;
  c3_eml_scalar_eg(chartInstance);
  c3_dimagree(chartInstance);
  c3_ak = c3_d_a;
  c3_e_a = c3_ak;
  c3_eml_scalar_eg(chartInstance);
  return c3_e_a * c3_e_a;
}

static void c3_eml_scalar_eg(SFc3_mfcMU_quadraInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c3_dimagree(SFc3_mfcMU_quadraInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static real_T c3_b_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a)
{
  real_T c3_c_a;
  real_T c3_d_a;
  real_T c3_ak;
  real_T c3_e_a;
  real_T c3_ar;
  c3_c_a = c3_b_a;
  c3_d_a = c3_c_a;
  c3_eml_scalar_eg(chartInstance);
  c3_dimagree(chartInstance);
  c3_ak = c3_d_a;
  c3_e_a = c3_ak;
  c3_eml_scalar_eg(chartInstance);
  c3_ar = c3_e_a;
  return muDoubleScalarPower(c3_ar, 4.0);
}

static real_T c3_c_mpower(SFc3_mfcMU_quadraInstanceStruct *chartInstance, real_T
  c3_b_a)
{
  real_T c3_c_a;
  real_T c3_d_a;
  real_T c3_ak;
  real_T c3_e_a;
  real_T c3_ar;
  c3_c_a = c3_b_a;
  c3_d_a = c3_c_a;
  c3_eml_scalar_eg(chartInstance);
  c3_dimagree(chartInstance);
  c3_ak = c3_d_a;
  c3_e_a = c3_ak;
  c3_eml_scalar_eg(chartInstance);
  c3_ar = c3_e_a;
  return muDoubleScalarPower(c3_ar, 3.0);
}

static const mxArray *c3_c_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_b_u;
  const mxArray *c3_y = NULL;
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_b_u = *(int32_T *)c3_inData;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static int32_T c3_c_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  int32_T c3_y;
  int32_T c3_i5;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_i5, 1, 6, 0U, 0, 0U, 0);
  c3_y = c3_i5;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_b_sfEvent;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  int32_T c3_y;
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)chartInstanceVoid;
  c3_b_sfEvent = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_sfEvent),
    &c3_thisId);
  sf_mex_destroy(&c3_b_sfEvent);
  *(int32_T *)c3_outData = c3_y;
  sf_mex_destroy(&c3_mxArrayInData);
}

static uint8_T c3_d_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_is_active_c3_mfcMU_quadra, const char_T
  *c3_identifier)
{
  uint8_T c3_y;
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_e_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c3_b_is_active_c3_mfcMU_quadra), &c3_thisId);
  sf_mex_destroy(&c3_b_is_active_c3_mfcMU_quadra);
  return c3_y;
}

static uint8_T c3_e_emlrt_marshallIn(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance, const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  uint8_T c3_y;
  uint8_T c3_u0;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_u0, 1, 3, 0U, 0, 0U, 0);
  c3_y = c3_u0;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void init_dsm_address_info(SFc3_mfcMU_quadraInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc3_mfcMU_quadraInstanceStruct
  *chartInstance)
{
  chartInstance->c3_u = (real_T (*)[3])ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c3_a = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    1);
  chartInstance->c3_J = (real_T (*)[3])ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c3_uopt = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c3_jopt = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 3);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c3_mfcMU_quadra_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2092500481U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(892260881U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(4266603318U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(725154742U);
}

mxArray* sf_c3_mfcMU_quadra_get_post_codegen_info(void);
mxArray *sf_c3_mfcMU_quadra_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("WqR1RDt4rXlIUeO8kz90qH");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,3,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo = sf_c3_mfcMU_quadra_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c3_mfcMU_quadra_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c3_mfcMU_quadra_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("pre");
  mxArray *fallbackReason = mxCreateString("hasBreakpoints");
  mxArray *hiddenFallbackType = mxCreateString("none");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c3_mfcMU_quadra_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c3_mfcMU_quadra_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c3_mfcMU_quadra(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x4'type','srcId','name','auxInfo'{{M[1],M[5],T\"a\",},{M[1],M[8],T\"jopt\",},{M[1],M[7],T\"uopt\",},{M[8],M[0],T\"is_active_c3_mfcMU_quadra\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 4, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c3_mfcMU_quadra_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc3_mfcMU_quadraInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc3_mfcMU_quadraInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _mfcMU_quadraMachineNumber_,
           3,
           1,
           1,
           0,
           5,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_mfcMU_quadraMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_mfcMU_quadraMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _mfcMU_quadraMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u");
          _SFD_SET_DATA_PROPS(1,1,1,0,"J");
          _SFD_SET_DATA_PROPS(2,2,0,1,"a");
          _SFD_SET_DATA_PROPS(3,2,0,1,"uopt");
          _SFD_SET_DATA_PROPS(4,2,0,1,"jopt");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1460);

        {
          unsigned int dimVector[2];
          dimVector[0]= 3;
          dimVector[1]= 1;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 3;
          dimVector[1]= 1;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_sf_marshallOut,(MexInFcnForType)c3_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_sf_marshallOut,(MexInFcnForType)c3_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_sf_marshallOut,(MexInFcnForType)c3_sf_marshallIn);
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _mfcMU_quadraMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static void chart_debug_initialize_data_addresses(SimStruct *S)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc3_mfcMU_quadraInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc3_mfcMU_quadraInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S)) {
      /* do this only if simulation is starting and after we know the addresses of all data */
      {
        _SFD_SET_DATA_VALUE_PTR(0U, *chartInstance->c3_u);
        _SFD_SET_DATA_VALUE_PTR(2U, chartInstance->c3_a);
        _SFD_SET_DATA_VALUE_PTR(1U, *chartInstance->c3_J);
        _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c3_uopt);
        _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c3_jopt);
      }
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "s3lY14P3tGvl85WtopIkwBE";
}

static void sf_opaque_initialize_c3_mfcMU_quadra(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc3_mfcMU_quadraInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*)
    chartInstanceVar);
  initialize_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c3_mfcMU_quadra(void *chartInstanceVar)
{
  enable_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c3_mfcMU_quadra(void *chartInstanceVar)
{
  disable_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c3_mfcMU_quadra(void *chartInstanceVar)
{
  sf_gateway_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c3_mfcMU_quadra(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  return get_sim_state_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
}

static void sf_opaque_set_sim_state_c3_mfcMU_quadra(SimStruct* S, const mxArray *
  st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  set_sim_state_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*)
    chartInfo->chartInstance, st);
}

static void sf_opaque_terminate_c3_mfcMU_quadra(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_mfcMU_quadra_optimization_info();
    }

    finalize_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*) chartInstanceVar);
    utFree(chartInstanceVar);
    if (crtInfo != NULL) {
      utFree(crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c3_mfcMU_quadra(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c3_mfcMU_quadra((SFc3_mfcMU_quadraInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c3_mfcMU_quadra(SimStruct *S)
{
  ssMdlUpdateIsEmpty(S, 1);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_mfcMU_quadra_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,3);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,1);
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,3,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,3);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,3,2);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,3,3);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=3; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 2; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,3);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(3999813900U));
  ssSetChecksum1(S,(2129816247U));
  ssSetChecksum2(S,(3568157615U));
  ssSetChecksum3(S,(2883508823U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c3_mfcMU_quadra(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c3_mfcMU_quadra(SimStruct *S)
{
  SFc3_mfcMU_quadraInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc3_mfcMU_quadraInstanceStruct *)utMalloc(sizeof
    (SFc3_mfcMU_quadraInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc3_mfcMU_quadraInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c3_mfcMU_quadra;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c3_mfcMU_quadra;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c3_mfcMU_quadra;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c3_mfcMU_quadra;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c3_mfcMU_quadra;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c3_mfcMU_quadra;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c3_mfcMU_quadra;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c3_mfcMU_quadra;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c3_mfcMU_quadra;
  chartInstance->chartInfo.mdlStart = mdlStart_c3_mfcMU_quadra;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c3_mfcMU_quadra;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->isEnhancedMooreMachine = 0;
  crtInfo->checksum = SF_RUNTIME_INFO_CHECKSUM;
  crtInfo->fCheckOverflow = sf_runtime_overflow_check_is_on(S);
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  crtInfo->compiledInfo = NULL;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  chart_debug_initialization(S,1);
}

void c3_mfcMU_quadra_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c3_mfcMU_quadra(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c3_mfcMU_quadra(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c3_mfcMU_quadra(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c3_mfcMU_quadra_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
