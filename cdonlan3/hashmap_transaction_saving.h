//
// Created by chris on 2/20/2017.
//
#ifndef HASHMAP_TRANSACTION_SAVING_H
#define HASHMAP_TRANSACTION_SAVING_H

#include <iostream>
#include<string>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
//#include <boost/filesystem/fstream.hpp>
//#include <boost/filesystem/path.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>



using namespace std;
template<typename T>
struct TransactionDataUnit{
	int position;
	bool assumed_source;
	size_t time_added;
	time_t transaction_time;
	std::string ipaddress;
	std::string transaction_string;
	T transaction;
	TransactionDataUnit(){

	};
	TransactionDataUnit(T transaction_data_,std::string transaction_string_,std::string ipaddress_,time_t transaction_time_,bool assumed_source_){
		transaction = transaction_data_;
		transaction_string = transaction_string_;
		transaction_time = transaction_time_;
		ipaddress = ipaddress_;
		position = 0;
		time_added = (size_t) time(NULL);
		assumed_source_ = assumed_source_;
	};
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & position;
		ar & time_added;
		ar & transaction_time;
		ar & transaction_string;
		ar & ipaddress;
		ar & transaction;
		ar & assumed_source;
	}
	void serialize_to_text(string filename){
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << this;
	}

};
template<typename T>
struct TransactionData{
	std::map<std::string,std::vector<TransactionDataUnit<T>>> hashmap;
	int max_same_transactions;
	TransactionData(int max_transaction_depth){
		max_same_transactions = max_transaction_depth;
	}
	TransactionData(){};
	void prune() {
		time_t now;
		std::vector<std::string> victims;
		time(&now);
		for (std::map<std::string, std::vector<TransactionDataUnit<T>>>::iterator it = hashmap.begin(); it != hashmap.end(); ++it) {
			if (difftime(now, it->second.transaction_time) > 5 * 60) {
				victims.push_back(it->first);
			}
		}
		for (std::vector<std::string>::iterator vit = victims.begin(); vit != victims.end(); ++it) {
			hashmap.erase(*vit);
		}
	}
	bool add(std::string transaction_hash,std::string ipaddress_,time_t transaction_time_,T transaction_data_,bool assumed_source_){
		if (hashmap.count(transaction_hash) == 1) {
			int sz = (int) hashmap[transaction_hash].size();
			if (sz < max_same_transactions) {
				TransactionDataUnit<T> dat(transaction_data_, transaction_hash, ipaddress_, transaction_time_,assumed_source_);
				dat.position = sz;
				hashmap[transaction_hash].push_back(dat);
				return true;
			} else {
				return false;
			}
		} else {
			TransactionDataUnit<T> dat(transaction_data_,transaction_hash,ipaddress_,transaction_time_,assumed_source_);
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
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & max_same_transactions;
		ar & hashmap;
	}
};
template<typename T>
struct TransactionDataWrapper{
	TransactionData<T> transaction_data;

	// Did not figure out how to get these members to point to the functions in `transaction_data`
	//bool (*add)(std::string transaction_hash,std::string ipaddress,time_t transaction_time,T transaction_data);
	//bool (*query)(std::string transaction_hash,std::vector<TransactionDataUnit<T>>* transaction_vector);

	TransactionDataWrapper(){};
	TransactionDataWrapper(TransactionData<T> transaction_data_){
		transaction_data = transaction_data_;
//		add = (transaction_data_.add);
//		query = (transaction_data_.query);
	}
	void clear_data(){
		int max_depth = transaction_data.max_same_transactions;
		delete(&transaction_data);
		TransactionData<T> new_transaction_data(max_depth);
		transaction_data = new_transaction_data;

	}
	void save_to_txt(string filename){
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << transaction_data;
		ofs.close();
	}
	void load_from_txt(string filename){
		std::ifstream ifs(filename);
		boost::archive::text_iarchive ia(ifs);
		ia >> transaction_data;
		ifs.close();
//		query = transaction_data.query;
//		add = transaction_data.add;
	}
	void log(string filename){
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << transaction_data;
		ofs.close();
		clear_data();
	}
};

// Guide for how to serialize a class that you can't find in a massive codebase
//namespace boost {
//	namespace serialization {
//		template<class Archive,typename T>
//		void serialize_transaction_data(Archive & ar,TransactionData<T> & td, const unsigned int version){
//			ar & td.max_same_transactions;
//			ar & td.hashmap;
//		};
//	}
//}
#endif
