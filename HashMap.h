//
//  HashMap.h
//  W24Proj4
//
//  Created by Tiffany Orian on 3/10/24.
//

#ifndef HASHMAP_H
#define HASHMAP_H
#include <iostream>
#include <vector>
#include <list>
#include <string>

template<typename T>
class HashMap {
public:
    HashMap(double max_load = 0.75): maxLoad_(max_load), maxBuckets_(10), size_(0), bucketNum_(0)
    {
            hashMap = new std::list<Node*>*[maxBuckets_](); // initialize with nullptrs
    }

    ~HashMap() 
    {
        clear(); // call custom delete func
        delete[] hashMap; // delete pointer to array
    }
    
    int size() const // return the number of associations in the hashmap
    {
        return size_;
    }

    void insert(const std::string &key, const T &value) 
    {
        size_t index = hashFunc(key) % maxBuckets_;
        auto& bucket = hashMap[index];

        if (bucket == nullptr) // if list ptr is uninitialized
        {
            bucket = new std::list<Node*>(); // creates a new list
            bucketNum_++;
            //std::cout << "Creating new bucket for index: " << index << std::endl;
        }

        for (auto nodePtr : *bucket)// iterate through individual list
        {
            if (nodePtr->key == key) // if matching key
            {
                nodePtr->value = value;// replace value
                ///std::cout << "Updated [" << key << "] with value: " << value << std::endl;
                return;
            }
        }

        bucket->push_back(new Node(key, value));// add new node at current list in bucket
        size_++;// increase number of elements

        if (static_cast<double>(bucketNum_) / maxBuckets_ > maxLoad_) // if buckets overflow
        {
            rehash(maxBuckets_ * 2);
            //std::cout << "Rehashing..." << std::endl;
        }
    }
    
    T& operator[](const std::string &key)
    {
        size_t hashValue = hashFunc(key);
        size_t index = hashValue % maxBuckets_;

             if (!hashMap[index]) 
             {
                 hashMap[index] = new std::list<Node*>();
             }

             for (Node* node : *hashMap[index]) 
             {
                 if (node->key == key) 
                 {
                     return node->value;
                 }
             }

             Node* newNode = new Node(key, T());
             hashMap[index]->push_back(newNode);
             size_++;

             if (static_cast<double>(size_) / maxBuckets_ > maxLoad_)
             {
                 rehash(maxBuckets_ * 2);
             }

             return newNode->value;
        
    }

    T* find(const std::string &key) const
    {
        size_t index = hashFunc(key) % maxBuckets_; // Use the hash function and current number of buckets
        
        if (hashMap[index] != nullptr) { // Check if there's a list at this index
            for (const auto& nodePtr : *hashMap[index])
            { // Iterate through the list
                if (nodePtr->key == key)
                {
                    return &(nodePtr->value); // Return a pointer to the value if key is found
                }
            }
        }
        return nullptr; // Return nullptr if the key isn't found
    }

    T* find(const std::string &key)
    {
        const auto &hm = *this;
        return const_cast<T *>(hm.find(key));
    }

private:
    struct Node // key value pair
    {
        std::string key;
        T value;
        Node(std::string key, T value) : key(key), value(value) {}
    };

    std::list<Node*>** hashMap; // pointer to store hashMap
    // size variables
    int size_;
    int bucketNum_;
    int maxBuckets_;
    double maxLoad_;// percentage of buckets that can be filled

    size_t hashFunc(const std::string &key) const
    {
        return std::hash<std::string>()(key);
    }

    void rehash(int newCapacity)// holds double the amount of previous buckets
    {
        std::list<Node*>** newHashMap = new std::list<Node*>*[newCapacity](); // create new hashMap
        
        int newBucketNum = 0;// temp holder of newHashMap size

        for (int i = 0; i < maxBuckets_; i++)// iterate thru old hashMap
        {
            std::list<Node*>* bucket = hashMap[i];// bucket holds pointer to old hashMaps current list
            
            if (bucket != nullptr)// if not empty
            {
                for (auto nodePtr : *bucket) // Node ptr points to node in the old hashmap
                {
                    size_t newIndex = hashFunc(nodePtr->key) % newCapacity;
                    if (newHashMap[newIndex] == nullptr) 
                    {
                        newHashMap[newIndex] = new std::list<Node*>(); // create new list
                        newBucketNum++;// increase size of new amount of buckets
                    }
                    newHashMap[newIndex]->push_back(nodePtr);// transfers node ptr to new hashMap
                }
                delete bucket;// delete old bucket after using
            }
        }

        delete[] hashMap;
        hashMap = newHashMap;// assign new hashMaps to replace old
        maxBuckets_ = newCapacity;
        bucketNum_ = newBucketNum;
        //std::cout << "Rehash completed. New capacity: " << newCapacity << std::endl;
    }

    void clear() 
    {
        for (size_t i = 0; i < maxBuckets_; i++) 
        {
            std::list<Node*>* bucket = hashMap[i];
            if (bucket) {
                for (auto nodePtr : *bucket) 
                {
                    delete nodePtr;
                }
                delete bucket;
            }
        }
    }
};





#endif // HASHMAP_H



