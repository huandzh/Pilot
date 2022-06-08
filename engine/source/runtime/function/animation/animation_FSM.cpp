#include "runtime/function/animation/animation_FSM.h"
#include "core/base/macro.h"
#include <iostream>
namespace Pilot
{
    AnimationFSM::AnimationFSM() {}
    float tryGetFloat(const json11::Json::object& json, const std::string& key, float default_value)
    {
        auto found_iter = json.find(key);
        if (found_iter != json.end() && found_iter->second.is_number())
        {
            return found_iter->second.number_value();
        }
        return default_value;
    }
    bool tryGetBool(const json11::Json::object& json, const std::string& key, float default_value)
    {
        auto found_iter = json.find(key);
        if (found_iter != json.end() && found_iter->second.is_bool())
        {
            return found_iter->second.bool_value();
        }
        return default_value;
    }
    bool AnimationFSM::update(const json11::Json::object& signals)
    {
        States last_state     = m_state;
        bool   is_clip_finish = tryGetBool(signals, "clip_finish", false);
        bool   is_jumping     = tryGetBool(signals, "jumping", false);
        float  speed          = tryGetFloat(signals, "speed", 0);
        bool   is_moving      = speed > 0.01f;
        bool   start_walk_end = tryGetBool(signals, "stop_moving", false);

        // TODO: PA03_2
        switch (m_state)
        {
            case States::_idle:
                /**** [0] ****/
                if (is_jumping)
                {
                    m_state = States::_jump_start_from_idle;
                }
                else if (is_moving)
                {
                    m_state = States::_walk_start;
                }
                break;
            case States::_walk_start:
                /**** [1] ****/
                if (is_clip_finish)
                {
                    m_state = States::_walk_run;
                }
                break;
            case States::_walk_run:
                if (is_jumping)
                {
                    m_state = States::_jump_start_from_walk_run;
                }
                else if (start_walk_end && is_clip_finish)
                {
                    m_state = States::_walk_stop;
                }
                else if (!is_moving)
                {
                    m_state = States::_idle;
                }
                /**** [2] ****/
                break;
            case States::_walk_stop:
                if (!is_moving && is_clip_finish)
                {
                    m_state = States::_idle;
                }
                else if (!start_walk_end)
                {
                    m_state = States::_walk_start;
                }
                /**** [3] ****/
                break;
            case States::_jump_start_from_idle:
                if (is_clip_finish)
                {
                    m_state = States::_jump_loop_from_idle;
                }
                /**** [4] ****/
                break;
            case States::_jump_loop_from_idle:
                if (!is_jumping)
                {
                    m_state = States::_jump_end_from_idle;
                }
                /**** [5] ****/
                break;
            case States::_jump_end_from_idle:
                if (is_clip_finish)
                {
                    m_state = States::_idle;
                }
                /**** [6] ****/
                break;
            case States::_jump_start_from_walk_run:
                /**** [7] ****/
                if (is_clip_finish)
                {
                    m_state = States::_jump_loop_from_walk_run;
                }
                break;
            case States::_jump_loop_from_walk_run:
                /**** [8] ****/
                if (!is_jumping)
                {
                    m_state = States::_jump_end_from_walk_run;
                }
                break;
            case States::_jump_end_from_walk_run:
                /**** [9] ****/
                if (is_clip_finish)
                {
                    m_state = States::_walk_run;
                }
                break;
            default:
                break;
        }
#ifdef NDEBUG
#else
        if (last_state != m_state)
        {
            DEBUG_LOG_DEBUG(getCurrentStateName());
        }
#endif
        return last_state != m_state;
    }

    std::string AnimationFSM::getCurrentClipBaseName() const
    {
        switch (m_state)
        {
            case States::_idle:
                return "idle_walk_run";
            case States::_walk_start:
                return "walk_start";
            case States::_walk_run:
                return "idle_walk_run";
            case States::_walk_stop:
                return "walk_stop";
            case States::_jump_start_from_walk_run:
            case States::_jump_start_from_idle:
                return "jump_start";
            case States::_jump_loop_from_walk_run:
            case States::_jump_loop_from_idle:
                return "jump_loop";
            case States::_jump_end_from_walk_run:
            case States::_jump_end_from_idle:
                return "jump_stop";
            default:
                return "idle_walk_run";
        }
    }

    std::string AnimationFSM::getCurrentStateName() const
    {
        switch (m_state)
        {
            case States::_idle:
                return PolitNameOf(States::_idle);
            case States::_walk_start:
                return PolitNameOf(States::_walk_start);
            case States::_walk_run:
                return PolitNameOf(States::_walk_run);
            case States::_walk_stop:
                return PolitNameOf(States::_walk_stop);
            case States::_jump_start_from_walk_run:
                return PolitNameOf(States::_jump_start_from_walk_run);
            case States::_jump_start_from_idle:
                return PolitNameOf(States::_jump_start_from_idle);
            case States::_jump_loop_from_walk_run:
                return PolitNameOf(States::_jump_loop_from_walk_run);
            case States::_jump_loop_from_idle:
                return PolitNameOf(States::_jump_loop_from_idle);
            case States::_jump_end_from_walk_run:
                return PolitNameOf(States::_jump_end_from_walk_run);
            case States::_jump_end_from_idle:
                return PolitNameOf(States::_jump_end_from_idle);
                ;
            default:
                return "StatesUnknown";
        }
    }
} // namespace Pilot
