#include "runtime/function/controller/character_controller.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/framework/component/motor/motor_component.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/physics/physics_scene.h"

namespace Pilot
{
    CharacterController::CharacterController(const Capsule& capsule) : m_capsule(capsule)
    {
        m_rigidbody_shape                                    = RigidBodyShape();
        m_rigidbody_shape.m_geometry                         = PILOT_REFLECTION_NEW(Capsule);
        *static_cast<Capsule*>(m_rigidbody_shape.m_geometry) = m_capsule;

        m_rigidbody_shape.m_type = RigidBodyShapeType::capsule;

        Quaternion orientation;
        orientation.fromAngleAxis(Radian(Degree(90.f)), Vector3::UNIT_X);

        m_rigidbody_shape.m_local_transform =
            Transform(Vector3(0, 0, capsule.m_half_height + capsule.m_radius), orientation, Vector3::UNIT_SCALE);
    }

    Vector3
    CharacterController::move(const Vector3& current_position, const Vector3& displacement, const bool& is_jumping)
    {
        std::shared_ptr<PhysicsScene> physics_scene =
            g_runtime_global_context.m_world_manager->getCurrentActivePhysicsScene().lock();
        ASSERT(physics_scene);

        std::vector<PhysicsHitInfo> hits;

        Transform world_transform =
            Transform(current_position + 0.1f * Vector3::UNIT_Z, Quaternion::IDENTITY, Vector3::UNIT_SCALE);

        Vector3 vertical_displacement   = displacement.z * Vector3::UNIT_Z;
        Vector3 horizontal_displacement = Vector3(displacement.x, displacement.y, 0.f);

        Vector3 vertical_direction   = vertical_displacement.normalisedCopy();
        Vector3 horizontal_direction = horizontal_displacement.normalisedCopy();

        Vector3 final_position = current_position;

        m_is_touch_ground = physics_scene->sweep(
            m_rigidbody_shape, world_transform.getMatrix(), Vector3::NEGATIVE_UNIT_Z, 0.105f, hits);

        hits.clear();

        world_transform.m_position -= 0.1f * Vector3::UNIT_Z;
        // vertical pass
        if (physics_scene->sweep(m_rigidbody_shape,
                                 world_transform.getMatrix(),
                                 vertical_direction,
                                 vertical_displacement.length(),
                                 hits))
        {
            DEBUG_LOG_DEBUG("vertical hit");
            final_position += hits[0].hit_distance * vertical_direction;
        }
        else
        {
            final_position += vertical_displacement;
        }

        hits.clear();

        // never count foot in floor as side hit
        world_transform.m_position += 0.05F * Vector3::UNIT_Z;
        // side pass
        float offset = 0.001F;
        if (is_jumping)
        {
            offset = 0.1F;
        }

        if (physics_scene->sweep(m_rigidbody_shape,
                                 //    /**** [0] ****/,
                                 world_transform.getMatrix(),
                                 //    /**** [1] ****/,
                                 horizontal_direction,
                                 //    /**** [2] ****/,
                                 horizontal_displacement.length() + offset,
                                 hits))
        {
            DEBUG_LOG_DEBUG("side hit");
            final_position += (hits[0].hit_distance - offset) * horizontal_direction;
        }
        else
        {
            final_position += horizontal_displacement;
        }

        return final_position;
    }

} // namespace Pilot
