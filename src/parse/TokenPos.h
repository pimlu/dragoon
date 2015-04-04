#pragma once

//this is layout compatible with YYLTYPE, thus reinterpret_cast is guaranteed
//to work correctly
struct TokenPos {
  int fline, fcol, lline, lcol;
};
