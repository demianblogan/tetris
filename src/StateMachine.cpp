#include "StateMachine.h"

void StateMachine::PushState(std::unique_ptr<State> state)
{
    states.push_back(std::move(state));
}

void StateMachine::PopState()
{
    if (!states.empty())
    {
        states.pop_back();
    }
}

void StateMachine::ChangeState(std::unique_ptr<State> state)
{
    pendingState = std::move(state);
    stateChangeRequested = true;
}

void StateMachine::ApplyPendingChanges()
{
    if (!stateChangeRequested)
    {
        return;
    }

    PopState();
    PushState(std::move(pendingState));

    stateChangeRequested = false;
}

State* StateMachine::GetCurrentState()
{
    if (states.empty())
    {
        return nullptr;
    }

    return states.back().get();
}