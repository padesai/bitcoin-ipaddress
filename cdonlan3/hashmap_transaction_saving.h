//
// Created by chris on 2/20/2017.
//

#ifndef MY_BITCOINCODE_HASHMAP_TRANSACTION_SAVING_H
#define MY_BITCOINCODE_HASHMAP_TRANSACTION_SAVING_H

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>

template<typename T>
struct TransactionDataUnit{
	int position;
	size_t time_added;
	time_t transaction_time;
	std::string ipaddress;
	std::string transaction;
	T data;
	TransactionDataUnit(T data_,std::string transaction_,std::string ipaddress_,time_t transaction_time_){
		position = 0;
		time_added = (size_t) time(NULL);
		transaction_time = transaction_time_;
		ipaddress = ipaddress_;
		transaction = transaction_;
		data = data_;
	};
};

template<typename T>
struct TransactionData{
	std::map<std::string,std::vector<TransactionDataUnit<T>>> hashmap;
	int max_same_transactions;
	TransactionData(int max_transaction_depth){
		max_same_transactions = max_transaction_depth;
	}
	bool add(std::string transaction_hash,std::string ipaddress_,time_t transaction_time_,T transaction_data_){
		if (hashmap.count(transaction_hash) == 1) {
			int sz = (int) hashmap[transaction_hash].size();
			if (sz < max_same_transactions) {
				TransactionDataUnit dat(transaction_data_, transaction_hash, ipaddress_, transaction_time_);
				dat.position = sz;
				hashmap[transaction_hash].push_back(dat);
				return true;
			} else {
				return false;
			}
		} else {
			TransactionDataUnit dat(transaction_data_,transaction_hash,ipaddress_,transaction_time_);
			dat.position = 0;
			std::vector<TransactionDataUnit<T>> transaction_vector {dat};
			hashmap[transaction_hash] =  transaction_vector;
			return true;
		}
	}

	bool query(std::string transaction_hash,std::vector<TransactionDataUnit<T>> * transaction_vector){
		if (hashmap.count(transaction_hash) == 1){
			*transaction_vector = hashmap[transaction_hash];
			return true;
		} else {
			return false;
		}
	}
};

#endif //MY_BITCOINCODE_HASHMAP_TRANSACTION_SAVING_H
