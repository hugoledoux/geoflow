// This file is part of Geoflow
// Copyright (C) 2018-2019  Ravi Peters, 3D geoinformation TU Delft

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>

#include "common.hpp"

namespace geoflow
{

Box::Box()
{
  clear();
}

std::array<float, 3> Box::min() const
{
  return pmin;
}
std::array<float, 3> Box::max() const
{
  return pmax;
}
void Box::set(std::array<float, 3> nmin, std::array<float, 3> nmax)
{
  pmin = nmin;
  pmax = nmax;
  just_cleared = false;
}
void Box::add(float p[])
{
  if (just_cleared)
  {
    pmin[0] = p[0];
    pmin[1] = p[1];
    pmin[2] = p[2];
    pmax[0] = p[0];
    pmax[1] = p[1];
    pmax[2] = p[2];
    just_cleared = false;
  }
  pmin[0] = std::min(p[0], pmin[0]);
  pmin[1] = std::min(p[1], pmin[1]);
  pmin[2] = std::min(p[2], pmin[2]);
  pmax[0] = std::max(p[0], pmax[0]);
  pmax[1] = std::max(p[1], pmax[1]);
  pmax[2] = std::max(p[2], pmax[2]);
}
void Box::add(arr3f a)
{
  add(a.data());
}
void Box::add(const Box &otherBox)
{
  add(otherBox.min());
  add(otherBox.max());
}
void Box::add(Box &otherBox)
{
  add(otherBox.min());
  add(otherBox.max());
}
void Box::add(vec3f &vec)
{
  for (auto &p : vec)
    add(p);
}
void Box::clear()
{
  pmin.fill(0);
  pmax.fill(0);
  just_cleared = true;
}
bool Box::isEmpty() const
{
  return just_cleared;
}
arr3f Box::center() const
{
  return {(pmax[0] + pmin[0]) / 2, (pmax[1] + pmin[1]) / 2, (pmax[2] + pmin[2]) / 2};
}

const Box &Geometry::box()
{
  if (!bbox.has_value())
  {
    compute_box();
  }
  return *bbox;
};
size_t Geometry::dimension()
{
  return 3;
}

// geometry types:

void LinearRing::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &t : *this)
    {
      bbox->add(t);
    }
  }
}
size_t LinearRing::vertex_count() const
{
  return size();
}
float *LinearRing::get_data_ptr()
{
  return (*this)[0].data();
}

void LineString::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &t : *this)
    {
      bbox->add(t);
    }
  }
}
size_t LineString::vertex_count() const
{
  return size();
}
float *LineString::get_data_ptr()
{
  return (*this)[0].data();
}

size_t PointCollection::vertex_count() const
{
  return size();
}
void PointCollection::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    bbox->add(*this);
  }
}
float *PointCollection::get_data_ptr()
{
  return (*this)[0].data();
}

size_t TriangleCollection::vertex_count() const
{
  return size() * 3;
}
void TriangleCollection::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &t : *this)
    {
      bbox->add(t[0]);
      bbox->add(t[1]);
      bbox->add(t[2]);
    }
  }
}
float *TriangleCollection::get_data_ptr()
{
  return (*this)[0][0].data();
}

size_t SegmentCollection::vertex_count() const
{
  return size() * 2;
}
void SegmentCollection::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &t : *this)
    {
      bbox->add(t[0]);
      bbox->add(t[1]);
    }
  }
}
float *SegmentCollection::get_data_ptr()
{
  return (*this)[0][0].data();
}

size_t LineStringCollection::vertex_count() const
{
  size_t result = 0;
  for (auto &vec : *this)
  {
    result += vec.size();
  }
  return result;
}
void LineStringCollection::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &vec : *this)
    {
      bbox->add(vec);
    }
  }
}
float *LineStringCollection::get_data_ptr()
{
  return (*this)[0][0].data();
}

size_t LinearRingCollection::vertex_count() const
{
  size_t result = 0;
  for (auto &vec : *this)
  {
    result += vec.size();
  }
  return result;
}
void LinearRingCollection::compute_box()
{
  if (!bbox.has_value())
  {
    bbox = Box();
    for (auto &vec : *this)
    {
      bbox->add(vec);
    }
  }
}
float *LinearRingCollection::get_data_ptr()
{
  return (*this)[0][0].data();
}

} // namespace geoflow
