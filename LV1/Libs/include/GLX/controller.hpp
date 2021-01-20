#pragma once
#include <string>

namespace glx
{
	class Controller
	{
		Controller(Controller const&) = delete;
		Controller& operator=(Controller const&) = delete;
		
		std::string const handle;

	protected:
		Controller(std::string const&);

	public:
		virtual ~Controller();
		std::string const& getHandle() const;
		virtual void tick(double delta, size_t channel = 0) = 0;
		virtual void trigger(size_t channel = 0) = 0;
	};
}