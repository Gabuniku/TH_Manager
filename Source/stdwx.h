#pragma once
#include "wx/wxprec.h"               // wxWidgetsのプリコンパイル済み／標準 ヘッダ
#include "wx/notebook.h"             // 必要な他のヘッダ
#include "wx/statline.h"
#include "wx/tglbtn.h"

// デバッグメモリアロケーションの強化
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

#pragma warning (disable:4786)
#include <map>
#include <vector>

using namespace std;