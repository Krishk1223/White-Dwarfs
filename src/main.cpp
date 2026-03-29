#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <filesystem>
#include "config.h"
#include "constants.h"
#include "solver.h"
#include "eqofstate.h"

int simulation(double star_mass, double planet_mass);

struct Body{
    Vector2 position; // x and y positions
    Vector2 velocity; // x and y velocities
    double mass; // mass in kg 
    float radius; // draw size
    Color Colour; 
};

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

    // write Y_E value and close up csv file:
    outfile << "Electron Fraction," << Y_e << "\n";
    outfile.close();
    std::cout << "Data written successfully in ./data/white_dwarfs.csv \n";
    
    // Testing raylib:
    #ifdef HAVE_RAYLIB
        simulation(MSUN, MEARTH);
    #else
        std::cout << "Simulation not built (raylib may be disabled)\n";
    #endif

    return 0;
}


int simulation(double star_mass, double planet_mass) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "White Dwarf Orbit Simulator (stage 1)");

    // Frame rate:
    SetTargetFPS(60);

    // Camera setup: default is left corner so we have to drag to centre
    Camera2D camera = { 0 };
    camera.target = { 0.0f, 0.0f };
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f }; // centre screen
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Bodies that will be in motion:
    Body Star = { {0, 0}, {0, 0}, star_mass, 15.0f, RAYWHITE };
    Body Planet = { {200.0f, 0.0f}, {0.0f, -10.0f}, planet_mass, 8.0f, SKYBLUE };

    while(!WindowShouldClose()) {
        // Motion:
        float dt = GetFrameTime();

        // lets try basic newtonian motion first:
        const double PIXELS_PER_AU = 200.0f;
        const double AU_TO_METRES = 1.49e11f;
        const double SCALE = (PIXELS_PER_AU)/(AU_TO_METRES);

        Vector2 r = Vector2Subtract(Star.position, Planet.position);
        double mag_r = Vector2Length(r) * SCALE;

        //prevent 0 division 
        if (mag_r > 0.1f) {
            float accel_mag = ((G * Star.mass)/(mag_r * mag_r)) / SCALE; // a = GM/r^2
            

            // Directional acceleration:
            Vector2 acceleration = Vector2Scale(Vector2Normalize(r), accel_mag);
            //velocity update:
            Planet.velocity = Vector2Add(Planet.velocity, Vector2Scale(acceleration, dt));
            //position update:
            Planet.position = Vector2Add(Planet.position, Vector2Scale(Planet.velocity, dt));
        } 


        BeginDrawing();
        ClearBackground(BLACK); // make sure to clear to prevent dragging
        BeginMode2D(camera);
            DrawCircleV(Star.position, Star.radius, Star.Colour);
            DrawCircleV(Planet.position, Planet.radius, Planet.Colour);
        EndMode2D();
        DrawText("Linear motion physics no gravity yet", 10, 10, 10, GREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;

}