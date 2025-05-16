#pragma once

#include <le-cpp-utils/logging.h>
#include <le-cpp-utils/CLI11.hpp>

namespace le
{
	class Subcommand
	{
	public:
		int process()
		{
			if (app->parsed())
				return execute();
			return 0;
		}

	protected:
		explicit Subcommand(
			CLI::App& parentApp,
			std::string name,
			std::string description
		) : 
			app(parentApp.add_subcommand(name, description))
		{}

		virtual int execute() = 0;

		void abortSubcommand(std::string message = "Subcommand aborted.")
		{
			throw std::runtime_error(message);
		}

		CLI::App* app;
	};
}