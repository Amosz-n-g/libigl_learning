#include "update.h"
#include <igl/heat_geodesics.h>
#include <igl/unproject_onto_mesh.h>

bool update(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double t,
  const double x1,
  const double y1,
  const double x2,
  const double y2,
  const Eigen::Matrix4f& view,
  const Eigen::Matrix4f& proj,
  const Eigen::Vector4f& viewport,
  const igl::HeatGeodesicsData<double>& data,
  Eigen::VectorXd& D)
{
  int fid1, fid2;
  Eigen::Vector3f bc1, bc2;
  // Cast a ray in the view direction starting from the mouse position
  if(igl::unproject_onto_mesh(Eigen::Vector2f(x1,y1), view,
    proj, viewport, V, F, fid1, bc1))
{
  if(igl::unproject_onto_mesh(Eigen::Vector2f(x2,y2), view,
    proj, viewport, V, F, fid2, bc2)
    )
  {
    // if big mesh, just use closest vertex. Otherwise, blend distances to
    // vertices of face using barycentric coordinates.
    if(F.rows()>100000)
    {
      // 3d position of hit
      Eigen::VectorXd Dtemp;
      const Eigen::RowVector3d m3 =
        V.row(F(fid1,0))*bc1(0) + V.row(F(fid1,1))*bc1(1) + V.row(F(fid1,2))*bc1(2);
      int cid1 = 0;
      Eigen::Vector3d(
          (V.row(F(fid1,0))-m3).squaredNorm(),
          (V.row(F(fid1,1))-m3).squaredNorm(),
          (V.row(F(fid1,2))-m3).squaredNorm()).minCoeff(&cid1);
      const int vid1 = F(fid1,cid1);
      igl::heat_geodesics_solve(data,(Eigen::VectorXi(1,1)<<vid1).finished(),Dtemp);
      const Eigen::RowVector3d m32 =
        V.row(F(fid2,0))*bc2(0) + V.row(F(fid2,1))*bc2(1) + V.row(F(fid2,2))*bc2(2);
      int cid2 = 0;
      Eigen::Vector3d(
          (V.row(F(fid2,0))-m32).squaredNorm(),
          (V.row(F(fid2,1))-m32).squaredNorm(),
          (V.row(F(fid2,2))-m32).squaredNorm()).minCoeff(&cid2);
      const int vid2 = F(fid2,cid2);
      igl::heat_geodesics_solve(data,(Eigen::VectorXi(1,1)<<vid2).finished(),D);
      D = D + Dtemp;
      D = D/2;
    }else
    {
      D = Eigen::VectorXd::Zero(V.rows());
      for(int cid1 = 0;cid1<3;cid1++)
      {
        const int vid = F(fid1,cid1);
        Eigen::VectorXd Dc;
        igl::heat_geodesics_solve(data,(Eigen::VectorXi(1,1)<<vid).finished(),Dc);
        D += Dc*bc1(cid1);
      }
      for(int cid2 = 0;cid2<3;cid2++)
      {
        const int vid = F(fid2,cid2);
        Eigen::VectorXd Dc;
        igl::heat_geodesics_solve(data,(Eigen::VectorXi(1,1)<<vid).finished(),Dc);
        D += Dc*bc2(cid2);
      }
      D = D / 2;
    }
    return true;
  }
}
  return false;
}
