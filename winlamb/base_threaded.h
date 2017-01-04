/**
 * Part of WinLamb - Win32 API Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include "base_inventory.h"
#include "base_wnd.h"

namespace wl {

class base_threaded final {
public:
	using funcT = std::function<void()>;
	static const UINT WM_THREAD_MESSAGE = WM_APP + 0x3FFF;
private:
	struct _callback_pack final {
		funcT func;
	};

	const base_wnd& _wnd;
	base_inventory& _inventory;
	LONG_PTR _processedVal;

public:
	base_threaded(const base_wnd& w, base_inventory& inventory, LONG_PTR processedVal) :
		_wnd(w), _inventory(inventory), _processedVal(processedVal)
	{
		this->_inventory.add_message(WM_THREAD_MESSAGE, [&](params p)->LONG_PTR {
			this->_process_threaded(p);
			return this->_processedVal;
		});
	}

	void ui_thread(funcT func) const {
		// This method is analog to SendMessage (synchronous), but intended to be called from another
		// thread, so a callback function can, tunelled by wndproc, run in the original thread of the
		// window, thus allowing GUI updates. This avoids the user to deal with a custom WM_ message.
		_callback_pack* pack = new _callback_pack{ std::move(func) };
		SendMessageW(this->_wnd.hwnd(), WM_THREAD_MESSAGE, 0, reinterpret_cast<LPARAM>(pack));
	}

private:
	void _process_threaded(params p) const {
		_callback_pack* pack = reinterpret_cast<_callback_pack*>(p.lParam);
		pack->func();
		delete pack;
	}
};

}//namespace wl