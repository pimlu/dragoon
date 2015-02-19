#include "Opts.h"
#include <cstring>
#include <unistd.h>

void help() {
  cout << "usage: dragoon [-t x86|js] [-o <output file>] [<input files>]"<<endl;
  std::exit(1);
}

Opts::Opts(int argc, char *argv[]) {

  if(argc == 1) {
    help();
  }

  int c;
  opterr = 0;
  while((c = getopt (argc, argv, "ht:o:")) != -1) {
    switch(c) {
      case 'h':
        help();
        break;
      case 't':
        if(!strcmp("js",optarg)) {
          target = JS;
        } else if(!strcmp("x86",optarg)) {
          cerr << "x86 not yet supported" << endl;
          std::exit(1);
        } else {
          cerr << "invalid target " << optarg <<": must be js or x86"<<endl;
          std::exit(1);
        }
        break;
      case 'o':
        ofile = string(optarg);
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
