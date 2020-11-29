/*Copyright (c) 2020 Gabuniku
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


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

#include <cppglob/glob.hpp>
#include <json11.hpp>

#include "TH_Panel.h"
#include "th_data.h"
#include "TH.h"


namespace fs = std::filesystem;

//バージョン設定
const float VERSION = 0.01f;
const std::string VERSION_STR = "0.0.1";
const std::string ABOUT = "東方マネージャー Ver : "+ VERSION_STR+ "\n"+ "by Gabuniku";
const std::string default_setting_json = R"({"dirs":[] })";

fs::path SELF_PATH;
std::string MATCH_PATTERN = R"(\**\**th**.exe)";
std::vector<TH> TH_vector;

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
	std::vector<TH_Panel*> TH_P_vector;
	
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
	void Add_TH_Panel(std::string);
	void Add_TH_Panel(TH_Panel*);

private:
	void OnClose(wxCloseEvent&event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSearch_dirs(wxCommandEvent& event);
	void OnSearch_TH_exe(wxCommandEvent& event);
	void show(wxCommandEvent& event);
	void Sort();
	void Sort(wxCommandEvent& event);
	void ReSet_THPanel(wxCommandEvent& event);
	void ReSet_THPanel();
	
};

std::vector<fs::path> Search_ThDir(std::string root_path = "") {
	std::vector<fs::path> dirs;
	dirs = cppglob::glob(root_path + MATCH_PATTERN, true);
	return dirs;
}


MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, "東方 マネージャー")
{

	this->SetSizeHints(1000, 500);

	wxMenu* menuFile = new wxMenu;
	menuFile->AppendSeparator();
	menuFile->Append(wxID_ADD);
	menuFile->Append(wxID_EXIT);
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&追加");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
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
	search_dir_btn->Bind(wxEVT_BUTTON, &MainFrame::OnSearch_dirs, this, wxID_ANY);
	wxButton* sord_btn = new wxButton(this->controller_panel, wxID_ANY, "昇順に並び替え");
	sord_btn->Bind(wxEVT_BUTTON, &MainFrame::Sort, this, wxID_ANY);
	this->controller_box->Add(search_btn, 1, wxEXPAND);
	this->controller_box->Add(search_dir_btn, 1, wxEXPAND);
	this->controller_box->Add(sord_btn, 1, wxEXPAND);

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
	for (json11::Json path : this->setting_json["dirs"].array_items()) {
		std::string path_t = path.string_value();
		this->Add_TH_Panel(path_t);
	}
	this->ROOT_PANEL->Layout();

}

void MainFrame::Sort(){
	std::sort(this->TH_P_vector.begin(), TH_P_vector.end(), [](TH_Panel* a,TH_Panel* b) {
		return a->numbering < b->numbering;
		});// numberingにて比較
	this->ReSet_THPanel();
}
void MainFrame::Sort(wxCommandEvent& event){
	this->Sort();
}
void MainFrame::ReSet_THPanel(wxCommandEvent& event){
	this->ReSet_THPanel();
}
void MainFrame::ReSet_THPanel(){
	std::vector<TH_P> TH_st;
	for (TH_Panel* th_p : this->TH_P_vector) {
		try {
			TH_st.push_back(th_p->OUTPUT());
		}
		catch (...) {}
	}
	this->TH_list_window->DestroyChildren();
	int i = 0;
	std::vector<TH_Panel*>().swap(this->TH_P_vector);
	std::vector<TH>().swap(TH_vector);
	for (TH_P thst : TH_st) {
		thst.th.index = i;
		TH_Panel* th_p = new TH_Panel(this->TH_list_window,thst);
		this->Add_TH_Panel(th_p);
		i++;
	}
}

void MainFrame::OnExit(wxCommandEvent& event)
{	
	Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(ABOUT,"About", wxOK | wxICON_INFORMATION);
}

void MainFrame::Add_TH_Panel(std::vector<fs::path> path){
	for(fs::path th_path : path){
		TH th = TH(th_path, (int)this->TH_list_box->GetItemCount());

		TH_Panel* th_p =  new TH_Panel(this->TH_list_window, th_path, th);
		
		this->TH_list_box->Add(th_p, wxSizerFlags().Expand().Proportion(1));
		SetStatusText(th_path.generic_string());
		this->TH_P_vector.push_back(th_p);
		TH_vector.push_back(th);
	}
	this->TH_list_window->SetSizer(this->TH_list_box);
	this->TH_list_box->Layout();
	this->ROOT_BOX->Layout();

}

void MainFrame::Add_TH_Panel(std::string path){
	fs::path th_path = path;
	TH th = TH(th_path, (int)this->TH_list_box->GetItemCount());
	TH_Panel* th_p =  new TH_Panel(this->TH_list_window, th_path, th);
	this->TH_list_box->Add(th_p, wxSizerFlags().Expand().Proportion(1));
	SetStatusText(th_path.generic_string());
	this->TH_list_window->SetSizer(this->TH_list_box);
	this->TH_list_box->Layout();
	this->ROOT_BOX->Layout();
	this->TH_P_vector.push_back(th_p);
	TH_vector.push_back(th);
}
void MainFrame::Add_TH_Panel(TH_Panel*th_p){
    th_p->index = (int)this->TH_list_box->GetItemCount();	
	this->TH_list_box->Add(th_p, wxSizerFlags().Expand().Proportion(1));
	SetStatusText(th_p->path.generic_string());
	this->TH_list_window->SetSizer(this->TH_list_box);
	this->TH_list_box->Layout();
	this->ROOT_BOX->Layout();
	this->TH_P_vector.push_back(th_p);
	TH_vector.push_back(th_p->th);
}

void MainFrame::OnSearch_dirs(wxCommandEvent& event) {
	wxDirDialog * dig = new wxDirDialog(NULL, "", "", wxDD_DIR_MUST_EXIST);
	int r = dig->ShowModal();
	if (r == wxID_OK) {
		wxString wxpath = dig->GetPath();
		std::string path = wxpath.ToStdString();
		std::vector<fs::path> dirs;
		dirs = Search_ThDir(path);
		if (dirs.size() > 0) {
			std::string th_count = std::to_string(dirs.size());
			wxMessageBox(th_count + "件の実行ファイルが見つかりました","東方はいいぞ!");
		}
		else {
			wxMessageBox("見つかりませんでした","残念!",wxICON_WARNING);
		}
		this->Add_TH_Panel(dirs);
	}
	delete dig;
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
		this->setting_json = json11::Json::parse(default_setting_json, err);
	}
	else {
		std::string file_str;
		std::getline(setting_file, file_str);
		setting_file.close();
		std::string err;
		this->setting_json = json11::Json::parse(file_str, err);
	}
}

void MainFrame::Save_setting_file() {
	std::vector<std::string> dirs;
	for (TH th : TH_vector) {
		dirs.push_back(th.path_str);
	}
	json11::Json::object parent = {
		{"dirs",dirs}
	};
	json11::Json json = json11::Json{ parent };
	std::string dump_str = json.dump();
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
	this->frame->Save_setting_file();
	
	return 0;
}
wxIMPLEMENT_APP(MyApp);
