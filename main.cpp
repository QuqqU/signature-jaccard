#include <bits/stdc++.h>
#include <random>
#include "CSVReader.hpp"
#define FASTIO std::cin.sync_with_stdio(false); std::cin.tie(0);

const std::string file = "test.csv";
const std::string delimeter = ",";
const int _K = 5;
const int _HASH_FUNCS = 15;
const int _SHINGLE_SIZE = 100;


namespace Shingle {
	int K;

	std::vector<std::string> make(std::string str, int k = K) {
		std::vector<std::string> shingles;
		for (int i = 0; i <= (int)str.length() - k; i++) {
			shingles.push_back(str.substr(i, k));
		}
		return shingles;
	}

}


int main() {
	FASTIO;
	
	// read given CSV
	// col 0 - no.
	// col 1 - str1
	// col 2 - str2
	CSVReader reader(file, delimeter, _SHINGLE_SIZE);
	auto sheet = reader.read();

	// K - shingle
	Shingle::K = _K;

	// make k-shingle from given CSV
	std::vector<std::string> shingles;
	std::vector<std::string> docs;
	for (auto line : sheet) {
		docs.push_back(line[1]);
		docs.push_back(line[2]);
		auto buf1 = Shingle::make(line[1]);
		auto buf2 = Shingle::make(line[2]);
		shingles.insert(shingles.end(), buf1.begin(), buf1.end());
		shingles.insert(shingles.end(), buf2.begin(), buf2.end());
	}
	sort(shingles.begin(), shingles.end());
	shingles.erase(unique(shingles.begin(), shingles.end()), shingles.end());
	
	
	// make permutation
	std::vector<int> perm(_SHINGLE_SIZE);
	std::iota(perm.begin(), perm.end(), 0);
	std::random_shuffle(perm.begin(), perm.end());
	

	// make hash table
	std::vector<std::vector<int>> hash_table;
	for (int i = 0; i < _HASH_FUNCS; i++) {
		std::vector<int> buf;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(1, 100);
		int hash_mul = dis(gen);
		int hash_add = dis(gen);
		auto hash = [&](int x) -> int { return (hash_mul * x + hash_add) % shingles.size(); };
		for (int j = 0; j < shingles.size(); j++) {
			buf.push_back(hash(j));
		}
		hash_table.push_back(buf);
	}

	// make reprensetation matrix
	std::vector<std::vector<int>> docs_mat;
	for (auto doc : docs) {
		std::vector<int> doc_mat;
		for (auto s : shingles) {
			if (doc.find(s) == std::string::npos) doc_mat.push_back(0);
			else doc_mat.push_back(1);
		}
		docs_mat.push_back(doc_mat);
	}

	// make signiture matrix
	std::vector<std::vector<int>> sig_mat;
	for (int i = 0; i < hash_table.size(); i++) {
		std::vector<int> sig(docs.size(), std::numeric_limits<int>::max());
		for (int e_idx = 0; e_idx < shingles.size(); e_idx++) {
			for (int d_idx = 0; d_idx < docs.size(); d_idx++) {
				if (docs_mat[d_idx][e_idx]) sig[d_idx] = std::min(sig[d_idx], hash_table[i][e_idx]);
			}
		}
		sig_mat.push_back(sig);
	}

	// evaluate
	
	auto eval_sig = [&](int doc1, int doc2) -> double {
		int cnt = 0;
		for (int i = 0; i < hash_table.size(); i++)
			if (sig_mat[i][doc1] == sig_mat[i][doc2]) cnt++;
		return 1.0 * cnt / hash_table.size();
	};

	auto eval_jac = [&](int doc1, int doc2) -> double {
		int intersec = 0;
		int uni = 0;
		for (int i = 0; i < shingles.size(); i++) {
			if (docs_mat[doc1][i] && docs_mat[doc2][i]) intersec++;
			if (docs_mat[doc1][i] || docs_mat[doc2][i]) uni++;
		}

		return 1.0 * intersec / uni;
	};

	std::cout << "=======================================================" << std::endl;
	std::cout << "       signiture similarity / jaccard similarity       " << std::endl;
	std::cout << "=======================================================" << std::endl;
	

	std::cout.fixed;
	std::cout.precision(4);
	//freopen("out_10.txt", "w", stdout);
	for(int i = 0; i < docs.size(); i++)
		for (int j = i + 1; j < docs.size(); j++) {
			std::cout << "pair " << i << ", " << j << " : \t" 
				<< eval_sig(i, j) << "\t\t\t" 
				<< eval_jac(i, j) << std::endl;
		}



	return 0;
}
