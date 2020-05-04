//
// 2015.8.19
// Mugichoko
//
// Ref.: http://markun.cs.shinshu-u.ac.jp/learn/cg/cg3/index5.html
//
#ifndef CATMULLROMSPLINE_H
#define CATMULLROMSPLINE_H
#include <iostream>
using namespace std;
static const float power = 0.5f;
template<typename vec2D>
class CatmullRomSpline
{
private:
    vector<vec2D> m_vData;
    float calcVal(float x0, float x1, float v0, float v1, float t)
    {
        return (2.0f * x0 - 2.0f * x1 + v0 + v1) * t * t * t
            + (-3.0f * x0 + 3.0f * x1 - 2.0f * v0 - v1) * t * t
            + v0 * t + x0;
    }
public:
    CatmullRomSpline(
        const vector<vec2D> &vData = vector<vec2D>()
        )
    {
        // Error handling
        const unsigned int n = unsigned int(vData.size());
        assert(n >= 2);
        // Copy the data
        this->m_vData.resize(n + 2);
        copy(vData.begin(), vData.end(), this->m_vData.begin() + 1);
        this->m_vData[0] = vData[0];
        this->m_vData[this->m_vData.size() - 1] = vData[vData.size() - 1];
    }
    ~CatmullRomSpline()
    {
    }
    vec2D getValue(
        unsigned int idx,
        float t)
    {
        // Error handling
        assert(t >= 0.0f && t <= 1.0f);
        assert(idx < this->m_vData.size());
        vec2D pos;
        vec2D &p1 = this->m_vData[idx];
        vec2D &p2 = this->m_vData[idx + 1];
        vec2D &p3 = this->m_vData[idx + 2];
        vec2D &p4 = this->m_vData[idx + 3];
        vec2D v0 = (p3 - p1) * power;   // (p3 - p1) * power (Usually power is 0.5f)
        vec2D v1 = (p4 - p2) * power;   // (p4 - p2) * power (Usually power is 0.5f)
        pos.x = this->calcVal(p2.x, p3.x, v0.x, v1.x, t);
        pos.y = this->calcVal(p2.y, p3.y, v0.y, v1.y, t);
        return pos;
    }
};
#endif