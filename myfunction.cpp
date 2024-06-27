
#include<stdexcept>
#include<type_traits>
#include"myvector.h"
#include"myalgorithm.h"

#include<iostream>
#include<fstream>
using std::cout;
using std::endl;








template<typename T1,typename T2,typename T3>
void print(const mystl::mymap<T1, T2, T3>& m) {
	for (const auto& p : m) {
		cout << p.first << "\t" << p.second << '\n';
	}
}
void foo1() {
	
	using mystl::mymap;
	mymap<int, std::string> m = { {114,"514"},{1919,"810"} };
	m[1] = "hello";
	m[2] = "world";
	m[3] = "c++";
	m[4] = "stl";
	m[5] = "algorithm";
	m[6] = "map";
	m[7] = "vector";
	m[7] = "Vector";
	m[8] = "string";
	m[9] = "type_traits";
	m[10] = "stdexcept";
	m[11] = "iostream";
	m[12] = "myvector";
	m[13] = "myalgorithm";
	m[14] = "mymap";
	print(m);
	cout << "size: " << m.size() << endl;

	cout << "empty: " << m.empty() << endl;

	cout << "count: " << m.count(1) << endl;
	cout << "移除键为偶数的元素:\n";
	m.erase_if([](const auto& p, const auto& v) { return p % 2 == 0; });
	cout << "size: " << m.size() << endl;
	cout << "count: " << m.count(1) << endl;
	cout << "map after erase_if:\n";  print(m);
}
void foo2() {
	using mystl::mymap;
	mymap<std::string,mymap<int,std::string>> m = { {"hello",{{1,"world"}}},{"c++",{{2,"stl"}}} };
	m["algorithm"] = { {3,"map"},{4,"vector"} };
	m["type_traits"] = { {5,"string"} };
	m["stdexcept"] = { {6,"type_traits"} };
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}
	if (m.find("hello") != m.end()) {
		for (int i = 0; i < 10; i++) {
			m["hello"][i] = "world";
		}
	}
	m.insert({ "iostream",{{7,"iostream"}} });
	{   mymap<int, std::string> m2 = { {114,"514"},{1919,"810"} };
	    for (int i = 0; i < 10; i++) {
	    m2.insert({ rand() % 1000,std::to_string(i)});
	    }
		m["type_traits"] = std::move(m2);
	}
	cout << "\nmap after add:\n";
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}

	m["type_traits"].erase_if([](const auto& p, const auto& v) { return p % 2 == 0; });
	cout << "\nmap after erase_if:\n";
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}
}
void foo3() {
	using mystl::myset;
	myset<int> s = { 1,2,3,4,5,6,7,8,9,10 };
	auto it = s.insert(11); if (it.success) { std::cout << "insert\t" << (*it.position) << " success" << std::endl; }
	cout<<"size: "<<s.size()<<endl;
	for (const auto& x : s) {
		cout << x << " ";
	}
	auto temp=s.insert(10); 
	if(!temp.success){std::cout<<"insert\t"<< (*temp.position) <<" failed"<<std::endl;}
	s.erase(s.find(6), s.find(10));
	cout<<"After erasing [6,10): " << "\tsize: " << s.size() << endl;
	for (const auto& x : s) {
		cout << x << " ";
	}
	struct mystruct {
		int a; double b;
	};
	myset<mystruct,decltype([](const mystruct& x,const mystruct& y){return x.a<y.a;})> s2 = {{1,2.5},{3,4.5}};
	for (const auto& x : s2) {
		cout <<"(" << x.a << "," << x.b << ")\t";
	}
	for (int i = 0; i < 10; i++) {
		s2.insert(mystruct{ i,i*2.5 });
	}
	cout << "\nAfter inserting 10 elements:\t";
	for (const auto& x : s2) {
		cout << "(" << x.a << "," << x.b << ")\t";
	}
	cout << endl;
}
void foo4() {
	using mystl::mymultimap;
	mymultimap<std::string, mymultimap<int, std::string>> m = {
		{"hello",{{1,"world"},{2,"c++"},{1,"another_world"}}},
		{"c++",{{2,"stl"}}}
	};
	m.insert("algorithm",{{3,"map"},{4,"vector"}});
	
	m.insert("type_traits", {{5,"string"},{5,"another_string"}});

	m.insert("stdexcept",{{6,"type_traits"},{6,"another_type_traits"}});
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}
	
	m.insert("iostream",{{7,"iostream"}});
	{
		mymultimap<int, std::string> m2 = { {114,"514"},{1919,"810"} };
		for (int i = 0; i < 10; i++) {
			m2.insert(rand() % 1000,std::to_string(i));
		}
		m2.insert(114, "1919");
		auto it=m.equal_range("type_traits");
		for (auto Ite = it.begin; Ite != it.end; Ite++) {
			(*Ite).second = std::move(m2); break;
		}
	}
	cout << "\nmap after add:\n";
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}
	auto it = m.equal_range("type_traits");
	for (auto Ite = it.begin; Ite != it.end; Ite++) {
		(*Ite).second.erase_if([](const auto& p, const auto& v) { return p % 2 == 0; });
	}
	cout << "\nmap after erase_if:\n";
	for (const auto& p : m) {
		cout << p.first << ":\n";
		for (const auto& q : p.second) {
			cout << q.first << "\t" << q.second << '\n';
		}
	}
}

template<typename umap>
void print(std::ostream& os, const umap& m) {
	for (const auto& p : m) {
		os << p<< '\n';
	}
}
void foo5() {
	using mystl::myhash_table;
	myhash_table<int, int, std::identity, std::hash<int>, std::equal_to<int>>ht;
	std::ofstream out("hash_table.txt");
	for (int i = 0; i < 10; i++) {
		ht.insert_unique(i); ht.insert_unique(i*2);
	}
	if (ht.find(10) != ht.end()) {
		out<<"find 10 success"<<endl;
	}
	
	ht._print_for_analysis(out);
	ht.clear();
    out << "After clear:" << std::endl;
	
	ht._print_for_analysis(out);

	for (int i = 0; i < 10; i++) {
		ht.insert_equal(i); ht.insert_equal(i * 2);
	}
	auto _euqal_range = ht.equal_range(4);
	out<<"equal_range(4):"<<std::endl;
	for (auto Ite = _euqal_range.begin; Ite != _euqal_range.end; Ite++) {
		out << *Ite << " ";
	}
	ht.erase(4);
	out << "After erase 4:" << std::endl;
	ht._print_for_analysis(out);
	ht.clear();
	out << "Re_insert_equal 20 elements:\n";
	for (int i = 0; i < 20; i++) {
		 ht.insert_equal(i); ht.insert_equal(rand() % 4);
	}

	ht._print_for_analysis(out);

	ht.erase(3);
	out << "After erase 3:" << std::endl;
	ht._print_for_analysis(out);

	auto num=ht.erase_if([](const auto& p, const auto& v) { return p % 2 == 0; });
	out << "After erase_if key % 2 == 0,deleted num is:" << num << std::endl;
	ht._print_for_analysis(out);
}
int main() {
	
	foo5();
	return 0;
}