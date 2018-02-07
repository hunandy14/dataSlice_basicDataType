#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;


vector<string> ReadFile(string file_name) {
	vector<string> v;
	fstream fs(file_name, ios::in);
	for(string str; fs >> str;) {
		v.emplace_back(str);
	} fs.seekg(0, fs.beg);
	fs.close();
	return v;
}
vector<vector<string>> Data_Slice(vector<string>& v) {
	vector<vector<string>> data;
	size_t item_len = 0, idx = 0, line_len=0;
	size_t end_mode = 0; // 0.干0;  1.干璣ゅ

						 // 干计
	auto Append_Num = [&]() {
		if(!isalpha(v[idx + 1][0])) // 琌计碞钡帝干
			data[line_len-1][data[line_len-1].size()-1] += v[++idx];
		--item_len;
	};

	// 秨﹍矪瞶
	for(; idx < v.size()-2; ++idx) {
		// 琌繷
		if(item_len == 0){
			++line_len;
			item_len = stoi(v[idx]);
			data.emplace_back(vector<string> {v[idx]});
		}
		// 獶繷琌璣ゅ患搭
		else if(isalpha(v[idx][0])) {
			data[line_len-1].emplace_back(v[idx]);
			if(item_len == 1) { // 程浪琩
				if(isalpha(v[idx + 2][0])) { // 程舱计
					--item_len;
					end_mode = 1;
					continue;
				}
			}
			Append_Num(); // 干计
			end_mode = 0;
		}
	}
	// 挡Ю矪瞶
	if(item_len==1 and end_mode ==0) {
		// 干舱
		data[line_len-1].emplace_back(v[idx]);
		Append_Num();
	} else {
		if(end_mode==0) {
			data.emplace_back(vector<string> {v[idx]}); // 干 0
		} else {
			data[line_len-1].emplace_back(v[++idx]); // 干璣ゅ
		}
	}
	return data;
}