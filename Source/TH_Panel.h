#pragma once

#include <wx/wx.h>
#include <wx/msw/private.h>
#include <filesystem>
#include <string>
#include <map>
#include <codecvt>
#include "th_data.h"
#include "TH.h"


namespace fs = std::filesystem;


class TH_Panel : public wxPanel
{
private:
	wxBoxSizer *Boxsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *namebox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* command_box = new wxBoxSizer(wxHORIZONTAL);
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
	TH th;
	
	void INIT(){
		this->name = this->th.name;
		this->numbering_str = this->th.numbering_str;
		this->index = this->th.index;		
	}
	/*
	wxIcon* Get_exe_icon(bool* f) {
		HINSTANCE instance = wxGetInstance();
		//std::wstring wfilepath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(this->path_str);
		//std::wstring wide;
		//widen(this->path_str, wide);
		PCTSTR filename = L"D:\\Appdatas\\上海アリス幻樂団\\東方地霊殿体験版";
		UINT icon_index = 0;
		HICON icon_handle = ExtractIcon(instance, filename, icon_index);
		if (icon_handle == 0) {
			*f = false;
			return new wxIcon();
		}
		wxIcon* icon = new wxIcon();
		icon->CreateFromHICON(icon_handle);
		//DestroyIcon(icon_handle);
		return icon;
	}
	*/

	void SetFrame() {
		wxSize size = wxSize(-1,100);
		this->SetSize(size);

		this->namepanel = new wxPanel(this);

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
		
		this->namebox->Add(this->name_t, 1,wxEXPAND);
		this->namebox->Add(this->demo_t, 1,wxEXPAND);
		this->namebox->Add(this->path_t, 1,wxEXPAND);
		this->namepanel->SetSizer(this->namebox);

		this->commandpanel = new wxPanel(this);
		this->command_box = new wxBoxSizer(wxVERTICAL);

		int command_id = this->index + 1000;

		wxButton* run_btn = new wxButton(this, command_id, "起動");
		wxButton* custom_btn = new wxButton(this, command_id, "設定");
		wxButton* open_btn = new wxButton(this->commandpanel, command_id, "フォルダーを開く");
		wxButton* open_save_btn = new wxButton(this->commandpanel, command_id, "セーブフォルダー\nを開く");
		wxButton* delete_btn = new wxButton(this, command_id, "一覧から削除");

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

	//コンストラクタ

	TH_Panel(wxPanel* ParentPanel,fs::path path,TH th) : wxPanel(ParentPanel){
		this->path = path;
		this->path_str = path.generic_string();
		this->th = th;
		this->INIT();
		this->SetFrame();
	}

};

