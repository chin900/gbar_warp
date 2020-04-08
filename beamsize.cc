#include <stdio.h>
#include "TMath.h"
void beamsize() {
  TString address = "/home/bclee/Analysis/Data_warp/shchoi/zposition_particle_data/";
	
  double ex[nDat], ey[nDat];
  double z[nDat];
  int nDat = 228;
  for (int id = 0; id < nDat; id++) {
    cout<<"Progress(%) : "<<100*id/nDat<<"\%\r";
    cout.flush();
    TH1D *h[2];
    for (int i = 0; i < 2; i++) h[i] = new TH1D(Form("h_%d%d", id, i), Form("h_%d%d", id, i), 2000, -1e-2, 1e-2);
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
	for (int ih = 0; ih < 2; ih++) {
	  h[ih]->Fill(val[ih+1]);
	}
      }
      if (feof(fp)) break;
    }
    fclose(fp);
    if(h[0]->GetEntries()!=0) ex[id]=1000*h[0]->GetStdDev();
    else ex[id]=ex[id-1], cout<<0.01 * (3 + id)<<endl;
    if(h[1]->GetEntries()!=0) ey[id]=1000*h[1]->GetStdDev();
    else ey[id]=ey[id-1];
  } // id

  TGraph *gx = new TGraph(nDat,z,ex);
  TGraph *gy = new TGraph(nDat,z,ey);

  TCanvas* c = new TCanvas("c","c", 1600, 600);
  c->Divide(2, 1);
  c->cd(1);
  gx->Draw();
  c->cd(2);
  gy->Draw();

  cout<<"sigma x at "<<z[227]<<"m : "<<ex[227]<<endl;
  cout<<"sigma y at "<<z[227]<<"m : "<<ey[227]<<endl;
}
