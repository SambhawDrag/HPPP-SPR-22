
struct vec3{
    float x,y,z;
};

struct vec3 sum(struct vec3 a, struct vec3 b)
{
    struct vec3 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

struct vec3 diff(struct vec3 a, struct vec3 b)
{
    struct vec3 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

float lenSquared(struct vec3 a)
{
    float l = a.x*a.x + a.y*a.y + a.z*a.z;
    return l;
}

struct vec3 normalize(struct vec3 a)
{
    float len = sqrt(lenSquared(a));
    struct vec3 c;
    c.x = a.x/len;
    c.y = a.y/len;
    c.z = a.z/len;
    return c;
}

struct vec3 constMul(struct vec3 a, float c)
{
    struct vec3 r;
    r.x = a.x*c;
    r.y = a.y*c;
    r.z = a.z*c;
    return r;
}

struct vec3 checkBounds(struct vec3 a, struct vec3 v, float x, float y, float z)
{
    if(a.x <= 0 || a.x >= x) v.x *= -1;
    if(a.y <= 0 || a.y >= y) v.y *= -1;
    if(a.z <= 0 || a.z >= z) v.z *= -1;
    return v;
}
