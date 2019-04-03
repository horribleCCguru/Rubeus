/**
 * @file		Source/logger_component.cpp.
 *
 * @brief	Implements the logger component functions
 */

#include <logger_component.h>

#include <math.h>

int toHex(int decimal)
{
	int hex = 0;

	for (int i = 0; decimal != 0; i++)
	{
		hex = hex + (int)((pow(10, i))*(decimal % 16));
		decimal /= 16;
	}

	return hex;
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

void DevILClearError()
{
	while (ilGetError() != IL_NO_ERROR)
		;
}

namespace Rubeus
{
	namespace UtilityComponents
	{
		std::ofstream RLogger::LogFile;
		RLogger * RLogger::CurrentInstance;
		std::map<std::string, short> RLogger::foregroundColorMap = {
				{"black", 30},
				{"red", 31},
				{"green", 32},
				{"yellow", 33},
				{"blue", 34},
				{"magenta", 35},
				{"cyan", 36},
				{"white", 37}
		};
		std::map<std::string, short> RLogger::BackgroundColorMap = {
			{"black", 40},
			{"red", 41},
			{"green", 42},
			{"yellow", 43},
			{"blue", 44},
			{"magenta", 45},
			{"cyan", 46},
			{"white", 47}
		};
		std::map<std::string, std::string> RLogger::SeverityMap ={
			{"ERROR", "red"},
			{"ASSERT", "yellow"},
			{"SUCCESS", "green"}
		};

		RLogger::RLogger() {
			CurrentInstance = this;
		}
		RLogger::~RLogger()
		{
		}

		void RLogger::printLog(std::string logMessage)
		{
			std::cout << "Rubeus: " << logMessage << std::endl;
			if (LogFile)
			{
				LogFile << "Rubeus: " << logMessage << std::endl;
			}
		}
		void RLogger::printExtendedLog(std::string logMessage, std::string file, int line)
		{
			std::cout << "RubeusLog:" << file << ":" << line << ":" << logMessage << "\n";
			if (LogFile)
			{
				LogFile << "RubeusLog:" << file << ":" << line << ":" << logMessage << std::endl;
			}
		}
		void RLogger::printExtendedLog(std::string logMessage, std::string severity, std::string file, int line) {
			std::cout << "\033[1;" << foregroundColorMap[SeverityMap[severity]] << "m" << "RubeusLog:" << file << ":" << line << ":" << logMessage << "\033[0m" << std::endl;
			if (LogFile)
			{
				LogFile << "RubeusLog:" << file << ":" << line << ":" << logMessage << std::endl;
			}
		}
		void RLogger::CreateLogFile()
		{
			std::string filename = "";
#ifdef _WIN32
			filename += "windows";
#elif __APPLE__
			filename += "osx";
#else
			filename += "linux";
#endif
#ifdef _DEBUG
			filename += "_debug";
#else
			filename += "_release";
#endif
			time_t t = time(0);
			struct tm * now = localtime(&t);
			char buffer[80];
			strftime(buffer, 80, "_%Y%m%d_%H%M%S.log", now);
			filename.append(buffer);
			LogFile.open(filename);
			if (!LogFile)
				ERRORLOG("Log file creation failed, Logs in this session will be lost");
			else
				SUCCESS("Log file creation successfull");
		}
		void RLogger::CloseLogFile()
		{
			LogFile.close();
		}
	}
} 
