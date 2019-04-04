#pragma once
#include <Macros.h>
#include "State.h"
namespace Fed
{
	template <class Owner>
	class StateMachine
	{
	private:

		Owner*          m_pOwner;
		State<Owner>*   m_pCurrentState;
		State<Owner>*   m_pPreviousState;
		// State Update called every step
		State<Owner>*   m_pGlobalState;

	public:
		// Standard Constructor
		StateMachine(Owner* owner) :m_pOwner(owner),
			m_pCurrentState(nullptr),
			m_pPreviousState(nullptr),
			m_pGlobalState(nullptr)
		{
		}

		// Initial State Constructor
		StateMachine(Owner* owner, State<Owner>* initial) : m_pOwner(owner),
			m_pCurrentState(initial),
			m_pPreviousState(nullptr),
			m_pGlobalState(nullptr)
		{
			m_pCurrentState->Enter(owner);
		}

		virtual ~StateMachine() {}

		//use these methods to initialize the FSM
		void SetCurrentState(State<Owner>* s) { m_pCurrentState = s; }
		void SetGlobalState(State<Owner>* s) { m_pGlobalState = s; }
		void SetPreviousState(State<Owner>* s) { m_pPreviousState = s; }

		// Call State's Update Funci
		void Update() const
		{
			// Execute Global State
			if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

			// Execute Current State
			if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
			int x = 1;
		}
		void ChangeState(State<Owner>* pNewState)
		{
			ASSERT(pNewState, "<StateMachine::ChangeState>:trying to assign null state to current");
			m_pPreviousState = m_pCurrentState;
			if (m_pCurrentState)
				m_pCurrentState->Exit(m_pOwner);
			m_pCurrentState = pNewState;
			m_pCurrentState->Enter(m_pOwner);
		}
		void RevertToPreviousState()
		{
			ChangeState(m_pPreviousState);
		}

		//returns true if the current state's type is equal to the type of the
		//class passed as a parameter. 
		bool  IsInState(const State<Owner>& s)const
		{
			return (typeid(*m_pCurrentState) == typeid(s));
		}

		State<Owner>* CurrentState()  const { return m_pCurrentState; }
		State<Owner>* GlobalState()   const { return m_pGlobalState; }
		State<Owner>* PreviousState() const { return m_pPreviousState; }

		// Debug method to get name of state
		std::string GetNameOfCurrentState()const
		{
			std::string s(typeid(*m_pCurrentState).name());

			//remove the 'class ' part from the front of the string
			if (s.size() > 5)
			{
				s.erase(0, 6);
			}

			return s;
		}
	};
}