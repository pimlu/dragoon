#include "Opts.h"
#include <cstring>
#include <unistd.h>

void help() {
  cout << "usage: dragoon [-t x86|js] [-PSI] [-o <output file>] [<input files>]"<<endl;
  cout << "-t: determines what compile target to use" << endl;
  cout << "-o: determines what output file (if t not specified, will decide type)"<<endl;
  cout << "-P: only output parse tree" << endl;
  cout << "-S: output semantic analysis" << endl;
  cout << "-I: output intermediate code" << endl;
  std::exit(1);
}

Opts::Opts(int argc, char *argv[]) {

  if(argc == 1) {
    help();
  }

  int c;
  opterr = 0;
  while((c = getopt (argc, argv, "ht:o:PSI")) != -1) {
    switch(c) {
      case 'h':
        help();
        break;
      case 't':
        if(!strcmp("x86",optarg)) {
          target = X86;
        } else if(!strcmp("js",optarg)) {
          cerr << "js not yet supported" << endl;
          std::exit(1);
        } else {
          cerr << "invalid target " << optarg <<": must be js or x86"<<endl;
          std::exit(1);
        }
        break;
      case 'o':
        ofile = string(optarg);
        break;
      case 'P':
        stage = PARSE;
        break;
      case 'S':
        stage = SEMANTIC;
        break;
      case 'I':
        stage = IR;
        break;
      case '?':
        if(optopt == 'o') {
          cerr << "Option -" << (char) optopt << " requires an argument.\n";
        } else if(isprint (optopt)) {
          cerr << "Unknown option `-" << (char) optopt << "'.\n";
        } else {
          cerr << "Unknown option character `\\x"<< std::hex << optopt <<"'.\n";
        }
        std::exit(1);
      default:
        cerr << "IT'S HAPPENING" << endl;
        std::abort();
    }
  }
  nonopts = optind;

  if(argc - nonopts == 0) {
    cerr << "No input files specified." << endl;
    help();
  } else if(argc - nonopts > 1) {
    cerr << "Only one input file is currently supported." << endl;
    help();
  }

  if(ofile.length() == 0) {
    ofile = "a.js";
  }
}
