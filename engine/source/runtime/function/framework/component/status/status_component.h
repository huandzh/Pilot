#pragma once

#include "runtime/resource/res_type/components/status.h"

#include "runtime/function/framework/component/component.h"

namespace Piccolo
{
    REFLECTION_TYPE(StatusComponent)
    CLASS(StatusComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(StatusComponent)
    public:
        StatusComponent() = default;

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;
        void tickPlayerStatus(float delta_time);
        bool getIsOutOfStamina() const { return m_is_out_of_stamina; }

    private:
        META(Enable)
        StatusComponentRes m_status_res;
        bool               m_is_out_of_stamina {false};
    };
} // namespace Piccolo
