// @(#)root/eve:$Id: triangleset.C 26876 2008-12-12 14:45:40Z matevz $
// Author: Matevz Tadel

// Demonstrates usage of class TEveTriangleSet.

#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TError.h"

class TEveTriangleSet;

TEveTriangleSet *ts1=0, *ts2=0, *ts3=0;

void triangleset()
{
   TEveManager::Create();

   {
      ts1 = TEveTriangleSet::ReadTrivialFile("broken_torus.tring");
      ts1->SetName("RandomColors");
      ts1->GenerateTriangleNormals();
      ts1->GenerateRandomColors();
      ts1->SetMainColor(0);
      TGeoHMatrix m;
      Double_t scale[3] = { 0.5, 0.5, 0.5 };
      m.SetScale(scale);
      ts1->SetTransMatrix(m);
      gEve->AddElement(ts1);
   }
   {
      ts2 = TEveTriangleSet::ReadTrivialFile("broken_torus.tring");
      ts2->SetName("SmallBlue");
      ts2->GenerateTriangleNormals();
      ts2->SetMainColor(4);
      TGeoHMatrix m;
      m.RotateY(90);
      Double_t scale[3] = { 0.8, 0.8, 1.2 };
      m.SetScale(scale);
      ts2->SetTransMatrix(m);
      gEve->AddElement(ts2);
   }
   {
      ts3 = TEveTriangleSet::ReadTrivialFile("broken_torus.tring");
      ts3->SetName("Spectrum");
      ts3->GenerateTriangleNormals();
      gStyle->SetPalette(1, 0);
      ts3->GenerateZNormalColors(50, -50, 50, kTRUE, kTRUE);
      ts3->SetMainColor(0);
      TGeoHMatrix m;
      m.RotateZ(90);
      Double_t scale[3] = { 1.3, 1.0, 1.6 };
      m.SetScale(scale);
      ts3->SetTransMatrix(m);
      gEve->AddElement(ts3);
   }

   gEve->Redraw3D(kTRUE);
}
