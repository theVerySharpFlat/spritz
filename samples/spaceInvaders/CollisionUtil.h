#ifndef SPACE_INVADERS_COLLISION_UTIL
#define SPACE_INVADERS_COLLISION_UTIL

namespace CollisionUtil {
struct Rect {
    float x, y, width, height;
};

struct Point {
    float x, y;
};

inline bool rectContainsPoint(const Rect rect, const Point point) {
    return (point.x <= rect.x + rect.width / 2.0f) &&
           (point.x >= rect.x - rect.width / 2.0f) &&
           (point.y <= rect.y + rect.height / 2.0f) &&
           (point.y >= rect.y - rect.height / 2.0f);
}

inline bool rectCollidesWithRect(const Rect a, const Rect b) {
    return rectContainsPoint(b,
                             {a.x - a.width / 2.0f, a.y + a.height / 2.0f}) ||
           rectContainsPoint(b,
                             {a.x + a.width / 2.0f, a.y + a.height / 2.0f}) ||
           rectContainsPoint(b,
                             {a.x - a.width / 2.0f, a.y - a.height / 2.0f}) ||
           rectContainsPoint(b, {a.x + a.width / 2.0f, a.y - a.height / 2.0f});
}
} // namespace CollisionUtil

#endif
