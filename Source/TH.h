#pragma once

#include <filesystem>
#include <string>
#include <Windows.h>
#include "th_data.h"

class TH
{

private:
	std::filesystem::path GetSavePath() {
		CHAR name[1024];
		DWORD dwUserSize = 1024;
		if (!GetUserNameA(name, &dwUserSize)) {
			return std::filesystem::path();
		}
		std::string num = this->numbering_str;
		if(this->Is_demo){
			num += "tr";
		}
		std::filesystem::path result(std::string("C:\\Users\\" + std::string(name) + "\\AppData\\Roaming\\ShanghaiAlice\\" + num));
		return result;
	}
	void INIT() {
		this->numbering_str = TH_data::Get_Numbering_str(this->path.stem().generic_string());
		this->numbering = TH_data::Get_Numbering(this->numbering_str);
		this->name = TH_data::Get_Fullname(this->numbering_str);
		//デモ版のdatファイルの有無
		this->path_str = this->path.string();
		std::filesystem::path dat_path = this->numbering_str + "tr.dat";
		std::filesystem::path demo_path = this->path.parent_path() /= dat_path;
		this->Is_demo = std::filesystem::exists(demo_path);
		this->save_path = this->GetSavePath();
		
	}

public:
	std::filesystem::path path;
	std::string path_str;
	std::string name;
	std::string numbering_str = "th00";
	float numbering = 0.0f;
	std::filesystem::path save_path;
	int index=0;
	bool Is_demo;
	bool NODATA = false;

	void DESTROY() {
		this->NODATA = true;
	}

	TH() {
	}

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
