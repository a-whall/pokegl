#pragma once

template<typename T>
struct Ordered_Pair
{
  constexpr Ordered_Pair<T>(T v1, T v2) : val{v1, v2} { };

  T& x() { return val[0]; }
  T& y() { return val[1]; }

  T x() const { return val[0]; }
  T y() const { return val[1]; }

  T& w() { return val[0]; }
  T& h() { return val[1]; }

  T w() const { return val[0]; }
  T h() const { return val[1]; }

  T operator[] (int i) const { return val[i]; }
  T& operator[] (int i) { return val[i]; }

private:

  T val[2];

};

typedef  Ordered_Pair<float>  Point, SpawnPoint, Dim2D;