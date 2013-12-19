#include <string>
#include <cstdio>
#include <iostream>
#include <dirent.h>
#include <algorithm>

#include "TFile.h"
#include "TH1I.h"
#include "TGraph.h"

using namespace std;

int main(int argc, char* argv[]) {

  string full_output = "entries.tex";
  FILE* file_out = fopen(full_output.c_str(), "w");

  DIR *pDIR;
  struct dirent *entry;
  if( pDIR=opendir("../PatternBanks/BE_5D/Eta6_Phi8/") ) {
    while(entry = readdir(pDIR)) {
      if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ) {
        string name(entry->d_name);
        size_t found = name.find(".root");
        if (found != string::npos) {

          TFile * input = new TFile(name.c_str(), "READ");

          TH1I * h = (TH1I*)input->Get("PT sector");
          int entries = h->GetEntries();

          size_t found = name.find("_report");
          name.erase(found);
          found = name.find("_low_");
          name.erase(0, found+5);
          replace(name.begin(), name.end(), '_', ':');
          fprintf(file_out, "\\vdef{%s} {\\ensuremath{{%d } } }\n", name.c_str(), entries);

          input->Close();
          delete input;
        }
      }

    }
  }
  closedir(pDIR);

  fclose(file_out);
  system(Form("cat %s", full_output.c_str()));


 return EXIT_SUCCESS;
}
