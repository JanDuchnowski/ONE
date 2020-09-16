/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "GenModelTest.h"

#include <memory>

TEST_F(GenModelTest, OneOp_Cast_BoolToFloat32)
{
  CircleGen cgen;
  int in = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_BOOL});
  int out = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_FLOAT32});
  cgen.addOperatorCast({{in}, {out}}, circle::TensorType::TensorType_BOOL,
                       circle::TensorType::TensorType_FLOAT32);
  cgen.setInputsAndOutputs({in}, {out});

  _context = std::make_unique<GenModelTestContext>(cgen.finish());
  TestCaseData tcd;
  tcd.addInput(std::vector<bool>{true, false, true, true});
  tcd.addOutput(std::vector<float>{1, 0, 1, 1});
  _context->addTestCase(tcd);
  _context->setBackends({"acl_cl", "cpu"});

  SUCCEED();
}

TEST_F(GenModelTest, OneOp_Cast_AfterEqual)
{
  CircleGen cgen;
  int lhs = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_FLOAT32});
  int rhs = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_FLOAT32});
  int equal_out = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_BOOL});
  int out = cgen.addTensor({{1, 2, 2, 1}, circle::TensorType::TensorType_FLOAT32});
  cgen.addOperatorEqual({{lhs, rhs}, {equal_out}});
  cgen.addOperatorCast({{equal_out}, {out}}, circle::TensorType::TensorType_BOOL,
                       circle::TensorType::TensorType_FLOAT32);
  cgen.setInputsAndOutputs({lhs, rhs}, {out});

  _context = std::make_unique<GenModelTestContext>(cgen.finish());
  _context->addTestCase(uniformTCD<float>({{1, 3, 2, 4}, {2, 3, 1, 4}}, {{0, 1, 0, 1}}));
  _context->setBackends({"acl_cl", "cpu"});

  SUCCEED();
}
