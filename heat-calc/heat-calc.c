#include "libmin.h"

#define N      100    // Number of grid points along the rod.
#define STEPS  500   // Number of time steps for the simulation.
#define ALPHA  1.0    // Thermal diffusivity constant.
#define DX     1.0    // Spatial step (distance between grid points).
#define DT     0.1    // Time step (should be small enough for stability).

int main() {
    double u[N];      // Temperature distribution at current time.
    double u_new[N];  // Temperature distribution for the next time step.
    int i, step;

    // Initialize the rod:
    // Set an initial temperature distribution with a single "hot spot" at the center.
    // Boundary conditions: fixed at 0.0 at both ends.
    for (i = 0; i < N; i++) {
        if (i == N / 2)
            u[i] = 100.0;
        else
            u[i] = 0.0;
    }

    libtarg_start_perf();
    // Main time-stepping loop: simulate STEPS time steps.
    for (step = 0; step < STEPS; step++) {
        // Update interior points using the explicit finite difference scheme:
        // u_new[i] = u[i] + DT * ALPHA * (u[i-1] - 2*u[i] + u[i+1]) / (DX*DX)
        for (i = 1; i < N - 1; i++) {
            u_new[i] = u[i] + DT * ALPHA * (u[i - 1] - 2 * u[i] + u[i + 1]) / (DX * DX);
        }
        
        // Copy boundary values (Dirichlet boundary conditions; they remain constant).
        u_new[0] = u[0];
        u_new[N - 1] = u[N - 1];
        
        // Update the current temperature distribution from the newly computed values.
        for (i = 0; i < N; i++) {
            u[i] = u_new[i];
        }
    }

    // Compute a simple checksum (sum of all temperatures) for validation.
    double checksum = 0.0;
    for (i = 0; i < N; i++) {
        checksum += u[i];
    }
    libtarg_stop_perf();

    // Output the final temperature distribution.
    libmin_printf("Final temperature distribution along the rod:\n");
    for (i = 0; i < N; i++) {
        libmin_printf("u[%d] = %.2f\n", i, u[i]);
    }

    libmin_printf("Checksum: %.2f\n", checksum);

    libmin_success();
    return 0;
}

