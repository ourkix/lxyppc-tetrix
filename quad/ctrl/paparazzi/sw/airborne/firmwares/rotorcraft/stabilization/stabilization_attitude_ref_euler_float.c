#include "firmwares/rotorcraft/stabilization.h"


struct FloatEulers stab_att_sp_euler;
struct FloatEulers stab_att_ref_euler;
struct FloatRates  stab_att_ref_rate;
struct FloatRates  stab_att_ref_accel;

void stabilization_attitude_ref_init(void) {

  FLOAT_EULERS_ZERO(stab_att_sp_euler);
  FLOAT_EULERS_ZERO(stab_att_ref_euler);
  FLOAT_RATES_ZERO(stab_att_ref_rate);
  FLOAT_RATES_ZERO(stab_att_ref_accel);

}


/*
 * Reference
 */
#define DT_UPDATE (1./512.)

#define REF_ACCEL_MAX_P STABILIZATION_ATTITUDE_FLOAT_REF_MAX_PDOT
#define REF_ACCEL_MAX_Q STABILIZATION_ATTITUDE_FLOAT_REF_MAX_QDOT
#define REF_ACCEL_MAX_R STABILIZATION_ATTITUDE_FLOAT_REF_MAX_RDOT

#define REF_RATE_MAX_P STABILIZATION_ATTITUDE_FLOAT_REF_MAX_P
#define REF_RATE_MAX_Q STABILIZATION_ATTITUDE_FLOAT_REF_MAX_Q
#define REF_RATE_MAX_R STABILIZATION_ATTITUDE_FLOAT_REF_MAX_R

#define OMEGA_P   STABILIZATION_ATTITUDE_FLOAT_REF_OMEGA_P
#define OMEGA_Q   STABILIZATION_ATTITUDE_FLOAT_REF_OMEGA_Q
#define OMEGA_R   STABILIZATION_ATTITUDE_FLOAT_REF_OMEGA_R

#define ZETA_P    STABILIZATION_ATTITUDE_FLOAT_REF_ZETA_P
#define ZETA_Q    STABILIZATION_ATTITUDE_FLOAT_REF_ZETA_Q
#define ZETA_R    STABILIZATION_ATTITUDE_FLOAT_REF_ZETA_R


#define USE_REF 1

void stabilization_attitude_ref_update() {

#if USE_REF

  /* dumb integrate reference attitude        */
  struct FloatRates delta_rate;
  RATES_SMUL(delta_rate, stab_att_ref_rate, DT_UPDATE);
  struct FloatEulers delta_angle;
  EULERS_ASSIGN(delta_angle, delta_rate.p, delta_rate.q, delta_rate.r);
  EULERS_ADD(stab_att_ref_euler, delta_angle );
  FLOAT_ANGLE_NORMALIZE(stab_att_ref_euler.psi);

  /* integrate reference rotational speeds   */
  struct FloatRates delta_accel;
  RATES_SMUL(delta_accel, stab_att_ref_accel, DT_UPDATE);
  RATES_ADD(stab_att_ref_rate, delta_accel);

  /* compute reference attitude error        */
  struct FloatEulers ref_err;
  EULERS_DIFF(ref_err, stab_att_ref_euler, stab_att_sp_euler);
  /* wrap it in the shortest direction       */
  FLOAT_ANGLE_NORMALIZE(ref_err.psi);

  /* compute reference angular accelerations */
  stab_att_ref_accel.p = -2.*ZETA_P*OMEGA_P*stab_att_ref_rate.p - OMEGA_P*OMEGA_P*ref_err.phi;
  stab_att_ref_accel.q = -2.*ZETA_Q*OMEGA_P*stab_att_ref_rate.q - OMEGA_Q*OMEGA_Q*ref_err.theta;
  stab_att_ref_accel.r = -2.*ZETA_R*OMEGA_P*stab_att_ref_rate.r - OMEGA_R*OMEGA_R*ref_err.psi;

  /*	saturate acceleration */
  const struct Int32Rates MIN_ACCEL = { -REF_ACCEL_MAX_P, -REF_ACCEL_MAX_Q, -REF_ACCEL_MAX_R };
  const struct Int32Rates MAX_ACCEL = {  REF_ACCEL_MAX_P,  REF_ACCEL_MAX_Q,  REF_ACCEL_MAX_R }; \
  RATES_BOUND_BOX(stab_att_ref_accel, MIN_ACCEL, MAX_ACCEL);

  /* saturate speed and trim accel accordingly */
  SATURATE_SPEED_TRIM_ACCEL();

#else   /* !USE_REF */
  EULERS_COPY(stab_att_ref_euler, stabilization_att_sp);
  FLOAT_RATES_ZERO(stab_att_ref_rate);
  FLOAT_RATES_ZERO(stab_att_ref_accel);
#endif /* USE_REF */

}
