#include "libmin.h"

#define NUM_POINTS 50    // Total number of points generated.
                         // Adjust this value to stress the algorithm further.

typedef struct {
    int x;
    int y;
} Point;

// Global pivot for sorting: the point with the lowest y (and x on tie).
Point p0;

// Returns the orientation of the triplet (p, q, r).
//  0  : p, q, r are collinear
// -1  : counterclockwise turn (left turn)
//  1  : clockwise turn (right turn)
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;
    return (val > 0) ? 1 : -1;
}

// Computes the squared Euclidean distance between points p and q.
int distanceSq(Point p, Point q) {
    int dx = q.x - p.x;
    int dy = q.y - p.y;
    return dx * dx + dy * dy;
}

// Comparator for sorting points with respect to global pivot p0.
// Returns a negative number if a comes before b (i.e. a has a smaller polar angle),
// zero if they are collinear (in which case the closer one comes first),
// and a positive number otherwise.
int comparePoints(Point a, Point b) {
    int o = orientation(p0, a, b);
    if (o == 0) {
        // Collinear; sort by distance to p0.
        return distanceSq(p0, a) - distanceSq(p0, b);
    }
    // If a is counterclockwise to b then a comes first.
    return (o < 0) ? -1 : 1;
}

// A simple insertion sort for an array of Points.
void sortPoints(Point points[], int n) {
    for (int i = 1; i < n; i++) {
        Point key = points[i];
        int j = i - 1;
        while (j >= 0 && comparePoints(points[j], key) > 0) {
            points[j + 1] = points[j];
            j--;
        }
        points[j + 1] = key;
    }
}

int main() {
    // Seed the random number generator with a fixed value for reproducibility.
    libmin_srand(42);

    // Generate a set of random points.
    Point points[NUM_POINTS];
    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].x = libmin_rand() % 101;  // x in range [0, 100]
        points[i].y = libmin_rand() % 101;  // y in range [0, 100]
    }

    // Find the pivot: the point with the lowest y-coordinate.
    // On a tie, choose the point with the lowest x-coordinate.
    int minIdx = 0;
    for (int i = 1; i < NUM_POINTS; i++) {
        if (points[i].y < points[minIdx].y ||
           (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
            minIdx = i;
        }
    }
    // Swap the pivot with the first element.
    Point temp = points[0];
    points[0] = points[minIdx];
    points[minIdx] = temp;
    p0 = points[0];  // Set the global pivot.

    libtarg_start_perf();
    // Sort the remaining points according to the polar angle relative to p0.
    sortPoints(points, NUM_POINTS);

    // Allocate an array to store the convex hull points.
    Point hull[NUM_POINTS];
    int hullSize = 0;

    // Apply the Graham scan: iterate through all points.
    for (int i = 0; i < NUM_POINTS; i++) {
        // While the last two points in the hull and the current point do not make
        // a counterclockwise turn, remove the top of the hull.
        while (hullSize >= 2 &&
               orientation(hull[hullSize - 2], hull[hullSize - 1], points[i]) <= 0) {
            hullSize--;  // Pop the last point.
        }
        hull[hullSize++] = points[i];  // Push the current point.
    }
    libtarg_stop_perf();

    // Print the convex hull result.
    libmin_printf("Convex Hull Points (in order):\n");
    for (int i = 0; i < hullSize; i++) {
        libmin_printf("(%d, %d)\n", hull[i].x, hull[i].y);
    }

    // Optionally, compute a checksum (sum of coordinates) and print hull size.
    int checksum = 0;
    for (int i = 0; i < hullSize; i++) {
        checksum += hull[i].x + hull[i].y;
    }
    libmin_printf("Hull size: %d, Checksum: %d\n", hullSize, checksum);

    libtarg_success();
    return 0;
}

