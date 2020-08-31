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

#include "TensorBuilder.h"

#include <util/logging.h>

#include <cassert>

namespace onert
{
namespace backend
{
namespace cpu
{

TensorBuilder::TensorBuilder(const std::shared_ptr<cpu_common::TensorRegistry> &tensor_reg)
    : _tensor_reg{tensor_reg},
      _dynamic_tensor_mgr{new cpu_common::DynamicTensorManager(_tensor_reg)},
      _static_tensor_mgr{new StaticTensorManager(_tensor_reg, _dynamic_tensor_mgr.get())}
{
  /* empty */
}

void TensorBuilder::registerTensorInfo(const ir::OperandIndex &ind, const ir::OperandInfo &info,
                                       ir::Layout layout)
{
  _tensor_info_map.emplace(ind, info);

  // CPU backend supports only one layout as NHWC
  assert(layout == ir::Layout::NHWC);
  if (info.isDynamic())
  {
    _dynamic_tensor_mgr->buildTensor(ind, info, layout);
  }
  else
  {
    _static_tensor_mgr->buildTensor(ind, info, layout, info.isConstant());
  }
}

void TensorBuilder::notifyFirstUse(const ir::OperandIndex &ind)
{
  assert(_tensor_info_map.find(ind) != _tensor_info_map.end());
  const auto tensor_info = _tensor_info_map.at(ind);

  if (!_tensor_reg->getNativeTensor(ind)->is_dynamic())
  {
    const auto size = tensor_info.total_size();
    _static_tensor_mgr->claimPlan(ind, size);
  }
}

void TensorBuilder::notifyLastUse(const ir::OperandIndex &ind)
{
  if (!_tensor_reg->getNativeTensor(ind)->is_dynamic())
  {
    _static_tensor_mgr->releasePlan(ind);
  }
}

bool TensorBuilder::isRegistered(const ir::OperandIndex &ind) const
{
  return _tensor_info_map.find(ind) != _tensor_info_map.end();
}

void TensorBuilder::prepare(void) { _static_tensor_mgr->allocateNonconsts(); }

void TensorBuilder::allocate()
{
  // NOTE For now nothing to do. Allocation is done in prepare stage, which is not appropriate
  //      This is because CPU kernels require `ITensor`s to be allocated before Kernel Generation.
}

std::unique_ptr<ITensorManager> TensorBuilder::releaseStaticTensorManager(void)
{
  return std::move(_static_tensor_mgr);
}

std::unique_ptr<ITensorManager> TensorBuilder::releaseDynamicTensorManager(void)
{
  return std::move(_dynamic_tensor_mgr);
}

} // namespace cpu
} // namespace backend
} // namespace onert
