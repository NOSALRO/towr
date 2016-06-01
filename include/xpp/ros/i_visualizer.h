/*
 * zmp_publisher.h
 *
 *  Created on: Apr 5, 2016
 *      Author: winklera
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_SRC_IVISUALIZER_H_
#define USER_TASK_DEPENDS_XPP_OPT_SRC_IVISUALIZER_H_

#include <xpp/zmp/spline_container.h>
#include <xpp/hyq/foothold.h>

namespace xpp {
namespace ros {

/**
 * @brief Interface to derive from to visualize the optimization results.
 * "Program to an interface, not an implementation"
 *
 * @todo decouple even more from actual implementation by removing foothold and
 * spline dependecies.
 */
class IVisualizer {
public:
  typedef xpp::zmp::SplineContainer::VecSpline VecSpline;
  typedef xpp::hyq::Foothold::VecFoothold VecFoothold;


public:
  IVisualizer() {};
  virtual
  ~IVisualizer () {};

  virtual void PublishMsg() { /* this interface does nothing in this case; */ };
};

// compilation unit scope object that can be used as default initialization
// "static" : each .cc files that includes this header has it's own copy of this variable
static IVisualizer dummy_visualizer;

} /* namespace ros */
} /* namespace xpp */

#endif /* USER_TASK_DEPENDS_XPP_OPT_SRC_IVISUALIZER_H_ */