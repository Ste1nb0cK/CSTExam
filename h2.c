// hydrogen atom with the metropolis method
//
//        h = -1/2 \nabla^2 - 1/r, psi=exp(-\beta r)
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// const double DELTA = 2.4;

double eucliddistance(double x, double y, double z);
double lnpsi_(double beta, double r);
double epot_(double beta, double r);
double ekin_(double beta, double r);
void updateproposedmove(double *rep, double *re, double delta);
void updateposition(double *re, double *rep);

int main (int argc, char ** argv){
  int i,n, seed;
  double re[3]; //position of the electron
  double rep[3]; //proposed position 
  double delta, r,rp,p,beta,lnpsi,lnpsip,ekin,epot,etot;
  seed = 1;
  // srand48((unsigned)time(NULL));        // initialization of rng
  srand48(seed);        // initialization of rng

  // n=10000;                              // number of monte carlo steps
  beta = atof(argv[1]);                 // variational parameter
  n = atoi(argv[2]);
  delta = atof(argv[3]);                 // variational parameter
  re[1]=10.0;                               // initial position x
  re[2]=0.1;                                // initial position y
  re[3]=0.01;                               // initial position z
  r = eucliddistance(re[1], re[2], re[3]);
  lnpsi=lnpsi_(beta, r);                        // log psi
  epot=epot_(beta, r);
  ekin=ekin_(beta, r);
  etot=epot+ekin;

  for (i=0; i<n ; i++) {
    updateproposedmove(rep, re, delta);
    rp=eucliddistance(rep[1], rep[2], rep[3]);
    lnpsip=lnpsi_(beta, rp);                    // log psi
    p=fmin(1.0,exp(2*(lnpsip-lnpsi)));  // probability to accept the move
    if(p>drand48()){
      updateposition(re, rep);
      r=rp;                             //  * and log psi               *
      lnpsi=lnpsip;                     //  *****************************
      epot=epot_(beta, r);
      ekin=ekin_(beta, r);
      etot=epot+ekin;
    }
    printf("%f %f %f %f %f\n",r,epot,ekin,etot,p);
  }
}

void updateposition(double *re, double *rep){
      re[1]=rep[1];
      re[2]=rep[2];
      re[3]=rep[3];
}

void updateproposedmove(double *rep, double *re, double delta){
    rep[1]=re[1]+delta*(drand48()-0.5);
    rep[2]=re[2]+delta*(drand48()-0.5);        // distance e-n at (xp,yp,zp)
    rep[3]=re[3]+delta*(drand48()-0.5);
                       }

double eucliddistance(double x, double y, double z){
   return sqrt(x*x+y*y+z*z);                  // distance e-n at (x,y,z)
}

double lnpsi_(double beta, double r){
  return -beta*r;                        // log psi
}

double epot_(double beta, double r){
  return -1.0/r;
}

double ekin_(double beta, double r){
  return -0.5*beta/r + beta*beta;
}
