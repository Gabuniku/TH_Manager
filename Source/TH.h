#pragma once

#include <filesystem>
#include <string>
#include <Windows.h>
#include "th_data.h"
#include <shellapi.h>

class TH
{

private:
	std::string GetSavePath() {
		CHAR name[1024];
		DWORD dwUserSize = 1024;
		if (!GetUserNameA(name, &dwUserSize)) {
			return "";
		}
		std::string result = std::string("C:\\Users\\" + std::string(name) + "\\AppData\\Roaming\\ShanghaiAlice\\" + this->numbering_str);
		return result;
	}
	void INIT() {
		this->numbering_str = this->path.stem().generic_string();
		this->save_path = this->GetSavePath();
		this->name = TH_data::Get_Fullname(this->numbering_str);
		//デモ版のdatファイルの有無
		std::filesystem::path dat_path = this->numbering_str + "tr.dat";
		std::filesystem::path demo_path = this->path.parent_path() /= dat_path;
		this->Is_demo = std::filesystem::exists(demo_path);
		
	}

public:
	std::filesystem::path path;
	std::string path_str;
	std::string name;
	std::string numbering_str = "th00";
	float numbering = 0.0f;
	std::string save_path;
	int index;
	bool Is_demo;

	TH() {}

	TH(std::string path,int index=0){
		this->path = std::filesystem::path(path);
		this->index = index;
		this->INIT();
	}
	TH(std::filesystem::path path,int index=0){
		this->path = path;
		this->index = index;
		this->INIT();
	}

};
