#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Rift::Editor {
	class ICommand {
	public:
		virtual ~ICommand() = default;
		virtual void Do() = 0;
		virtual void Undo() = 0;
		virtual const char* Name() const = 0;
	};

	class LambdaCommand final : public ICommand {
	public:
		LambdaCommand(std::string name, std::function<void()> doFn, std::function<void()> undoFn)
			: _name(std::move(name)), _do(std::move(doFn)), _undo(std::move(undoFn)) {}
		void Do() override { _do(); }
		void Undo() override { _undo(); }
		const char* Name() const override { return _name.c_str(); }
	private:
		std::string _name;
		std::function<void()> _do, _undo;
	};

	class CommandStack {
	public:
		static CommandStack& Get() {
			static CommandStack s;
			return s;
		}

		void Push(std::unique_ptr<ICommand> cmd, bool alreadyApplied = false) {
			if (!alreadyApplied) cmd->Do();
			_redo.clear();
			_undo.push_back(std::move(cmd));
			if (_undo.size() > kMaxDepth) _undo.erase(_undo.begin());
		}

		void Push(std::string name, std::function<void()> doFn, std::function<void()> undoFn, bool alreadyApplied = false) {
			Push(std::make_unique<LambdaCommand>(std::move(name), std::move(doFn), std::move(undoFn)), alreadyApplied);
		}

		bool CanUndo() const { return !_undo.empty(); }
		bool CanRedo() const { return !_redo.empty(); }
		const char* UndoName() const { return _undo.empty() ? "" : _undo.back()->Name(); }
		const char* RedoName() const { return _redo.empty() ? "" : _redo.back()->Name(); }

		void Undo() {
			if (_undo.empty()) return;
			auto cmd = std::move(_undo.back()); _undo.pop_back();
			cmd->Undo();
			_redo.push_back(std::move(cmd));
		}

		void Redo() {
			if (_redo.empty()) return;
			auto cmd = std::move(_redo.back()); _redo.pop_back();
			cmd->Do();
			_undo.push_back(std::move(cmd));
		}

		void Clear() { _undo.clear(); _redo.clear(); }
	private:
		static constexpr size_t kMaxDepth = 256;
		std::vector<std::unique_ptr<ICommand>> _undo, _redo;
	};
}