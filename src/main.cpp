#include "global.h"
#include "cli/Opts.h"
#include "parse/parse.h"

int main(int argc, char *argv[]) {
  Opts opts(argc, argv);
  cout<<"ofile:"<<opts.ofile<<" target:"<<opts.target<<endl;
  cout<<"ifile:"<<argv[opts.nonopts]<<endl;

  //go through each stage, progressing only if no fatal errors, and
  //flags suggest we should proceeed

  //all stages need parsing
  Module *program = parse(argv[opts.nonopts]);
  if(!program) return 1;
  //if we need SEMANTIC, do it
  if(opts.stage >= Opts::SEMANTIC) {
    cout << "semantic" << endl;
  }
  //if stage is PARSE or SEMANTIC, we quit here, outputting our tree
  if(opts.stage <= Opts::SEMANTIC) {
    cout << *program << endl;
    return program->errs;
  }
  cout << "IR" << endl;
  delete program; //to test leaks

  return 0;
}
