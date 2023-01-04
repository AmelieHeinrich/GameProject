/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 22:54
 */

#include "event_system.hpp"
#include "log_system.hpp"

#include <vector>
#include <unordered_map>

struct registered_event
{
    void *Listener;
    PFN_OnEvent Callback;
};

struct event_type_entry
{
    std::vector<registered_event> Events;
};

struct event_system
{
    std::unordered_map<event_type, event_type_entry> Registered;
};

static event_system EventSystem;

void EventSystemInit()
{
    EventSystem = {};
}

void EventSystemExit()
{
    EventSystem.Registered.clear();
}

void EventSystemRegister(event_type Type, void *Listener, PFN_OnEvent Callback)
{
    uint64_t RegisteredCount = EventSystem.Registered[Type].Events.size();
    for (uint64_t RegisterIndex = 0; RegisterIndex < RegisteredCount; RegisterIndex++)
    {
        if (EventSystem.Registered[Type].Events[RegisterIndex].Listener == Listener) 
        {
            LogWarn("Trying to register an eventthat is already registered!");
            return;
        }
    }

    registered_event Event;
    Event.Listener = Listener;
    Event.Callback = Callback;
    EventSystem.Registered[Type].Events.push_back(Event);
}

void EventSystemUnregister(event_type Type, void *Listener, PFN_OnEvent Callback)
{
    if (EventSystem.Registered[Type].Events.empty())
    {
        LogWarn("Trying to unregister an event listener that isn't registered!");
        return;
    }

    uint64_t RegisteredCount = EventSystem.Registered[Type].Events.size();
    for (uint64_t RegisterIndex = 0; RegisterIndex < RegisteredCount; RegisterIndex++)
    {
        if (EventSystem.Registered[Type].Events[RegisterIndex].Callback == Callback) 
        {
            registered_event PoppedEvent = EventSystem.Registered[Type].Events[RegisterIndex];
            EventSystem.Registered[Type].Events.erase(EventSystem.Registered[Type].Events.begin() + RegisterIndex);
        }
    }
}

bool EventSystemFire(event_type Type, void *Sender, event_data Data)
{
    if (EventSystem.Registered[Type].Events.empty())
        return false;
    
    uint64_t RegisteredCount = EventSystem.Registered[Type].Events.size();
    for (uint64_t RegisterIndex = 0; RegisterIndex < RegisteredCount; RegisterIndex++)
    {
        registered_event Event = EventSystem.Registered[Type].Events[RegisterIndex];
        if (Event.Callback(Type, Sender, Event.Listener, Data))
            return true;
    }
    return false;
}
