// eqofstate.h:
#pragma once


namespace EOS{
    /*
    Calculates gamma factor for dp/drho.
    Formula = gamma(y) = y^(2/3)/(3*sqrt(1 + y^(2/3))
    returns: gamma(y) value
    */
    double gamma_function(double y);

    /*
    Calculates rho_0 value. Depends on Y_e (the electron fraction)
    Formula: rho_0 = 9.79*10^(8) / Y_e 
    returns: rho_0 in kgm^(-3)
    */
    double get_rho_0();
 }

