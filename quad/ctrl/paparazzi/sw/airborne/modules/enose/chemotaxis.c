#include "modules/enose/chemotaxis.h"
#include "generated/airframe.h"
#include "estimator.h"
#include "std.h"
#include "subsystems/nav.h"
#include "generated/flight_plan.h"
#include "ap_downlink.h"
#include "modules/enose/chemo_detect.h"

#define MAX_RADIUS 250
#define ALPHA 0.5

static uint8_t last_plume_value;

static float radius;
static int8_t sign;

bool_t nav_chemotaxis_init( uint8_t c, uint8_t plume ) {
  radius = MAX_RADIUS;
  last_plume_value = 0;
  sign = 1;
  waypoints[plume].x = waypoints[c].x;
  waypoints[plume].y = waypoints[c].y;
  return FALSE;
}

bool_t nav_chemotaxis( uint8_t c, uint8_t plume ) {

  if (chemo_sensor > last_plume_value) {
    /* Move the circle in this direction */
    float x = estimator_x - waypoints[plume].x;
    float y = estimator_y - waypoints[plume].y;
    waypoints[c].x = waypoints[plume].x + ALPHA * x;
    waypoints[c].y = waypoints[plume].y + ALPHA * y;
    //    DownlinkSendWp(c);
    /* Turn in the right direction */
    float dir_x = cos(M_PI_2 - estimator_hspeed_dir);
    float dir_y = sin(M_PI_2 - estimator_hspeed_dir);
    float pvect = dir_x*y - dir_y*x;
    sign = (pvect > 0 ? -1 : 1);
    /* Reduce the radius */
    radius = sign * (DEFAULT_CIRCLE_RADIUS+(MAX_CHEMO-chemo_sensor)/(float)MAX_CHEMO*(MAX_RADIUS-DEFAULT_CIRCLE_RADIUS));


    /* Store this plume */
    waypoints[plume].x = estimator_x;
    waypoints[plume].y = estimator_y;
    // DownlinkSendWp(plume);
    last_plume_value = chemo_sensor;
  }

  NavCircleWaypoint(c, radius);
  return TRUE;
}
