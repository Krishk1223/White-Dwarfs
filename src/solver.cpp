#include "solver.h"
#include "constants.h" 
#include "eqofstate.h"
#include "config.h"
#include <cmath>

namespace Solver{

    // Derivatives kept in a struct compute based on nondimensionalised equations.
    struct Derivatives{
        double dmu_dx;
        double dy_dx;
    };

    Derivatives compute_derivatives(const State& s) {
        Derivatives d;

        // const State& s used to avoid multiple copies of state and keep state read only.
        
        /*
        ODE 1: dmu/dx = 3*x^2*y.
        Obtained via (r = R0 x) and (m(r) = (4*pi*rho_0*R0/3) mu) as well as dm/dr = (4*pi*rr^2) rho
        substitued to find expression for dmu/dx. 
        */
       d.dmu_dx = 3.0 * s.x * s.x * s.y;

       /*
        ODE 2: dy/dx = (mu*y)/(x^2 * gamma(y)).
        Obtained via (r = R0 x) and (m(r) = (4*pi*rho_0*R0/3) mu) substituted into
        dP/dr = - G*m(r)*rho(r)/r^2 = (drho/dr)*(dP/drho) where
        dP/drho = (Y_e)*M_E*c^2*gamma(y)/M_P
        giving us R0 when we nondimensionalise.
       */
      if (s.y <= 0 || s.x <=1e-10){ // for small value errors
        d.dy_dx = 0.0;
      }
      else{
        d.dy_dx = -(s.mu * s.y)/(s.x * s.x * EOS::gamma_function(s.y));
      }
      return d;
    }

    State rk4_step(State s, double dx) {
        // k1: (compute derivatives)
        Derivatives k1 = compute_derivatives(s);

        // k2: using k1 (midpoint)
        State s2 = {
            s.x + (0.5 * dx),
            s.mu + (0.5 * dx * k1.dmu_dx),
            s.y + (0.5 * dx * k1.dy_dx)
        };
        Derivatives k2 = compute_derivatives(s2);

        // k3: using k2 (midpoint)
        State s3 = {
            s.x + (0.5 * dx),
            s.mu + (0.5 * dx * k2.dmu_dx),
            s.y + (0.5 * dx * k2.dy_dx)
        };
        Derivatives k3 = compute_derivatives(s3);

        //  k4: using k3 (full length)
        State s4 = {
            s.x + dx,
            s.mu + (dx * k3.dmu_dx),
            s.y + (dx * k3.dy_dx)
        };
        Derivatives k4 = compute_derivatives(s4);

        // next state: 
        State next_state;
        next_state.x = s.x + dx;
        next_state.mu = s.mu + ((dx/6.0) * (k1.dmu_dx + (2.0 * k2.dmu_dx) + (2.0 * k3.dmu_dx) + k4.dmu_dx));
        next_state.y = s.y + ((dx/6.0) * (k1.dy_dx + (2.0 * k2.dy_dx) + (2.0 * k3.dy_dx) + k4.dy_dx));

        return next_state;
    }

    StarResult solve(double y_central) {

        double dx = 1e-4;
        // initial state set up as a bit above 0 to prevent divide by zero errors:
        State current = {
            1e-6,
            0.0,
            y_central
        };
        
        // integrate until density hits 0 (i.e surface)
        while (current.y > 0.0) {
            current = rk4_step(current, dx);
        }

        // Fetch rho_0:
        double rho_0 = EOS::get_rho_0();

        // calculate star radius and mass:
        double R_0 = std::sqrt((3.0 * Y_e * ME * SOL * SOL)/(4 * PI * G * MP * rho_0));
        double M_0 = (4 * PI/3) * rho_0 * (R_0 * R_0 * R_0);

        // return stellar mass and radius as a fraction of solar mass and radius  
        StarResult result;
        result.M_solar = (M_0 * current.mu)/MSUN;
        result.R_solar = (R_0 * current.x)/RSUN;
        
        return result;
        }
}
