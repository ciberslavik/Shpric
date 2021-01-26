#ifndef PIREGULATOR_H
#define PIREGULATOR_H

#include <QObject>
//#include <QDebug>
typedef struct PID_DATA{
  //! Last process value, used to find derivative of process value.
  int16_t lastProcessValue;
  //! Summation of errors, used for integrate calculations
  int32_t sumError;
  //! The Proportional tuning constant, multiplied with SCALING_FACTOR
  int16_t P_Factor;
  //! The Integral tuning constant, multiplied with SCALING_FACTOR
  int16_t I_Factor;
  //! The Derivative tuning constant, multiplied with SCALING_FACTOR
  int16_t D_Factor;
  //! Maximum allowed error, avoid overflow
  int16_t maxError;
  //! Maximum allowed sumerror, avoid overflow
  int32_t maxSumError;
} pidData_t;

class PIDImpl;
class PIRegulator
{
public:
    PIRegulator(double dt, double max, double min, int Kp, int Kd, int Ki);

    qint16  calculate(double setpoint,double value);
    void reset_itegrator();
private:
    PIDImpl *pimpl;
};


#endif
