/*
 * Copyright (c) 2019, NVIDIA CORPORATION.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SORT_GROUPBY_HPP
#define SORT_GROUPBY_HPP

#include <cudf/cudf.h>
#include <cudf/groupby.hpp>
#include <cudf/types.hpp>

#include <vector>

namespace cudf {
namespace groupby {
namespace sort {
namespace detail {

std::pair<cudf::table, cudf::table> groupby(cudf::table const& keys,
                                             cudf::table const& values,
                                             std::vector<operators> const& ops,
                                             Options options,
                                             cudaStream_t stream = 0);

std::pair<cudf::table, std::vector<gdf_column*>> groupby(cudf::table const& keys,
                                            cudf::table const& values,
                                            std::vector<operation> const& ops,
                                            Options options,
                                            cudaStream_t stream = 0);
}  // namespace detail
}  // namespace hash
}  // namespace sort
}  // namespace cudf

#endif
