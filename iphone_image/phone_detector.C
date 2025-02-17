#include "TASImage.h"
#include "TROOT.h"
#include "TH2.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;


void phone_detector( const char* folder = "cosmic_test", float threshold = 0.01 )
{
  TCanvas* c = new TCanvas("c","",1000,800);

  char tempname[1000];
  sprintf( tempname, ".! ls -x1 %s/* > temp.txt", folder );
  gROOT->ProcessLine(tempname);

  FILE* f = fopen("temp.txt", "r");
  char  line[200];
  int   k = sizeof(line)+1;
  int   nfiles = 0; 
  int   hits = 0; 
  int   thisHit = 0;
  char  fname[200];

  if( f == NULL ) 
    cerr << "Error: can't open input file temp.txt" << endl; 
  else {      
    while( fgets( line, k, f ) != NULL )  // read one line from file
      {
	if( (sscanf( line, "%s", &fname ) == 1 ) ) {
	  nfiles++;
	  
	  TASImage image( fname );
	  UInt_t yPixels = image.GetHeight();
	  UInt_t xPixels = image.GetWidth();
	  UInt_t *argb   = image.GetArgbArray();

	  int thisHit = 0;
	  int rowmax = -1;
	  int colmax = -1;
	  float max  = -1;

	  for ( UInt_t row = 0; row < xPixels; ++row ) {
	    for ( UInt_t col = 0; col < yPixels; ++col ) {
	      UInt_t index = col*xPixels+row;
	      float grey   = float(argb[index]&0xffffff)/16777216;

	      if( grey > threshold ) { 
		cout << "Hit in row " << row << " in column " << (yPixels - col) << "\tof " << grey << " in " << fname << endl;
		hits++;
		thisHit = 1;
		if( grey > max ) {
		  max = grey;
		  rowmax = row;
		  colmax = (yPixels - col);
		}
	      }
	    }
	  }
	  if( thisHit ) {
	    sprintf( tempname, ".x phone_image.C(\"%s\", %d, %d)", fname, rowmax, colmax );
	    gROOT->ProcessLine(tempname);
	  }
	}
      }  
    fclose( f );
  }
  
  cout << endl << "----------------------------------------------------------------" << endl << endl;
  cout << "Total number of hits above threshold  = " << hits << " / " << nfiles << endl << endl;
  cout  << "----------------------------------------------------------------" << endl << endl;
  gROOT->ProcessLine(".! rm temp.txt");

  
}
