#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
	/**
	TODO:
	  * predict the state
	*/
	x_ = F_ * x_;
	P_ = F_ * P_*F_.transpose() + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {

	auto y = z - H_ * x_;
	auto H_transpose_ = H_.transpose();
	auto S = H_ * P_*H_transpose_ + R_;
	auto K = P_ * H_transpose_*S.inverse();

	x_ += K * y;
	auto I = MatrixXd::Identity(4, 4);
	P_ = (I - K * H_)*P_;
}


void KalmanFilter::LidarUpdate(const VectorXd &z,
	const MatrixXd &R,
	const MatrixXd& H) {

	auto H_transpose_ = H.transpose();
	VectorXd y = z - H * x_;
	MatrixXd S = H * P_*H.transpose() + R;
	MatrixXd K = P_ * H.transpose()*S.inverse();
	x_ += K * y;

	auto I = MatrixXd::Identity(4, 4);
	P_ = (I - K * H)*P_;
}

void KalmanFilter::RadarUpdate(const VectorXd& z,
	const VectorXd& h_x,
	const MatrixXd& Hj,
	const MatrixXd& R
)
{
	auto Hj_transpose_ = Hj.transpose();

	VectorXd y = z - h_x;
	while (y(1) > M_PI) y(1) -= 2.*M_PI;
	while (y(1) < -M_PI) y(1) += 2.*M_PI;
	MatrixXd S = Hj * P_*Hj_transpose_ + R;
	MatrixXd K = P_ * Hj_transpose_*S.inverse();
	x_ += K * y;
	auto I = MatrixXd::Identity(4, 4);
	P_ = (I - K * Hj)*P_;

}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
	/**
	TODO:
	  * update the state by using Extended Kalman Filter equations
	  *
	*/
	Predict();
	Update(z);
}
