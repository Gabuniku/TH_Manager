#include <cstdlib>
#include <new>
#include <memory>
#include <crtdbg.h>
// crtdbg.h ���C���N���[�h�������Ƃ� _CRTDBG_MAP_ALLOC ���`���Ă��
// �O�ł����Ȃ����ǁA���ꂶ��܂��̂Łi��
#define _CRTDBG_MAP_ALLOC

#define NEW  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)