#ifndef TREE_BST_INDIVIDUAL
#define TREE_BST_INDIVIDUAL

#include "btree.h"
#include "DoubleLinkedList.h"

namespace idv
{
	template <typename T>
	BinarySearchTree<T> merge(const BinarySearchTree<T>& first, const BinarySearchTree<T>& second) {
		BinarySearchTree<T> merged = first;

		for (auto item : second) {
			merged.push(item);
		}

		return merged;
	}

	template <typename T>
	DoubleLinkedList<T> toLinkedList(const BinarySearchTree<T> &first) {
		DoubleLinkedList<T> ll;

		for (auto item : first) {
			ll.push_back(item);
		}

		return ll;
	}
}

#endif