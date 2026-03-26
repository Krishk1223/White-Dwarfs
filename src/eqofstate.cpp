#include "eqofstate.h"
#include <cmath>
#include "config.h"
#include "constants.h"

namespace EOS{
    /*
    Calculates gamma factor for dp/drho.
    Formula = gamma(y) = y^(2/3)/(3*sqrt(1 + y^(2/3))
    returns: gamma(y) value
    */
    double gamma_function(double y){
        if (y <= 0.0) return 0.0; // guarding against 0 errors just in case
        double y_23 = std::pow(y, 2.0/3.0);
        return y_23/(3.0 * std::sqrt(1.0 + y_23));
    }

    /*
    Calculates rho_0 value. Depends on Y_e (the electron fraction)
    Formula: rho_0 = 9.79*10^(8) / Y_e 
    returns: rho_0 in kgm^(-3)
    */
    double get_rho_0(){
        double RHO_0_num = (MP * (ME * ME * ME) * (SOL * SOL * SOL))/(3.0 * (PI * PI)  * (HBAR * HBAR * HBAR));
        return RHO_0_num / Y_e;
    }
 }