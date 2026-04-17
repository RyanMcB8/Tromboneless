#include <cmath>

class hpf {
public:
    hpf(float sr_, float fc_) : sr(sr_), fc(fc_) {
        dt = 1.0f / sr;
        tc = 1.0f / (2.0f * M_PI * fc);
        alpha = tc / (tc + dt);
    }

    float filter(float x) {
        float y = alpha * (prev_y + x - prev_x);
        prev_x = x;
        prev_y = y;
        return y;
    }

private:
    float sr;
    float fc;
    float dt;
    float tc;
    float alpha;
    float prev_x = 0.0f;
    float prev_y = 0.0f;
};