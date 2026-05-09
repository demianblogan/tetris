#pragma once

#include <memory>
#include <vector>
#include "State.h"

class StateMachine
{
private:
    std::vector<std::unique_ptr<State>> states;
    std::unique_ptr<State> pendingState;
    bool stateChangeRequested = false;

public:
    void PushState(std::unique_ptr<State> state);
    void PopState();
    void ChangeState(std::unique_ptr<State> state);
    [[nodiscard]] State* GetCurrentState();

    void ApplyPendingChanges();

    void ClearStates();
    void RenderStates(sf::RenderWindow& window);
};