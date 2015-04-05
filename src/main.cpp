#include "global.h"
#include "cli/Opts.h"
#include "parse/parse.h"

int main(int argc, char *argv[]) {
  Opts opts(argc, argv);
  cout<<"ofile:"<<opts.ofile<<" target:"<<opts.target<<endl;
  cout<<"ifile:"<<argv[opts.nonopts]<<endl;
  Module *program = parse(argv[opts.nonopts]);
  if(program) cout << *program << endl;
  else cout << "program is null (parse fail)" << endl;
  delete program; //to test leaks
  return 0;
}
