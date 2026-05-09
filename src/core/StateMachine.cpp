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

void StateMachine::RenderStates(sf::RenderWindow& window)
{
	if (states.empty())
	{
		return;
	}

	int startIndex = static_cast<int>(states.size()) - 1;

	while (startIndex > 0 && states[startIndex]->IsTransparent())
	{
		startIndex--;
	}

	for (int i = startIndex; i < static_cast<int>(states.size()); i++)
	{
		states[i]->Render(window);
	}
}

State* StateMachine::GetCurrentState()
{
	if (states.empty())
	{
		return nullptr;
	}

	return states.back().get();
}