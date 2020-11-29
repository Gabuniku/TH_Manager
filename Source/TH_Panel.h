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

class TH_Icon_Panel : public wxPanel {
	
public:
	wxIcon* icon;
	HICON Licon_handle, Sicon_handle;
	PCTSTR filename = L"D:\\Appdatas\\上海アリス幻樂団\\東方地霊殿体験版\\th11.exe";

	TH_Icon_Panel(wxPanel* parent,std::string name):wxPanel(parent){
		//this->filename = (PCTSTR)name.c_str();
		this->SetSize(wxSize(100,100));
		this->Set_exe_ico();
		wxStaticBitmap bit = wxStaticBitmap();
		bit.SetIcon(*this->icon);
	}
	~TH_Icon_Panel(){
		DestroyIcon(Licon_handle);
		DestroyIcon(Sicon_handle);
	}


	void OnPaint() {}

	void Set_exe_ico() {
		UINT icon_index = 0;
		UINT get = ExtractIconEx(this->filename, 0, &this->Licon_handle, &this->Sicon_handle, 1);
		if (get > 0) {
		//wxMessageBox(std::to_string(get));
			this->icon = new wxIcon();
			this->icon->CreateFromHICON(Licon_handle);
		}
	}
	void render(wxPaintEvent& evt) { this->render(); }
	void render() {
		wxMessageBox("");
		wxClientDC dc(this);
		dc.SetBrush(*wxGREEN_BRUSH); // green filling
		dc.SetPen(wxPen(wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline
		dc.DrawCircle(wxPoint(0, 0), 25 /* radius */);
		//HDC hdc = dc.GetHDC();
		//DrawIconEx(hdc,0,0,Licon_handle,100,100,0,NULL,DI_NORMAL);
	}
};

class TH_Panel : public wxPanel
{
private:
	wxBoxSizer *Boxsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *namebox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* command_box = new wxBoxSizer(wxVERTICAL);
	TH_Icon_Panel* th_iconPanel;
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
	TH th;
	
	void INIT(){
		this->name = this->th.name;
		this->numbering_str = this->th.numbering_str;
		this->index = this->th.index;		
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
		this->path_str = path.string();
		this->th = th;
		this->INIT();
		this->SetFrame();
	}

	~TH_Panel() {
		delete this->BigFont;
		delete this->image;
	}

};

