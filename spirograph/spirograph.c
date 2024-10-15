/**
 * @file
 * @author [Krishna Vedala](https://github.com/kvedala)
 * @brief Implementation of
 * [Spirograph](https://en.wikipedia.org/wiki/Spirograph)
 *
 * @details
 * Implementation of the program is based on the geometry shown in the figure
 * below:
 *
 * <a
 * href="https://commons.wikimedia.org/wiki/File:Resonance_Cascade.svg"><img
 * src="https://upload.wikimedia.org/wikipedia/commons/3/39/Resonance_Cascade.svg"
 * alt="Spirograph geometry from Wikipedia" style="width: 250px"/></a>
 */
#include "libmin.h"

/** Generate spirograph curve into arrays `x` and `y` such that the i^th point
 * in 2D is represented by `(x[i],y[i])`. The generating function is given by:
 * \f{eqnarray*}{
 * x &=& R\left[ (1-k) \cos (t) + l\cdot k\cdot\cos \left(\frac{1-k}{k}t\right)
 * \right]\\
 * y &=& R\left[ (1-k) \sin (t) - l\cdot k\cdot\sin \left(\frac{1-k}{k}t\right)
 * \right] \f}
 * where
 * * \f$R\f$ is the scaling parameter that we will consider \f$=1\f$
 * * \f$l=\frac{\rho}{r}\f$ is the relative distance of marker from the centre
 * of inner circle and \f$0\le l\le1\f$
 * * \f$\rho\f$ is physical distance of marker from centre of inner circle
 * * \f$r\f$ is the radius of inner circle
 * * \f$k=\frac{r}{R}\f$ is the ratio of radius of inner circle to outer circle
 * and \f$0<k<1\f$
 * * \f$R\f$ is the radius of outer circle
 * * \f$t\f$ is the angle of rotation of the point i.e., represents the time
 * parameter
 *
 * Since we are considering ratios, the actual values of \f$r\f$ and
 * \f$R\f$ are immaterial.
 *
 * @param [out] x output array containing absicca of points (must be
 * pre-allocated)
 * @param [out] y output array containing ordinates of points (must be
 * pre-allocated)
 * @param l the relative distance of marker from the centre of
 * inner circle and \f$0\le l\le1\f$
 * @param k the ratio of radius of inner circle to outer circle and
 * \f$0<k<1\f$
 * @param N number of sample points along the trajectory (higher = better
 * resolution but consumes more time and memory)
 * @param num_rot the number of rotations to perform (can be fractional value)
 */
void spirograph(double *x, double *y, double l, double k, size_t N, double rot)
{
    double dt = rot * 2.f * M_PI / N;
    double t = 0.f, R = 1.f;
    const double k1 = 1.f - k;

    for (size_t dk = 0; dk < N; dk++, t += dt)
    {
        x[dk] = R * (k1 * libmin_cos(t) + l * k * libmin_cos(k1 * t / k));
        y[dk] = R * (k1 * libmin_sin(t) - l * k * libmin_sin(k1 * t / k));
    }
}

/**
 * @brief Test function to save resulting points to a CSV file.
 *
 */
void test(void)
{
    size_t N = 500;
    double l = 0.3, k = 0.75, rot = 10.;
    double *x = (double *)libmin_malloc(N * sizeof(double));
    double *y = (double *)libmin_malloc(N * sizeof(double));

    spirograph(x, y, l, k, N, rot);

    for (size_t i = 0; i < N; i++)
    {
        libmin_printf("%.5lf, %.5lf\n", x[i], y[i]);
    }

    libmin_free(x);
    libmin_free(y);
}

/** Main function */
int main(int argc, char **argv)
{
    test();

    libmin_success();
    return 0;
}

