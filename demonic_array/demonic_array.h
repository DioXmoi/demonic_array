#pragma once

#ifndef _23_09_2024_DEMONIC_ARRAY_H
#define _23_09_2024_DEMONIC_ARRAY_H

#include <algorithm>
#include <cassert>
#include <initializer_list>

template <typename T>
class demonic_array {

public:
	
	class iterator {

	public:
		iterator(T* ptr = nullptr) 
			: m_ptr{ ptr }
		{
		}

		T& operator*() { return *m_ptr; }
		T* operator->() { return m_ptr; }
		T* operator++() { return ++m_ptr; }
		T* operator--() { return --m_ptr; }
		bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
		bool operator!=(const iterator& other) const { return !(*this == other); }
		friend T* operator+(iterator iter, int n) { return iter.m_ptr + n; }
		friend T* operator-(iterator iter, int n) { return iter.m_ptr - n; }
		friend T* operator+(int n, iterator iter) { return iter.m_ptr + n; }
		friend T* operator-(int n, iterator iter) { return iter.m_ptr - n; }

	private:
		T* m_ptr;

	};

	demonic_array(int size = 0);
	demonic_array(std::initializer_list<T> list);
	demonic_array(const demonic_array<T>& arr);
	demonic_array(demonic_array<T>&& arr) noexcept;
	~demonic_array();

	demonic_array<T>& operator=(const demonic_array<T>& arr);
	demonic_array<T>& operator=(demonic_array<T>&& arr) noexcept;

	iterator begin() const { return m_data; }
	iterator end() const { return m_data + m_size; }

	int capacity() const { return m_capacity; }
	int size() const { return m_size; }
	bool empty() const{ return m_size == 0; }
	void reserve(int newCapacity);

	T& operator[](int index);
	const T& operator[](int index) const;
	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

	void clear();
	void insert(int index, const T& val);
	void erase(int index);
	void push_back(const T& val);
	void push_back(T&& val);
	void push_back(std::initializer_list<T> list);
	bool pop_back();
	void resize(int count);

private:

	int m_size{ 0 };
	int m_capacity{ 0 };
	T* m_data{ nullptr };

};

template <typename T>
demonic_array<T>::demonic_array(int size) 
	: m_size{ size }
	, m_capacity{ m_size }

{
	assert(size >= 0 && "The size cannot be negative");
	m_data = new T[m_size]{ };
}

template <typename T>
demonic_array<T>::demonic_array(std::initializer_list<T> list) 
	: demonic_array(static_cast<int>(list.size()))
{
	std::copy_n(list.begin(), list.size(), begin());
}

template <typename T>
demonic_array<T>::demonic_array(const demonic_array<T>& arr) {
	m_size = arr.m_size;
	m_capacity = m_size;
	m_data = new T[m_size]{ };
	std::copy_n(arr.begin(), arr.m_size, m_data);
}

template <typename T>
demonic_array<T>::demonic_array(demonic_array<T>&& arr) noexcept {
	m_size = arr.m_size;
	m_capacity = arr.m_capacity;
	m_data = arr.m_data;

	arr.m_data = nullptr;
	arr.m_size = 0;
	arr.m_capacity = 0;
}

template <typename T>
demonic_array<T>::~demonic_array() {
	delete[] m_data;
	m_capacity = 0;
	m_size = 0;
}

template <typename T>
demonic_array<T>& demonic_array<T>::operator=(const demonic_array<T>& arr) {
	if (this == &arr) {
		return *this;
	}	

	delete[] m_data;

	m_size = arr.m_size;
	m_capacity = m_size;
	m_data = new T[m_size]{ };
	std::copy_n(arr.begin(), arr.m_size, m_data);

	return *this;
}

template <typename T>
demonic_array<T>& demonic_array<T>::operator=(demonic_array<T>&& arr) noexcept {
	if (this == &arr) {
		return *this;
	}

	delete[] m_data;

	m_size = arr.m_size;
	m_capacity = arr.m_capacity;
	m_data = arr.m_data;

	arr.m_data = nullptr;
	arr.m_size = 0;
	arr.m_capacity = 0;

	return *this;
}

template <typename T>
T& demonic_array<T>::operator[](int index) {
	assert(index >= 0 && "Going beyond the boundaries of the array");
	assert(index < m_size && "Going beyond the boundaries of the array");

	return m_data[index];
}

template <typename T>
const T& demonic_array<T>::operator[](int index) const {
	assert(index >= 0 && "Going beyond the boundaries of the array");
	assert(index < m_size && "Going beyond the boundaries of the array");

	return m_data[index];
}

template <typename T>
T& demonic_array<T>::back() {
	return m_data[m_size - 1];
}

template <typename T>
T& demonic_array<T>::front() {
	return m_data[0];
}

template <typename T>
const T& demonic_array<T>::back() const {
	return m_data[m_size - 1];
}

template <typename T>
const T& demonic_array<T>::front() const {
	return m_data[0];
}

template <typename T>
void demonic_array<T>::reserve(int newCapacity) {
	assert(newCapacity >= 0 && "The newCapacity cannot be negative");

	m_capacity = newCapacity;
	m_size = std::min(m_size, m_capacity);

	T* newData{ new T[m_capacity]{ } };
	std::copy_n(begin(), size(), newData);

	delete[] m_data;

	m_data = newData;
}

template <typename T>
void demonic_array<T>::clear() {
	m_size = 0;
}

template <typename T>
void demonic_array<T>::insert(int index, const T& val) {
	assert(index >= 0 && "Going beyond the boundaries of the array");
	assert(index < m_size && "Going beyond the boundaries of the array");
	if (size() == capacity()) {
		reserve(capacity() * 2);
	}

	for (int i{ size() }; i > index; --i) {
		m_data[i] = m_data[i - 1];
	}

	m_data[index] = val;

	++m_size;
}

template <typename T>
void demonic_array<T>::erase(int index) {
	assert(index >= 0 && "Going beyond the boundaries of the array");
	assert(index < m_size && "Going beyond the boundaries of the array");

	if (index != size() - 1) {
		std::copy_n(begin() + index + 1, size() - index, begin() + index);
	}
	--m_size;
}

template <typename T>
bool demonic_array<T>::pop_back() {
	if (size() == 0) {
		return false;
	}

	--m_size;

	return true;
}

template <typename T>
void demonic_array<T>::resize(int count) {
	assert(count >= 0 && "The size cannot be negative");

	if (count < capacity()) {
		m_size = count;
	}
	else if (count > size()) {
		reserve(count);
		m_size = count;
	}
}

template <typename T>
void demonic_array<T>::push_back(const T& val) {
	if (size() == capacity()) {
		reserve(size() == 0 ? 1 : size() * 2);
	}

	m_data[m_size] = val;
	++m_size;
}

template <typename T>
void demonic_array<T>::push_back(T&& val) {
	if (size() == capacity()) {
		reserve(size() == 0 ? 1 : size() * 2);
	}

	m_data[m_size] = val;
	++m_size;
}

template <typename T>
void demonic_array<T>::push_back(std::initializer_list<T> list) {
	int length = static_cast<int>(list.size());
	if (size() + list.size() >= capacity()) {
		reserve(size() == 0 ? length : (size() + length) * 2);
	}

	std::copy_n(list.begin(), list.size(), begin() + size());
	m_size += length;
}

#endif
