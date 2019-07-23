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

#include <cudf/cudf.h>
#include <jit/type.h>
#include <nvstrings/NVCategory.h>
#include <utilities/cudf_utils.h>
#include <bitmask/legacy/bitmask_ops.hpp>
#include <bitmask/legacy/legacy_bitmask.hpp>
#include <cudf/copying.hpp>
#include <string/nvcategory_util.hpp>
#include <utilities/error_utils.hpp>
#include "jit/core/launcher.h"

#include <utilities/column_utils.hpp>

namespace cudf {
namespace transformation {

/**---------------------------------------------------------------------------*
 * @brief Computes output valid mask for op between a column and a scalar
 *
 * @param out_null_coun[out] number of nulls in output
 * @param valid_out preallocated output mask
 * @param valid_col input mask of column
 * @param num_values number of values in input mask valid_col
 *---------------------------------------------------------------------------**/

namespace jit {

void unary_operation(gdf_column* out, const gdf_column* in,
                     const std::string& ptx, const std::string& output_type, bool is_ptx) {
  Launcher(ptx, output_type, is_ptx).setKernelInst("kernel", out, in).launch(out, in);

}

}  // namespace jit

}  // namespace transformation

gdf_column transform(const gdf_column& input,
                     const std::string& ptx_unary_function,
                     gdf_dtype output_type, bool is_ptx) {
  
  // First create a gdf_column and then call the above function
  gdf_column output = allocate_size_dtype(input.size, output_type, input.valid != nullptr);
  output.null_count = input.null_count;

  // Check for 0 sized data
  if ((output.size == 0) && (input.size == 0)){
      return output;
  }

  // Check for null data pointer
  CUDF_EXPECTS((input.data != nullptr), "Input column data pointers are null");

  // Check for datatype
  // Input data types can be different but they have to be one of the
  // following four.
  // CUDF_EXPECTS( input.dtype == GDF_FLOAT32 ||
  //               input.dtype == GDF_FLOAT64 ||
  //               input.dtype == GDF_INT64   || 
  //               input.dtype == GDF_INT32   || 
  //               input.dtype == GDF_INT16, "Invalid/Unsupported input datatype" );
  
  if (input.valid != nullptr) {
    gdf_size_type num_bitmask_elements = gdf_num_bitmask_elements(input.size);
    CUDA_TRY(cudaMemcpy(output.valid, input.valid, num_bitmask_elements, cudaMemcpyDeviceToDevice));
  }

  transformation::jit::unary_operation(&output, &input, ptx_unary_function, cudf::jit::getTypeName(output_type), is_ptx);
  
  return output;
}

}  // namespace cudf
