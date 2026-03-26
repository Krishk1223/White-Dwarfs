#include <iostream>
#include <fstream>
#include <cmath>
#include <filesystem>
#include "solver.h"
#include "eqofstate.h"

// testing main function for now:
int main(){
    // CSV file
    std::ofstream outfile("data/white_dwarfs.csv");
    if (!outfile.is_open() ){
        std::cout << "Error: File could not be opened for writing" << std::endl;
        return 1; // failed execution code
    }

    // CSV Headers:
    outfile << "Central_Density(kgm^-3),Mass_MSUN,Radius_RSUN\n";
    std::cout << "Calculating Mass-Radius Data for stars based on varying central densities. \n";
    double rho_0 = EOS::get_rho_0(); 
    
    // for a range of y_central = 0.01 to y = 1,000,000
    for (double log_phi = -2.0; log_phi <= 6.0; log_phi += 0.1) {
        double y_central = std::pow(10.0, log_phi); // y_central = rho_central/rho_0
        double rho_central =  y_central * rho_0;

        // Use RK4 solver to get a radius in terms of solar radii and mass in terms of solar mass
        StarResult result = Solver::solve(y_central);

        // write to csv
        outfile << rho_central << "," << result.M_solar << "," << result.R_solar << "\n";
    }

    // close up csv file:
    outfile.close();
    std::cout << "Data written successfully in ./data/white_dwarfs.csv \n";

    return 0;
}
