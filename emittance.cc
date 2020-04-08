#include <stdio.h>
#include "TMath.h"
void emittance() {
  int nDat = 228;
  double s[2][nDat];
  double z[nDat];

  TString address = "/home/bclee/Analysis/Data_warp/hep2011/zposition_particle_data/";
  for (int id = 0; id < nDat; id++) {
    cout<<"Progress(%) : "<<100*id/nDat<<"\%\r";
    cout.flush();
    TH1D *h[2][3];
    for (int i = 0; i < 2; i++){
      for(int j = 0; j < 3; j++)
	h[i][j] = new TH1D(Form("h_%d%d%d", id, i, j), Form("h_%d%d%d", id, i, j), 1000, 0, 1e-4);
    } // i
    z[id]=0.01 * (3 + id);

    TString fname = Form("z_particle_data_%1.5lfm.txt", 0.01 * (3 + id));
    FILE* fp;
    fp = fopen(address + fname, "r");
    if (fp == NULL) {
      cout << "Error : failed to open file " << 0.01 * (3 + id) << endl;
      exit(0);
    }
    char dummy[255];
    fscanf(fp, "%s	%s	%s	%s	%s	%s	%s", dummy, dummy, dummy, dummy, dummy, dummy, dummy);
    double val[7];
    while (1) {
      if (!fscanf(fp, "%lf	%lf	%lf	%lf	%lf	%lf	%lf", &val[0], &val[1], &val[2], &val[3], &val[4], &val[5], &val[6])) {
	cout << "Error : failed to open file " << 0.01 * (3 + id) << endl;
	exit(0);
      }
      else {
	for (int i = 0; i < 2; i++) {
	  h[i][0]->Fill(val[i+1]*val[i+1]);
	  h[i][1]->Fill(val[i+4]*val[i+4]/(val[6]*val[6]));
	  h[i][2]->Fill(val[i+1]*val[i+4]/val[6]);
	}
      }
      if (feof(fp)) break;
    }
    fclose(fp);

    for (int i = 0; i < 2; i++) {
      double tmp1 = h[i][0]->GetMean();
      double tmp2 = h[i][1]->GetMean();
      double tmp3 = h[i][2]->GetMean();
      if(h[i][0]->GetEntries()!=0) s[i][id]=1e6*TMath::Sqrt(TMath::Abs(tmp1*tmp2-tmp3*tmp3));
      else s[i][id]=s[i][id-1], cout<<0.01 * (3 + id)<<endl;
    }

  } // id

  TGraph *gx = new TGraph(nDat,z,s[0]);
  TGraph *gy = new TGraph(nDat,z,s[1]);

  TCanvas* c = new TCanvas("c","c", 1600, 600);
  c->Divide(2, 1);
  c->cd(1);
  gx->Draw();
  c->cd(2);
  gy->Draw();

  cout<<"emmitance x at "<<z[67]<<"m : "<<s[0][67]<<endl;
  cout<<"emmitance x at "<<z[227]<<"m : "<<s[0][227]<<endl;
  cout<<"emmitance y at "<<z[227]<<"m : "<<s[1][227]<<endl;

}
