/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file     IndexIterator.h
 * @brief    This file contains IndexIterator class
 * @ingroup  COM_AI_RUNTIME
 */

#ifndef __NNFW_MISC_MATRIX_INDEX_ITERATOR_H__
#define __NNFW_MISC_MATRIX_INDEX_ITERATOR_H__

#include "misc/matrix/Shape.h"

namespace nnfw
{
namespace misc
{
namespace matrix
{

/**
 * @brief  Class to iterate Callable with Index of matrix
 */
class IndexIterator
{
public:
  /**
   * @brief  Construct IndexIterator object with Shape of matrix
   * @param[in]  shape   Shape reference of matrix
   */
  IndexIterator(const Shape &shape) : _shape{shape}
  {
    // DO NOTHING
  }

public:
  /**
   * @brief   Call a function iterated
   * @param[in]   cb     A callback function
   * @return  Current IndexIterator object
   */
  template <typename Callable> IndexIterator &iter(Callable cb)
  {
    for (uint32_t row = 0; row < _shape.H; ++row)
    {
      for (uint32_t col = 0; col < _shape.W; ++col)
      {
        cb(row, col);
      }
    }

    return (*this);
  }

private:
  /**
   * @brief   Shape for matrix
   */
  const Shape _shape;
};

/**
 * @brief   Create an object of IndexIterator for matrix
 * @param[in]   Shape   reference of matrix
 * @return  Created IndexIterator object
 */
inline IndexIterator iterate(const Shape &shape) { return IndexIterator{shape}; }

/**
 * @brief   Call a function iterated using IndexIterator of matrix
 *          Overloaded operator<<
 * @param[in]   it    An IndexIterator reference
 * @param[in]   cb    A callback function
 * @return  created IndexIterator object
 */
template <typename Callable> IndexIterator &operator<<(IndexIterator &&it, Callable cb)
{
  return it.iter(cb);
}

} // namespace matrix
} // namespace misc
} // namespace nnfw

#endif // __NNFW_MISC_MATRIX_INDEX_ITERATOR_H__
