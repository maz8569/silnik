#include "Physics/Collisions/Collision.h"

GameEngine::Collision::Collision()
{
}

bool GameEngine::Collision::TestAABBAABB(std::shared_ptr<AABB> a, std::shared_ptr<AABB> b)
{
    if (abs(a->center[0] - b->center[0]) > (a->extents[0] + b->extents[0])) return 0;
    if (abs(a->center[1] - b->center[1]) > (a->extents[1] + b->extents[1])) return 0;
    if (abs(a->center[2] - b->center[2]) > (a->extents[2] + b->extents[2])) return 0;
    return 1;
}

void GameEngine::Collision::AddAABB(std::shared_ptr<AABB> a)
{
    collisions.push_back(a);
}

void GameEngine::Collision::CollisionCheck()
{
    if (collisions.size() > 1)
    {
        for (unsigned int i = 0; i < collisions.size(); i++)
        {
            for (unsigned int j = i+1; j < collisions.size(); j++)
            {
                if (collisions[i]->collides(collisions[j]))
                {
                    //std::cout << "coll";
                    collisions[i]->parent->reactOnCollision(collisions[j]->parent);
                    collisions[j]->parent->reactOnCollision(collisions[i]->parent);
                }
            }
        }
    }
}

bool GameEngine::Collision::IntersectRayAABB(glm::vec3& start, glm::vec3& direction, float& t, glm::vec3& point)
{
    if (collisions.size() > 0)
    {
        for (auto aabb : collisions)
        {
            if (aabb->tag == "terrain")
            {
                if (IntersectRayTerrain(start, direction, aabb, t, point))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameEngine::Collision::IntersectRayTerrain(glm::vec3& p, glm::vec3& d, Ref<AABB> a, float& tmin, glm::vec3& q)
{
    tmin = 0.0f;
    float tmax = 40.f;
    glm::vec3 min = a->getMin();
    glm::vec3 max = a->getMax();
    std::cout << min[0] << " " << min[1] << " " << min[2] << "\n";
    std::cout << max[0] << " " << max[1] << " " << max[2] << "\n";
    // For all three slabs
    for (int i = 0; i < 3; i++) {
        if (fabsf(d[i]) < EPSILON) {
            // Ray is parallel to slab. No hit if origin not within slab
            if (p[i] < min[i] || p[i] > max[i]) return false;
        }
        else {
            // Compute intersection t value of ray with near and far plane of slab
            float ood = 1.0f / d[i];
            float t1 = (min[i] - p[i]) * ood;
            float t2 = (max[i] - p[i]) * ood;
            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) swap(t1, t2);
            // Compute the intersection of slab intersection intervals
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            // Exit with no collision as soon as slab intersection becomes empty
            if (tmin > tmax) return false;
        }
    }
    // Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
    q = p + d * tmin;
    std::cout << q.x << " " << q.y << " " << q.z << " " << std::endl;
    std::cout << tmin << std::endl;
    return true;
}
