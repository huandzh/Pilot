#include "runtime/function/framework/component/status/status_component.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/character/character.h"
#include "runtime/function/controller/character_controller.h"
#include "runtime/function/framework/component/motor/motor_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/object/object.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/physics/physics_scene.h"
#include <algorithm>

namespace Piccolo
{
    void StatusComponent::postLoadResource(std::weak_ptr<GObject> parent_object) { m_parent_object = parent_object; }

    void StatusComponent::tick(float delta_time) { tickPlayerStatus(delta_time); }

    void StatusComponent::tickPlayerStatus(float delta_time)
    {

        if (!m_parent_object.lock())
            return;

        std::shared_ptr<Level> current_level = g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        std::shared_ptr<Character> current_character = current_level->getCurrentActiveCharacter().lock();
        if (current_character == nullptr)
            return;

        if (current_character->getObjectID() != m_parent_object.lock()->getID())
            return;

        MotorComponent* motor_component = m_parent_object.lock()->tryGetComponent<MotorComponent>("MotorComponent");

        //  jump up consume stamina
        if (motor_component->getIsJumpingUp())
        {
            m_status_res.m_stamina -= m_status_res.m_stamina_rate * delta_time;
        }
        // else recover to max
        else if (m_status_res.m_stamina < m_status_res.m_max_stamina)
        {
            m_status_res.m_stamina = std::min(m_status_res.m_stamina + m_status_res.m_stamina_rate * delta_time *
                                                                           m_status_res.m_stamina_recover_modifier,
                                              m_status_res.m_max_stamina);
        }

        m_is_out_of_stamina = m_status_res.m_stamina < m_status_res.m_min_stamina;
    }

} // namespace Piccolo
