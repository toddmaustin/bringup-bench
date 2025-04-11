#include "libmin.h"

#define N_BODIES 3      // Number of particles
#define NUM_STEPS 1000  // Number of simulation steps
#define DT 0.01         // Time step (seconds)
#define G 6.67430e-11   // Gravitational constant (m^3 kg^-1 s^-2)
#define EPS 1e-9        // Softening factor to avoid singularities

typedef struct {
    double mass;
    double pos[3];
    double vel[3];
} Particle;

int main(void) {
    // Initialize three particles with arbitrary masses, positions (in meters), and velocities (in m/s)
    Particle bodies[N_BODIES] = {
        {1e24, {0.0,    0.0,    0.0},    {0.0,    0.0, 0.0}},      // Body 0
        {1e24, {1e8,    0.0,    0.0},    {0.0,  1e3, 0.0}},      // Body 1
        {1e24, {0.0,    1e8,    0.0},    {-1e3,   0.0, 0.0}}       // Body 2
    };

    // Time integration loop using Euler integration
    for (int step = 0; step < NUM_STEPS; step++) {
        // Array to store computed accelerations for each particle
        double acc[N_BODIES][3] = { {0.0} };
        
        // Compute gravitational acceleration for each particle
        for (int i = 0; i < N_BODIES; i++) {
            for (int j = 0; j < N_BODIES; j++) {
                if (i == j) 
                    continue; // Skip self-interaction
                
                // Compute difference vector between particle j and i
                double dx = bodies[j].pos[0] - bodies[i].pos[0];
                double dy = bodies[j].pos[1] - bodies[i].pos[1];
                double dz = bodies[j].pos[2] - bodies[i].pos[2];
                
                // Compute squared distance (with softening factor to avoid division by zero)
                double r2 = dx * dx + dy * dy + dz * dz + EPS;
                double r = libmin_sqrt(r2);
                
                // Compute acceleration magnitude: a = G * m_j / r^2
                // Multiply by (dx, dy, dz)/r to get the vector
                double a = G * bodies[j].mass / r2;
                acc[i][0] += a * (dx / r);
                acc[i][1] += a * (dy / r);
                acc[i][2] += a * (dz / r);
            }
        }
        
        // Update velocities and positions of each particle using the computed accelerations
        for (int i = 0; i < N_BODIES; i++) {
            bodies[i].vel[0] += acc[i][0] * DT;
            bodies[i].vel[1] += acc[i][1] * DT;
            bodies[i].vel[2] += acc[i][2] * DT;
            
            bodies[i].pos[0] += bodies[i].vel[0] * DT;
            bodies[i].pos[1] += bodies[i].vel[1] * DT;
            bodies[i].pos[2] += bodies[i].vel[2] * DT;
        }
    }

    // Print final positions and velocities after the simulation
    libmin_printf("Final state after %d steps:\n", NUM_STEPS);
    for (int i = 0; i < N_BODIES; i++) {
        libmin_printf("Body %d:\n", i);
        libmin_printf("  Position = (%f, %f, %f) m\n", 
               bodies[i].pos[0], bodies[i].pos[1], bodies[i].pos[2]);
        libmin_printf("  Velocity = (%f, %f, %f) m/s\n\n", 
               bodies[i].vel[0], bodies[i].vel[1], bodies[i].vel[2]);
    }
    
    return 0;
}

