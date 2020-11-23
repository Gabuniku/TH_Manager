#include "NEW.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/event.h>
#endif
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <filesystem>
#include <fstream>

// 外部ライブラリー
#include <cppglob/glob.hpp>
#include <json11.hpp>


//自作ライブラリー
#include "TH_Panel.h"
#include "th_data.h"
#include "TH.h"


namespace fs = std::filesystem;

//バージョン設定
const float VERSION = 0.01f;
const std::string VERSION_STR = "0.0.1";

const std::string default_setting_json = R"({"dirs":[] })";

static fs::path SELF_PATH;
static std::vector<std::string> PATH_VECTOR;

const void Get_self_path() {
	wchar_t path[MAX_PATH + 1];
	if (0 != GetModuleFileName(NULL, path, MAX_PATH)) {
		SELF_PATH = fs::current_path();
	}
	else {
		SELF_PATH = fs::path(path);
	}
}


class MainFrame : public wxFrame
{
public:
	MainFrame();
	wxPanel* ROOT_PANEL;
	//std::vector<TH_Panel> THp_VECTOR;
	wxBoxSizer* ROOT_BOX = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* controller_box = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* controller_panel;
	wxBoxSizer* TH_list_box = new wxBoxSizer(wxVERTICAL);
	wxScrolledWindow* TH_list_window;
	json11::Json setting_json;
	std::fstream setting_file;
	fs::path self_path;
	
	void Load_setting_file();
	void Save_setting_file(wxCommandEvent& event);
	void Save_setting_file();
	void Add_TH_Panel(std::vector<fs::path>);

private:
	void OnHello(wxCommandEvent& event);
	void OnClose(wxCloseEvent&event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSearch_dirs(wxCommandEvent& event);
	void OnSearch_TH_exe(wxCommandEvent& event);
	void show(wxCommandEvent& event);
	
};
enum
{
	ID_Hello = 1
};

std::vector<fs::path> Search_ThDir(std::string root_path = "") {
	std::vector<fs::path> dirs;
	dirs = cppglob::glob(root_path + "/**th**.exe", true);
	return dirs;
}


MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, "東方 マネージャー")
{

	this->SetSizeHints(1000, 500);

	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
		"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_ADD);
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnSearch_TH_exe, this, wxID_ADD);

	//Bind(wxEVT_DESTROY, &MainFrame::OnExit, this, wxID_EXIT);

	//GUI
	this->ROOT_PANEL = new wxPanel(this);
	this->TH_list_window = new wxScrolledWindow(this->ROOT_PANEL);
	this->TH_list_window->SetScrollRate(10, 10);
	this->controller_panel = new wxPanel(this->ROOT_PANEL);

	wxButton* search_btn = new wxButton(this->controller_panel, wxID_ANY, "追加");
	search_btn->Bind(wxEVT_BUTTON, &MainFrame::OnSearch_TH_exe, this, wxID_ANY);
	wxButton* search_dir_btn = new wxButton(this->controller_panel, wxID_ANY, "フォルダー内を捜索");
	search_dir_btn->Bind(wxEVT_BUTTON, &MainFrame::show, this, wxID_ANY);
	this->controller_box->Add(search_btn, 1, wxEXPAND);
	this->controller_box->Add(search_dir_btn, 1, wxEXPAND);

	this->controller_panel->SetSizer(this->controller_box);


	this->TH_list_window->SetSizer(this->TH_list_box);

	//ROOT PANEL への関連付け
	this->ROOT_BOX->Add(this->controller_panel, 0, wxGROW);
	this->ROOT_BOX->Add(this->TH_list_window, 1, wxGROW);
	this->ROOT_PANEL->SetSizer(this->ROOT_BOX);

	this->self_path = SELF_PATH;

	SetStatusText(this->self_path.c_str());
	
	//設定ファイル読み込み
	this->Load_setting_file();

	this->ROOT_PANEL->Layout();

	//Bind(wxEVT_CLOSE_WINDOW,&MainFrame::Save_setting_file,this,wxID_ANY);

}

void MainFrame::show(wxCommandEvent& event) {
	wxMessageBox(std::to_string(PATH_VECTOR.size()));
}

void MainFrame::OnExit(wxCommandEvent& event)
{	
	Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void MainFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");

}

void MainFrame::Add_TH_Panel(std::vector<fs::path> path){
	for(fs::path th_path : path){
		TH th = TH(th_path, (int)this->TH_list_box->GetItemCount());

		TH_Panel* th_p =  NEW TH_Panel(this->TH_list_window, th_path, th);
		
		this->TH_list_box->Add(th_p, wxSizerFlags().Expand().Proportion(1));
		PATH_VECTOR.push_back(th_path.generic_string());
		SetStatusText(th_path.generic_string());
	}
	SetStatusText(std::to_string(PATH_VECTOR.size()));
	this->TH_list_window->SetSizer(this->TH_list_box);
	this->TH_list_box->Layout();
	this->ROOT_BOX->Layout();
}

void MainFrame::OnSearch_dirs(wxCommandEvent& event) {
	wxDirDialog dig = new wxDirDialog(NULL, "", "", wxDD_DIR_MUST_EXIST);
	int r = dig.ShowModal();
	if (r == wxID_OK) {
		wxString wxpath = dig.GetPath();
		std::string path = wxpath.ToStdString();
		std::vector<fs::path> dirs;
		dirs = Search_ThDir(path);
		this->Add_TH_Panel(dirs);
	}

}
void MainFrame::OnSearch_TH_exe(wxCommandEvent& event) {
	wxFileDialog * fdig = new wxFileDialog(NULL, "", "");
	fdig->SetWildcard("東方Project実行ファイル (*th.exe or **thr.exe) | *.exe");
	int r = fdig->ShowModal();
	if (r == wxID_OK) {
		fs::path* path = new fs::path(fdig->GetPath().ToStdString());
		std::vector<fs::path>* path_vect = new std::vector<fs::path>{ *path };

		this->Add_TH_Panel(*path_vect);
		delete path;
		delete path_vect;
	}
	delete fdig;
}
// file 関係
void MainFrame::Load_setting_file() {
	std::ifstream setting_file("setting.json");
	if (this->setting_file.fail()) {
		wxMessageBox("設定ファイルの読み込みに失敗しました。", "エラー", wxICON_ERROR);
		setting_file.close();
		std::ofstream file("setting.json");
		file.close();
		std::ofstream setting_file("setting.json");
		setting_file << default_setting_json;
		setting_file.close();
		std::string err;
		setting_json = json11::Json::parse(default_setting_json, err);
	}
	else {
		std::string file_str;
		std::getline(setting_file, file_str);
		setting_file.close();
		std::string err;
		setting_json = json11::Json::parse(file_str, err);
	}
}
void MainFrame::Save_setting_file() {
	std::vector<std::string> dirs = PATH_VECTOR;

	json11::Json::object object = json11::Json::object{
		{"dirs",json11::Json::array{dirs}}
	};
	json11::Json::object parent;
	parent["dirs"] = json11::Json::array{dirs};
	json11::Json json = json11::Json{ parent };
	std::string dump_str = json.dump();
	wxMessageBox(dump_str);
	std::ofstream setting_file("setting.json");
	setting_file << dump_str;
	setting_file.close();
}

void MainFrame::Save_setting_file(wxCommandEvent& event) {
	this->Save_setting_file();
}


class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	MainFrame* frame;
};

bool MyApp::OnInit() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	this->frame = new MainFrame();
	this->frame->Show(true);
	return true;
}
int MyApp::OnExit() {
	//wxMessageBox(std::to_string(PATH_VECTOR.size()));
	this->frame->Save_setting_file();
	std::vector<std::string>().swap(PATH_VECTOR);
	
	return 0;
}
wxIMPLEMENT_APP(MyApp);
