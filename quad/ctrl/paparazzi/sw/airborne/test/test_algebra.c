#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "std.h"

#include "math/pprz_algebra_float.h"
#include "math/pprz_algebra_double.h"
#include "math/pprz_algebra_int.h"
#include "pprz_algebra_print.h"

static void test_1(void);
static void test_2(void);
static void test_3(void);

/* euler to quat to euler */
static void test_4_int(void);
static void test_4_float(void);
static void test_5(void);
static void test_6(void);
static void test_7(void);
static void test_8(void);
static void test_9(void);
static void test_10(void);

void test1234(void);


float test_eulers_of_quat(struct FloatQuat fq, int display);
float test_eulers_of_rmat(struct FloatRMat frm, int display);
float test_rmat_comp(struct FloatRMat ma2b_f, struct FloatRMat mb2c_f, int display);
float test_quat_comp(struct FloatQuat qa2b_f, struct FloatQuat qb2c_f, int display);
float test_rmat_comp_inv(struct FloatRMat ma2c_f, struct FloatRMat mb2c_f, int display);
float test_quat_comp_inv(struct FloatQuat qa2c_f, struct FloatQuat qb2c_f, int display);
float test_quat_of_rmat(void);
float test_rmat_of_eulers_312(void);

float test_quat(void);
float test_quat2(void);

float test_INT32_QUAT_OF_RMAT(struct FloatEulers* eul, bool_t display);

void test_of_axis_angle(void);

int main(int argc, char** argv) {

  //  test_1();
  //  test_2();
  // test_3();
  printf("\n");
  //test_4_int();
  printf("\n");
  //  test_4_float();
  printf("\n");

  //  test_5();
  //test_6();
  //test_7();
  //  test_of_axis_angle();
  //test1234();
  //  test_quat_of_rmat();
  printf("\n");
  //  test_rmat_of_eulers_312();
  //  test_quat2();
  test_8();
  //test_9();
  test_10();
  return 0;

}

static void test_1(void) {

  struct FloatEulers euler_f = { RadOfDeg(45.), RadOfDeg(0.), RadOfDeg(0.)};
  DISPLAY_FLOAT_EULERS("euler_f", euler_f);

  struct Int32Eulers euler_i;
  EULERS_BFP_OF_REAL(euler_i, euler_f);
  DISPLAY_INT32_EULERS("euler_i", euler_i);

  struct FloatQuat quat_f;
  FLOAT_QUAT_OF_EULERS(quat_f, euler_f);
  DISPLAY_FLOAT_QUAT("quat_f", quat_f);

  struct Int32Quat quat_i;
  INT32_QUAT_OF_EULERS(quat_i, euler_i);
  DISPLAY_INT32_QUAT("quat_i", quat_i);

  struct Int32RMat rmat_i;
  INT32_RMAT_OF_QUAT(rmat_i, quat_i);
  DISPLAY_INT32_RMAT("rmat_i", rmat_i);

}


static void test_2(void) {

  struct Int32Vect3 v1 = { 5000, 5000, 5000 };
  DISPLAY_INT32_VECT3("v1", v1);

  struct FloatEulers euler_f = { RadOfDeg(45.), RadOfDeg(0.), RadOfDeg(0.)};
  DISPLAY_FLOAT_EULERS("euler_f", euler_f);

  struct Int32Eulers euler_i;
  EULERS_BFP_OF_REAL(euler_i, euler_f);
  DISPLAY_INT32_EULERS("euler_i", euler_i);

  struct Int32Quat quat_i;
  INT32_QUAT_OF_EULERS(quat_i, euler_i);
  DISPLAY_INT32_QUAT("quat_i", quat_i);
  INT32_QUAT_NORMALIZE(quat_i);
  DISPLAY_INT32_QUAT("quat_i_n", quat_i);

  struct Int32Vect3 v2;
  INT32_QUAT_VMULT(v2, quat_i, v1);
  DISPLAY_INT32_VECT3("v2", v2);

  struct Int32RMat rmat_i;
  INT32_RMAT_OF_QUAT(rmat_i, quat_i);
  DISPLAY_INT32_RMAT("rmat_i", rmat_i);

  struct Int32Vect3 v3;
  INT32_RMAT_VMULT(v3, rmat_i, v1);
  DISPLAY_INT32_VECT3("v3", v3);

  struct Int32RMat rmat_i2;
  INT32_RMAT_OF_EULERS(rmat_i2, euler_i);
  DISPLAY_INT32_RMAT("rmat_i2", rmat_i2);

  struct Int32Vect3 v4;
  INT32_RMAT_VMULT(v4, rmat_i2, v1);
  DISPLAY_INT32_VECT3("v4", v4);

  struct FloatQuat quat_f;
  FLOAT_QUAT_OF_EULERS(quat_f, euler_f);
  DISPLAY_FLOAT_QUAT("quat_f", quat_f);

  struct FloatVect3 v5;
  VECT3_COPY(v5, v1);
  DISPLAY_FLOAT_VECT3("v5", v5);
  struct FloatVect3 v6;
  FLOAT_QUAT_VMULT(v6, quat_f, v5);
  DISPLAY_FLOAT_VECT3("v6", v6);

}

static void test_3(void) {

  /* Compute BODY to IMU eulers */
  struct Int32Eulers b2i_e;
  EULERS_ASSIGN(b2i_e, ANGLE_BFP_OF_REAL(RadOfDeg(10.66)), ANGLE_BFP_OF_REAL(RadOfDeg(-0.7)), ANGLE_BFP_OF_REAL(RadOfDeg(0.)));
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("b2i_e", b2i_e);

  /* Compute BODY to IMU quaternion */
  struct Int32Quat b2i_q;
  INT32_QUAT_OF_EULERS(b2i_q, b2i_e);
  DISPLAY_INT32_QUAT_AS_EULERS_DEG("b2i_q", b2i_q);
  //  INT32_QUAT_NORMALIZE(b2i_q);
  //  DISPLAY_INT32_QUAT_AS_EULERS_DEG("b2i_q_n", b2i_q);

  /* Compute BODY to IMU rotation matrix */
  struct Int32RMat b2i_r;
  INT32_RMAT_OF_EULERS(b2i_r, b2i_e);
  //  DISPLAY_INT32_RMAT("b2i_r", b2i_r);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("b2i_r", b2i_r);

  /* Compute LTP to IMU eulers */
  struct Int32Eulers l2i_e;
  EULERS_ASSIGN(l2i_e, ANGLE_BFP_OF_REAL(RadOfDeg(0.)), ANGLE_BFP_OF_REAL(RadOfDeg(20.)), ANGLE_BFP_OF_REAL(RadOfDeg(0.)));
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("l2i_e", l2i_e);

  /* Compute LTP to IMU quaternion */
  struct Int32Quat l2i_q;
  INT32_QUAT_OF_EULERS(l2i_q, l2i_e);
  DISPLAY_INT32_QUAT_AS_EULERS_DEG("l2i_q", l2i_q);

  /* Compute LTP to IMU rotation matrix */
  struct Int32RMat l2i_r;
  INT32_RMAT_OF_EULERS(l2i_r, l2i_e);
  //  DISPLAY_INT32_RMAT("l2i_r", l2i_r);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("l2i_r", l2i_r);


  /* again but from quaternion */
  struct Int32RMat l2i_r2;
  INT32_RMAT_OF_QUAT(l2i_r2, l2i_q);
  //  DISPLAY_INT32_RMAT("l2i_r2", l2i_r2);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("l2i_r2", l2i_r2);

  /* Compute LTP to BODY quaternion */
  struct Int32Quat l2b_q;
  INT32_QUAT_COMP_INV(l2b_q, b2i_q, l2i_q);
  DISPLAY_INT32_QUAT_AS_EULERS_DEG("l2b_q", l2b_q);

  /* Compute LTP to BODY rotation matrix */
  struct Int32RMat l2b_r;
  INT32_RMAT_COMP_INV(l2b_r, l2i_r, b2i_r);
  //  DISPLAY_INT32_RMAT("l2b_r", l2b_r);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("l2b_r2", l2b_r);

 /* again but from quaternion */
  struct Int32RMat l2b_r2;
  INT32_RMAT_OF_QUAT(l2b_r2, l2b_q);
  //  DISPLAY_INT32_RMAT("l2b_r2", l2b_r2);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("l2b_r2", l2b_r2);


  /* compute LTP to BODY eulers */
  struct Int32Eulers l2b_e;
  INT32_EULERS_OF_RMAT(l2b_e, l2b_r);
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("l2b_e", l2b_e);

  /* again but from quaternion */
  struct Int32Eulers l2b_e2;
  INT32_EULERS_OF_QUAT(l2b_e2, l2b_q);
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("l2b_e2", l2b_e2);

}




static void test_4_int(void) {

  printf("euler to quat to euler - int\n");
  /* initial euler angles */
  struct Int32Eulers _e;
  EULERS_ASSIGN(_e, ANGLE_BFP_OF_REAL(RadOfDeg(-10.66)), ANGLE_BFP_OF_REAL(RadOfDeg(-0.7)), ANGLE_BFP_OF_REAL(RadOfDeg(0.)));
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("euler orig ", _e);

  /* transform to quaternion */
  struct Int32Quat _q;
  INT32_QUAT_OF_EULERS(_q, _e);
  DISPLAY_INT32_QUAT_AS_EULERS_DEG("quat1 ", _q);
  //  INT32_QUAT_NORMALIZE(_q);
  //  DISPLAY_INT32_QUAT_2("_q_n", _q);

  /* back to eulers */
  struct Int32Eulers _e2;
  INT32_EULERS_OF_QUAT(_e2, _q);
  DISPLAY_INT32_EULERS_AS_FLOAT_DEG("back to euler ", _e2);


}



static void test_4_float(void) {

  printf("euler to quat to euler - float\n");
  /* initial euler angles */
  struct FloatEulers e;
  EULERS_ASSIGN(e, RadOfDeg(-10.66), RadOfDeg(-0.7), RadOfDeg(0.));
  DISPLAY_FLOAT_EULERS_DEG("e", e);

  /* transform to quaternion */
  struct FloatQuat q;
  FLOAT_QUAT_OF_EULERS(q, e);
  //  DISPLAY_FLOAT_QUAT("q", q);
  FLOAT_QUAT_NORMALIZE(q);
  DISPLAY_FLOAT_QUAT("q_n", q);
  DISPLAY_FLOAT_QUAT_AS_INT("q_n as int", q);
  /* back to eulers */
  struct FloatEulers e2;
  FLOAT_EULERS_OF_QUAT(e2, q);
  DISPLAY_FLOAT_EULERS_DEG("e2", e2);


}

static void test_5(void) {

  struct FloatEulers fe;
  EULERS_ASSIGN(fe, RadOfDeg(-10.66), RadOfDeg(-0.7), RadOfDeg(0.));
  DISPLAY_FLOAT_EULERS_DEG("fe", fe);
  printf("\n");
  struct FloatQuat fq;
  FLOAT_QUAT_OF_EULERS(fq, fe);
  test_eulers_of_quat(fq, 1);
  printf("\n");
  struct FloatRMat frm;
  FLOAT_RMAT_OF_EULERS(frm, fe);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("frm", frm);
  test_eulers_of_rmat(frm, 1);
  printf("\n");

}


float test_eulers_of_quat(struct FloatQuat fq, int display) {

  struct FloatEulers fe;
  FLOAT_EULERS_OF_QUAT(fe, fq);
  struct Int32Quat iq;
  QUAT_BFP_OF_REAL(iq, fq);
  struct Int32Eulers ie;
  INT32_EULERS_OF_QUAT(ie, iq);
  struct FloatEulers fe2;
  EULERS_FLOAT_OF_BFP(fe2, ie);
  EULERS_SUB(fe2, ie);
  float norm_err = FLOAT_EULERS_NORM(fe2);

  if (display) {
    printf("euler of quat\n");
    DISPLAY_FLOAT_QUAT("fq", fq);
    DISPLAY_FLOAT_EULERS_DEG("fe", fe);
    DISPLAY_INT32_EULERS("ie", ie);
    DISPLAY_INT32_EULERS_AS_FLOAT_DEG("ieaf", ie);
  }

  return norm_err;
}


float test_eulers_of_rmat(struct FloatRMat frm, int display) {

  struct FloatEulers fe;
  FLOAT_EULERS_OF_RMAT(fe, frm);
  struct Int32RMat irm;
  RMAT_BFP_OF_REAL(irm, frm);
  struct Int32Eulers ie;
  INT32_EULERS_OF_RMAT(ie, irm);
  struct FloatEulers fe2;
  EULERS_FLOAT_OF_BFP(fe2, ie);
  EULERS_SUB(fe2, ie);
  float norm_err = FLOAT_EULERS_NORM(fe2);

  if (display) {
    printf("euler of rmat\n");
    //    DISPLAY_FLOAT_RMAT("fr", fr);
    DISPLAY_FLOAT_EULERS_DEG("fe", fe);
    DISPLAY_INT32_EULERS("ie", ie);
    DISPLAY_INT32_EULERS_AS_FLOAT_DEG("ieaf", ie);
  }

  return norm_err;

}




static void test_6(void) {

  printf("\n");
  struct FloatEulers ea2b;
  EULERS_ASSIGN(ea2b, RadOfDeg(45.), RadOfDeg(22.), RadOfDeg(0.));
  DISPLAY_FLOAT_EULERS_DEG("ea2b", ea2b);
  struct FloatEulers eb2c;
  EULERS_ASSIGN(eb2c, RadOfDeg(0.), RadOfDeg(0.), RadOfDeg(90.));
  DISPLAY_FLOAT_EULERS_DEG("eb2c", eb2c);
  struct FloatRMat fa2b;
  FLOAT_RMAT_OF_EULERS(fa2b, ea2b);
  struct FloatRMat fb2c;
  FLOAT_RMAT_OF_EULERS(fb2c, eb2c);

  printf("\n");
  test_rmat_comp(fa2b, fb2c, 1);

  struct FloatQuat qa2b;
  FLOAT_QUAT_OF_EULERS(qa2b, ea2b);
  struct FloatQuat qb2c;
  FLOAT_QUAT_OF_EULERS(qb2c, eb2c);

  printf("\n");
  test_quat_comp(qa2b, qb2c, 1);
  printf("\n");

}



float test_rmat_comp(struct FloatRMat ma2b_f, struct FloatRMat mb2c_f, int display) {

  struct FloatRMat ma2c_f;
  FLOAT_RMAT_COMP(ma2c_f, ma2b_f, mb2c_f);
  struct Int32RMat ma2b_i;
  RMAT_BFP_OF_REAL(ma2b_i, ma2b_f);
  struct Int32RMat mb2c_i;
  RMAT_BFP_OF_REAL(mb2c_i, mb2c_f);
  struct Int32RMat ma2c_i;
  INT32_RMAT_COMP(ma2c_i, ma2b_i, mb2c_i);

  struct FloatRMat err;
  RMAT_DIFF(err, ma2c_f, ma2c_i);
  float norm_err = FLOAT_RMAT_NORM(err);

  if (display) {
    printf("rmap comp\n");
    DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("a2cf", ma2c_f);
    DISPLAY_INT32_RMAT_AS_EULERS_DEG("a2ci", ma2c_i);
  }

  return norm_err;

}


float test_quat_comp(struct FloatQuat qa2b_f, struct FloatQuat qb2c_f, int display) {

  struct FloatQuat qa2c_f;
  FLOAT_QUAT_COMP(qa2c_f, qa2b_f, qb2c_f);
  struct Int32Quat qa2b_i;
  QUAT_BFP_OF_REAL(qa2b_i, qa2b_f);
  struct Int32Quat qb2c_i;
  QUAT_BFP_OF_REAL(qb2c_i, qb2c_f);
  struct Int32Quat qa2c_i;
  INT32_QUAT_COMP(qa2c_i, qa2b_i, qb2c_i);

  struct FloatQuat err;
  QUAT_DIFF(err, qa2c_f, qa2c_i);
  float norm_err = FLOAT_QUAT_NORM(err);

  if (display) {
    printf("quat comp\n");
    DISPLAY_FLOAT_QUAT_AS_EULERS_DEG("a2cf", qa2c_f);
    DISPLAY_INT32_QUAT_AS_EULERS_DEG("a2ci", qa2c_i);
  }

  return norm_err;

}



static void test_7(void) {






  printf("\n");
  struct FloatEulers ea2c;
  EULERS_ASSIGN(ea2c, RadOfDeg(29.742755), RadOfDeg(-40.966522), RadOfDeg(69.467265));
  DISPLAY_FLOAT_EULERS_DEG("ea2c", ea2c);

  struct FloatEulers eb2c;
  EULERS_ASSIGN(eb2c, RadOfDeg(0.), RadOfDeg(0.), RadOfDeg(90.));
  DISPLAY_FLOAT_EULERS_DEG("eb2c", eb2c);
  struct FloatRMat fa2c;
  FLOAT_RMAT_OF_EULERS(fa2c, ea2c);
  struct FloatRMat fb2c;
  FLOAT_RMAT_OF_EULERS(fb2c, eb2c);

  printf("\n");
  test_rmat_comp_inv(fa2c, fb2c, 1);

  struct FloatQuat qa2c;
  FLOAT_QUAT_OF_EULERS(qa2c, ea2c);
  struct FloatQuat qb2c;
  FLOAT_QUAT_OF_EULERS(qb2c, eb2c);

  printf("\n");
  test_quat_comp_inv(qa2c, qb2c, 1);
  printf("\n");



}

#define NB_ITER 100000

static void test_8(void) {
  float max_err = 0.;
  float sum_err = 0.;
  int i;
  for (i=0; i<NB_ITER; i++) {
    struct FloatEulers eul;
    eul.phi   = ((double)random()/(double)RAND_MAX - 0.5)*M_PI*2.;
    eul.theta = ((double)random()/(double)RAND_MAX - 0.5)*M_PI*2.;
    eul.psi   = ((double)random()/(double)RAND_MAX - 0.5)*M_PI*2.;
    float err = test_INT32_QUAT_OF_RMAT(&eul, FALSE);
    sum_err += err;
    if (err > max_err) max_err = err;
    if (err > .01) {
      printf("%f\n", err);
      DISPLAY_FLOAT_EULERS_DEG("eul", eul);
      test_INT32_QUAT_OF_RMAT(&eul, TRUE);
    }
  }
  printf("err %f (%f)\n", sum_err/NB_ITER, max_err);
}


static void test_9(void) {
  struct FloatEulers eul;
  eul.phi   = RadOfDeg(80.821376);
  eul.theta = RadOfDeg(44.227319);
  eul.psi   = RadOfDeg(-134.047298);
  float err = test_INT32_QUAT_OF_RMAT(&eul, TRUE);
}

static void test_10(void) {

  struct FloatEulers euler;
  EULERS_ASSIGN(euler , RadOfDeg(0.), RadOfDeg(10.), RadOfDeg(0.));
  DISPLAY_FLOAT_EULERS_DEG("euler", euler);
  struct FloatQuat quat;
  FLOAT_QUAT_OF_EULERS(quat, euler);
  DISPLAY_FLOAT_QUAT("####quat", quat);

  struct Int32Eulers euleri;
  EULERS_BFP_OF_REAL(euleri, euler);
  DISPLAY_INT32_EULERS("euleri", euleri);
  struct Int32Quat quati;
  INT32_QUAT_OF_EULERS(quati, euleri);
  DISPLAY_INT32_QUAT("####quat", quati);
  struct Int32RMat rmati;
  INT32_RMAT_OF_EULERS(rmati, euleri);
  DISPLAY_INT32_RMAT("####rmat", rmati);

  struct Int32Quat quat_ltp_to_body;
  struct Int32Quat body_to_imu_quat;
  INT32_QUAT_ZERO( body_to_imu_quat);


  INT32_QUAT_COMP_INV(quat_ltp_to_body, body_to_imu_quat, quati);
  DISPLAY_INT32_QUAT("####quat_ltp_to_body", quat_ltp_to_body);

}

float test_rmat_comp_inv(struct FloatRMat ma2c_f, struct FloatRMat mb2c_f, int display) {

  struct FloatRMat ma2b_f;
  FLOAT_RMAT_COMP_INV(ma2b_f, ma2c_f, mb2c_f);
  struct Int32RMat ma2c_i;
  RMAT_BFP_OF_REAL(ma2c_i, ma2c_f);
  struct Int32RMat mb2c_i;
  RMAT_BFP_OF_REAL(mb2c_i, mb2c_f);
  struct Int32RMat ma2b_i;
  INT32_RMAT_COMP_INV(ma2b_i, ma2c_i, mb2c_i);

  struct FloatRMat err;
  RMAT_DIFF(err, ma2b_f, ma2b_i);
  float norm_err = FLOAT_RMAT_NORM(err);

  if (display) {
    printf("rmap comp_inv\n");
    DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("a2cf", ma2b_f);
    DISPLAY_INT32_RMAT_AS_EULERS_DEG("a2ci", ma2b_i);
  }

  return norm_err;

}

float test_quat_comp_inv(struct FloatQuat qa2c_f, struct FloatQuat qb2c_f, int display) {

  struct FloatQuat qa2b_f;
  FLOAT_QUAT_COMP_INV(qa2b_f, qa2c_f, qb2c_f);
  struct Int32Quat qa2c_i;
  QUAT_BFP_OF_REAL(qa2c_i, qa2c_f);
  struct Int32Quat qb2c_i;
  QUAT_BFP_OF_REAL(qb2c_i, qb2c_f);
  struct Int32Quat qa2b_i;
  INT32_QUAT_COMP_INV(qa2b_i, qa2c_i, qb2c_i);

  struct FloatQuat err;
  QUAT_DIFF(err, qa2b_f, qa2b_i);
  float norm_err = FLOAT_QUAT_NORM(err);

  if (display) {
    printf("quat comp_inv\n");
    DISPLAY_FLOAT_QUAT_AS_EULERS_DEG("a2bf", qa2b_f);
    DISPLAY_INT32_QUAT_AS_EULERS_DEG("a2bi", qa2b_i);
  }

  return norm_err;

}



void test_of_axis_angle(void) {

  struct FloatVect3 axis = { 0., 1., 0.};
  FLOAT_VECT3_NORMALIZE(axis);
  DISPLAY_FLOAT_VECT3("axis", axis);
  const float angle = RadOfDeg(30.);
  printf("angle %f\n", DegOfRad(angle));

  struct FloatQuat my_q;
  FLOAT_QUAT_OF_AXIS_ANGLE(my_q, axis, angle);
  DISPLAY_FLOAT_QUAT_AS_EULERS_DEG("quat", my_q);

  struct FloatMat33 my_r1;
  FLOAT_RMAT_OF_QUAT(my_r1, my_q);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat1", my_r1);
  DISPLAY_FLOAT_RMAT("rmat1", my_r1);

  struct FloatMat33 my_r;
  FLOAT_RMAT_OF_AXIS_ANGLE(my_r, axis, angle);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat", my_r);
  DISPLAY_FLOAT_RMAT("rmat", my_r);

  printf("\n");

  struct FloatEulers eul = {RadOfDeg(30.), RadOfDeg(30.), 0.};

  struct FloatVect3 uz = { 0., 0., 1.};
  struct FloatMat33 r_yaw;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_yaw, uz, eul.psi);

  struct FloatVect3 uy = { 0., 1., 0.};
  struct FloatMat33 r_pitch;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_pitch, uy, eul.theta);

  struct FloatVect3 ux = { 1., 0., 0.};
  struct FloatMat33 r_roll;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_roll, ux, eul.phi);

  struct FloatMat33 r_yaw_pitch;
  FLOAT_RMAT_COMP(r_yaw_pitch, r_yaw, r_pitch);

  struct FloatMat33 r_yaw_pitch_roll;
  FLOAT_RMAT_COMP(r_yaw_pitch_roll, r_yaw_pitch, r_roll);

  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat", r_yaw_pitch_roll);
  DISPLAY_FLOAT_RMAT("rmat", r_yaw_pitch_roll);

  DISPLAY_FLOAT_EULERS_DEG("eul", eul);
  struct FloatMat33 rmat1;
  FLOAT_RMAT_OF_EULERS(rmat1, eul);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat1", rmat1);
  DISPLAY_FLOAT_RMAT("rmat1", rmat1);

}

float test_quat_of_rmat(void) {

  //  struct FloatEulers eul = {-0.280849, 0.613423, -1.850440};
  struct FloatEulers eul = {RadOfDeg(0.131579),  RadOfDeg(-62.397659), RadOfDeg(-110.470299)};
  //  struct FloatEulers eul = {RadOfDeg(0.13), RadOfDeg(180.), RadOfDeg(-61.)};

  struct FloatMat33 rm;
  FLOAT_RMAT_OF_EULERS(rm, eul);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat", rm);

  struct FloatQuat q;
  FLOAT_QUAT_OF_RMAT(q, rm);
  DISPLAY_FLOAT_QUAT("q_of_rm   ", q);
  DISPLAY_FLOAT_QUAT_AS_EULERS_DEG("q_of_rm   ", q);

  struct FloatQuat qref;
  FLOAT_QUAT_OF_EULERS(qref, eul);
  DISPLAY_FLOAT_QUAT("q_of_euler", qref);
  DISPLAY_FLOAT_QUAT_AS_EULERS_DEG("q_of_euler", qref);

  printf("\n\n\n");

  struct FloatMat33 r_att;
  struct FloatEulers e312 = { eul.phi, eul.theta, eul.psi };
  FLOAT_RMAT_OF_EULERS_312(r_att, e312);
  DISPLAY_FLOAT_RMAT("r_att  ", r_att);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("r_att  ", r_att);

  struct FloatQuat q_att;
  FLOAT_QUAT_OF_RMAT(q_att, r_att);

  struct FloatEulers e_att;
  FLOAT_EULERS_OF_RMAT(e_att, r_att);
  DISPLAY_FLOAT_EULERS_DEG("of rmat", e_att);

  FLOAT_EULERS_OF_QUAT(e_att, q_att);
  DISPLAY_FLOAT_EULERS_DEG("of quat", e_att);

  return 0.;
}


float test_rmat_of_eulers_312(void) {

  struct FloatEulers eul312_f;
  EULERS_ASSIGN(eul312_f, RadOfDeg(45.), RadOfDeg(22.), RadOfDeg(0.));
  DISPLAY_FLOAT_EULERS_DEG("eul312_f", eul312_f);
  struct Int32Eulers eul312_i;
  EULERS_BFP_OF_REAL(eul312_i, eul312_f);
  DISPLAY_INT32_EULERS("eul312_i", eul312_i);

  struct FloatRMat rmat_f;
  FLOAT_RMAT_OF_EULERS_312(rmat_f, eul312_f);
  DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat float", rmat_f);

  struct Int32RMat rmat_i;
  INT32_RMAT_OF_EULERS_312(rmat_i, eul312_i);
  DISPLAY_INT32_RMAT_AS_EULERS_DEG("rmat int", rmat_i);

  return 0;

}


void test1234(void) {
  struct FloatEulers eul = {RadOfDeg(33.), RadOfDeg(25.), RadOfDeg(26.)};

  struct FloatVect3 uz = { 0., 0., 1.};
  struct FloatMat33 r_yaw;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_yaw, uz, eul.psi);

  struct FloatVect3 uy = { 0., 1., 0.};
  struct FloatMat33 r_pitch;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_pitch, uy, eul.theta);

  struct FloatVect3 ux = { 1., 0., 0.};
  struct FloatMat33 r_roll;
  FLOAT_RMAT_OF_AXIS_ANGLE(r_roll, ux, eul.phi);

  struct FloatMat33 r_tmp;
  FLOAT_RMAT_COMP(r_tmp, r_yaw, r_roll);

  struct FloatMat33 r_att;
  FLOAT_RMAT_COMP(r_att, r_tmp, r_pitch);
  DISPLAY_FLOAT_RMAT("r_att_ref  ", r_att);

  FLOAT_RMAT_OF_EULERS_312(r_att, eul);
  DISPLAY_FLOAT_RMAT("r_att312  ", r_att);




}


float test_quat(void) {
  struct FloatVect3 u = { 1., 2., 3.};
  FLOAT_VECT3_NORMALIZE(u);
  float angle = RadOfDeg(30.);

  struct FloatQuat q;
  FLOAT_QUAT_OF_AXIS_ANGLE(q, u, angle);
  DISPLAY_FLOAT_QUAT("q ", q);

  struct FloatQuat iq;
  FLOAT_QUAT_INVERT(iq, q);
  DISPLAY_FLOAT_QUAT("iq", iq);

  struct FloatQuat q1;
  FLOAT_QUAT_COMP(q1, q, iq);
  DISPLAY_FLOAT_QUAT("q1", q1);

  struct FloatQuat q2;
  FLOAT_QUAT_COMP(q2, q, iq);
  DISPLAY_FLOAT_QUAT("q2", q2);

  struct FloatQuat qe;
  QUAT_EXPLEMENTARY(qe, q);
  DISPLAY_FLOAT_QUAT("qe", qe);

  struct FloatVect3 a = { 2., 1., 3.};
  DISPLAY_FLOAT_VECT3("a ", a);
  struct FloatVect3 a1;
  FLOAT_QUAT_VMULT(a1, q, a);
  DISPLAY_FLOAT_VECT3("a1", a1);

  struct FloatVect3 a2;
  FLOAT_QUAT_VMULT(a2, qe, a);
  DISPLAY_FLOAT_VECT3("a2", a2);





  return 0.;

}

float test_quat2(void) {

  struct FloatEulers eula2b;
  EULERS_ASSIGN(eula2b, RadOfDeg(70.), RadOfDeg(0.), RadOfDeg(0.));
  //  DISPLAY_FLOAT_EULERS_DEG("eula2b", eula2b);

  struct FloatQuat qa2b;
  FLOAT_QUAT_OF_EULERS(qa2b, eula2b);
  DISPLAY_FLOAT_QUAT("qa2b", qa2b);

  struct DoubleEulers eula2b_d;
  EULERS_ASSIGN(eula2b_d, RadOfDeg(70.), RadOfDeg(0.), RadOfDeg(0.));
  struct DoubleQuat qa2b_d;
  DOUBLE_QUAT_OF_EULERS(qa2b_d, eula2b_d);
  DISPLAY_FLOAT_QUAT("qa2b_d", qa2b_d);

   struct FloatVect3 u = { 1., 0., 0.};
   float angle = RadOfDeg(70.);

  struct FloatQuat q;
  FLOAT_QUAT_OF_AXIS_ANGLE(q, u, angle);
  DISPLAY_FLOAT_QUAT("q ", q);





  struct FloatEulers eula2c;
  EULERS_ASSIGN(eula2c, RadOfDeg(80.), RadOfDeg(0.), RadOfDeg(0.));
  //  DISPLAY_FLOAT_EULERS_DEG("eula2c", eula2c);

  struct FloatQuat qa2c;
  FLOAT_QUAT_OF_EULERS(qa2c, eula2c);
  DISPLAY_FLOAT_QUAT("qa2c", qa2c);


  struct FloatQuat qb2a;
  FLOAT_QUAT_INVERT(qb2a, qa2b);
  DISPLAY_FLOAT_QUAT("qb2a", qb2a);


  struct FloatQuat qb2c1;
  FLOAT_QUAT_COMP(qb2c1, qb2a, qa2c);
  DISPLAY_FLOAT_QUAT("qb2c1", qb2c1);

  struct FloatQuat qb2c2;
  FLOAT_QUAT_INV_COMP(qb2c2, qa2b, qa2c);
  DISPLAY_FLOAT_QUAT("qb2c2", qb2c2);

  return 0.;

}


float test_INT32_QUAT_OF_RMAT(struct FloatEulers* eul_f, bool_t display) {

  struct Int32Eulers eul312_i;
  EULERS_BFP_OF_REAL(eul312_i, (*eul_f));
  if (display)  DISPLAY_INT32_EULERS("eul312_i", eul312_i);

  struct FloatRMat rmat_f;
  FLOAT_RMAT_OF_EULERS_312(rmat_f, (*eul_f));
  if (display) DISPLAY_FLOAT_RMAT_AS_EULERS_DEG("rmat float", rmat_f);
  if (display) DISPLAY_FLOAT_RMAT("rmat float", rmat_f);

  struct Int32RMat rmat_i;
  INT32_RMAT_OF_EULERS_312(rmat_i, eul312_i);
  if (display) DISPLAY_INT32_RMAT_AS_EULERS_DEG("rmat int", rmat_i);
  if (display) DISPLAY_INT32_RMAT("rmat int", rmat_i);
  if (display) DISPLAY_INT32_RMAT_AS_FLOAT("rmat int", rmat_i);

  struct FloatQuat qf;
  FLOAT_QUAT_OF_RMAT(qf, rmat_f);
  FLOAT_QUAT_WRAP_SHORTEST(qf);
  if (display) DISPLAY_FLOAT_QUAT("qf", qf);

  struct Int32Quat qi;
  INT32_QUAT_OF_RMAT(qi, rmat_i);
  INT32_QUAT_WRAP_SHORTEST(qi);
  if (display) DISPLAY_INT32_QUAT("qi", qi);
  if (display) DISPLAY_INT32_QUAT_2("qi", qi);

  struct FloatQuat qif;
  QUAT_FLOAT_OF_BFP(qif, qi);
  struct FloatQuat qerr;
  QUAT_DIFF(qerr, qif, qf);

  float err_norm = FLOAT_QUAT_NORM(qerr);
  if (display) printf("err %f\n", err_norm);
  if (display) printf("\n");

  return err_norm;

}
