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
	size_t end_mode = 0; // 0.��0;  1.�ɭ^��

						 // �ɼƦr
	auto Append_Num = [&]() {
		if(!isalpha(v[idx + 1][0])) // �U�@�ӬO�Ʀr�N���۸ɤW
			data[line_len-1][data[line_len-1].size()-1] += v[++idx];
		--item_len;
	};

	// �}�l�B�z
	for(; idx < v.size()-2; ++idx) {
		// �O�Y���׮�
		if(item_len == 0){
			++line_len;
			item_len = stoi(v[idx]);
			data.emplace_back(vector<string> {v[idx]});
		}
		// �D�Y���׬O�^�廼��
		else if(isalpha(v[idx][0])) {
			data[line_len-1].emplace_back(v[idx]);
			if(item_len == 1) { // �̫�@���ˬd
				if(isalpha(v[idx + 2][0])) { // �̫�@�կʼƦr
					--item_len;
					end_mode = 1;
					continue;
				}
			}
			Append_Num(); // �ɼƦr
			end_mode = 0;
		}
	}
	// �����B�z
	if(item_len==1 and end_mode ==0) {
		// �ɤ@��
		data[line_len-1].emplace_back(v[idx]);
		Append_Num();
	} else {
		if(end_mode==0) {
			data.emplace_back(vector<string> {v[idx]}); // �� 0
		} else {
			data[line_len-1].emplace_back(v[++idx]); // �ɭ^��
		}
	}
	return data;
}