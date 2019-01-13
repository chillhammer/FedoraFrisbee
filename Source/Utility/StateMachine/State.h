#pragma once
/**
	States are the engine for the state machine.
	They are swappable and have a reference to the owner 
	to have customizable functionality.
**/
template <class Owner>
class State
{
public:
	virtual ~State() {}

	// Executed when state is entered
	virtual void Enter(Owner*) = 0;

	// Update function
	virtual void Execute(Owner*) = 0;

	// Executed when state is exited. 
	virtual void Exit(Owner*) = 0;
};
