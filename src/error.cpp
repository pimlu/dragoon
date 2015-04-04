#include "error.h"

void errormsg(string file, TokenPos pos, string msg) {
  cout << file << ":" << pos.fline << ":" << pos.fcol
  << ": error:" << endl << msg << endl;
}
