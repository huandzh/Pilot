#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo
{
    REFLECTION_TYPE(StatusComponentRes)
    CLASS(StatusComponentRes, Fields)
    {
        REFLECTION_BODY(StatusComponentRes);

    public:
        StatusComponentRes() = default;

        float m_hitpoints {0.F};
        float m_max_hitpoints {0.F};

        float m_stamina {0.F};
        float m_min_stamina {0.F};
        float m_max_stamina {0.F};
        float m_stamina_rate {0.F};
        float m_stamina_recover_modifier {0.F};
    };
} // namespace Piccolo
