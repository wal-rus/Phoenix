
#ifndef JOYSTICKEVENTS_H
#define JOYSTICKEVENTS_H

#include <QHash>
#include <SDL2/SDL.h>

#include "inputdeviceevent.h"


inline uint qHash(SDL_GameControllerButton button, uint seed = 0)
{
    return qHash(static_cast<int>(button), seed);
}

inline uint qHash(SDL_GameControllerAxis axis, uint seed = 0)
{
    return qHash(static_cast<int>(axis), seed);
}

namespace std
{
    template <>
    struct hash<SDL_GameControllerButton>
    {
        std::size_t operator()(const SDL_GameControllerButton button) const
        {
            return hash<int>()(static_cast<int>(button));
        }
    };

    template <>
    struct hash<SDL_GameControllerAxis>
    {
        std::size_t operator()(const SDL_GameControllerAxis axis) const
        {
            return hash<int>()(static_cast<int>(axis));
        }
    };
}

template<>
struct TEvent_default_t<SDL_GameControllerButton>
{
    operator SDL_GameControllerButton() const { return SDL_CONTROLLER_BUTTON_INVALID; }
};

class ControllerButtonEvent : public InputDeviceEventHelper<InputDeviceButtonEvent,
                                                            ControllerButtonEvent,
                                                            SDL_GameControllerButton>
{
public:
    ControllerButtonEvent() {}
    ControllerButtonEvent(EventType&& button) : HelperType(std::forward<EventType>(button)) {}

    static ControllerButtonEvent fromSDLEvent(const SDL_ControllerButtonEvent &event)
    {
        return ControllerButtonEvent(std::move(static_cast<SDL_GameControllerButton>(event.button)));
    }

    static ControllerButtonEvent fromString(const QString &str)
    {
        QByteArray evname = str.toLatin1();
        auto btn = SDL_GameControllerGetButtonFromString(evname.constData());
        if (btn != SDL_CONTROLLER_BUTTON_INVALID)
            return ControllerButtonEvent(std::move(btn));

        return ControllerButtonEvent();
    }

protected:
    virtual QString toString() const override
    {
        return QString("controller_button[%1]")
                    .arg(SDL_GameControllerGetStringForButton(event()));
    }
};

template<>
struct TEvent_default_t<SDL_GameControllerAxis>
{
    operator SDL_GameControllerAxis() const { return SDL_CONTROLLER_AXIS_INVALID; }
};

class ControllerAxisEvent : public InputDeviceEventHelper<InputDeviceAnalogEvent, ControllerAxisEvent, SDL_GameControllerAxis>
{
public:
    ControllerAxisEvent() : HelperType() {}
    ControllerAxisEvent(SDL_GameControllerAxis&& button) : HelperType(button) {}

    static ControllerAxisEvent fromSDLEvent(const SDL_ControllerAxisEvent &event)
    {
        return ControllerAxisEvent(std::move(static_cast<SDL_GameControllerAxis>(event.axis)));
    }

    static ControllerAxisEvent fromString(const QString &str)
    {
        QByteArray evname = str.toLatin1();
        auto axis = SDL_GameControllerGetAxisFromString(evname.constData());
        if (axis != SDL_CONTROLLER_AXIS_INVALID)
            return ControllerAxisEvent(std::move(axis));

        return ControllerAxisEvent();
    }

protected:
    virtual QString toString() const override
    {
        return QString("controller_axis[%1]")
                    .arg(SDL_GameControllerGetStringForAxis(event()));
    }
};


#endif
