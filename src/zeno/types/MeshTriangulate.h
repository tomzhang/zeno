#pragma once

#include <zeno/types/Mesh.h>
#include <zeno/math/vec.h>
#include <vector>

ZENO_NAMESPACE_BEGIN
namespace types {

//void meshToTriangleVerticesCPU(Mesh const &mesh, std::vector<math::vec3f> &vertices);
zycl::vector<math::vec3f> meshToTriangleVertices(Mesh const &mesh);
zycl::vector<math::vec3i> meshToTriangleIndices(Mesh const &mesh);

}
ZENO_NAMESPACE_END
