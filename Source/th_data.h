/*#ifndef TH_DATA
#define TH_DATA*/
#pragma once
#include <map>
#include <string>


namespace TH_data {
    //THナンバリング(str) to float ナンバリング
    const std::map<std::string, float> TH_name_toNumbering = {
    {u8"紅魔郷"					, 6.0f},
    {u8"妖々夢"					, 7.0f},
    {u8"永夜抄"					, 8.0f},
    {u8"花映塚"					, 9.0f},
    {u8"文花帖"					, 9.5f},
    {u8"風神録"					, 10.0f},
    {u8"地霊殿"					, 11.0f},
    {u8"星蓮船"					, 12.0f},
    {u8"ダブルスポイラー"		, 12.5f},
    {u8"妖精大戦争"    			, 12.8f},
    {u8"神霊廟"					, 13.0f},
    {u8"輝針城"					, 14.0f},
    {u8"弾幕アマノジャク"		, 14.3f},
    {u8"紺珠伝"					, 15.0f},
    {u8"天空璋"					, 16.0f},
    {u8"秘封ナイトメアダイヤリー", 16.5f},
    {u8"鬼形獣"					, 17.0f}
    };

    //THナンバリング(str) to ナンバリング
    const std::map<std::string, float> TH_THname_toNumbering = {
    { "th06"	 , 6.0f },
    { "th07"	 , 7.0f },
    { "th08"	 , 8.0f },
    { "th09"	 , 9.0f },
    { "th10"	 , 9.5f },
    { "th11"	 , 10.0f },
    { "th12"	 , 11.0f },
    { "th12"	 , 12.0f },
    { "th125" , 12.5f },
    { "th128" , 12.8f },
    { "th13"	 , 13.0f },
    { "th14"	 , 14.0f },
    { "th143" , 14.3f },
    { "th15"	 , 15.0f },
    { "th16"	 , 16.0f },
    { "th165" , 16.5f },
    { "th17"	 , 17.0f }
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
    {"th17"  , "東方鬼形獣 ～ Wily Beast and Weakest Creature."}
    };

    static const std::string Get_Fullname(std::string name) {
        std::string th_name = name;
        if (th_name.substr(0, 2) == u8"東方") {
            th_name = th_name.substr(2, th_name.size());
        }
        if (th_name.substr(0, 2) != u8"th") {
            return "test";
        }
        try {
            std::string th_fname = TH_fullname.at(th_name);
            return th_fname;
        }
        catch (...) {
            return "error";
        }
    }

    static const std::string Get_Numbering_str(std::string name) {
        if (TH_fullname.find(name) != end(TH_fullname)) {
            return name;
        }
     }
}
//#endif


