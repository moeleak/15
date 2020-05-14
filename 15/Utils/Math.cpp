#include "Math.h"

void Math::SinCos(float radians, float *sine, float *cosine)
{
    *sine = sin(radians);
    *cosine = cos(radians);
}

void Math::VectorAngles(const Vector& forward, Vector& angles)
{
    if (forward[1] == 0.0f && forward[0] == 0.0f)
    {
        angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
        angles[1] = 0.0f;  //yaw left/right
    }
    else
    {
        angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
        angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

        if (angles[1] > 90) angles[1] -= 180;
        else if (angles[1] < 90) angles[1] += 180;
        else if (angles[1] == 90) angles[1] = 0;
    }

    angles[2] = 0.0f;
}

void Math::VectorAngles(const Vector& forward, QAngle& angles)
{
    if (forward[1] == 0.0f && forward[0] == 0.0f)
    {
        angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
        angles[1] = 0.0f;  //yaw left/right
    }
    else
    {
        angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
        angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

        if (angles[1] > 90) angles[1] -= 180;
        else if (angles[1] < 90) angles[1] += 180;
        else if (angles[1] == 90) angles[1] = 0;
    }

    angles[2] = 0.0f;
}

void Math::AngleVectors(const QAngle &angles, Vector *forward)
{
    float sp, sy, cp, cy;

    SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
    SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

    forward->x = cp * cy;
    forward->y = cp * sy;
    forward->z = -sp;

}

void Math::AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
    float sr, sp, sy, cr, cp, cy;

    SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
    SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
    SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

    if (forward)
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }

    if (right)
    {
        right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
        right->y = (-1 * sr*sp*sy + -1 * cr*cy);
        right->z = -1 * sr*cp;
    }

    if (up)
    {
        up->x = (cr*sp*cy + -sr * -sy);
        up->y = (cr*sp*sy + -sr * cy);
        up->z = cr * cp;
    }
}

QAngle Math::CalculateAngle(Vector src, Vector dst)
{
    QAngle angles;
    Vector delta = src - dst;
    Math::VectorAngles(delta, angles);
    delta.Normalize();
    return angles;
}

void Math::VectorTransform(Vector& in1, matrix3x4_t& in2, Vector& out)
{
    auto dot_product = [&](Vector& v, matrix3x4_t& m, int i)
    {
        return(v.x * m[i][0] + v.y * m[i][1] + v.z * m[i][2]);
    };

    out.x = dot_product(in1, in2, 0) + in2[0][3];
    out.y = dot_product(in1, in2, 1) + in2[1][3];
    out.z = dot_product(in1, in2, 2) + in2[2][3];
}

void Math::ClampAngles(QAngle& angle)
{
    if (angle.y > 180.0f) angle.y = 180.0f;
    else if (angle.y < -180.0f) angle.y = -180.0f;

    if (angle.x > 89.0f) angle.x = 89.0f;
    else if (angle.x < -89.0f) angle.x = -89.0f;

    angle.z = 0;
}

void Math::NormalizeAngles(QAngle& angle)
{
    for (auto i = 0; i < 3; i++) 
    {
        while (angle[i] < -180.0f) angle[i] += 360.0f;
        while (angle[i] >  180.0f) angle[i] -= 360.0f;
    }
}

void Math::NormalizeVector(Vector& vec)
{
    for (auto i = 0; i < 3; i++) 
    {
        while (vec[i] < -180.0f) vec[i] += 360.0f;
        while (vec[i] >  180.0f) vec[i] -= 360.0f;
    }
    vec[2] = 0.f;
}

float Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
    QAngle delta = aimAngle - viewAngle;
    Math::NormalizeAngles(delta);
    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}
