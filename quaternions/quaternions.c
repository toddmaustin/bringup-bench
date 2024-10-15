/**
 * @file
 * @brief Functions related to 3D quaternions and Euler angles.
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
 * @addtogroup quats 3D Quaternion operations
 * @{
 */

/**
 * Function to convert given Euler angles to a quaternion.
 * \f{eqnarray*}{
 * q_{0} & =
 * &\cos\left(\frac{\phi}{2}\right)\cos\left(\frac{\theta}{2}\right)\cos\left(\frac{\psi}{2}\right)
 * +
 * \sin\left(\frac{\phi}{2}\right)\sin\left(\frac{\theta}{2}\right)\sin\left(\frac{\psi}{2}\right)\\
 * q_{1} & =
 * &\sin\left(\frac{\phi}{2}\right)\cos\left(\frac{\theta}{2}\right)\cos\left(\frac{\psi}{2}\right)
 * -
 * \cos\left(\frac{\phi}{2}\right)\sin\left(\frac{\theta}{2}\right)\sin\left(\frac{\psi}{2}\right)\\
 * q_{2} & =
 * &\cos\left(\frac{\phi}{2}\right)\sin\left(\frac{\theta}{2}\right)\cos\left(\frac{\psi}{2}\right)
 * +
 * \sin\left(\frac{\phi}{2}\right)\cos\left(\frac{\theta}{2}\right)\sin\left(\frac{\psi}{2}\right)\\
 * q_{3} & =
 * &\cos\left(\frac{\phi}{2}\right)\cos\left(\frac{\theta}{2}\right)\sin\left(\frac{\psi}{2}\right)
 * -
 * \sin\left(\frac{\phi}{2}\right)\sin\left(\frac{\theta}{2}\right)\cos\left(\frac{\psi}{2}\right)\\
 * \f}
 *
 * @param [in] in_euler input Euler angles instance
 * @returns converted quaternion
 */
quaternion quat_from_euler(const euler *in_euler)
{
    quaternion out_quat = {{0.0}, {{0.0, 0.0, 0.0}}}; 

    if (!in_euler)  // if null
    {
        libmin_printf("%s: Invalid input.", __func__);
        return out_quat;
    }

    quaternion temp;

    double cy = libmin_cos(in_euler->yaw * 0.5);
    double sy = libmin_sin(in_euler->yaw * 0.5);
    double cp = libmin_cos(in_euler->pitch * 0.5);
    double sp = libmin_sin(in_euler->pitch * 0.5);
    double cr = libmin_cos(in_euler->roll * 0.5);
    double sr = libmin_sin(in_euler->roll * 0.5);

    temp.w = cr * cp * cy + sr * sp * sy;
    temp.q1 = sr * cp * cy - cr * sp * sy;
    temp.q2 = cr * sp * cy + sr * cp * sy;
    temp.q3 = cr * cp * sy - sr * sp * cy;

    return temp;
}

/**
 * Function to convert given quaternion to Euler angles.
 * \f{eqnarray*}{
 * \phi & = &
 * \tan^{-1}\left[\frac{2\left(q_0q_1+q_2q_3\right)}{1-2\left(q_1^2+q_2^2\right)}\right]\\
 * \theta & =
 * &-\sin^{-1}\left[2\left(q_0q_2-q_3q_1\right)\right]\\
 * \psi & = &
 * \tan^{-1}\left[\frac{2\left(q_0q_3+q_1q_2\right)}{1-2\left(q_2^2+q_3^2\right)}\right]\\
 * \f}
 *
 * @param [in] in_quat input quaternion instance
 * @returns converted euler angles
 */
euler euler_from_quat(const quaternion *in_quat)
{
    euler out_euler = {{0.0}, {0.0}, {0.0}};
    if (!in_quat)  // if null
    {
        libmin_printf("%s: Invalid input.", __func__);
        return out_euler;
    }

    out_euler.roll = libmin_atan2(
        2.0 * (in_quat->w * in_quat->q1 + in_quat->q2 * in_quat->q3),
        1.0 - 2.0 * (in_quat->q1 * in_quat->q1 + in_quat->q2 * in_quat->q2));
    out_euler.pitch =
        libmin_asin(2.0 * (in_quat->w * in_quat->q2 + in_quat->q1 * in_quat->q3));
    out_euler.yaw = libmin_atan2(
        2.0 * (in_quat->w * in_quat->q3 + in_quat->q1 * in_quat->q2),
        1.0 - 2.0 * (in_quat->q2 * in_quat->q2 + in_quat->q3 * in_quat->q3));

    return out_euler;
}

/**
 * Function to multiply two quaternions.
 * \f{eqnarray*}{
 * \mathbf{c} & = & \mathbf{a}\otimes\mathbf{b}\\
 * & = & \begin{bmatrix}a_{0} & a_{1} & a_{2} &
 *  a_{3}\end{bmatrix}\otimes\begin{bmatrix}b_{0} & b_{1} & b_{2} &
 *  b_{3}\end{bmatrix}\\
 * & = &
 * \begin{bmatrix}
 *  a_{0}b_{0}-a_{1}b_{1}-a_{2}b_{2}-a_{3}b_{3}\\
 *  a_{0}b_{1}+a_{1}b_{0}+a_{2}b_{3}-a_{3}b_{2}\\
 *  a_{0}b_{2}-a_{1}b_{3}+a_{2}b_{0}+a_{3}b_{1}\\
 *  a_{0}b_{3}+a_{1}b_{2}-a_{2}b_{1}+a_{3}b_{0}
 * \end{bmatrix}^{T}
 * \f}
 *
 * @param [in] in_quat1 first input quaternion instance
 * @param [in] in_quat2 second input quaternion instance
 * @returns resultant quaternion
 */
quaternion quaternion_multiply(const quaternion *in_quat1,
                               const quaternion *in_quat2)
{
    quaternion out_quat = {{0.0}, {{0.0, 0.0, 0.0}}}; 
    if (!in_quat1 || !in_quat2)  // if null
    {
        libmin_printf("%s: Invalid input.", __func__);
        return out_quat;
    }

    out_quat.w = in_quat1->w * in_quat2->w - in_quat1->q1 * in_quat2->q1 -
                 in_quat1->q2 * in_quat2->q2 - in_quat1->q3 * in_quat2->q3;
    out_quat.q1 = in_quat1->w * in_quat2->q1 + in_quat1->q1 * in_quat2->w +
                  in_quat1->q2 * in_quat2->q3 - in_quat1->q3 * in_quat2->q2;
    out_quat.q2 = in_quat1->w * in_quat2->q2 - in_quat1->q1 * in_quat2->q3 +
                  in_quat1->q2 * in_quat2->w + in_quat1->q3 * in_quat2->q1;
    out_quat.q3 = in_quat1->w * in_quat2->q3 + in_quat1->q1 * in_quat2->q2 -
                  in_quat1->q2 * in_quat2->q1 + in_quat1->q3 * in_quat2->w;

    return out_quat;
}

/** @} */

static void test()
{
    quaternion quat = {{0.7071}, {{0.7071, 0.0, 0.0}}};
    euler eul = euler_from_quat(&quat);
    libmin_printf("Euler: %.4lf, %.4lf, %.4lf\n", eul.pitch, eul.roll, eul.yaw);

    quaternion test_quat = quat_from_euler(&eul);
    libmin_printf("Quaternion: %.4lf %+.4lf %+.4lf %+.4lf\n", test_quat.w,
           test_quat.dual.x, test_quat.dual.y, test_quat.dual.z);

    libmin_assert(libmin_fabs(test_quat.w - quat.w) < .01);
    libmin_assert(libmin_fabs(test_quat.q1 - quat.q1) < .01);
    libmin_assert(libmin_fabs(test_quat.q2 - quat.q2) < .01);
    libmin_assert(libmin_fabs(test_quat.q3 - quat.q3) < .01);
    libmin_printf("All tests passed!\n");
}

int main()
{
    test();

    libmin_success();
    return 0;
}
