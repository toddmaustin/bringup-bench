/*
 * ransac_line_fit.c
 *
 * A simple RANSAC application that fits a line (y = m*x + b)
 * to a set of 2D points. The data is generated with a known ground-truth
 * line plus some noise (inliers) along with a number of outlier points.
 *
 * To compile:
 *     gcc ransac_line_fit.c -lm -o ransac_line_fit
 *
 * Then run the executable:
 *     ./ransac_line_fit
 */

#include "libmin.h"

#define NUM_POINTS 100         // Total number of points (inliers + outliers)
#define NUM_ITERATIONS 500    // Number of RANSAC iterations
#define DIST_THRESHOLD 1.0     // Maximum distance (error) for a point to be considered an inlier

typedef struct {
    double x;
    double y;
} Point;

// Compute the perpendicular distance from a point to the line: y = m*x + b.
double line_distance(Point p, double m, double b) {
    return libmin_fabs(m * p.x - p.y + b) / libmin_sqrt(m * m + 1.0);
}

// A simple RANSAC function to fit a line to a set of points.
void ransac_line_fitting(Point points[], int numPoints, double *best_m, double *best_b, int *best_inlier_count) {
    *best_inlier_count = 0;

    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        // Randomly select two distinct points.
        int idx1 = libmin_rand() % numPoints;
        int idx2 = libmin_rand() % numPoints;
        while (idx2 == idx1) {
            idx2 = libmin_rand() % numPoints;
        }
        Point p1 = points[idx1];
        Point p2 = points[idx2];

        // Avoid vertical lines for simplicity; skip if x values are too close.
        if (libmin_fabs(p2.x - p1.x) < 1e-6)
            continue;

        // Compute candidate line parameters.
        double m = (p2.y - p1.y) / (p2.x - p1.x);
        double b = p1.y - m * p1.x;

        // Count inliers: points whose distance to the line is within DIST_THRESHOLD.
        int inlierCount = 0;
        for (int i = 0; i < numPoints; i++) {
            if (line_distance(points[i], m, b) < DIST_THRESHOLD)
                inlierCount++;
        }
        // If this candidate is the best so far, remember its parameters.
        if (inlierCount > *best_inlier_count) {
            *best_inlier_count = inlierCount;
            *best_m = m;
            *best_b = b;
        }
    }
}

int main(void) {
    libmin_srand(42);
    Point points[NUM_POINTS];

    // Generate inlier points along the line: y = 2*x + 1
    int inlierCount = NUM_POINTS / 2;
    for (int i = 0; i < inlierCount; i++) {
        // Spread x values over a range.
        double x = ((double) i / inlierCount) * 50.0;
        // Add a small random noise in the y value.
        double noise = ((double) libmin_rand() / RAND_MAX - 0.5) * 2.0;  // Noise in range [-1, 1]
        points[i].x = x;
        points[i].y = 2 * x + 1 + noise;
    }

    // Generate outlier points randomly.
    for (int i = inlierCount; i < NUM_POINTS; i++) {
        points[i].x = ((double) libmin_rand() / RAND_MAX) * 50.0;
        points[i].y = ((double) libmin_rand() / RAND_MAX) * 100.0;
    }

    double best_m = 0, best_b = 0;
    int best_inlier_count = 0;

    // Run RANSAC to estimate the line parameters.
    ransac_line_fitting(points, NUM_POINTS, &best_m, &best_b, &best_inlier_count);

    // Display the results.
    libmin_printf("RANSAC estimated line: y = %f * x + %f\n", best_m, best_b);
    libmin_printf("Number of inliers: %d / %d\n", best_inlier_count, NUM_POINTS);

    libmin_success();
    return 0;
}

