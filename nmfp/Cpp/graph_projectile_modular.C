#include <ostream>
#include <TMath.h>

int iStep, maxStep = 1000000;   // Maximum number of steps
double *xplot, *yplot, *xNoAir, *yNoAir, *ttime;

void graph_projectile_modular() {

   //
   //Modified: Edward J. Brash
   //		
  

  //* Set initial position and velocity of the baseball
  double y1, speed, theta;
  double r1[2+1], v1[2+1], r[2+1], v[2+1], accel[2+1]; 
  cout << "Enter initial height (meters): "; cin >> y1;
  r1[1] = 0;  r1[2] = y1;     // Initial vector position
  cout << "Enter initial speed (m/s): "; cin >> speed; 
  cout << "Enter initial angle (degrees): "; cin >> theta;
  const double pi = 3.141592654; 
  v1[1] = speed*cos(theta*pi/180.);   // Initial velocity (x)
  v1[2] = speed*sin(theta*pi/180.);   // Initial velocity (y)
  r[1] = r1[1];  r[2] = r1[2];  // Set initial position and velocity
  v[1] = v1[1];  v[2] = v1[2];             

  //* Set physical parameters (mass, Cd, etc.)
  double Cd = 0.35;      // Drag coefficient (dimensionless)
  double area = 0.004359;  // Cross-sectional area of projectile (m^2)
  double grav = 9.81;    // Gravitational acceleration (m/s^2)
  double mass = 0.145;   // Mass of projectile (kg)
  double airFlag, rho;
  cout << "Air resistance? (Yes:1, No:0): "; cin >> airFlag;
  if( airFlag == 0 )
    rho = 0;      // No air resistance
  else
    rho = 1.2;    // Density of air (kg/m^3)
  double air_const = -0.5*Cd*rho*area/mass;  // Air resistance constant

  //* Loop until ball hits ground or max steps completed
  double tau;
  cout << "Enter timestep, delta_t (sec): "; cin >> tau;
  xplot = new double [maxStep+1];  yplot = new double [maxStep+1];
  xNoAir = new double [maxStep+1]; yNoAir = new double [maxStep+1];
  ttime = new double [maxStep+1];

  for( iStep=1; iStep<=maxStep; iStep++ ) {

    //* Record position (computed and theoretical) for plotting
    xplot[iStep] = r[1];   // Record trajectory for plot
    yplot[iStep] = r[2];
    double t = (iStep-1)*tau;     // Current time
    xNoAir[iStep] = r1[1] + v1[1]*t;
    yNoAir[iStep] = r1[2] + v1[2]*t - 0.5*grav*t*t;
  
    //* Calculate the acceleration of the ball 
    double normV = sqrt( v[1]*v[1] + v[2]*v[2] );
    accel[1] = air_const*normV*v[1];   // Air resistance
    accel[2] = air_const*normV*v[2]-grav;   // Air resistance - Gravity
  
    //* Calculate the new position and velocity using Euler method
    r[1] += tau*v[1];                 		// Euler step
    r[2] += tau*v[2];                 
    //r[1] += tau*v[1]+0.5*accel[1]*tau*tau;      // Midpoint step
    //r[2] += tau*v[2]+0.5*accel[2]*tau*tau;                 
    v[1] += tau*accel[1];     
    v[2] += tau*accel[2];     

    //* Fill time array
    ttime[iStep]=tau*(iStep-1);
  
    //* If ball reaches ground (y<0), break out of the loop
    if( r[2] < 0 )  {
          xplot[iStep+1] = r[1];  // Record last values computed
	  yplot[iStep+1] = r[2];
	  ttime[iStep+1] = tau*iStep;
          break;                  // Break out of the for loop
    } 
  }

  //* Print maximum range and time of flight

  cout << "Maximum range is " << r[1] << " meters" << endl;
  cout << "Time of flight is " << iStep*tau << " seconds" << endl;

}

TCanvas *plot_xy(){
  
   Double_t max_x = TMath::MaxElement(iStep,xNoAir)+1;
   Double_t max_y = TMath::MaxElement(iStep,yNoAir)+1;
 
   TCanvas *c1 = new TCanvas("c1","XY Projectile",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
  
   gStyle->SetOptFit(1);	
   const Int_t n=iStep;
   TGraph *gr = new TGraph(n,xplot,yplot);
   TGraph *grr = new TGraph(n,xNoAir,yNoAir);
   
   gr->SetMarkerColor(kBlue);
   grr->SetMarkerColor(kRed);
   gr->SetMarkerStyle(21);
   grr->SetMarkerStyle(22);
   grr->SetTitle("Projectile XY");
   grr->GetXaxis()->SetTitle("X");
   grr->GetYaxis()->SetTitle("Y");
   
   TAxis *axis = grr->GetXaxis();
   axis->SetLimits(-0.1*max_x,1.1*max_x);
   grr->GetHistogram()->SetMaximum(1.1*max_y);
   grr->GetHistogram()->SetMinimum(-0.1*max_y);

   c1->cd();
   grr->Draw("AP");
   gr->Draw("P");

   // TCanvas::Update() draws the frame, after which one can change it
   c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();
   
   return c1;
}

TCanvas *plot_xt(){

   Double_t max_x = TMath::MaxElement(iStep,xNoAir)+1;
   Double_t max_t = TMath::MaxElement(iStep,ttime)+1;
   
   TCanvas *c2 = new TCanvas("c2","XT Projectile",300,110,700,500);
   c2->SetFillColor(42);
   c2->SetGrid();
   
   gStyle->SetOptFit(1);	
   const Int_t n=iStep;
   TGraph *gxt = new TGraph(n,ttime,xplot);
   TGraph *gxtt = new TGraph(n,ttime,xNoAir);

   gxt->SetMarkerColor(kBlue);
   gxtt->SetMarkerColor(kRed);
   gxt->SetMarkerStyle(21);
   gxtt->SetMarkerStyle(22);
   gxtt->SetTitle("Projectile XT");
   gxtt->GetXaxis()->SetTitle("X");
   gxtt->GetYaxis()->SetTitle("T");
   
   TAxis *axisx = gxtt->GetXaxis();
   axisx->SetLimits(-0.1*max_t,1.1*max_t);
   gxtt->GetHistogram()->SetMaximum(1.1*max_x);
   gxtt->GetHistogram()->SetMinimum(-0.1*max_x);

   c2->cd();
   gxtt->Draw("AP");
   gxt->Draw("P");
  
   // TCanvas::Update() draws the frame, after which one can change it
   c2->Update();
   c2->GetFrame()->SetFillColor(21);
   c2->GetFrame()->SetBorderSize(12);
   c2->Modified();
   
   return c2;
}

TCanvas *plot_yt(){

   Double_t max_y = TMath::MaxElement(iStep,yNoAir)+1;
   Double_t max_t = TMath::MaxElement(iStep,ttime)+1;
   
   TCanvas *c3 = new TCanvas("c3","YT Projectile",400,210,700,500);
   c3->SetFillColor(42);
   c3->SetGrid();

   gStyle->SetOptFit(1);	
   const Int_t n=iStep;
   TGraph *gyt = new TGraph(n,ttime,yplot);
   TGraph *gytt = new TGraph(n,ttime,yNoAir);

   gyt->SetMarkerColor(kBlue);
   gytt->SetMarkerColor(kRed);
   gyt->SetMarkerStyle(21);
   gytt->SetMarkerStyle(22);
   gytt->SetTitle("Projectile YT");
   gytt->GetXaxis()->SetTitle("Y");
   gytt->GetYaxis()->SetTitle("T");
   
   TAxis *axisy = gytt->GetXaxis();
   axisy->SetLimits(-0.1*max_t,1.1*max_t);
   gytt->GetHistogram()->SetMaximum(1.1*max_y);
   gytt->GetHistogram()->SetMinimum(-0.1*max_y);

   c3->cd();
   gytt->Draw("AP");
   gyt->Draw("P");
   
   // TCanvas::Update() draws the frame, after which one can change it
   c3->Update();
   c3->GetFrame()->SetFillColor(21);
   c3->GetFrame()->SetBorderSize(12);
   c3->Modified();

   return c3;
}
