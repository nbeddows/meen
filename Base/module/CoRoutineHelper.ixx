module;
#include <coroutine>

export module CoRoutineHelper;

export struct CoObj
{
	//MUST define a struct called 'promise_type'
	struct promise_type
	{
		/*
			TODO: Write custom allocator, biggest allocation is Daa with 576 bytes.
				  Just need to recycle blocks (5-10 should be enough) of size 576,
				  this will save on the excessive dynamic allocations and deletions.
		*/
		//void* operator new (std::size_t sz) { }
		//void operator delete(void* buf) { }

		CoObj get_return_object() { return CoObj{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() noexcept {}
		void unhandled_exception() { throw; } //throw the right exception, look me up ... std::exception::broken_promise
	};

	constexpr bool await_ready() const noexcept { return false; }
	constexpr void await_suspend(std::coroutine_handle<promise_type> h) {}
	constexpr void await_resume() const noexcept {}

	explicit CoObj(const std::coroutine_handle<promise_type> coroutine) : coh{ coroutine } {}
	CoObj() = default;

	~CoObj()
	{
		if (coh)
		{
			coh.destroy();
		}
	}

	CoObj& operator=(CoObj&& other) noexcept
	{
		if (this != &other)
		{
			if (coh)
			{
				coh.destroy();
			}

			coh = other.coh;
			other.coh = {};
		}

		return *this;
	}

	std::coroutine_handle<promise_type> coh;
};