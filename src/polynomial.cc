/**
@file   polynomial.cc
@author Alexander W. Winkler
@date   29.07.2016

@brief  A virtual class spliner with ready to use derived spliners

Spliners ready to use:
        - Linear Spliner
        - Cubic Spliner
        - Quintic Spliner
*/

#include <xpp/opt/polynomial.h>

#include <cassert>
#include <cmath>

namespace xpp {
namespace opt {

Polynomial::Polynomial (int order, int dim, const std::string& id )
    : Component(-1, id)
{

//  std::cout << "order: " << order << std::endl;
  int n_coeff = order+1;
  for (int c=A; c<n_coeff; ++c) {
//    coeff_.push_back(VectorXd::Zero(dim));
    coeff_ids_.push_back(static_cast<PolynomialCoeff>(c));
  }

//  n_coeff_per_dimension_ = n_coeff;
  n_dim_ = dim;
  int n_variables = n_coeff*n_dim_;

  SetRows(n_variables);

//  // zmp_ use to SetRows()
  all_coeff_ = VectorXd::Zero(n_variables);
}

void
Polynomial::SetCoefficients (PolynomialCoeff coeff, const VectorXd& value)
{
//  all_coeff_.middleRows(Index(coeff, 0), n_coeff_per_dimension_) = value;

  for (int dim=0; dim<value.rows(); ++dim) {
    all_coeff_(Index(coeff, dim)) = value(dim);
//    SetCoefficient(dim, coeff, value(dim));
  }
}

//void Polynomial::SetCoefficient(int dim, PolynomialCoeff coeff, double value)
//{
//  all_coeff_(Index(coeff, dim)) = value;
//}
//
//double Polynomial::GetCoefficient(int dim, PolynomialCoeff coeff) const
//{
//  return all_coeff_(Index(coeff, dim));
//}

//Polynomial::CoeffVec
//Polynomial::GetCoeffIds () const
//{
//  return coeff_ids_;
//}


VectorXd
Polynomial::GetValues () const
{
//  int n_dim = coeff_.front().rows();
//  int n_coeff =
//  int n = coeff_.size()*coeff_.front().rows(); // zmp_ replace with GetRows()
//  VectorXd x(n);
//
//  int i=0;
//  for (auto coeff : coeff_) {
//    x.middleRows(
//  }

  return all_coeff_;
}

void
Polynomial::SetValues (const VectorXd& optimized_coeff)
{
  all_coeff_ = optimized_coeff;
}



int
Polynomial::Index(PolynomialCoeff coeff, int dim) const
{
  return coeff*n_dim_ + dim;
}

VectorXd
Polynomial::GetCoefficients (PolynomialCoeff coeff) const
{
  return all_coeff_.middleRows(Index(coeff, 0), n_dim_);
}


/**
 * The spliner always calculates the splines in the same way, but if the
 * spline coefficients are zero (as set by @ref Spliner()), the higher-order
 * terms have no effect.
 */
StateLinXd Polynomial::GetPoint(double t_) const
{
  // sanity checks
  if (t_ < 0.0)
    throw std::runtime_error("spliner.cc called with dt<0");

  StateLinXd out(n_dim_);

  for (auto d : {kPos, kVel, kAcc}) {
    for (PolynomialCoeff c : coeff_ids_) {
      // zmp_ remove
//      out.GetByIndex(d) += GetDerivativeWrtCoeff(d, c)*coeff_[c];
      out.GetByIndex(d) += GetDerivativeWrtCoeff(t_, d, c)*GetCoefficients(c);
    }
  }

  return out;
}

Jacobian
Polynomial::GetJacobian (double t_, MotionDerivative dxdt) const
{
//  int n = all_coeff_.rows();
  Jacobian jac(n_dim_, GetRows());

//  std::cout << "DEBUUUUG:\n";
//  std::cout << "n:  " << n << std::endl;
//  std::cout << "n_dim_: " << n_dim_ << std::endl;

  for (int dim=0; dim<n_dim_; ++dim) {
    for (PolynomialCoeff c : coeff_ids_) {

      int idx = Index(c, dim);

//      std::cout << "dim: " << dim << std::endl;
//      std::cout << "coeff: " << c << std::endl;
//      std::cout << "idx: " << idx << std::endl;

      jac.insert(dim, idx) = GetDerivativeWrtCoeff(t_, dxdt, c);
    }
  }

  return jac;
}

double
Polynomial::GetDerivativeWrtCoeff (double t_, MotionDerivative deriv, PolynomialCoeff c) const
{
  if (c<deriv)  // risky/ugly business...
    return 0.0; // derivative not depended on this coefficient.

  switch (deriv) {
    case kPos:   return               std::pow(t_,c);   break;
    case kVel:   return c*            std::pow(t_,c-1); break;
    case kAcc:   return c*(c-1)*      std::pow(t_,c-2); break;
    case kJerk:  return c*(c-1)*(c-2)*std::pow(t_,c-3); break;
  }
}


//void
//ConstantPolynomial::SetPolynomialCoefficients (double T,
//                                               const StateLinXd& start,
//                                               const StateLinXd& end)
//{
//  coeff_[A] = start.p_;
//}
//
//double
//ConstantPolynomial::GetDerivativeOfPosWrtPos (double t, PointType p) const
//{
//  switch (p) {
//    case Start: return 1.0;
//    case Goal:  return 0.0;
//    default: assert(false); // point type not defined
//  }
//}
//
//void LinearPolynomial::SetPolynomialCoefficients(double T, const StateLinXd& start, const StateLinXd& end)
//{
//  coeff_[A] = start.p_;
//  coeff_[B] = (end.p_ - start.p_) / T;
//}
//
//double
//LinearPolynomial::GetDerivativeOfPosWrtPos (double t, PointType p) const
//{
//  switch (p) {
//    case Start: return 1.0-t/duration;
//    case Goal:  return    +t/duration;
//    default: assert(false); // point type not defined
//  }
//}

//void CubicPolynomial::SetPolynomialCoefficients(double T, const StateLinXd& start, const StateLinXd& end)
//{
//  double T1 = T;
//  double T2 = T1 * T1;
//  double T3 = T1 * T2;
//
//  coeff_[A] = start.p_;
//  coeff_[B] = start.v_;
//  coeff_[C] = -( 3*(start.p_ - end.p_) +  T1*(2*start.v_ + end.v_) ) / T2;
//  coeff_[D] =  ( 2*(start.p_ - end.p_) +  T1*(  start.v_ + end.v_) ) / T3;
//}
//
//double
//CubicPolynomial::GetDerivativeOfPosWrtPos (double t, PointType p) const
//{
//  double T2 = duration*duration;
//  double T3 = T2*duration;
//
//  switch (p) {
//    case Start: return (2*t*t*t)/T3 - (3*t*t)/T2 + 1;
//    case Goal:  return (3*t*t)/T2   - (2*t*t*t)/T3;
//    default: assert(false); // point type not defined
//  }
//}
//
//void QuarticPolynomial::SetPolynomialCoefficients(double T, const StateLinXd& start, const StateLinXd& end)
//{
//  // only uses position, velocity and initial acceleration
//  double T1 = T;
//  double T2 = T1 * T1;
//  double T3 = T1 * T2;
//  double T4 = T1 * T3;
//
//  coeff_[A] = start.p_;
//  coeff_[B] = start.v_;
//  coeff_[C] = start.a_/2;
//  coeff_[D] = -(4*start.p_ - 4*end.p_ + 3*T1*start.v_ +   T1*end.v_ + T2*start.a_)/T3;
//  coeff_[E] =  (6*start.p_ - 6*end.p_ + 4*T1*start.v_ + 2*T1*end.v_ + T2*start.a_)/(2*T4);
//}

//void QuinticPolynomial::SetPolynomialCoefficients(double T, const StateLinXd& start, const StateLinXd& end)
//{
//  double T1 = T;
//  double T2 = T1 * T1;
//  double T3 = T1 * T2;
//  double T4 = T1 * T3;
//  double T5 = T1 * T4;
//
//  coeff_[A] = start.p_;
//  coeff_[B] = start.v_;
//  coeff_[C] = start.a_ / 2.;
//  coeff_[D] =  (-20*start.p_ + 20*end.p_ + T1*(-3*start.a_*T1 +   end.a_*T1 - 12* start.v_ -  8*end.v_))  / (2*T3);
//  coeff_[E] =  ( 30*start.p_ - 30*end.p_ + T1*( 3*start.a_*T1 - 2*end.a_*T1 + 16* start.v_ + 14*end.v_))  / (2*T4);
//  coeff_[F] = -( 12*start.p_ - 12*end.p_ + T1*(   start.a_*T1 -   end.a_*T1 +  6*(start.v_ +    end.v_))) / (2*T5);
//}
//
//
//void
//LiftHeightPolynomial::SetPolynomialCoefficients (
//    double T, const StateLinXd& start, const StateLinXd& end)
//{
//  VectorXd h_ = height_*VectorXd::Ones(start.kNumDim);
//
//  double n2 = std::pow(n_,2);
//  double n3 = std::pow(n_,3);
//  double n4 = std::pow(n_,4);
//  double n5 = std::pow(n_,5);
//
//  double T1 = T;
//  double T2 = T1 * T1;
//  double T3 = T1 * T2;
//  double T4 = T1 * T3;
//  double T5 = T1 * T4;
//
//  // see matlab script "swingleg_z_height.m" for generation of these values
//  coeff_[A] =  start.p_;
//  coeff_[B].setZero();
//  coeff_[C] =  (6*end.p_ - 6*start.p_ - 15*n_*end.p_ + 15*n_*start.p_ + 2*h_*n4 - h_*n5 + 10*n2*end.p_ - 10*n2*start.p_)/(- n3*T2 + 4*n2*T2 - 5*n_*T2 + 2*T2);
//  coeff_[D] =  (2*(2*end.p_ - 2*start.p_ - 5*n_*end.p_ + 5*n_*start.p_ + 2*h_*n4 - h_*n5 + 5*n3*end.p_ - 5*n3*start.p_))/((n_ - 2)*(n2*T3 - 2*n_*T3 + T3));
//  coeff_[E] = -(2*h_*n4 - h_*n5 - 10*n2*end.p_ + 15*n3*end.p_ + 10*n2*start.p_ - 15*n3*start.p_)/(T4*(n_ - 2)*(n2 - 2*n_ + 1));
//  coeff_[F] = -(2*(2*n2*end.p_ - 3*n3*end.p_ - 2*n2*start.p_ + 3*n3*start.p_))/(T5*(n_ - 2)*(n2 - 2*n_ + 1));
//}


} // namespace opt
} // namespace xpp
