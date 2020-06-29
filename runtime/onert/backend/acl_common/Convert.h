/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ONERT_BACKEND_ACL_COMMON_CONVERT_H__
#define __ONERT_BACKEND_ACL_COMMON_CONVERT_H__

#include <arm_compute/core/TensorInfo.h>
#include <arm_compute/core/SubTensorInfo.h>
#include <arm_compute/core/TensorShape.h>

#include "ir/Layout.h"
#include "ir/InternalType.h"
#include "ir/Operand.h"
#include "ir/Shape.h"
#include "ir/TypeInfo.h"
#include "ir/Coordinates.h"
#include "ir/Padding.h"
#include "misc/feature/Shape.h"
#include "misc/kernel/Shape.h"

#include "AclFunction.h"

namespace onert
{
namespace backend
{
namespace acl_common
{

::arm_compute::TensorShape asTensorShape(const ir::Shape &shape, ir::Layout frontend_layout,
                                         ir::Layout backend_layout,
                                         bool apply_dim_correction = true);
::arm_compute::Coordinates asTensorCoordinate(const ir::Coordinates &coord,
                                              ir::Layout frontend_layout,
                                              ir::Layout backend_layout);
::arm_compute::DataType asDataType(ir::DataType type);
::arm_compute::TensorInfo asTensorInfo(const ir::Shape &shape, const ir::TypeInfo &typeInfo,
                                       ir::Layout frontend_layout, ir::Layout backend_layout,
                                       bool apply_dim_correction = true);

::arm_compute::PadStrideInfo asPadStrideInfo(const ir::ExplicitPadding &padding,
                                             const ir::Stride &stride);

::arm_compute::ActivationLayerInfo asActivationLayerInfo(ir::Activation act_code);

arm_compute::Coordinates asCoordinates(const ir::Operand &operand, int rank,
                                       ir::Layout frontend_layout, ir::Layout backend_layout);

std::unique_ptr<AclFunction> asAclFunction(std::unique_ptr<::arm_compute::IFunction> &&layer);
std::unique_ptr<AclClFunction> asAclClFunction(std::unique_ptr<::arm_compute::IFunction> &&layer);

template <typename T_Function>
std::unique_ptr<T_Function> asFunction(std::unique_ptr<::arm_compute::IFunction> &&fn)
{
  return std::make_unique<T_Function>(std::move(fn));
}

ir::Layout asRuntimeLayout(::arm_compute::DataLayout data_layout);
ir::DataType asRuntimeDataType(::arm_compute::DataType data_type);

} // namespace acl_common
} // namespace backend
} // namespace onert

#endif // __ONERT_BACKEND_ACL_COMMON_CONVERT_H__
