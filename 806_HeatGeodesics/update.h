#include <Eigen/Core>
namespace igl
{
  template <typename Scalar>
  struct HeatGeodesicsData;
}

bool update(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double t,
  const double x1,
  const double y1,
  const double x2,
  const double y2,
  const Eigen::Matrix4f& model,
  const Eigen::Matrix4f& proj,
  const Eigen::Vector4f& viewport,
  const igl::HeatGeodesicsData<double>& data,
  Eigen::VectorXd& D);
