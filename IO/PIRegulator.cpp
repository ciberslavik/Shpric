#include "PIRegulator.h"

class PIDImpl
{
    public:
        PIDImpl(pidData_t *pidData);
        ~PIDImpl();
        qint16  calculate( double setpoint, double pv );
        void reset();
        const qint16 MAX_INT = INT16_MAX;
        const qint32 MAX_LONG = INT32_MAX;
        const qint32 MAX_I_TERM = (MAX_LONG / 4);
    private:
        pidData_t *pid_st;

        const qint16 SCALING_FACTOR = 128;
        double _dt;
        double _max;
        double _min;
        double _Kp;
        double _Kd;
        double _Ki;
        double _pre_error;
        double _integral;
};



PIRegulator::PIRegulator(double dt, double max, double min, int Kp, int Kd, int Ki)
{
    pidData_t *data = new pidData_t();
    data->sumError = 0;
    data->lastProcessValue = 0;
    data->P_Factor = Kp * 128;
    data->I_Factor = Ki * 128;
    data->D_Factor = Kd * 128;
    data->maxError = INT16_MAX / (data->P_Factor + 1);
    data->maxSumError = (INT32_MAX / 2) / (data->I_Factor + 1);
    pimpl = new PIDImpl(data);
}

qint16  PIRegulator::calculate(double setpoint, double value)
{
    return pimpl->calculate(setpoint, value);
}

void PIRegulator::reset_itegrator()
{
    pimpl->reset();
}

PIDImpl::PIDImpl(pidData_t *data)
{
    pid_st = data;
}

qint16 PIDImpl::calculate(double setpoint, double pv)
{
    int16_t error, p_term, d_term;
      int32_t i_term, ret, temp;

      error = setpoint - pv;

      // Calculate Pterm and limit error overflow
      if (error > pid_st->maxError){
        p_term = MAX_INT;
      }
      else if (error < -pid_st->maxError){
        p_term = -MAX_INT;
      }
      else{
        p_term = pid_st->P_Factor * error;
      }

      // Calculate Iterm and limit integral runaway
      temp = pid_st->sumError + error;
      if(temp > pid_st->maxSumError){
        i_term = MAX_I_TERM;
        pid_st->sumError = pid_st->maxSumError;
      }
      else if(temp < -pid_st->maxSumError){
        i_term = -MAX_I_TERM;
        pid_st->sumError = -pid_st->maxSumError;
      }
      else{
        pid_st->sumError = temp;
        i_term = pid_st->I_Factor * pid_st->sumError;
      }

      // Calculate Dterm
      d_term = pid_st->D_Factor * (pid_st->lastProcessValue - pv);

      pid_st->lastProcessValue = pv;

      ret = (p_term + i_term + d_term) / SCALING_FACTOR;
      //qDebug()<<"P:"<<p_term<<"-I:"<<i_term<<"-D:"<<d_term;
      if(ret > MAX_INT){
        ret = MAX_INT;
      }
      else if(ret < -MAX_INT){
        ret = -MAX_INT;
      }

      return((int16_t)ret);
}

void PIDImpl::reset()
{
    pid_st->sumError = 0;
}
