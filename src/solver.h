// solver.h header file
#pragma once

struct State{
    double x; // dimensionless radius x. r = R0*x where R0 = sqrt(3*Y_e*M_E*c^2/4*PI*G*M_P*rho_0)
    double mu; // dimensionless mass mu. m = M0*mu where M0 = (4*pi*rho_0*R0/3) 
    double y; // dimensionless density (rho/rho_0) y. rho = rho_0*y where rho_0 = M_P*M_E^3*C^3/3*(PI)^2*(HBAR)^3
};

struct StarResult{
    double M_solar; // mass in solar masses
    double R_solar; // radius in solar radii
};

namespace Solver{
    State rk4_step(State s, double dx); // dx is an infinitesimal radius step
    StarResult solve(double y_central); // y_c is central dimensionless density
}


