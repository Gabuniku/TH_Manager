/*#ifndef TH_DATA
#define TH_DATA*/
#pragma once
#include <map>
#include <string>


namespace TH_data {
	//THナンバリング(str) to float ナンバリング
	const std::map<std::string, float> TH_name_toNumbering = {
	{"紅魔郷"					, 6.0f},
	{"妖々夢"					, 7.0f},
	{"永夜抄"					, 8.0f},
	{"花映塚"					, 9.0f},
	{"文花帖"					, 9.5f},
	{"風神録"					, 10.0f},
	{"地霊殿"					, 11.0f},
	{"星蓮船"				    , 12.0f},
	{"ダブルスポイラー"		    , 12.5f},
	{"妖精大戦争"    			, 12.8f},
	{"神霊廟"					, 13.0f},
	{"輝針城"					, 14.0f},
	{"弾幕アマノジャク"		    , 14.3f},
	{"紺珠伝"					, 15.0f},
	{"天空璋"					, 16.0f},
	{"秘封ナイトメアダイヤリー" , 16.5f},
	{"鬼形獣"					, 17.0f}
	};

	//THナンバリング(str) to str ナンバリング
	const std::map<std::string, std::string> TH_name_toNumbering_str = {
	{"紅魔郷"					, "th06"    },
	{"妖々夢"					, "th07"    },
	{"永夜抄"					, "th08"    },
	{"花映塚"					, "th09"    },
	{"文花帖"					, "th10"    },
	{"風神録"					, "th11"    },
	{"地霊殿"					, "th12"    },
	{"星蓮船"				    , "th12"    },
	{"ダブルスポイラー"		    , "th125"   },
	{"妖精大戦争"    			, "th128"   },
	{"神霊廟"					, "th13"    },
	{"輝針城"					, "th14"    },
	{"弾幕アマノジャク"		    , "th143"   },
	{"紺珠伝"					, "th15"    },
	{"天空璋"					, "th16"    },
	{"秘封ナイトメアダイヤリー" , "th165"   },
	{"鬼形獣"					, "th17"    },
	{"虹龍洞"					, "th18"    }
	};

	//THナンバリング(str) to ナンバリング
	const std::map<std::string, float> TH_THname_toNumbering = {
	{ "th00"     , 0.0f },
	{ "th06"	 , 6.0f },
	{ "th07"	 , 7.0f },
	{ "th08"	 , 8.0f },
	{ "th09"	 , 9.0f },
	{ "th10"	 , 9.5f },
	{ "th11"	 , 10.0f },
	{ "th12"	 , 11.0f },
	{ "th12"	 , 12.0f },
	{ "th125"    , 12.5f },
	{ "th128"    , 12.8f },
	{ "th13"	 , 13.0f },
	{ "th14"	 , 14.0f },
	{ "th143"    , 14.3f },
	{ "th15"	 , 15.0f },
	{ "th16"	 , 16.0f },
	{ "th165"    , 16.5f },
	{ "th17"	 , 17.0f },
	{ "th18"	 , 18.0f }
	};

	const std::map<std::string, std::string> TH_fullname = {
	{"th00 " , "不明"                                           },
	{"th06"  , "東方紅魔郷 ～ the Embodiment of Scarlet Devil."},
	{"th07"  , "東方妖々夢 ～ Perfect Cherry Blossom."         },
	{"th08"  , "東方永夜抄 ～ Imperishable Night."             },
	{"th09"  , "東方花映塚 ～ Phantasmagoria of Flower View."  },
	{"th095" , "東方文花帖 ～ Shoot the Bullet."               },
	{"th10"  , "東方風神録 ～ Mountain of Faith."              },
	{"th11"  , "東方地霊殿 ～ Subterranean Animism."           },
	{"th12"  , "東方星蓮船 ～ Undefined Fantastic Object."     },
	{"th125" , "ダブルスポイラー ～ 東方文花帖"                },
	{"th128" , "妖精大戦争 ～ 東方三月精"                      },
	{"th13"  , "東方神霊廟 ～ Ten Desires."                    },
	{"th14"  , "東方輝針城 ～ Double Dealing Character."       },
	{"th143" , "弾幕アマノジャク ～ Impossible Spell Card."    },
	{"th15"  , "東方紺珠伝 ～ Legacy of Lunatic Kingdom."      },
	{"th16"  , "東方天空璋 ～ Hidden Star in Four Seasons."    },
	{"th165" , "秘封ナイトメアダイアリー ～ Violet Detector."  },
	{"th17"  , "東方鬼形獣 ～ Wily Beast and Weakest Creature."},
	{"th18"  , "東方虹龍洞 ～ Unconnected Marketeers."}
	};

	static const std::string Get_Fullname(std::string name) {
		std::string th_name = name;

		if (th_name.substr(0, 4) == "東方") {//ワイド文字なので4
			th_name = TH_name_toNumbering_str.at(th_name.substr(4, th_name.size()));
		}

		if (th_name.substr(th_name.size() - 2, th_name.size()) == "tr") {
			th_name = th_name.substr(0, th_name.size() - 2);
		}

		try {
			std::string th_fname = TH_fullname.at(th_name);
			return th_fname;
		}
		catch (...) {
			return "不明";
		}
	}

	static const bool Is_Numbering_str(std::string str) {
		try {
			TH_fullname.at(str);
			return true;
		}
		catch (...) {
			return false;
		}
	}

	static const std::string Get_Numbering_str(std::string name) {
		if (TH_fullname.find(name) != end(TH_fullname)) {
			return name;
		}
		else {
			if (name.substr(0, 4) == "東方") {//ワイド文字なので4
				name = TH_name_toNumbering_str.at(name.substr(4, name.size()));
			}
			if (name.substr(name.size() - 2, name.size()) == "tr") {
				name = name.substr(0, name.size() - 2);
			}
			if (TH_data::Is_Numbering_str(name)) {
				return name;
			}
			else {
				return "th00";
			}
		}
	}
	static const float Get_Numbering(std::string numbering_str) {

		return TH_THname_toNumbering.at(numbering_str);
	}
}
//#endif


