#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

#include "clap/include/cmdarg.hh"
#include "clap/include/cmdline.hh"

#include "ofxget.h"

using ofxget::OfxGetContext;
using std::cout;
using std::endl;
using std::map;
using std::ostream;
using std::string;
using std::vector;

int main(int argc, char** argv) {
  CmdArgStr request_filename('r', "request", "request_file", "Request file name under the requests directory. See investment.txt for an example.");
  CmdArgStr passwords_filename('r', "passwords", "passwords_file", "Optional passwords file. If used, supplies passwords for an institution. See example_passwords.txt. Storing passwords in plain text is not safe. This file should only be used for testing purposes.", CmdArg::isOPT);
  CmdArgInt institution('i', "institution", "institution_id", "Institution id. Chooses which institution to read from institutions.txt.");
  CmdLine cmd(argv[0], &request_filename, &institution, &passwords_filename, nullptr);
  cmd.parse(argc, argv);

  OfxGetContext ofxget;
  ofxget.AddApp("QuickBooks_2008").AddInstitution(institution)
      .AddRequestTemplate(ofxget.GetRequestTemplate("requests/" + string(request_filename)));
  if (passwords_filename.isFound()) {
    ofxget.AddPasswordsForTest(institution, passwords_filename);
  } else {
    ofxget.AddPasswordsForTest(institution, "passwords.txt");
  }

  ofxget.PostRequest();
  if (ofxget.is_error()) {
    cout << "ERROR" << ofxget.error_string() << endl;
  } else {
    cout << "REQUEST" << endl << ofxget.request() << endl;
    cout << "RESPONSE" << endl << endl << ofxget.response() << endl;
  }

  return 0;
}
