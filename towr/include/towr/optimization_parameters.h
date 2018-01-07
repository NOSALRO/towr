/**
@file    motion_type.h
@author  Alexander W. Winkler (winklera@ethz.ch)
@date    Jan 11, 2017
@brief   Brief description
 */

#ifndef TOWR_OPTIMIZATION_PARAMETERS_H_
#define TOWR_OPTIMIZATION_PARAMETERS_H_


#include <utility>
#include <vector>


namespace towr {

enum ConstraintName  { Dynamic, EndeffectorRom, TotalTime, Terrain,
                       Force, Swing, BaseRom };
enum CostName        { ComCostID, RangOfMotionCostID, PolyCenterCostID,
                       FinalComCostID, FinalStanceCostID, ForcesCostID };

/** This class holds all the hardcoded values describing a motion.
  * This is specific to the robot and the type of motion desired.
  */
class OptimizationParameters {
public:
  using CostWeights      = std::vector<std::pair<CostName, double>>;
  using UsedConstraints  = std::vector<ConstraintName>;
  using VecTimes         = std::vector<double>;

  OptimizationParameters();
  virtual ~OptimizationParameters() = default;

  UsedConstraints GetUsedConstraints() const;
  CostWeights GetCostWeights() const;

  void SetTotalDuration(double d) {t_total_ = d; };
  double GetTotalTime() const { return t_total_;} ;

  VecTimes GetBasePolyDurations() const;
  bool OptimizeTimings() const;


  int ee_splines_per_swing_phase_;
  int force_splines_per_stance_phase_;

  double dt_base_polynomial_;
  double dt_range_of_motion_;
  double dt_base_range_of_motion_;

  double min_phase_duration_;
  double max_phase_duration_;

private:
  double t_total_ = 3.0;
  UsedConstraints constraints_;
  CostWeights cost_weights_;
};

} // namespace towr

#endif /* TOWR_OPTIMIZATION_PARAMETERS_H_ */
