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
#include <ctime>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>



using namespace std;
template<typename T>
struct TransactionDataUnit{
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
		time_added = (size_t) time(NULL);
		assumed_source = assumed_source_;
	};
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
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
	time_t timer;
	int max_same_transactions;
	TransactionData(int max_transaction_depth){
		max_same_transactions = max_transaction_depth;
		timer = time(0);
	}
	TransactionData(){
		timer = time(0);
	};
	void prune() {
		time_t now;
		std::vector<std::string> victims;
		time(&now);
		for (typename std::map<std::string,std::vector<TransactionDataUnit<T>>>::iterator it = hashmap.begin(); it != hashmap.end(); ++it) {
			if (difftime(now, it->second[0].transaction_time) > 5 * 60) {
				victims.push_back(it->first);
			}
		}
		for (std::vector<std::string>::iterator vit = victims.begin(); vit != victims.end(); ++vit) {
			hashmap.erase(*vit);
		}
	}
	bool add(std::string transaction_hash,std::string ipaddress_,time_t transaction_time_,T transaction_data_,bool assumed_source_){
		if (hashmap[transaction_hash].size() < max_same_transactions) {
			TransactionDataUnit<T> dat(transaction_data_, transaction_hash, ipaddress_, transaction_time_,assumed_source_);
			hashmap[transaction_hash].push_back(dat);
			return true;
		} else {
			// Added this so that the source is always added in at position 0.
			// ToDo: this could become a subtle bug. So watch out.
			if (assumed_source_){
				TransactionDataUnit<T> dat(transaction_data_, transaction_hash, ipaddress_, transaction_time_,assumed_source_);
				hashmap[transaction_hash][0] = dat;
				return true;
			}
			return false;
		}
	}
	bool add(TransactionDataUnit<T> tdu){
		if (hashmap[tdu.transaction_string].size() < max_same_transactions){
			hashmap[tdu.transaction_string].push_back(tdu);
			return true;
		} else {
			if (tdu.assumed_source) {
				hashmap[tdu.transaction_string][0] = tdu;
				return true;
			}
			return false;
		}
	}
	bool query(std::string transaction_hash,std::vector<TransactionDataUnit<T>> * transaction_vector){
		if (hashmap[transaction_hash].size() > 0){
			for (typename vector<TransactionDataUnit<T>>::iterator it = hashmap[transaction_hash].begin();
			     it != hashmap[transaction_hash].end(); it++){
				(*transaction_vector).push_back(*it);
			}
			return true;
		}
		return false;
	}
	bool query(TransactionDataUnit<T> tdu, vector<TransactionDataUnit<T>>*tvector){
		if (hashmap[tdu.transaction_string].size() > 0){
			for (typename vector<TransactionDataUnit<T>>::iterator it = hashmap[tdu.transaction_string].begin();
			     it != hashmap[tdu.transaction_string].end(); ++it){
				(*tvector).push_back(*it);
			}
			return true;
		}
		return false;
	}
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & max_same_transactions;
		ar & hashmap;
	}
};
template<typename T>
struct TransactionDataWrapper{
	TransactionData<T> tmap;

	// Did not figure out how to get these members to point to the functions in `tmap`
	//bool (*add)(std::string transaction_hash,std::string ipaddress,time_t transaction_time,T tmap);
	//bool (*query)(std::string transaction_hash,std::vector<TransactionDataUnit<T>>* transaction_vector);

	TransactionDataWrapper(){};
	TransactionDataWrapper(TransactionData<T> transaction_data_){
		tmap = transaction_data_;
//		add = (transaction_data_.add);
//		query = (transaction_data_.query);
	}
	void clear_data(){
		int max_depth = tmap.max_same_transactions;
		delete(&tmap);
		TransactionData<T> new_transaction_data(max_depth);
		tmap = new_transaction_data;

	}
	void save_to_txt(string filename){
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << tmap;
		ofs.close();
	}
	void load_from_txt(string filename){
		std::ifstream ifs(filename);
		boost::archive::text_iarchive ia(ifs);
		ia >> tmap;
		ifs.close();
//		query = tmap.query;
//		add = tmap.add;
	}
	void log(string filename){
		std::ofstream ofs(filename);
		boost::archive::text_oarchive oa(ofs);
		oa << tmap;
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
#endif