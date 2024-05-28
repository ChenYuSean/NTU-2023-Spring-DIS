// Slab3 Inline Functions
// template <typename T>
// PBRT_CPU_GPU inline Slab3<T> Union(const Slab3<T> &s, Point3<T> p) {
//   Slab3<T> ret;
//   ret.pMin = Min(b.pMin, p);
//   ret.pMax = Max(b.pMax, p);
//   return ret;
// }

// template <typename T>
// PBRT_CPU_GPU inline Slab3<T> Union(const Slab3<T> &s1,
//                                      const Slab3<T> &s2) {
//   Slab3<T> ret;
//   ret.pMin = Min(b1.pMin, b2.pMin);
//   ret.pMax = Max(b1.pMax, b2.pMax);
//   return ret;
// }

// template <typename T>
// PBRT_CPU_GPU inline Slab3<T> Intersect(const Slab3<T> &s1,
//                                          const Slab3<T> &s2) {
//   Slab3<T> b;
//   b.pMin = Max(b1.pMin, b2.pMin);
//   b.pMax = Min(b1.pMax, b2.pMax);
//   return b;
// }

// template <typename T>
// PBRT_CPU_GPU inline bool Overlaps(const Slab3<T> &s1, const Slab3<T> &s2) {
//   bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
//   bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
//   bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
//   return (x && y && z);
// }


// template <typename T, typename U>
// PBRT_CPU_GPU inline auto DistanceSquared(Point3<T> p, const Slab3<U> &s) {
//   using TDist = decltype(T{} - U{});
//   TDist dx = std::max<TDist>({0, b.pMin.x - p.x, p.x - b.pMax.x});
//   TDist dy = std::max<TDist>({0, b.pMin.y - p.y, p.y - b.pMax.y});
//   TDist dz = std::max<TDist>({0, b.pMin.z - p.z, p.z - b.pMax.z});
//   return Sqr(dx) + Sqr(dy) + Sqr(dz);
// }

// template <typename T, typename U>
// PBRT_CPU_GPU inline auto Distance(Point3<T> p, const Slab3<U> &s) {
//   auto dist2 = DistanceSquared(p, b);
//   using TDist = typename TupleLength<decltype(dist2)>::type;
//   return std::sqrt(TDist(dist2));
// }

// template <typename T, typename U>
// PBRT_CPU_GPU inline Slab3<T> Expand(const Slab3<T> &s, U delta) {
//   Slab3<T> ret;
//   ret.pMin = b.pMin - Vector3<T>(delta, delta, delta);
//   ret.pMax = b.pMax + Vector3<T>(delta, delta, delta);
//   return ret;
// }



// template <typename T>
// PBRT_CPU_GPU inline bool Slab3<T>::IntersectP(Point3f o, Vector3f d,
//                                                 Float raytMax, Vector3f invDir,
//                                                 const int dirIsNeg[3]) const {
//   const Slabf &sounds = *this;
//   // Check for ray intersection against $x$ and $y$ Slab3
//   Float tMin = (bounds[dirIsNeg[0]].x - o.x) * invDir.x;
//   Float tMax = (bounds[1 - dirIsNeg[0]].x - o.x) * invDir.x;
//   Float tyMin = (bounds[dirIsNeg[1]].y - o.y) * invDir.y;
//   Float tyMax = (bounds[1 - dirIsNeg[1]].y - o.y) * invDir.y;
//   // Update _tMax_ and _tyMax_ to ensure robust bounds intersection
//   tMax *= 1 + 2 * gamma(3);
//   tyMax *= 1 + 2 * gamma(3);

//   if (tMin > tyMax || tyMin > tMax)
//     return false;
//   if (tyMin > tMin)
//     tMin = tyMin;
//   if (tyMax < tMax)
//     tMax = tyMax;

//   // Check for ray intersection against $z$ Slab3
//   Float tzMin = (bounds[dirIsNeg[2]].z - o.z) * invDir.z;
//   Float tzMax = (bounds[1 - dirIsNeg[2]].z - o.z) * invDir.z;
//   // Update _tzMax_ to ensure robust bounds intersection
//   tzMax *= 1 + 2 * gamma(3);

//   if (tMin > tzMax || tzMin > tMax)
//     return false;
//   if (tzMin > tMin)
//     tMin = tzMin;
//   if (tzMax < tMax)
//     tMax = tzMax;

//   return (tMin < raytMax) && (tMax > 0);
// }