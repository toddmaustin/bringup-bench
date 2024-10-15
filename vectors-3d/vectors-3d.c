/**
 * @file
 * @brief Functions related to 3D vector operations.
 * @author Krishna Vedala
 */

#include "libmin.h"

/**
 * @addtogroup quaternions Library for 3D Vectors & Quaternions
 * @{
 * @file
 * @brief Generic header that provides data types for 3D vectors and quaternions
 * @author Krishna Vedala
 */

/** Minimum recognizable value. Any value less than this is considered to be
 * @f$=0@f$ */
#define EPSILON 1e-9

/**
 * @addtogroup vec_3d 3D Vector operations
 * @{
 */
/** 3D vector type */
typedef struct vec_3d_
{
    double x; /**< X co-ordinate */
    double y; /**< Y co-ordinate */
    double z; /**< Z co-ordinate */
} vec_3d;
/** @} */

/**
 * @addtogroup matrix Matrix operations
 * @{
 */
/** A 3x3 Matrix type definition */
typedef struct mat_3x3_
{
    union
    { /**< 3 element row 1 */
        double row1[3];
        vec_3d vec1;
    };
    union
    { /**< 3 element row 2 */
        double row2[3];
        vec_3d vec2;
    };
    union
    { /**< 3 element row 3 */
        double row3[3];
        vec_3d vec3;
    };
} mat_3x3;
/** @} */

/** @addtogroup quats 3D Quaternion operations
 * @{
 */
/** a Quaternion type represented using a scalar \f$w\f$ or \f$q_0\f$ and a
 * 3D vector \f$\left(q_1,q_2,q_3\right)\f$
 */
typedef struct quaternion_
{
    union
    {
        double w;  /**< real part of quaternion */
        double q0; /**< real part of quaternion */
    };
    /**< dual part of quaternion */
    union
    {
        vec_3d dual; /**< can be a 3D vector */
        /** or individual values */
        struct
        {
            double q1, q2, q3;
        };
    };
} quaternion;

/** 3D Euler or Tait-Bryan angles (in radian) */
typedef struct euler_
{
    union
    {
        double roll; /**< or bank \f$\phi\f$ = rotation about X axis */
        double bank; /**< or roll \f$\phi\f$ = rotation about X axis */
    };
    union
    {
        double pitch; /**< or elevation \f$\theta\f$ = rotation about Y axis */
        double elevation; /**< or pitch \f$\theta\f$ = rotation about Y axis */
    };
    union
    {
        double yaw;     /**< or heading \f$\psi\f$ = rotation about Z axis */
        double heading; /**< or yaw \f$\psi\f$ = rotation about Z axis */
    };
} euler;

/** @} */

/** @addtogroup dual_quats 3D Dual-Quaternion operations
 * @{
 */
/** a dual quaternion type */
typedef struct dual_quat_
{
    quaternion real; /**< real part of dual quaternion */
    quaternion dual; /**< dual part of dual quaternion */
} dual_quat;

/**
 * @addtogroup vec_3d 3D Vector operations
 * @{
 */

/**
 * Subtract one vector from another. @f[
 * \vec{c}=\vec{a}-\vec{b}=\left(a_x-b_x\right)\hat{i}+
 * \left(a_y-b_y\right)\hat{j}+\left(a_z-b_z\right)\hat{k}@f]
 * @param[in] a vector to subtract from
 * @param[in] b vector to subtract
 * @returns resultant vector
 */
vec_3d vector_sub(const vec_3d *a, const vec_3d *b)
{
    vec_3d out;
    out.x = a->x - b->x;
    out.y = a->y - b->y;
    out.z = a->z - b->z;

    return out;
}

/**
 * Add one vector to another. @f[
 * \vec{c}=\vec{a}+\vec{b}=\left(a_x+b_x\right)\hat{i}+
 * \left(a_y+b_y\right)\hat{j}+\left(a_z+b_z\right)\hat{k}@f]
 * @param[in] a vector to add to
 * @param[in] b vector to add
 * @returns resultant vector
 */
vec_3d vector_add(const vec_3d *a, const vec_3d *b)
{
    vec_3d out;
    out.x = a->x + b->x;
    out.y = a->y + b->y;
    out.z = a->z + b->z;

    return out;
}

/**
 * Obtain the dot product of two 3D vectors.
 * @f[
 * \vec{a}\cdot\vec{b}=a_xb_x + a_yb_y + a_zb_z
 * @f]
 * @param[in] a first vector
 * @param[in] b second vector
 * @returns resulting dot product
 */
double dot_prod(const vec_3d *a, const vec_3d *b)
{
    double dot;
    dot = a->x * b->x;
    dot += a->y * b->y;
    dot += a->z * b->z;

    return dot;
}

/**
 * Compute the vector product of two 3d vectors.
 * @f[\begin{align*}
 * \vec{a}\times\vec{b} &= \begin{vmatrix}
 *  \hat{i} & \hat{j} & \hat{k}\\
 *  a_x & a_y & a_z\\
 *  b_x & b_y & b_z
 *  \end{vmatrix}\\
 *  &= \left(a_yb_z-b_ya_z\right)\hat{i} - \left(a_xb_z-b_xa_z\right)\hat{j}
 * + \left(a_xb_y-b_xa_y\right)\hat{k} \end{align*}
 * @f]
 * @param[in] a first vector @f$\vec{a}@f$
 * @param[in] b second vector @f$\vec{b}@f$
 * @returns resultant vector @f$\vec{o}=\vec{a}\times\vec{b}@f$
 */
vec_3d vector_prod(const vec_3d *a, const vec_3d *b)
{
    vec_3d out;  // better this way to avoid copying results to input
                 // vectors themselves
    out.x = a->y * b->z - a->z * b->y;
    out.y = -a->x * b->z + a->z * b->x;
    out.z = a->x * b->y - a->y * b->x;

    return out;
}

/**
 * Print formatted vector on stdout.
 * @param[in] a vector to print
 * @param[in] name  name of the vector
 * @returns string representation of vector
 */
const char *print_vector(const vec_3d *a, const char *name)
{
    static char vec_str[100];  // static to ensure the string life extends the
                               // life of function

    libmin_snprintf(vec_str, 99, "vec(%s) = (%.0lf)i + (%.0lf)j + (%.0lf)k\n", name, a->x,
             a->y, a->z);
    return vec_str;
}

/**
 * Compute the norm a vector.
 * @f[\lVert\vec{a}\rVert = \sqrt{\vec{a}\cdot\vec{a}} @f]
 * @param[in] a input vector
 * @returns norm of the given vector
 */
double vector_norm(const vec_3d *a)
{
    double n = dot_prod(a, a);
    n = libmin_sqrt(n);

    return n;
}

/**
 * Obtain unit vector in the same direction as given vector.
 * @f[\hat{a}=\frac{\vec{a}}{\lVert\vec{a}\rVert}@f]
 * @param[in] a input vector
 * @returns n unit vector in the direction of @f$\vec{a}@f$
 */
vec_3d unit_vec(const vec_3d *a)
{
    vec_3d n = {0};

    double norm = vector_norm(a);
    if (libmin_fabs(norm) < EPSILON)
    {  // detect possible divide by 0
        return n;
    }

    if (norm != 1.0)  // perform division only if needed
    {
        n.x = a->x / norm;
        n.y = a->y / norm;
        n.z = a->z / norm;
    }
    return n;
}

/**
 * The cross product of vectors can be represented as a matrix
 * multiplication operation. This function obtains the `3x3` matrix
 * of the cross-product operator from the first vector.
 * @f[\begin{align*}
 * \left(\vec{a}\times\right)\vec{b} &= \tilde{A}_a\vec{b}\\
 * \tilde{A}_a &=
 * \begin{bmatrix}0&-a_z&a_y\\a_z&0&-a_x\\-a_y&a_x&0\end{bmatrix}
 * \end{align*}@f]
 * @param[in] a input vector
 * @returns the `3x3` matrix for the cross product operator
 * @f$\left(\vec{a}\times\right)@f$
 */
mat_3x3 get_cross_matrix(const vec_3d *a)
{
    mat_3x3 A = {{{0., -a->z, a->y}}, {{a->z, 0., -a->x}}, {{-a->y, a->x, 0.}}};
    return A;
}

/**
 * Obtain the angle between two given vectors.
 * @f[\alpha=acos\left(\frac{\vec{a} \cdot \vec{b}}{\lVert\vec{a}\rVert \cdot \lVert\vec{b}\rVert}\right)@f]
 * @param[in] a first input vector
 * @param[in] b second input vector
 * @returns angle between @f$\vec{a}@f$ and @f$\vec{b}@f$ in radians
 */

double get_angle(const vec_3d *a, const vec_3d *b)
{
    double alpha, cos_alpha;
    double norm_a = vector_norm(a); ///< The norm of vector a
    double norm_b = vector_norm(b); ///< The norm of vector b
    if (libmin_fabs(norm_a) < EPSILON || libmin_fabs(norm_b) < EPSILON) /// detect possible division by 0 - the angle is not defined in this case
    {  
        return NAN; 
    }

    cos_alpha = dot_prod(a, b) / (norm_a * norm_b);
    alpha = libmin_acos(cos_alpha); // delivers the radian
    return alpha; // in range from -1 to 1
}

/** @} */

/**
 * @brief Testing function
 * @returns `void`
 */
static void test()
{
    vec_3d a = {1., 2., 3.};
    vec_3d b = {1., 1., 1.};
    double d;

    libmin_printf("%s", print_vector(&a, "a"));
    libmin_printf("%s", print_vector(&b, "b"));

    d = vector_norm(&a);
    libmin_printf("|a| = %.4lf\n", d);
    libmin_assert(libmin_fabs(d - 3.742) < 0.01);
    d = vector_norm(&b);
    libmin_printf("|b| = %.4lf\n", d);
    libmin_assert(libmin_fabs(d - 1.732) < 0.01);

    d = dot_prod(&a, &b);
    libmin_printf("Dot product: %lf\n", d);
    libmin_assert(libmin_fabs(d - 6.0) < 0.01);

    vec_3d c = vector_prod(&a, &b);
    libmin_printf("Vector product ");
    libmin_printf("%s", print_vector(&c, "c"));
    libmin_assert(libmin_fabs(c.x - (-1.0)) < 0.01);
    libmin_assert(libmin_fabs(c.y - (2.0)) < 0.01);
    libmin_assert(libmin_fabs(c.z - (-1.0)) < 0.01);

    double alpha = get_angle(&a, &b);
    libmin_printf("The angle is %lf\n", alpha);
    libmin_assert(libmin_fabs(alpha - 0.387597) < 0.01);

    libmin_printf("All tests passed!\n");
}

/**
 * @brief Main function
 *
 * @return 0 on exit
 */
int main(void)
{
    test();

    libmin_success();
    return 0;
}

