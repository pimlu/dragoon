#include "global.h"
#include "Opts.h"
#include "parse/parse.h"

int main(int argc, char *argv[]) {
  Opts opts(argc, argv);
  cout<<"ofile:"<<opts.ofile<<" target:"<<opts.target<<endl;
  cout<<"ifile:"<<argv[opts.nonopts]<<endl;
  parse(argv[opts.nonopts]);

  return 0;
}