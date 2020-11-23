
#include "TH.h"
#include <string>


int main() {
	std::string s = "test";
	TH t = TH(s,0);
	printf(t.save_path.c_str());
}