/**
 @file    nlp_structure.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jun 8, 2016
 @brief   Declares the class NlpStructure
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_NLP_STRUCTURE_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_NLP_STRUCTURE_H_

#include <xpp/zmp/a_constraint.h> // Bound

#include <Eigen/Dense>
#include <memory>
#include <vector>

namespace xpp {
namespace zmp {

class VariableSet;

/** @brief Holds the optimization variables.
  *
  * This class is responsible for holding the current values of the optimization
  * variables and providing a name what each variable represents. It
  * returns only exactly those values.
  */
class NlpStructure {
public:
  typedef Eigen::VectorXd VectorXd;
  typedef std::unique_ptr<VariableSet> VariableSetPtr;
  typedef std::vector<VariableSetPtr> VariableSetVector;
  typedef AConstraint::VecBound VecBound;

  NlpStructure();
  virtual ~NlpStructure();

  /** A variable set is a block of variables with some semantic information.
    *
    * @param idx The index (order) of this variable set compared to the others.
    * @param n_variables The number of variables.
    */
  void AddVariableSet(int id, int n_variables);

  int GetOptimizationVariableCount() const;
  VectorXd GetAllOptimizationVariables() const;
  VecBound GetAllBounds () const;

  void SetAllVariables(const VectorXd& x_all);
  void SetVariables(int id, const VectorXd& values);
  VectorXd GetVariables(int id) const;

  int GetStartIndex(int set_id) const;
  void Reset();

private:
  VariableSetVector variable_sets_;
  int n_variables_;
};


} // namespace zmp
} // namespace xpp

#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_NLP_STRUCTURE_H_ */
