#include <bits/stdc++.h>
#include <shared_mutex>
#include <atomic>
#include <mutex>

using namespace std;

#define HASH_SIZE_DEFAULT 1024

// Thread-Safe的Has
template <typename K, typename V> 
class HashNode
{
public:
	K key;   
	V value; 
	HashNode* next = nullptr; 

	HashNode(K _key, V _value) : key(_key) , value(_value) {}
	~HashNode() {next = nullptr; }// ?

	HashNode(const HashNode&)  = delete;
	HashNode(const HashNode&&) = delete;
	HashNode& operator=(const HashNode&) = delete;
	HashNode& operator=(const HashNode&&) = delete;
	
};

template <typename K, typename V> 
class HashBucket
{
private:
	HashNode<K, V>* head = nullptr;         
	HashNode<K, V>* tail = nullptr;         
	mutable std::shared_timed_mutex b_mutex;

	using iterator = HashNode<K , V>*;
public:
	
	~HashBucket() {this->clear();}

	// Function to find an entry in the bucket matching the key
	iterator find(K _key)
	{
		std::shared_lock<std::shared_timed_mutex> lock{b_mutex};

		for (iterator node = head ; node != nullptr ; node = node->next) {
			if (node->key == _key) {
				return node;
			}
		}
		return nullptr;
	}

	

	// Function to insert into the bucket
	bool insert(const K _key, const V _value)
	{
		// Exclusive lock to enable single write in the bucket
		std::unique_lock<std::shared_timed_mutex> lock{b_mutex};

		if (head == nullptr) {
			tail = head = new HashNode<K , V> {_key , _value};
			return true;
		}

		for (iterator node = head ; node != nullptr ; node = node->next ) {
			if (node->key == _key) {
				return false;
			}
		}

		tail = tail->next = new HashNode<K , V> {_key , _value};

		return true;
	}

	// Function to remove an entry from the bucket
	bool erase(const K _key)
	{
		std::unique_lock<std::shared_timed_mutex> lock{b_mutex};

		for (iterator node = head , prev = nullptr ; 
			node != nullptr ; 
			node = node->next) {

			if (node->key == _key) {
				prev->next = node->next;
				delete node;
				return true;
			}
			prev = node;
		}

		return false;
	}

	// Function to clear the bucket
	void clear()
	{
		std::unique_lock<std::shared_timed_mutex> lock{b_mutex};

		for (iterator node = head , prev = nullptr ; 
			node != nullptr ; 
			node = node->next) {

			if (prev != nullptr) {
				delete prev;
			}
			prev = node;
		}

		tail = head = nullptr;
	}


};

template <
	typename K, 
	typename V, 
	size_t _table_size = HASH_SIZE_DEFAULT,
	typename F = std::hash<K>
> 
class HashMap
{
private:
	HashBucket<K, V> *hashTable;
	F hasher;
	size_t table_size = 0;
	std::atomic<size_t> node_num ;

public:
	using iterator = HashNode<K , V>*;
	inline static const iterator npos = nullptr;

	HashMap() : table_size(_table_size) {
		node_num = 0;
		hashTable = new HashBucket<K, V>[_table_size]; // create the hash table as an array of hash buckets
	}

	~HashMap() {
		delete[] hashTable;
	}
	// Copy and Move of the HashMap are not supported at this moment
	HashMap(const HashMap&)  = delete;
	HashMap(const HashMap&&) = delete;
	HashMap& operator=(const HashMap&)  = delete;
	HashMap& operator=(const HashMap&&) = delete;

	size_t size() const {return node_num;}
	bool empty() const {return !node_num;}


	V operator[](K key) {
		size_t hashValue = hasher(key) % table_size;
		return (hashTable[hashValue].find(key))->value;
	}
	// Function to find an entry in the hash map matching the key.
	iterator find(K key) noexcept 
	{
		size_t hashValue = hasher(key) % table_size;
		return hashTable[hashValue].find(key);
	}

	// Function to insert into the hash map.
	bool insert(const K key, const V value)
	{
		size_t hashValue = hasher(key) % table_size;
		return hashTable[hashValue].insert(key, value) ? 
				({node_num++; true;}) : false;
	}

	// Function to remove an entry from the bucket, if found
	bool erase(const K key)
	{
		size_t hashValue = hasher(key) % table_size;
		return hashTable[hashValue].erase(key) ? 
				({node_num--; true;}) : false;
	}

	// Function to clean up the hasp map, i.e., remove all entries from it
	void clear()
	{
		for (size_t i = 0; i < table_size; i++) {
			(hashTable[i]).clear();
		}
	}

};
