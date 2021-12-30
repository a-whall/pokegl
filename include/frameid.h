#pragma once

enum Frame_ID
{
  idle_u,
  idle_l,
  idle_d,
  idle_r,
  
  stride_u1,
  stride_u2,
  stride_l1,
  stride_d1,
  stride_d2,
  stride_r1,

  bike_idle_u,
  bike_idle_d,
  bike_idle_l,
  bike_idle_r,

  bike_stride_u1,
  bike_stride_u2,
  bike_stride_d1,
  bike_stride_d2, 
  bike_stride_l1,
  bike_stride_r1
};

typedef Frame_ID Frame_ID_enum;