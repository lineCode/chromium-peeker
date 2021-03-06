
#include "Dlg_Dn.h"
#include <winlamb-more/sysdlg.h>
#include "res/resource.h"
using namespace wl;
using std::wstring;

Dlg_Dn::~Dlg_Dn()
{
}

Dlg_Dn::Dlg_Dn(progress_taskbar& tb)
	: m_taskbarProg(tb)
{
	setup.dialogId = DLG_PROGRESS;
}

void Dlg_Dn::init_controls()
{
	center_on_parent();

	m_lblTitle.assign(this, LBL_LBL);
	m_progBar.assign(this, PRO_PRO)
		.set_range(0, 100)
		.set_pos(0);
}

void Dlg_Dn::handle_close_msg()
{
	on_message(WM_CLOSE, [](params&)
	{
		return TRUE; // don't close the dialog, EndDialog() not called
	});
}

bool Dlg_Dn::show_err_and_close(const wchar_t* msg, const wstring& err)
{
	// Intended to be used form within a separate thread.
	run_ui_thread([&]() {
		m_taskbarProg.set_error(true);
		sysdlg::msgbox(this, msg, err, MB_ICONERROR);
		m_taskbarProg.clear();
		EndDialog(hwnd(), IDCANCEL);
	});
	return false;
}