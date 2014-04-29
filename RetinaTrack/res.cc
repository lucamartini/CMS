/*
 * res.cc
 *
 *  Created on: 28/apr/2014
 *      Author: lucamartini
 */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"

using namespace std;

struct row {
  int xm;
  int xp;
  double z_res;
  double eta_res;
  double z_res_err;
  double eta_res_err;
};

int main(int argc, char* argv[]) {

  ifstream infile("z_eta_resolutions.csv");

  vector <struct row> rows;

  for (string line; getline(infile, line); )  {
      std::cout << line << endl;
      if (line.at(0) == '#') continue;
      struct row row_i;


      string s_temp = line;
      size_t pos = s_temp.find(',');
      row_i.xm = stoi(s_temp.substr(0, pos));

      s_temp = s_temp.substr(pos + 1);
      pos = s_temp.find(',');
      row_i.xp = stoi(s_temp.substr(0, pos));

      s_temp = s_temp.substr(pos + 1);
      pos = s_temp.find(',');
      row_i.z_res = stod(s_temp.substr(0, pos));

      s_temp = s_temp.substr(pos + 1);
      pos = s_temp.find(',');
      row_i.z_res_err = stod(s_temp.substr(0, pos));

      s_temp = s_temp.substr(pos + 1);
      pos = s_temp.find(',');
      row_i.eta_res = stod(s_temp.substr(0, pos));

      s_temp = s_temp.substr(pos + 1);
      pos = s_temp.find(',');
      row_i.eta_res_err = stod(s_temp.substr(0, pos));

      rows.push_back(row_i);
  }

	TGraphErrors graph_z(rows.size());
	TGraphErrors graph_eta(rows.size());

	for (size_t i = 0; i < rows.size(); i++) {
		double cells = rows[i].xm*rows[i].xp;
		graph_z.SetPoint(i, cells, rows[i].z_res);
		graph_z.SetPointError(i, 0., rows[i].z_res_err);

		graph_eta.SetPoint(i, cells, rows[i].eta_res);
		graph_eta.SetPointError(i, 0., rows[i].eta_res_err);
	}

	gStyle->SetOptFit();




	TCanvas c_z("c_z", "c_z", 600, 600);
	TF1 * fit_z = new TF1("hyp_z", "[0]/([1] + [2]*x) + [3]");
		fit_z->SetParameters(1., 0., 1., 0.);
	graph_z.Fit("hyp_z");
	graph_z.Draw("A*");
	c_z.Print("figs/z_res.pdf");

	TCanvas c_eta("c_eta", "c_eta", 600, 600);
	TF1 * fit_eta = new TF1("hyp_eta", "[0]/([1] + [2]*x) + [3]");
	fit_eta->SetParameters(1., 0., 1., 0.);
	graph_eta.Fit("hyp_eta");
	graph_eta.Draw("A*");
	c_eta.Print("figs/eta_res.pdf");

	return 1;
}

