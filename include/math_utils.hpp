#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cuda.h>
#include "device_launch_parameters.h"
#include <cuda_runtime.h>

#include "defs.hpp"
#include "double_vec_ops.h"
#include "helper_math.h"


template <typename T>
__host__ __device__ T sign(T x) {
  return x > 0 ? 1 : (x < 0 ? -1 : 0);
}


template <typename T>
__host__ __device__ __forceinline__ float vec_abs_diff(const vec3<T> &vec1,
                                                       const vec3<T> &vec2) {
  return fabs(vec1.x - vec2.x) + fabs(vec1.y - vec2.y) + fabs(vec1.z - vec2.z);
}

template <typename T>
__host__ __device__ __forceinline__ float vec_sq_diff(const vec3<T> &vec1,
                                                      const vec3<T> &vec2) {
  return dot(vec1 - vec2, vec1 - vec2);
}

template <typename T>
__host__ __device__ __forceinline__ T clamp(T value, T min_value, T max_value) {
  return min(max(value, min_value), max_value);
}

#endif // ifndef MATH_UTILS_H