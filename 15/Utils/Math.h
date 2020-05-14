#pragma once
#include "../SDK/Vector.h"
#include "../SDK/VMatrix.h"
#include <cmath>

namespace Math
{
    void SinCos(float radians, float* sine, float* cosine);
    void VectorAngles(const Vector& forward, Vector& angles);
    void VectorAngles(const Vector& forward, QAngle& angles);
    void AngleVectors(const QAngle& angles, Vector* forward);
    void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
    QAngle CalculateAngle(Vector src, Vector dst);
    void VectorTransform(Vector& in1, matrix3x4_t& in2, Vector& out);
    void ClampAngles(QAngle& angle);
    void NormalizeAngles(QAngle& angle);
    void NormalizeVector(Vector& vec);
    float GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
}
