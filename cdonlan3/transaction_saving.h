//
// Created by chris on 2/18/2017.
//

#ifndef MY_BITCOINCODE_TRANSACTION_SAVING_H
#define MY_BITCOINCODE_TRANSACTION_SAVING_H

#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

template<typename T>
struct DataUnit{
	int length;
	size_t time_added;
	T data;
	DataUnit(T){
		length = 0;
		time_added = (size_t) time(NULL);
	}
};
template<typename T>
struct DataStructure{
	std::vector<std::vector<DataUnit<T>>> data_table;
	size_t tableSize;
	std::hash<std::string> str_hash;
	int max_same_transactions;
	DataStructure(size_t table_size,T dummy_data_type,int max_same_transaction){
		tableSize = table_size;
		max_same_transactions = max_same_transaction;
		size_t i;
		for (i = 0; i < table_size; i++){
			data_table.push_back(std::vector<DataUnit<T>>());
			DataUnit<T> dummy_dataunit(dummy_data_type);
			dummy_dataunit.length = -1;
			data_table[i].push_back(dummy_dataunit);
		}
	}

	bool add(std::string transaction_hash,T transaction){
		size_t index = str_hash(transaction_hash)%tableSize;
		if (data_table[index][0].length == max_same_transactions - 1){
			return false;
		}
		else if (data_table[index][0].length == -1){
			DataUnit<T> unit(transaction);
			data_table[index][0] = unit; // 0 by default
		} else {
			DataUnit<T> unit(transaction);
			data_table[index].push_back(unit);
			data_table[index][0].length += 1;
		}
	}

	size_t compare_dataunit_vectors(std::vector<DataUnit<T>> dunit_vector){
		if (dunit_vector[0].length == -1){
			return (size_t) -1;
		} else {
			return dunit_vector[0].time_added;
		}
	}
	void sort_dataTable(){
		std::sort(data_table.begin(),data_table.end() + (int)tableSize,compare_dataunit_vectors);
	}
	void save_dataTable(){
		std::string x = "do stuff here";
	}
	void refresh_dataTable(){
		std::string x = "refresh the data table--care with memory!!!";
	}
};





struct HashSeed{
private:
	int seed;
	int a;
	int b;
	int n;
	int hash1(unsigned int x, int n, unsigned int s){
		srand(s + (unsigned int) x);
		return rand()%n;
	};
public:
	HashSeed(int table_size,unsigned int seed_s){
		seed = seed_s;
		n = table_size;
		srand(seed_s);
	};
	int hash(unsigned int x, int n, unsigned int s,int hash_type){
		return hash1(x,n,seed);
	};
	int hash()
};



#endif //MY_BITCOINCODE_TRANSACTION_SAVING_H
