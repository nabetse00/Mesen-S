#include "stdafx.h"
#include "../Core/Console.h"
#include "../Core/Debugger.h"
#include "../Core/TraceLogger.h"
#include "../Core/MemoryDumper.h"
#include "../Core/Disassembler.h"
#include "../Core/DebugTypes.h"

extern shared_ptr<Console> _console;

shared_ptr<Debugger> GetDebugger()
{
	return _console->GetDebugger();
}

extern "C"
{
	//Debugger wrapper
	DllExport void __stdcall InitializeDebugger()
	{
		GetDebugger();
	}

	DllExport void __stdcall ReleaseDebugger()
	{
		//_debugger.reset();
		//_console->StopDebugger();
	}

	DllExport bool __stdcall IsDebuggerRunning()
	{
		return _console->GetDebugger(false).get() != nullptr;
	}

	DllExport bool __stdcall IsExecutionStopped() { return GetDebugger()->IsExecutionStopped(); }
	DllExport void __stdcall ResumeExecution() { GetDebugger()->Run(); }
	DllExport void __stdcall Step(uint32_t count) { GetDebugger()->Step(count); }

	DllExport void __stdcall GetDisassemblyLineData(uint32_t lineIndex, CodeLineData &data) { GetDebugger()->GetDisassembler()->GetLineData(lineIndex, data); }
	DllExport uint32_t __stdcall GetDisassemblyLineCount() { return GetDebugger()->GetDisassembler()->GetLineCount(); }
	DllExport uint32_t __stdcall GetDisassemblyLineIndex(uint32_t cpuAddress) { return GetDebugger()->GetDisassembler()->GetLineIndex(cpuAddress); }
	DllExport int32_t __stdcall SearchDisassembly(const char* searchString, int32_t startPosition, int32_t endPosition, bool searchBackwards) { return GetDebugger()->GetDisassembler()->SearchCode(searchString, startPosition, endPosition, searchBackwards); }

	DllExport void __stdcall SetTraceOptions(TraceLoggerOptions options) { GetDebugger()->GetTraceLogger()->SetOptions(options); }
	DllExport void __stdcall StartTraceLogger(char* filename) { GetDebugger()->GetTraceLogger()->StartLogging(filename); }
	DllExport void __stdcall StopTraceLogger() { GetDebugger()->GetTraceLogger()->StopLogging(); }
	DllExport const char* GetExecutionTrace(uint32_t lineCount) { return GetDebugger()->GetTraceLogger()->GetExecutionTrace(lineCount); }

	DllExport int32_t __stdcall EvaluateExpression(char* expression, EvalResultType *resultType, bool useCache) { return GetDebugger()->EvaluateExpression(expression, *resultType, useCache); }

	DllExport void __stdcall GetState(DebugState *state) { GetDebugger()->GetState(state); }

	DllExport void __stdcall SetMemoryState(SnesMemoryType type, uint8_t *buffer, int32_t length) { GetDebugger()->GetMemoryDumper()->SetMemoryState(type, buffer, length); }
	DllExport uint32_t __stdcall GetMemorySize(SnesMemoryType type) { return GetDebugger()->GetMemoryDumper()->GetMemorySize(type); }
	DllExport void __stdcall GetMemoryState(SnesMemoryType type, uint8_t *buffer) { GetDebugger()->GetMemoryDumper()->GetMemoryState(type, buffer); }
	DllExport uint8_t __stdcall GetMemoryValue(SnesMemoryType type, uint32_t address) { return GetDebugger()->GetMemoryDumper()->GetMemoryValue(type, address); }
	DllExport void __stdcall SetMemoryValue(SnesMemoryType type, uint32_t address, uint8_t value) { return GetDebugger()->GetMemoryDumper()->SetMemoryValue(type, address, value); }
	DllExport void __stdcall SetMemoryValues(SnesMemoryType type, uint32_t address, uint8_t* data, int32_t length) { return GetDebugger()->GetMemoryDumper()->SetMemoryValues(type, address, data, length); }
};