#pragma once
#include "NEW.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/event.h>
#endif
#ifdef __WXMSW__
#include <wx/msw/private.h>
#endif
#include <filesystem>
#include <string>
#include <map>
#include <codecvt>
#include "th_data.h"
#include "TH.h"

namespace fs = std::filesystem;

struct TH_P {
	fs::path path;
	TH th;
};

class TH_Panel : public wxPanel
{
private:
	wxBoxSizer *Boxsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *namebox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* command_box = new wxBoxSizer(wxVERTICAL);
	wxStaticBitmap* th_icon;
	wxPanel *namepanel;
	wxPanel *commandpanel;
	wxStaticText *name_t;
	wxStaticText *path_t;
	wxStaticText *demo_t;
	wxStaticText *version_t;
	wxStaticText *index_t;
	wxStaticText* number_t;
	wxStaticBitmap* image;
	wxFont* BigFont = new wxFont(15,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
	HICON Licon_handle;

	
	void INIT(){
		this->name = this->th.name;
		this->numbering_str = this->th.numbering_str;
		this->numbering = this->th.numbering;
		this->index = this->th.index;
	}

	void OnRUN_th(wxEvent &e){
		 STARTUPINFO si = { sizeof(STARTUPINFO) };
		 PROCESS_INFORMATION pi = {};
		 CreateProcess(this->path.wstring().c_str(),(LPWSTR)L"",NULL,NULL,FALSE,0,NULL,(LPWSTR)this->path.parent_path().wstring().c_str(),&si,&pi);
		 CloseHandle(pi.hThread);
		 CloseHandle(pi.hProcess);
	}
	void OnRUN_custom(wxEvent &e){
		 STARTUPINFO si = { sizeof(STARTUPINFO) };
		 PROCESS_INFORMATION pi = {};
		 fs::path custom_path = this->path.parent_path().append("custom.exe");
		 CreateProcess(custom_path.wstring().c_str(),(LPWSTR)L"",NULL,NULL,FALSE,0,NULL,this->path.parent_path().wstring().c_str(),&si,&pi);
		 CloseHandle(pi.hThread);
		 CloseHandle(pi.hProcess);
	}
	void OnOpenfolder(wxEvent& e) {
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi = {};
		std::wstring cmd_str = std::wstring(L"explorer.exe ") + this->path.parent_path().make_preferred().wstring();
		LPWSTR cmd = (LPWSTR)cmd_str.c_str();
		CreateProcess(NULL,cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	void OnOpenSavefolder(wxEvent& e) {
		//セーブフォルダー
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION pi = {};
		std::wstring cmd_str = std::wstring(L"explorer.exe ") + this->th.save_path.wstring();
		LPWSTR cmd = (LPWSTR)cmd_str.c_str();
		CreateProcess(NULL,cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	void OnDelete(wxEvent &e){
		wxMessageDialog* mes = new wxMessageDialog(this, this->name + "をリストから削除しますか？", "確認", wxICON_QUESTION | wxOK | wxCANCEL);
		if(mes->ShowModal() == wxID_OK){
			this->Destroy();
		}
		delete mes;
	}
	
	wxIcon Get_exe_icon() {
		ExtractIconEx(this->path.wstring().c_str(), 0, &this->Licon_handle,NULL, 1);
		if (this->Licon_handle == NULL) {
			this->Licon_handle = LoadIcon(NULL, IDI_QUESTION);
		}
		wxIcon icon = wxIcon();
		icon.CreateFromHICON(this->Licon_handle);
		return icon;
	}
	bool operator<(const TH_Panel& another) const
	{
		return this->numbering < another.numbering;
	};

	void SetFrame() {
		wxSize size = wxSize(-1,100);
		this->image = nullptr;
		this->SetSize(size);

		this->namepanel = new wxPanel(this);
		wxBitmap bitmap = wxBitmap();
		bitmap.CopyFromIcon(this->Get_exe_icon());
		this->th_icon = new wxStaticBitmap(this->namepanel,-1,bitmap);
		this->th_icon->SetSize(wxSize(100, 100));
		this->name_t = new wxStaticText(this->namepanel, wxID_ANY, this->name,wxDefaultPosition,wxDefaultSize,wxTE_CENTER);
		this->name_t->SetFont(*this->BigFont);

		std::string demo;
		if (this->th.Is_demo) {
			demo = "体験版"; 
			this->numbering_str += "r";
		}
		else { demo = "製品版"; }

		this->demo_t = new wxStaticText(this->namepanel, wxID_ANY, demo, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
		this->path_t = new wxStaticText(this->namepanel, wxID_ANY, this->path_str, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);

		this->index_t = new wxStaticText(this, wxID_ANY, std::to_string(this->index), wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
		this->number_t = new wxStaticText(this, wxID_ANY, this->numbering_str, wxDefaultPosition, wxDefaultSize, wxTE_CENTER);
		
		this->namebox->Add(this->th_icon, 1,wxEXPAND);
		this->namebox->Add(this->name_t, 1,wxEXPAND);
		this->namebox->Add(this->demo_t, 1,wxEXPAND);
		this->namebox->Add(this->path_t, 1,wxEXPAND);
		this->namepanel->SetSizer(this->namebox);

		this->commandpanel = new wxPanel(this);

		int command_id = this->index + 1000;

		wxButton* run_btn = new wxButton(this, command_id, "起動");
		wxButton* custom_btn = new wxButton(this, command_id, "設定");
		wxButton* open_btn = new wxButton(this->commandpanel, command_id, "フォルダーを開く");
		wxButton* open_save_btn = new wxButton(this->commandpanel, command_id, "セーブフォルダー\nを開く");
		wxButton* delete_btn = new wxButton(this, command_id, "一覧から削除");

		run_btn->Bind(wxEVT_BUTTON, &TH_Panel::OnRUN_th,this);
		custom_btn->Bind(wxEVT_BUTTON, &TH_Panel::OnRUN_custom,this);
		open_btn->Bind(wxEVT_BUTTON, &TH_Panel::OnOpenfolder,this);
		open_save_btn->Bind(wxEVT_BUTTON, &TH_Panel::OnOpenSavefolder,this);
		delete_btn->Bind(wxEVT_BUTTON, &TH_Panel::OnDelete,this);

		if(!fs::exists(this->path.parent_path().append("custom.exe"))) {
			custom_btn->Disable();
		}
		if (this->numbering < 12.5f) {
			open_save_btn->Hide();
		}


		this->command_box->Add(open_btn, 1,		wxGROW | wxALL , 5);
		this->command_box->Add(open_save_btn, 1,wxGROW | wxALL, 5);

		this->commandpanel->SetSizer(this->command_box);

		this->Boxsizer->Add(this->index_t,0,		wxGROW | wxALL, 5);
		this->Boxsizer->Add(this->number_t,0,		wxGROW | wxALL, 5);
		this->Boxsizer->Add(this->namepanel,1,		wxGROW | wxALL, 5);
		this->Boxsizer->Add(run_btn,0,				wxGROW | wxALL, 5);
		this->Boxsizer->Add(custom_btn,0,			wxGROW | wxALL, 5);
		this->Boxsizer->Add(this->commandpanel,0,	wxGROW | wxALL, 5);
		this->Boxsizer->Add(delete_btn,0,			wxGROW | wxALL, 5);
		this->SetSizer(this->Boxsizer);

	}

public:
	fs::path path;
	int index;
	std::string path_str;
	std::string name;
	std::string version_str;
	std::string fullname;
	std::string numbering_str;
	std::function<void(int)> run_fn;
	std::function<void(int)> setting_fn;
	float numbering = 0.0f;
	TH th;

	//コンストラクタ

	TH_Panel(wxPanel* ParentPanel,fs::path path,TH th) : wxPanel(ParentPanel){
		this->path = path;
		this->path_str = path.string();
		this->th = th;
		this->INIT();
		this->SetFrame();
	}
	TH_Panel(wxPanel* ParentPanel,TH_P th_p) : wxPanel(ParentPanel){
		this->path = th_p.path;
		this->path_str = path.string();
		this->th = th_p.th;
		this->INIT();
		this->SetFrame();
	}

	~TH_Panel() {
		delete this->BigFont;
		delete this->image;
	}

	TH_P OUTPUT() {
		TH_P th_p;
		th_p.path = this->path;
		th_p.th = this->th;
		return th_p;
	}

};

