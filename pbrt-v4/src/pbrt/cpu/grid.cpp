GridAggregate::GridAggregate(const std::vector<Primitive> &primitives) {
    long long primitives_num{static_cast<long long>(primitives.size())};

    // PrintVar(primitives_num);

    for (long long p{0}; p < primitives_num; ++p) {
        this->bounds = Union(this->bounds, primitives[p].Bounds());
    }

    // PrintVar(this->bounds.pMin);
    // PrintVar(this->bounds.pMax);

    Vector3f delta{this->bounds.pMax - this->bounds.pMin};

    // PrintVar(delta);

    int max_axis{0};

    for (int axis = 1; axis < 3; ++axis) {
        if (delta[max_axis] < delta[axis]) {
            max_axis = axis;
        }
    }

    // PrintVar(max_axis);

    float max_delta{delta[max_axis]};
    float cube_root{3.f * powf(static_cast<float>(primitives_num), 1.f / 3.f)};
    float voxelsPerUnitDist{64 * cube_root / max_delta};

    for (int axis{0}; axis < 3; ++axis) {
        long long voxel_num{Clamp(
            static_cast<long long>(std::ceil(delta[axis] * voxelsPerUnitDist)), 1, 1024)};
        float voxel_width = delta[axis] / voxel_num;

        this->voxel_widths[axis] = voxel_width;
        this->voxel_nums[axis] = voxel_num;
    }

    // PrintVar(this->voxel_nums[0]);
    // PrintVar(this->voxel_nums[1]);
    // PrintVar(this->voxel_nums[2]);

    long long total_voxel_nums{this->voxel_nums[0] * this->voxel_nums[1] *
                               this->voxel_nums[2]};

    this->voxels.resize(total_voxel_nums);

    for (long long p{0}; p < primitives_num; ++p) {
        const Primitive &primitive = primitives[p];

        Bounds3f p_bounds = primitive.Bounds();

        long long vmin[3];
        long long vmax[3];

        for (int axis = 0; axis < 3; ++axis) {
            vmin[axis] = PointToVoxel_(axis, p_bounds.pMin[axis]);
            vmax[axis] = PointToVoxel_(axis, p_bounds.pMax[axis]);
        }

        for (long long vx = vmin[0]; vx <= vmax[0]; ++vx) {
            for (long long vy = vmin[1]; vy <= vmax[1]; ++vy) {
                for (long long vz = vmin[2]; vz <= vmax[2]; ++vz) {
                    long long vi = this->voxel_nums[1] * this->voxel_nums[2] * vx +
                                   this->voxel_nums[2] * vy + vz;
                    this->voxels[vi].primitives.push_back(primitive);
                }
            }
        }
    }

    /*
    for (long long vx{0}; vx < this->voxel_nums[0]; ++vx) {
        for (long long vy{0}; vy < this->voxel_nums[1]; ++vy) {
            for (long long vz{0}; vz < this->voxel_nums[2]; ++vz) {
                long long vi = this->voxel_nums[1] * this->voxel_nums[2] * vx +
                               this->voxel_nums[2] * vy + vz;

                if (100 < this->voxels[vi].primitives.size()) {
                    PrintVar(vx);
                    PrintVar(vy);
                    PrintVar(vz);
                    PrintVar(vi);
                    PrintVar(this->voxels[vi].primitives.size());
                }
            }
        }
    }
    */
}

GridAggregate *GridAggregate::Create(std::vector<Primitive> prims,
                                     const ParameterDictionary &parameters) {
    return new GridAggregate(std::move(prims));
}

Bounds3f GridAggregate::Bounds() const {
    return this->bounds;
}

pstd::optional<ShapeIntersection> GridAggregate::Intersect(const Ray &ray,
                                                           Float tMax) const {
    float min_ts[3];
    float max_ts[3];
    float dts[3];

    float min_t = 0;
    float max_t = tMax;

    for (int axis = 0; axis < 3; ++axis) {
        if (ray.d[axis] == 0) {
            if (ray.o[axis] < this->bounds.pMin[axis] ||
                this->bounds.pMax[axis] < ray.o[axis]) {
                return {};
            }

            continue;
        }

        float a_t = (this->bounds.pMin[axis] - ray.o[axis]) / ray.d[axis];
        float b_t = (this->bounds.pMax[axis] - ray.o[axis]) / ray.d[axis];

        min_ts[axis] = std::min(a_t, b_t);
        max_ts[axis] = std::max(a_t, b_t);
        dts[axis] = std::abs(this->voxel_widths[axis] / ray.d[axis]);

        min_t = std::max(min_t, min_ts[axis]);
        max_t = std::min(max_t, max_ts[axis]);
    }

    if (max_t <= min_t) {
        return {};
    }

    for (int axis = 0; axis < 3; ++axis) {
        if (ray.d[axis] == 0) {
            min_ts[axis] = tMax;
            continue;
        }

        while (min_ts[axis] < min_t) {
            min_ts[axis] += dts[axis];
        }
    }

    std::unordered_set<Primitive, PrimitiveHasher> intersected_primitives;

    float prv_t = min_t;

    long long check_voxel_num{0};

    for (;;) {
        int target_axis = 0;

        for (int axis = 1; axis < 3; ++axis) {
            if (min_ts[axis] < min_ts[target_axis]) {
                target_axis = axis;
            }
        }

        float cur_t = min_ts[target_axis];
        min_ts[target_axis] += dts[target_axis];

        float mid_t = (prv_t + cur_t) / 2;

        Point3f mid_point = ray.o + ray.d * mid_t;

        for (int axis = 0; axis < 3; ++axis) {
            if (mid_point[axis] < this->bounds.pMin[axis] ||
                this->bounds.pMax[axis] < mid_point[axis]) {
                // PrintVar(check_voxel_num);
                // PrintVar(intersected_primitives.size());
                return {};
            }
        }

        long long vi[3] = {
            PointToVoxel_(0, mid_point[0]),
            PointToVoxel_(1, mid_point[1]),
            PointToVoxel_(2, mid_point[2]),
        };

        const Voxel &voxel =
            this->voxels[this->voxel_nums[1] * this->voxel_nums[2] * vi[0] +
                         this->voxel_nums[2] * vi[1] + vi[2]];

        pstd::optional<ShapeIntersection> cur_si;

        for (auto iter{voxel.primitives.begin()}, end{voxel.primitives.end()};
             iter != end; ++iter) {
            const Primitive &primitive{*iter};

            if (!intersected_primitives.insert(primitive).second) {
                continue;
            }

            pstd::optional<ShapeIntersection> si = primitive.Intersect(ray, tMax);

            if (!si) {
                continue;
            }

            if (!cur_si || si->tHit < cur_si->tHit) {
                cur_si = si;
            }
        }

        ++check_voxel_num;

        if (cur_si) {
            // PrintVar(check_voxel_num);
            // PrintVar(intersected_primitives.size());
            return cur_si;
        }

        prv_t = cur_t;
    }

    // PrintVar(check_voxel_num);
    // PrintVar(intersected_primitives.size());

    return {};
}

bool GridAggregate::IntersectP(const Ray &ray, Float tMax) const {
    return static_cast<bool>(this->Intersect(ray, tMax));
}

long long GridAggregate::PointToVoxel_(int axis, float val) const {
    long long ret =
        std::floor((val - this->bounds.pMin[axis]) / this->voxel_widths[axis]);

    return Clamp(ret, 0, this->voxel_nums[axis] - 1);
}