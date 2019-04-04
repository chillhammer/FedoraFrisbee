#pragma once
namespace Fed
{
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

	/**
		Creates State class declaration that is 
		actually a singleton. 
	**/
	#define STATE_CLASS_SINGLETON(ownerClass, className, variables)	\
	class className : public State<ownerClass>							\
	{																	\
	private:															\
		className() {}													\
		className(const className& o) {ASSERT(false, "Singleton")}		\
		~className() {}													\
		variables														\
	public:																\
		static className* Instance() {									\
			static className instance;									\
			return &instance;											\
		}																\
		void Enter(ownerClass* owner);									\
		void Execute(ownerClass* owner);								\
		void Exit(ownerClass* owner);									\
	};														
}