#include "stdafx.h"
#include "log.h"

using namespace System::Diagnostics;

namespace GTA {
	/// <summary>
	/// Creates the log file stream and sets the initial log level.
	/// </summary>
	/// <param name="filename">The output filename. This file will be overwritten.</param>
	/// <param name="logLevel">The <see cref="LogLevel" /> value which sets the type of messages to output.</param>
	void Log::Initialize(String^ filename, LogLevel logLevel) {
		try {
			_logWriter = gcnew StreamWriter(filename, false);
		} catch (IOException^) {
			_logWriter = gcnew StreamWriter(filename + "." + Process::GetCurrentProcess()->Id.ToString());
		}
		_logLevel = logLevel;
	}

	/// <summary>
	/// Internal method which writes a message directly to the log file.
	/// </summary>
	void Log::Write(String^ message) {
		StackTrace^ trace = gcnew StackTrace();
		StackFrame^ frame;

		for (int i = 0; i < trace->FrameCount; i++) {
			frame = trace->GetFrame(i);

			try {
				if (!frame->GetMethod()->DeclaringType->Assembly->FullName->Contains("GTAScript")) {
					break;
				}
			} catch (NullReferenceException^) { }
		}

		String^ caller = "";
		
		if (frame->GetMethod()->DeclaringType != nullptr) {
			caller = frame->GetMethod()->DeclaringType->Name + ": ";
		}

		String^ text = DateTime::Now.ToString("yyyy-MM-dd HH:mm:ss", CultureInfo::InvariantCulture) + " - " + caller + message;

		_logWriter->WriteLine(text);
		_logWriter->Flush();
	}

	/// <summary>
	/// Checks whether the log level contains the specified flag.
	/// </summary>
	/// <param name="type">The <see cref="LogLevel" /> value to check.</param>
	bool Log::MayWriteType(LogLevel type) {
		return ((_logLevel & type) == type);
	}

	/// <summary>
	/// Writes an error to the log file.
	/// </summary>
	/// <param name="message">The message to be written</param>
	void Log::Error(String^ message) {
		if (!MayWriteType(LogLevel::Error)) {
			return;
		}

		Write("ERROR: " + message);
	}

	/// <summary>
	/// Writes a warning to the log file.
	/// </summary>
	/// <param name="message">The message to be written</param>
	void Log::Warn(String^ message) {
		if (!MayWriteType(LogLevel::Warning)) {
			return;
		}

		Write("WARNING: " + message);
	}

	/// <summary>
	/// Writes an informative string to the log file.
	/// </summary>
	/// <param name="message">The message to be written</param>
	void Log::Info(String^ message) {
		if (!MayWriteType(LogLevel::Info)) {
			return;
		}

		Write("INFO: " + message);
	}

	/// <summary>
	/// Writes a debug string to the log file.
	/// </summary>
	/// <param name="message">The message to be written</param>
	void Log::Debug(String^ message) {
		if (!MayWriteType(LogLevel::Debug)) {
			return;
		}

		Write("DEBUG: " + message);
	}
}