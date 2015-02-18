#include "global.h"
#include "Opts.h"

int main(int argc, char *argv[]) {
  Opts opts(argc, argv);
  cout<<"ofile:"<<opts.ofile<<" target:"<<opts.target<<endl;
  cout<<"ifiles:";
  for(int i = opts.nonopts; i < argc; i++) {
    cout<<argv[i]<<" ";
  }
  cout<<endl;
  return 0;
}
