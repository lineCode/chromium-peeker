/**
 * Part of WinLamb - Windows Lambda Library
 * @author Rodrigo Cesar de Freitas Dias
 * @see https://github.com/rodrigocfd/winlamb
 */

#pragma once
#include <functional>
#include "wnd.h"
#include "callbacks.h"

/**
 * wnd <-- wnd_proc
 */

namespace winlamb {

template<typename traitsT>
class wnd_proc : virtual public wnd {
public:
	struct params final {
		UINT   message;
		WPARAM wParam;
		LPARAM lParam;
	};
	using callback_message_type = std::function<typename traitsT::ret_type(params)>;

private:
	callbacks<UINT, params, traitsT> _callbacksMessage;
	bool _loopStarted;

public:
	virtual ~wnd_proc() = default;

protected:
	wnd_proc() : _loopStarted(false) { }

	void on_message(UINT msg, callback_message_type callback)
	{
		if (!this->_loopStarted) {
			this->_callbacksMessage.add(msg, std::move(callback));
		}
	}

	void on_message(std::initializer_list<UINT> msgs, callback_message_type callback)
	{
		if (!this->_loopStarted) {
			this->_callbacksMessage.add(msgs, std::move(callback));
		}
	}

	static typename traitsT::ret_type CALLBACK _raw_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		wnd_proc* pSelf = traitsT::get_instance_pointer<wnd_proc>(hWnd, msg, lp);
		if (pSelf) {
			if (!pSelf->_loopStarted) {
				pSelf->_loopStarted = true; // no more messages can be added
				pSelf->_hWnd = hWnd; // store HWND
			}			
			traitsT::ret_type retVal =
				pSelf->_callbacksMessage.process(hWnd, msg, msg, {msg, wp, lp});
			
			if (msg == WM_NCDESTROY) { // cleanup
				SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
				pSelf->_hWnd = nullptr;
				pSelf->_loopStarted = false; // allows window object to be recreated
			}
			return retVal;
		}
		return traitsT::default_proc(hWnd, msg, wp, lp);
	}

private:
	wnd::_hWnd;
};

}//namespace winlamb