#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>

template <typename K, typename V>
class Treemap {
 public:
  // * Capacity
  // Returns number of key-value mappings in map --O(1)
  size_t Size();
  // Returns true if map is empty --O(1)
  bool Empty();

  // * Modifiers
  // Inserts @key in map --O(log N) on average
  // Throws exception if key already exists
  void Insert(const K &key, const V &value);
  // Remove @key from map --O(log N) on average
  // Throws exception if key doesn't exists
  void Remove(const K &key);

  // * Lookup
  // Return value corresponding to @key --O(log N) on average
  // Throws exception if tree is empty or key doesn't exists
  const V& Get(const K &key);

  // Return greatest key less than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no floor exists for key
  const K& FloorKey(const K &key);
  // Return least key greater than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no ceil exists for key
  const K& CeilKey(const K &key);

  // Return whether @key is found in map --O(log N) on average
  bool ContainsKey(const K& key);
  // Return whether @value is found in map --O(N)
  bool ContainsValue(const V& value);

  // Return max key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MaxKey();
  // Return min key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MinKey();


 private:
  struct Node{
    K key;
    V value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  size_t size = 0;
  std::unique_ptr<Node> root;
  // helper functions
  void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
  void Remove(std::unique_ptr<Node> &n, const K &key);
  bool PreOrderRecur(Node *n, const V& value);
  Node* MinKey(Node *n);
  void Print(Node *n, int level);

};


template <typename K, typename V>
size_t Treemap<K, V>::Size() {
  return size;
}

template <typename K, typename V>
bool Treemap<K, V>::Empty() {
  if (size == 0) {
    return true;
  }
  return false;
}

template <typename K, typename V>
void Treemap<K, V>::Insert(const K &key, const V &value) {
  Insert(root, key, value);
}

template <typename K, typename V>
void Treemap<K, V>::Insert(std::unique_ptr<Node> &n,
  const K &key, const V &value) {
  if (!n) {
    n = std::unique_ptr<Node>(new Node{key, value});
    size++;
  } else if (key < n->key) {
    Insert(n->left, key, value);
  } else if (key > n->key) {
    Insert(n->right, key, value);
  } else {
     throw std::invalid_argument("Duplicate Key");
  }
}

template <typename K, typename V>
void Treemap<K, V>::Remove(const K &key) {
  Remove(root, key);
}

template <typename K, typename V>
void Treemap<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
  if (!n) return;
  if (!ContainsKey(key)) {
    throw std::invalid_argument("Invalid Key");
  }
  if (key < n->key) {
    Remove(n->left, key);
  } else if (key > n->key) {
    Remove(n->right, key);
  } else {
    // Found node
    if (n->left && n->right) {
      // Two children: replace with min node in right subtree
      n->key = MinKey(n->right.get())->key;
      n->value = MinKey(n->right.get())->value;
      size--;
    } else {
      // Replace with only child or with nullptr
      n = std::move((n->left) ? n->left : n->right);
      size--;
    }
  }
}

template <typename K, typename V>
const V& Treemap<K, V>::Get(const K &key) {
  Node* n = root.get();
  if (size == 0) {
    throw std::out_of_range("Empty tree");
  } else if (!ContainsKey(key)) {
    throw std::invalid_argument("Invalid Key");
  } else {
      while (n) {
        if (key == n->key) {
          return n->value;
        } else if (key < n->key) {
          n = n->left.get();
        } else {
          n = n->right.get();
        }
    }
  }
  return n->value;
}

template <typename K, typename V>
const K& Treemap<K, V>::FloorKey(const K &key) {
  Node *n = root.get();
  K* floorValue = new K;
  if (size == 0) {
    throw std::out_of_range("Empty tree");
  }
  if (key < MinKey()) {
    throw std::invalid_argument("No Floor exists for the given key");
  }
  if (key == MinKey()) {
    return key;
  }
  if (!n) {
    return *floorValue;
  }
  while (n) {
    if (n->key == key) {
      return n->key;
    } else if (key < n->key) {
      n = n->left.get();
    } else if (key > n->key) {
        floorValue = &n->key;
        n = n->right.get();
    }
  }
  return *floorValue;
}

template <typename K, typename V>
const K& Treemap<K, V>::CeilKey(const K &key) {
  Node *n = root.get();
  K* ceilValue = new K;
  if (size == 0) {
    throw std::out_of_range("Empty tree");
  }

  if (key > MaxKey()) {
    throw std::invalid_argument("No Ceil exists for the given key");
  }

  if (key == MaxKey()) {
    return key;
  }

  if (!n) {
    return *ceilValue;
  }
  while (n) {
     if (n->key == key) {
        return n->key;
      } else if (key > n->key) {
        n = n->right.get();
      } else if (key < n->key) {
          ceilValue = &n->key;
          n = n->left.get();
      }
    }
  return *ceilValue;
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K& key) {
  Node *n = root.get();
  while (n) {
    if (key == n->key)
      return true;
    if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }
  return false;
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V& value) {
  return (PreOrderRecur(root.get(), value));
}

template <typename K, typename V>
bool Treemap<K, V>::PreOrderRecur(Node *n, const V& value) {
  if (!n) {
    return false;
  }
  if (value == n->value) {
    return true;
  }
  if (PreOrderRecur(n->left.get(), value)) {
    return true;
  }
  if (PreOrderRecur(n->right.get(), value)) {
    return true;
  }
  return false;
}

template <typename K, typename V>
const K& Treemap<K, V>::MaxKey(void) {
  if (size == 0) {
    throw std::out_of_range("Empty tree");
  }

  Node *n = root.get();
  while (n->right) {
    n = n->right.get();
  }
  return n->key;
}

template <typename K, typename V>
const K& Treemap<K, V>::MinKey(void) {
  if (size == 0) {
    throw std::out_of_range("Empty tree");
  }
  return MinKey(root.get())->key;
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::MinKey(Node *n) {
  if (n->left) {
    return MinKey(n->left.get());
  } else {
    return n;
  }
}


#endif  // TREEMAP_H_
