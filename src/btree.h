#ifndef TREE_BST
#define TREE_BST

#include<iostream>
#include <stdexcept>
#include <iterator>
#include <memory>
#include <vector>

using namespace std;

template<typename T>
class BinarySearchTree {
	struct Node {
		T key;

		shared_ptr<Node> left, right;

		Node(const T &item) {
			key = item;
			left = right = nullptr;
		}
	};

	shared_ptr<Node> root;

	void printPreorder(weak_ptr<Node> node) const {
		if (node.expired())
			return;
		auto lck = node.lock();
		cout << lck->key << " ";
		printPreorder(lck->left);
		printPreorder(lck->right);
	}

	// digits or lowercase
	weak_ptr<Node> min(weak_ptr<Node> node) {
		while (node.lock()->left.get() != nullptr) {
			node = node.lock()->left;
		}

		return node;
	}

	// 5 dm
	shared_ptr<Node> Delete(const T &key, weak_ptr<Node> tree) {
		weak_ptr<Node> temp;
		auto ltree = tree.lock();
		if (tree.expired())
			return shared_ptr<Node>();
		else if (key < ltree->key)
			ltree->left = Delete(key, ltree->left);
		else if (key > ltree->key)
			ltree->right = Delete(key, ltree->right);
		else if (ltree->left.get() != nullptr && ltree->right.get() != nullptr) {
			temp = min(ltree->right).lock();
			ltree->key = temp.lock()->key;
			ltree->right = Delete(ltree->key, ltree->right);
		} else {
			temp = tree;
			if (ltree->left.get() == nullptr)
				tree = ltree->right;
			else if (ltree->right.get() == nullptr)
				tree = ltree->left;
			temp.lock().reset();
		}

		return tree.lock();
	}

	size_t height(weak_ptr<Node> root) const {
		size_t leftH, rightH;
		if (root.expired())
			return 0;
		else {
			leftH = height(root.lock()->left);
			rightH = height(root.lock()->right);

			if (leftH > rightH)
				return leftH + 1;
			else
				return rightH + 1;
		}
	}

	size_t count(weak_ptr<Node> root) const {
		if (root.expired())
			return 0;

		if (!root.lock()->left && !root.lock()->right) {
			return 1;
		}
		return count(root.lock()->right) + count(root.lock()->left) + 1;
	}

	vector<size_t> path(const T& key, weak_ptr<Node> root, size_t distance) const {
		vector<size_t> paths;
		if (!root.expired()) {
			auto lck = root.lock();
			if (lck->key == key) {
				paths.push_back(distance);
			}
			auto lver = path(key, lck->left, distance + 1);
			auto rver = path(key, lck->right, distance + 1);

			paths.insert(paths.end(), lver.begin(), lver.end());
			paths.insert(paths.end(), rver.begin(), rver.end());
		}
		return paths;
	}

//	a22f565da52bee36751cf194d6a31eff/74bd5f72a85733e073fbedb121227875
	void BSToVector(weak_ptr<Node> root, vector<shared_ptr<Node>> &nodes) const {
		if (root.expired())
			return;

		auto lck = root.lock();
		BSToVector(lck->left, nodes);
		nodes.push_back(lck);
		BSToVector(lck->right, nodes);
	}

	void BSToWeakVector(weak_ptr<Node> root, vector<weak_ptr<Node>> &nodes) const {
		if (root.expired())
			return;

		auto lck = root.lock();
		BSToWeakVector(lck->left, nodes);
		nodes.push_back(lck);
		BSToWeakVector(lck->right, nodes);
	}

	shared_ptr<Node> BuildItem(vector<shared_ptr<Node>> &nodes, int start, int end) {
		if (start > end)
			return shared_ptr<Node>();

		int mid = (start + end) / 2;
		shared_ptr<Node> current = nodes[mid];
		
		current->left = BuildItem(nodes, start, mid - 1);
		current->right = BuildItem(nodes, mid + 1, end);

		return current;
	}

	shared_ptr<Node> buildTree(shared_ptr<Node> root) {
		vector<shared_ptr<Node>> nodes;
		BSToVector(root, nodes);

		int n = nodes.size();
		return BuildItem(nodes, 0, n - 1);
	}

public:
	BinarySearchTree() = default;

	BinarySearchTree(const std::initializer_list<T> &list) {
		for (auto item : list) {
			push(item);
		}
	}

	BinarySearchTree(const BinarySearchTree &arg) {
		vector<weak_ptr<Node>> nodes;
		BSToWeakVector(arg.root, nodes);
		for (auto item : nodes) {
			push(item.lock()->key);
		}
	}

	BinarySearchTree &operator=(const BinarySearchTree &arg) {
		if (this != &arg) {
			vector<weak_ptr<Node>> nodes;
			BSToWeakVector(arg.root, nodes);
			for (auto item : nodes) {
				push(item.lock()->key);
			}
		}
		return *this;
	}


	const T* find(const T& key) const {
		weak_ptr<Node> n = root;
		while (!n.expired()) {
			if (n.lock()->key == key)
				return &(n.lock()->key);
			else if (key > n.lock()->key)
				n = n.lock()->right;
			else if (key < n.lock()->key)
				n = n.lock()->left;
		}
		return nullptr;
	}

	size_t height() const {
		size_t leftH, rightH;
		if (root.get() == nullptr)
			return 0;
		else {
			leftH = height(root->left);
			rightH = height(root->right);

			if (leftH > rightH)
				return leftH + 1;
			else
				return rightH + 1;
		}
	}

	void balance() {
		root = buildTree(root);
	}

	vector<size_t> path(const T& key) const {
		vector<size_t> paths;
		if (root.get() != nullptr) {
			if (root->key == key) {
				paths.push_back(0);
			}

			auto lver = path(key, root->left, 1);
			auto rver = path(key, root->right, 1);
			paths.insert(paths.end(), lver.begin(), lver.end());
			paths.insert(paths.end(), rver.begin(), rver.end());
		}
		return paths;
	}

	size_t count() const {
		if (root.get() == nullptr)
			return 0;

		if (root->left.get() == nullptr && root->right.get() == nullptr) {
			return 1;
		}
		return count(root->right) + count(root->left) + 1;
	}

	void push(const T &key) {
		shared_ptr<Node> tmp1, tmp2 = root;

		while (tmp2.get() != nullptr) {
			tmp1 = tmp2;
			if (key < tmp2->key)
				tmp2 = tmp2->left;
			else
				tmp2 = tmp2->right;
		}

		tmp2 = shared_ptr<Node>(new Node(key));

		if (tmp1.get() == nullptr)
			root = tmp2;
		else if (tmp2->key < tmp1->key)
			tmp1->left = tmp2;
		else
			tmp1->right = tmp2;
	}

	void pop(const T& key) {
		root = Delete(key, root);
	}

	void print() {
		printPreorder(root);
	}

	class iterator {
		friend class BinarySearchTree;

		vector<weak_ptr<Node>> tree;
		size_t tree_begin, tree_end, current;

		iterator(vector<weak_ptr<Node>> tree, bool is_begin): tree(tree), tree_begin(0), tree_end(tree.size()) {
			if (is_begin)
				current = tree_begin;
			else
				current = tree_end;
		}
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T *;
		using reference = T &;

		bool operator!=(iterator const &other) const {
			return current != other.current;
		}

		bool operator==(iterator const &other) const {
			return current == other.current;
		}

		T &operator*() const {
			if (current == tree_end) throw std::logic_error("Can not dereference end iterator!");
			return tree[current].lock()->key;
		}

		T *operator->() const {
			if (current == tree_end) throw std::logic_error("Can not dereference end iterator!");
			return &(tree[current].lock()->key);
		}

		iterator &operator++() {
			if (current == tree_end) throw std::out_of_range("Iterator out of range!");
			++current;
			return *this;
		}

		iterator operator++(int) {
			if (current == tree_end) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			++current;
			return temp;
		}
		iterator &operator--() {
			if (current == tree_begin) throw std::out_of_range("Iterator out of range!");
			--current;
			return *this;
		}

		iterator operator--(int) {
			if (current == tree_begin) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			--current;
			return temp;
		}
	};

	iterator begin() const {
		vector<weak_ptr<Node>> nodes;
		BSToWeakVector(root, nodes);

		return iterator(nodes, true);
	}

	iterator end() const {
		vector<weak_ptr<Node>> nodes;
		BSToWeakVector(root, nodes);

		return iterator(nodes, false);
	}

	bool operator==(const BinarySearchTree &arg) const {
		auto my_count = count(), other_count = arg.count();

		if (my_count != other_count)
			return false;

		if (my_count) {
			auto my = begin(), mye = end(), other = arg.begin();

			do {
				if (*my != *other)
					return false;

				++my; ++other;
			} while (my != mye);
		}

		return true;
	}

	bool operator>(const BinarySearchTree &arg) const {
		auto my_count = count(), other_count = arg.count();

		if (my_count != 0) {
			if (other_count != 0) {
				auto my = begin(), mye = end(), other = arg.begin(), othere = arg.end();

				while (true) {
					if (*my > *other)
						return true;
					else if (*my < *other)
						return false;

					if (++my == mye) return false;
					if (++other == othere) return true;
				}
			} else
				return true;
		}

		return false;
	}

	bool operator!=(const BinarySearchTree &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const BinarySearchTree &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const BinarySearchTree &arg) const {
		return !(*this > arg);
	}

	bool operator<(const BinarySearchTree &arg) const {
		return *this <= arg && *this != arg;
	}
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const BinarySearchTree<T> &buffer) {
	for (auto item : buffer) {
		out << item << std::endl;
	}
	return out;
}

#endif