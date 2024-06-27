#pragma once
#include<iostream>

#include<string>

#include"myalgorithm.h"

#include<type_traits>
namespace mystl{


//����C++��vectorʵ��һ���򵥵�������
//��1���๹�ɣ�����һ��T* data���飬һ��size_t size��һ��size_t capacity
template<typename T = int>
class myvector {
private:
    T* data; //�������Ԫ�ص����飨ԭ�������ָ����������������ʣ����Բ��õ�������)
    size_t size; //�����Ĵ�С
    size_t capacity; //����������
    static const size_t BROADEN_CAPACITY_INDEX = 3;//�������ӣ�����ʱ�������Ը�����
public:
    using iterator = T*;
    myvector() :size(0), capacity(1) { //Ĭ�Ϲ��캯��
       data = new T[capacity];
    }//Ĭ�Ϲ��캯��
    myvector(const size_t &n) : size(n), capacity(n) { //���캯�������������Ĵ�С
        data = new T[capacity]{};//����ռ�
        //����ռ�
    }//���캯�������������Ĵ�С
    myvector(const myvector<T>& other) : size(other.size), capacity(other.capacity) { //�������캯��
        data=new T[capacity];// std::cerr << "���ƹ��캯������\n";
        for (size_t i = 0; i < size; i++) 
            data[i] = other.data[i];
    }//�������캯��
    template<typename U>
    myvector(const U &begin, const U &end) :myvector() {
        for (auto it = begin; it != end; it++)
            push_back(*it);
    }
    myvector(std::initializer_list<T> il) :myvector(il.begin(), il.end()) {}
    myvector(myvector&& other)noexcept :data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    ~myvector() { //��������
        delete[] data;data = nullptr;
    }//��������
    T& operator[](const size_t &index) { //�±����������
        if (index < 0 || index >= size + 1) {//Խ����
            throw "Index out of range";
        }
        else if (index == size) {//���±����������С��������β�����Ԫ��
            push_back(T{});//����β�����Ԫ��
        }
        return data[index];
    }//�±���������أ�����ͨ��[]����β�壻
    const T& operator[](const size_t &index) const {
        if (index < 0 || index >= size) {
            throw "Index out of range";
        }
        return data[index];
    }//const�汾���±����������
    T& at(const size_t &index) {
        if (index < 0 || index >= size) {
            throw "Index out of range";
        }
        return data[index];
    }//at����,����ָ��λ�õ�Ԫ��
    const T* begin()const {
        return data;
    }//����ָ��������Ԫ�ص�ָ�루const�汾��
    const T* end()const {
        return data + size;
    }//����ָ������βԪ�ص�ָ�� (const�汾)
    T* begin() {
        return data;
    }//����ָ��������Ԫ�ص�ָ�루ԭ�������ָ����������������ʣ����Բ��õ�������)
    T* end() {
        return data + size;
    }//����ָ������βԪ�ص�ָ��
    size_t getsize() const { //���������Ĵ�С
        return size;
    }//���������Ĵ�С
    size_t getcapacity() const { //��������������
        return capacity;
    }//��������������
    void push_back(const T& value) { //����β�����Ԫ��
        if (size == capacity) {
            reserve(capacity * BROADEN_CAPACITY_INDEX);
        }
        data[size++] = value;
    }//����β�����Ԫ��
    void push_back(T&& value) { //����β�����Ԫ��
        if (size == capacity) {
            reserve(capacity * BROADEN_CAPACITY_INDEX);
        }
        data[size++] = std::move(value);
    }
    template<typename... Args>
    void emplace_back(Args&&... args) {
        static_assert(std::is_constructible<T, Args...>::value, "Type T must be constructible with Args...");
        if (size == capacity) {
            reserve(capacity * BROADEN_CAPACITY_INDEX);
        }
        data[size++] = T{ std::forward<Args>(args)... };
    }
    bool insert(const size_t &index, const T& value) { //����ָ��λ�ò���Ԫ��
        if (index > size) {// ����������ڵ�ǰ��С����ִ�в������
            std::cerr << "Index out of range";
            return false;
        }
        if (size >= capacity) {
            size_t new_capacity = capacity * BROADEN_CAPACITY_INDEX;
            // �����ǰ��С�Ѿ��ﵽ�������ޣ�����չ����
            reserve(new_capacity);
        }
        for (size_t i = size; i > index; --i) {// ������λ��֮���Ԫ����������ƶ�һλ
            data[i] = std::move(data[i - 1]);
        }
        data[index] = value; // ��ָ��λ�ò�����Ԫ�أ������´�С
        ++size; return true;
    }//����ָ��λ�ò���Ԫ��
    bool erase(size_t index) { //����ָ��λ��ɾ��Ԫ��
        if (index < 0 || index >= size) {
            std::cerr << "Index out of range"; return false;
        }
        for (size_t i = index; i < size - 1; i++) {
            data[i] = std::move(data[i + 1]);
        }
        size--; return true;
    }//����ָ��λ��ɾ��Ԫ��
    void pop_back() { //����β��ɾ��Ԫ��
        if (size == 0) {
            return;
        }
        size--;
    }//����β��ɾ��Ԫ��
    void clear() { //�������
        size = 0; resize(0);
    }
    void resize(size_t newsize) { //����������С
        if (newsize < 0) {
            throw "Invalid size";
        }
        if (newsize == 0) {
            delete[] data;
            data = new T[1]{};//����һ���յĿռ�
            capacity = 1;
        }
        else if (newsize < size) {
            for (size_t i = newsize; i < size; i++) {
                data[i] = T();
            }
            size = newsize;
        }
        else if (newsize > capacity) {
            T* newdata = new T[newsize * BROADEN_CAPACITY_INDEX]{};//�����µĿռ�
            for (int i = 0; i < size; i++) {
                newdata[i] = std::move(data[i]);
            }
            delete[] data;//�ͷžɵĿռ�
            data = newdata;// ��������ָ��
            capacity = newsize * BROADEN_CAPACITY_INDEX;
        }
        size = newsize;
    }//����������С
    void sort() {
        mysort(data, data + size);
    }
    template<typename function>
    void sort(function func) {
        mysort(data, data + size, func);
    }
    template<typename function>
    void remove_if(function func) {
        size_t index = 0;
        for (size_t i = 0; i < size; i++) {//��������
            if (!func(data[i])) {//���func����false���򽫵�ǰԪ���ƶ���indexλ��
                data[index++] = std::move(data[i]);
            }
        }
        size = index;
    }
    myvector& operator=(const myvector& other) {
        if (this != &other) {
            clear();
            for (size_t i = 0; i < other.size; i++)
                push_back(other.data[i]);
        }
        return *this;
    }
    myvector& operator=(myvector&& other)noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }
    void vec_swap(myvector& other)noexcept {
        T* tempdata = data;
        data = other.data;
        other.data = tempdata;
        swap(size, other.size);
        swap(capacity, other.capacity);
    }
    void reserve(size_t n = 1000) {
        if (n < capacity) return;
        
        T* newdata = new T[n];
        for (size_t i = 0; i < capacity; i++) {
            newdata[i] = std::move(data[i]);
        }
        delete[]data;
        data = newdata;
        capacity = n;
    }
    template<typename function>
    void traverse(function func) {
        for (size_t i = 0; i < size; i++) {
            func(data[i]);
        }
    }
    bool empty() const {
        return size == 0;
    }
};

template<typename T>
static std::ostream& operator<<(std::ostream& os, const myvector<T>& v) {//�������
    os << "{";
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << *it << ", ";
    }
    os << "}";
    return os;
}


//����������
// ��3���๹�ɣ�listnode��mylist_iterator��mylist
// listnode������ڵ㣬�������ݣ�����ָ��
// mylist_iterator���������࣬���ڱ�������
// mylist�������࣬����ͷβָ�룬��С
//����ͷβָ�룬��С���Լ�������
template<typename T>
struct mylist_node {
    using pointer = mylist_node<T>*;
    T data;
    pointer prev, next;
    mylist_node(const T& val = T(), pointer p = nullptr, pointer n = nullptr) :
        data(val), prev(p), next(n) {}
    static mylist_node* create_node(const T& val = T()) {
        return new mylist_node(val);
    }
    static void destroy_node(mylist_node* p) {
        delete p;
    }
};
template<typename T, typename Ref = T&, typename Ptr = T*>
struct mylist_iterator {
    using iterator = mylist_iterator<T, T&, T*>;
    using self = mylist_iterator<T, Ref, Ptr>;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using link_type = mylist_node<T>*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    mylist_node<T>* node;
    //constructor
    mylist_iterator(link_type x = nullptr) :node(x) {}
    mylist_iterator(const iterator& x) :node(x.node) {}

    bool operator==(const mylist_iterator<T, Ref, Ptr>& x) const { return node == x.node; }
    bool operator!=(const mylist_iterator<T, Ref, Ptr>& x) const { return node != x.node; }

    reference operator*() const { return node->data; }
    pointer operator->() const { return &node->data; }
    self& operator++() {
        node = node->next;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        node = node->prev;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
};
template<typename T>
class mylist {
protected:
    using list_node = mylist_node<T>;
    using link_type = list_node*;
    using ll = long long;

    link_type node;

public:
    using iterator = mylist_iterator<T, T&, T*>;
    mylist() :node(list_node::create_node()) {
        node->prev = node->next = node;
    }
    template<typename Iter>
    mylist(Iter first, Iter last) {
        node = list_node::create_node();
        link_type p = node;
        for (Iter it = first; it != last; ++it) {
            list_node* q = list_node::create_node(*it);
            q->prev = p;
            q->next = node;
            p->next = q;
            p = q;
        }
        p->next = node;
        node->prev = p;
    }
    ~mylist() {
        link_type p = node->next;
        while (p != node) {
            link_type q = p;
            p = p->next;
            list_node::destroy_node(q);
        }
        list_node::destroy_node(node);
    }
    mylist(const mylist<T>& x) {
        node = list_node::create_node();
        link_type p = node;
        for (link_type p1 = x.node->next; p1 != x.node; p1 = p1->next) {
            link_type q = list_node::create_node(p1->data);
            q->prev = p;
            q->next = node;
            p->next = q;
            p = q;
        }
        p->next = node;
        node->prev = p;
    }
    mylist(mylist<T>&& x) :node(x.node) {
        x.node = list_node::create_node();
        x.node->prev = x.node->next = x.node;
    }
    mylist<T>& operator=(const mylist<T>& x) {
        if (this != &x) {
            clear();
            link_type p = x.node->next;
            while (p != x.node) {
                link_type q = list_node::create_node(p->data);
                q->prev = node->prev;
                q->next = node;
                node->prev->next = q;
                node->prev = q;
                p = p->next;
            }
        }
        return *this;
    }
    mylist<T>& operator=(mylist<T>&& x) {
        if (this != &x) {
            clear();
            node = x.node;
            x.node = list_node::create_node();
            x.node->prev = x.node->next = x.node;
        }
        return *this;
    }
    //iterator
    iterator begin() { return iterator(node->next); }
    iterator end() { return iterator(node); }
    const iterator begin()const { return iterator(node->next); }
    const iterator end()const { return iterator(node); }
    bool empty()const { return node->next == node; }
    size_t size()const {
        size_t count = 0;
        for (list_node* p = node->next; p != node; p = p->next)
            ++count;
        return count;
    }
    T& front() { return node->next->data; }
    T& back() { return node->prev->data; }
    //������
    iterator insert(iterator pos, const T& val) {
        link_type tmp = list_node::create_node(val);
        tmp->prev = pos.node->prev;
        tmp->next = pos.node;
        pos.node->prev->next = tmp;
        pos.node->prev = tmp;
        return iterator(tmp);
    }
    void push_front(const T& val) { insert(begin(), val); }
    void push_back(const T& val) { insert(end(), val); }
    void pop_front() { erase(iterator(node->next)); }
    void pop_back() { erase(iterator(node->prev)); }
    //ɾ����
    void clear() {
        link_type p = node->next;
        while (p != node) {
            link_type q = p;
            p = p->next;
            list_node::destroy_node(q);
        }
        node->next = node->prev = node;
    }
    iterator erase(iterator pos) {
        if (node->next == node) return iterator(node);
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        auto temp = pos.node->next;
        list_node::destroy_node(pos.node);
        return iterator(temp);
    }
    template<typename Compare>
    void remove_if(Compare cmp) {
        link_type p = node->next;
        while (p != node) {
            link_type q = p;
            p = p->next;
            if (cmp(q->data)) {
                erase(iterator(q));
            }
        }
    }
    void unique() {//ɾ���ظ���Ԫ��
        link_type p = node->next;
        while (p != node) {
            link_type q = p;
            p = p->next;
            if (q->data == p->data) {
                erase(iterator(q));
            }
        }
    }
    //�ƶ���
    void transfer(iterator position, iterator first, iterator last) {//��[first,last)�����Ԫ���ƶ�����list��positionλ��֮ǰ
        if (position == last) return;
        last.node->prev->next = position.node;
        first.node->prev->next = last.node;
        position.node->prev->next = first.node;
        link_type tmp = position.node->prev;
        position.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }
    void splice(iterator position, mylist<T>& x) {//��x�е�����Ԫ���ƶ�����list��positionλ��֮ǰ
        if (x.empty()) return;
        transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, mylist<T>&, iterator i) {//��x��iָ���Ԫ���ƶ�����list��positionλ��֮ǰ
        if (position == i || position == iterator(i.node->next))return;
        transfer(position, i, iterator(i.node->next));
    }
    void splice(iterator position, mylist<T>&, iterator first, iterator last) {
        if (first == last) return;
        transfer(position, first, last);
    }
    template<typename Compare>
    void merge(mylist<T>& x, Compare cmp) {
        iterator b1 = begin(); iterator e1 = end();
        iterator b2 = x.begin(); iterator e2 = x.end();
        while (b1 != e1 && b2 != e2) {
            if (cmp(*b2, *b1)) {
                auto next = b2;
                transfer(b1, b2, ++next);
                b2 = next;
            }
            else {
                ++b1;
            }
        }
        if (b2 != e2) {
            transfer(end(), b2, e2);
        }
    }
    void reverse() {
        if (node->next == node || node->next->next == node)return;
        iterator first = begin(); ++first;
        while (first != end()) {
            iterator temp = first;
            ++first;
            transfer(begin(), temp, first);
        }
    }
    void list_swap(mylist<T>& x) {
        auto temp = node;
        node = x.node;
        x.node = temp;
    }
    template<typename Compare>
    void sort(Compare cmp) {
        if (empty()) return;
        mylist<T> carry;
        int size = 64;
        mylist<T>* counter = new mylist<T>[size];
        int fill = 0;
        auto expand = [&]() {
            if (fill == size)
            {
                mylist<T>* new_counter = new mylist<T>[size * 2];
                for (int i = 0; i < size; i++) 
                    new_counter[i].list_swap(counter[i]);
                delete[] counter;
                counter = new_counter;
                size *= 2;
            }
        };

        while (!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                counter[i].merge(carry, cmp);
                carry.list_swap(counter[i++]);
            }
            carry.list_swap(counter[i]);
            if (i == fill) { expand(); ++fill; }

        }
        for (int i = 1; i < fill; i++)counter[i].merge(counter[i - 1], cmp);
        this->list_swap(counter[fill - 1]);
        delete[] counter;
       
    }
    T& operator[](int i) {
        auto it = begin();
        for (int j = 0; j < i; j++) {
            ++it;
        }
        return *it;
    }
    T& at(int i) {
        auto it = begin();
        for (int j = 0; j < i; j++) {
            ++it;
        }
        return *it;
    }
    template<typename U>
    void traverse(U func) {
        for (auto it = node->next; it != node; it=it->next) {
            func(it->data);
        }
    }
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const mylist<T>& lst) {
    os << "[";
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        os << *it << " ";
    }
    os << "]";
    return os;
}
//����Ϊhashmapʵ��
//��4���๹�ɣ�
//1.mypair<K, V>����ֵ���࣬����������Ա����first��second���ֱ��ʾ����ֵ
//2.node<T>���ڵ��࣬����������Ա����data��next���ֱ��ʾ��ֵ�Ժ���һ���ڵ��ָ��
//3.myhash_map<K, V>����ϣ���࣬����������Ա����buckets��size���ֱ��ʾɢ�б��Ͱ�����Ԫ�ظ���
//4.myhashmap_iterator<K, V>���������࣬�����ĸ���Ա����table��bucket_index��current��m_capacity���ֱ��ʾɢ�б��Ͱ���顢��ǰ������Ͱ��������ǰ������ָ��Ľڵ㡢ɢ�б������
template<typename K, typename V>
struct mypair {
    K first; V second;
    mypair(const K &first=K(),const V &second=V()) : first(first), second(second) {}
};
template<typename K, typename V>
mypair<K, V> mymake_pair(const K& first = K(), const V& second = V()) {
    return mypair<K, V>(first, second);
}

template<typename T>
struct node {
    T data;
    node<T>* next;
    node(const T &data=T(), node<T>* next=nullptr) : data(data), next(next) {}
    static void _insert_after(node<T>* pos, const T& val) {
        if (!pos)return;
        node<T>* new_node = new node<T>(val);
        new_node->next = pos->next;
        pos->next = new_node;
    }
    static void _erase_after(node<T>* pos) {
        if (!pos||!pos->next)return;
        node<T>* temp = pos->next;
        pos->next = temp->next;
        delete temp;
    }
};
template<typename K, typename V>
node<mypair<K, V>>* get_node(K key, V value) {
    return new node<mypair<K, V>>(mypair<K, V>(key, value));
}

template<typename K_, typename V_>
struct myhashmap_iterator {
    using value_type = mypair<K_, V_>;
    using pointer = mypair<K_, V_>*;
    using reference = mypair<K_, V_>&;
    using node_pair = node<mypair<K_, V_>>;
    using myiterator = myhashmap_iterator<K_, V_>;
    node_pair** table;            // ɢ�б��Ͱ����
    size_t bucket_index;          // ��ǰ������Ͱ����
    node_pair* current;           // ��������ǰָ��Ľڵ�
    size_t m_capacity;              // ɢ�б������
    myhashmap_iterator(node_pair** tb, size_t bi, node_pair* cur, size_t cap)
        : table(tb), bucket_index(bi), current(cur), m_capacity(cap) {}
    // �����ò�������mypair<K, V>&
    mypair<K_, V_>& operator*() {
        return current->data;
    }
    // ��Ա���ʷ��ţ� mypair<K, V>*;
    mypair<K_, V_>* operator->() {
        return &(current->data);
    }
    myiterator& operator++() {
        if (current->next) {
            current = current->next;
        }
        else {
            do {
                ++bucket_index;  // ǰ������һ��Ͱ
            } while (bucket_index < m_capacity && !table[bucket_index]);
            // �������Ͱ����ǰ������Ͱ�ĵ�һ���ڵ㣻����current��Ϊ��ָ�룬��ʾ����ĩβ
            current = (bucket_index < m_capacity) ? table[bucket_index] : nullptr;
        }
        return *this;
    }
    myiterator operator++(int) {
        auto temp=*this;
        ++(*this);
        return temp;
    }
    bool operator==(const myiterator& other) const {
        return current == other.current && bucket_index == other.bucket_index;
    }
    bool operator!=(const myiterator& other) const {
        return !(*this == other);
    }
};
static const size_t prime_num_size = 31;
static const size_t prime_num[prime_num_size] = {
   5ul,         13ul,        29ul,
   53ul,        97ul,        193ul,        389ul,      769ul,
   1543ul,      3079ul,      6151ul,       12289ul,    24593ul,
   49157ul,     98317ul,     196613ul,     393241ul,   786433ul,
   1572869ul,   3145739ul,   6291469ul,    12582917ul, 25165843ul,
   50331653ul,  100663319ul, 201326611ul,  402653189ul,805306457ul,
   1610612741ul,3221225473ul,4294967291ul
};
template<typename K = int, typename V = int>
class myhash_map {
private:
    using pair_type = mypair<const K, V>;
    using node_pair = node<pair_type>;
    using iterator = myhashmap_iterator<const K, V>; using myiterator = iterator;
    node_pair** buckets;
    myhash<K> hasher;
    size_t size; size_t capacity;size_t prime_index;
    struct product {
        bool success;
        iterator it;
        product(bool success = false, iterator it = iterator()) : success(success), it(it) {}
        operator bool() const {return success;}
        pair_type& operator*() {return  it.current->data;}
        pair_type* operator->() {return &(it.current->data);}
    };
    void  __Tidy() {
        for (size_t i = 0; i < capacity; i++) {
            node_pair* temp = buckets[i];
            while (temp) {
                node_pair* temp2 = temp;
                temp = temp->next;
                delete temp2;
            }
        }
        delete[]buckets;buckets=nullptr;
    }
public:
    myhash_map() :size(0), prime_index(0) {
        capacity = prime_num[prime_index];
        buckets = new node_pair* [capacity] {};
    }
    myhash_map(const myhash_map<K, V>& other) : myhash_map() {
        for (size_t i = 0; i < other.capacity; i++) {
            node_pair* temp = other.buckets[i];
            while (temp) {
                insert(temp->data.first, temp->data.second);
                temp = temp->next;
            }
        }
    }
    myhash_map(const std::initializer_list<mypair<K, V>>& init_list) : myhash_map() {
        for (const auto& pair : init_list) {
            insert(pair.first, pair.second);
        }
    }
    myhash_map<K, V>& operator=(const myhash_map<K, V>& other) {
        if (this == &other) {
            return *this;
        }
        __Tidy();
        prime_index = other.prime_index;
        capacity = prime_num[prime_index];
        buckets = new node_pair * [capacity] {};
        for (size_t i = 0; i < other.capacity; i++) {
            node_pair* temp = other.buckets[i];
            while (temp) {
                insert(temp->data.first, temp->data.second);
                temp = temp->next;
            }
        }
        return *this;
    }
    myhash_map<K, V>& operator=(myhash_map<K, V>&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        __Tidy();
        capacity = other.capacity;
        buckets = other.buckets;
        size = other.size;
        prime_index = other.prime_index;
        other.capacity = 0;
        other.buckets = nullptr;
        other.size = 0;
        other.prime_index = 0;
        return *this;
    }
    size_t gethash(const K &val)const {//��hasher������capacityȡ��
        return hasher(val) % capacity;
    }
    size_t get_size() const {
        return size;
    }
    size_t get_capacity() const {
        return capacity;
    }
    double get_load_factor() const {
        return (double)size / capacity;
    }
    ~myhash_map() {
        __Tidy();
    }
    //���������
    iterator begin() {
        for (size_t i = 0; i < capacity; ++i) {
            if (buckets[i]) {
                return myiterator(buckets, i, buckets[i], capacity);
            }
        }
        return iterator(buckets, capacity, nullptr, capacity);  // �����ϣ��Ϊ�գ�����ָ������ĵ�����
    }
    const iterator begin()const {
        for (size_t i = 0; i < capacity; ++i) {
            if (buckets[i]) {
                return iterator(buckets, i, buckets[i], capacity);
            }
        }
        return iterator(buckets, capacity, nullptr, capacity);  // �����ϣ��Ϊ�գ�����ָ������ĵ�����
    }
    iterator end() {
        return  iterator(buckets, capacity, nullptr, capacity);
    }
    const iterator end()const {
        return  iterator(buckets, capacity, nullptr, capacity);
    }
    //��ϣ��������
    product insert(const K& key, const V& value) {
        if (size >= capacity) {
            rehash();
        }
        size_t pos = gethash(key); node_pair* temp = buckets[pos];
        node_pair* temp2 = nullptr;//��¼ǰһ�������ݷǿսڵ�
        while (temp) {
            if (temp->data.first == key) {//�������ظ��ļ����֣�
                temp->data.second = value;
                return product(false, myiterator(buckets, pos, temp, capacity));
            }temp = temp->next;
        }
        node_pair* new_node = get_node<const K, V>(key, value);
        new_node->next = buckets[pos];
        buckets[pos] = new_node;

        size++;

        return product(true, myiterator(buckets, pos, new_node, capacity));
    }
    product insert(const mypair<K, V>& pair) {
        return insert(pair.first, pair.second);
    }
    node_pair* find(const K &key)const {
        size_t pos = gethash(key); node_pair* temp = buckets[pos];
        node<mypair<K, V>>* temp2 = nullptr;

        while (temp) {
            if (temp->data.first == key)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    bool erase(const K& key) {
        size_t i = gethash(key);
        node_pair D(pair_type(), buckets[i]);
        node_pair* prev = &D; node_pair* curr = D.next;
        bool found = false;
        while (curr) {
            if (key==(curr->data)) {
                prev->next = curr->next;
                delete curr;
                size--; found = true;
                curr = prev->next;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
        buckets[i] = D.next;
        return found;
    }
    template<typename F>
    void remove_if(F func) {
        try {
            for (size_t i = 0; i < capacity; i++) {
                node_pair D(pair_type(), buckets[i]);
                node_pair* prev = &D; node_pair* curr = D.next;
                while (curr) {
                    if (func(curr->data)) {
                        prev->next = curr->next;
                        delete curr;
                        size--;
                        curr = prev->next;
                    }
                    else {
                        prev = curr;
                        curr = curr->next;
                    }
                }
                buckets[i] = D.next;
            }
        }
        catch (...) {
            throw "remove_if error";
        }
    }
    void clear() {
        __Tidy(); prime_index = 0;
        capacity = prime_num[prime_index];
        buckets = new node_pair * [capacity] {};
        size = 0;
    }
    void rehash() {
        size_t new_prime_index = (prime_index != prime_num_size - 1) ? (prime_index + 1) : prime_index;
        size_t new_capacity = prime_num[new_prime_index];
        node_pair** new_bucket = new node_pair * [new_capacity] {};
        for (size_t i = 0; i < capacity; i++) {
            node_pair* temp = buckets[i];
            while (temp) {
                size_t index = hasher(temp->data.first) % new_capacity;//��hashֵ��
                node_pair* next = temp->next;
                temp->next = new_bucket[index];
                new_bucket[index] = temp;
                temp = next;
            }
            buckets[i] = nullptr;
        }
        delete[]buckets;
        buckets = new_bucket;
        prime_index = new_prime_index;
        capacity = new_capacity;
    }
    //����[]�����
    V& operator[](const K &key) {
        size_t index = gethash(key);
        node_pair* temp = buckets[index];
        node_pair* temp2 = nullptr;
        while (temp) {
            if (temp->data.first == key) {
                return temp->data.second;
            }
            temp = temp->next;
        }
        node_pair* newnode = get_node<const K, V>(key, V());
        //temp = newnode;
        if (size >= capacity) {
            rehash();
        }
        if (!buckets[index]) {
            buckets[index] = newnode;
        }
        else {
            node_pair* temp2 = buckets[index];
            while (temp2->next) {
                temp2 = temp2->next;
            }
            temp2->next = newnode;
        }
        size++;
        return newnode->data.second;
    }
    size_t getbucketsize(size_t index)const {
        size_t count = 0;
        node_pair* temp = buckets[index];
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }
    template<typename F>
    void traverse(F func) {
        for (size_t i = 0; i < capacity; i++) {
            node_pair* temp = buckets[i];
            if (!buckets[i])continue;
            while (temp) {
                func(temp->data.first, temp->data.second);
                temp = temp->next;
            }
        }
    }
};
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const myhash_map<K, V>& map) {
    os << "size:" << map.get_size() << " capacity:" << map.get_capacity() << " load_factor:" << map.get_load_factor() << std::endl;
    for (auto it = map.begin(); it != map.end(); ++it) {
        os << it->first << "\t:" << it->second << "\t";
    }
    return os;
}
template<typename T, size_t N>
class myarray {
private:
    T arr[N];
public:
    myarray() : arr{} {}
    myarray(const std::initializer_list<T>& init_list) {
        size_t i = 0;
        for (const auto& val : init_list) {
            if (i >= N) {
                break;
            }
            arr[i] = val;
            i++;
        }
    }
    T& operator[](size_t index) {
        if (index >= N)throw "Invalid index";
        return arr[index];
    }
    const T& operator[](size_t index) const {
        if (index >= N)throw "Invalid index";
        return arr[index];
    }
    size_t getsize() const {

        return N;
    }
   
    T* begin() {
        return arr;
    }
    T* end() {
        return arr + N;
    }
    const T* begin() const {
        return arr;
    }
    const T* end() const {
        return arr + N;
    }
   
    ~myarray() {}
    friend std::ostream& operator<<(std::ostream& os, const myarray<T, N>& arr) {
        for (size_t i = 0; i < arr.getsize(); i++) {
            os << arr.arr[i] << " ";
        }
        os << std::endl;
        return os;
    }
    void clear() {
        for (size_t i = 0; i < N; i++) {
            arr[i] = T();
        }
    }
   
};
template<typename T, typename Container = mylist<T>>
class myquene {//������
private:
    Container data;

public:
    myquene() {}
    bool empty() const {
        return data.empty();
    }
    size_t size() const {
        return data.size();
    }
    void push(const T& val) {
        data.push_back(val);
    }
    void  pop() {
        if (empty()) {
            throw "queue is empty";
        }
        data.pop_front();

    }
    T& front() {
        if (empty()) {
            throw "queue is empty";
        }
        return data.front();
    }
    ~myquene() {
    }
};
template<typename T, typename Container = mylist<T>>
class mystack {
private:
    Container data;
public:
    mystack() {}
    bool empty() const {
        return data.empty();
    }
    size_t size() const {
        return data.size();
    }
    void push(const T& val) {
        data.push_back(val);
    }
    void  pop() {
        if (empty()) {
            throw "stack is empty";
        }
        data.pop_back();

    }
    T& top() {
        if (empty()) {
            throw "stack is empty";
        }
        return data.back();
    }
    ~mystack() {
    }
};
template <typename T>
struct avl_node {
    T data;
    size_t height;
    avl_node<T>* left;
    avl_node<T>* right;

    avl_node(T dat) : data(dat), height(1), left(nullptr), right(nullptr) {}
    static size_t getHeight(avl_node<T>* data) {
        if (data == nullptr) {
            return 0;
        }
        size_t leftHeight = getHeight(data->left);
        size_t rightHeight = getHeight(data->right);
        return 1 + mymax(leftHeight, rightHeight);
    }
};

template <typename K, typename V>
class myavlmap {
private:
    using node_pair = avl_node<mypair<K, V>>;
    node_pair* root; size_t size;

public:
    myavlmap() : root(nullptr), size(0) {}
    myavlmap(const myavlmap<K, V>& other) {
        root = deepCopy(other.root); size = other.size;
    }
    myavlmap(const std::initializer_list<mypair<K, V>>& init_list) {
        root = nullptr; size = 0;
        for (const auto& pair : init_list) {
            insert(pair.first, pair.second);
        }
    }
    myavlmap<K, V>& operator =(const myavlmap<K, V>& other) {
        if (this != &other) {
            clear();
            root = deepCopy(other.root);
            size = other.size;
        }
        return *this;
    }
    myavlmap<K, V>& operator =(myavlmap<K, V>&& other)noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            size = other.size;
            other.root = nullptr;
            other.size = 0;
        }
        return *this;
    }
    void insert(K key, V value) {
        root = insertNode(root, key, value);
    }
    bool empty() const {
        return (root == nullptr);
    }
    V* find(K key) {
        node_pair* current = root;
        while (current) {
            if (key < current->data.first) {
                current = current->left;
            }
            else if (key > current->data.first) {
                current = current->right;
            }
            else {
                return &(current->data.second);
            }
        }
        return nullptr;
    }

    void remove(K key) {
        // ʵ�ּ�ֵ��ɾ��
        root = removeNode(root, key);
    }
    template<typename compare>
    void remove_if(compare predicate) {
        try {
            root = remove_if_node(root, predicate);
        }
        catch (...) {
            throw "remove_if error";
        }
    }
    V& operator[](const K& key) {
        V* valuePtr = find(key);
        if (valuePtr) {
            return *valuePtr; // ��������ֵ������
        }
        else {
            insert(key, V{}); // �����½ڵ�
            return *find(key); // �����²���ڵ��ֵ������
        }
    }
    void clear() {
        clear(root); // ����˽�г�Ա����������нڵ�
        root = nullptr;
        size = 0;
    }
    ~myavlmap() {
        clear(); // ������нڵ�

    }
    template<typename F>
    void traverse(F func) {
        try {
            traverse(root, func);
        }
        catch (...) {
            throw "traverse error";
        }
    }
    template<typename F>
    void traverse(F func)const {
        try {
            traverse(root, func);
        }
        catch (...) {
            throw "traverse error";
        }
    }

    size_t getsize() const {
        return size;
    }
    size_t getmaxheight() const {
        return node_pair::getHeight(root);
    }
    friend std::ostream& operator<<(std::ostream& os, const myavlmap<K, V>& map) {
        map.traverse([&](const K& key, const V& val) {
            os << "{" << key << "," << val << "}";
            });
        os << std::endl;
        return os;
    }
private:

    // ˽�г�Ա����������ִ�����
    node_pair* deepCopy(const node_pair* node) {
        if (node == nullptr) {
            return nullptr;
        }

        // �ݹ鴴���½ڵ㲢����ֵ
        node_pair* newNode = new node_pair(node->data);
        newNode->left = deepCopy(node->left);
        newNode->right = deepCopy(node->right);
        return newNode;
    }
    int height(node_pair* node)const {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int balanceFactor(node_pair* node)const {
        if (!node) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    node_pair* insertNode(node_pair* node, K key, V value) {
        // ʵ�ֲ����߼�
        if (!node) {
            size++; return new node_pair(mypair<K, V>(key, value));
        }

        if (key < node->data.first) {
            node->left = insertNode(node->left, key, value);
        }
        else if (key > node->data.first) {
            node->right = insertNode(node->right, key, value);
        }
        else {
            // ���Ѵ��ڣ�����ֵ
            node->data.second = value;
            return node;
        }

        // ���½ڵ�ĸ߶�
        node->height = 1 + mymax(height(node->left), height(node->right));

        // ���ƽ�����ӣ������ƽ���������ת
        int balance = balanceFactor(node);

        if (balance > 1 && key < node->left->data.first) {
            return rotateRight(node);
        }
        if (balance < -1 && key > node->right->data.first) {
            return rotateLeft(node);
        }
        if (balance > 1 && key > node->left->data.first) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && key < node->right->data.first) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node; // ƽ��
    }
    node_pair* rotateLeft(node_pair* z) {
        // ʵ������ת����
        node_pair* y = z->right;
        node_pair* T2 = y->left;

        // ִ������ת
        y->left = z;
        z->right = T2;

        // ���½ڵ�ĸ߶�
        z->height = 1 + mymax(height(z->left), height(z->right));
        y->height = 1 + mymax(height(y->left), height(y->right));

        return y;
    }
    node_pair* rotateRight(node_pair* z) {
        // ʵ������ת����
        node_pair* y = z->left;
        node_pair* T2 = y->right;

        // ִ������ת
        y->right = z;
        z->left = T2;

        // ���½ڵ�ĸ߶�
        z->height = 1 + mymax(height(z->left), height(z->right));
        y->height = 1 + mymax(height(y->left), height(y->right));

        return y;
    }
    node_pair* removeNode(node_pair* node, K key) {
        if (!node) {
            return node;
        }

        if (key < node->data.first) {
            node->left = removeNode(node->left, key);
        }
        else if (key > node->data.first) {
            node->right = removeNode(node->right, key);
        }
        else {
            // �ҵ�Ҫɾ���Ľڵ�
            if (!node->left || !node->right) {
                node_pair* temp = (node->left) ? node->left : node->right;

                // û���ӽڵ�����
                if (!temp) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    // һ���ӽڵ�����
                    *node = *temp;  // �����ӽڵ������
                }
                delete temp; node = nullptr; size--;
            }
            else {
                // �������ӽڵ��������ҵ�����������С�ڵ������ɾ���Ľڵ�
                node_pair* temp = minValueNode(node->right);
                node->data.first = temp->data.first;
                node->data.second = temp->data.second;
                node->right = removeNode(node->right, temp->data.first);
            }
        }

        if (!node) {
            return node;
        }

        // ���½ڵ�ĸ߶�
        node->height = 1 + mymax(height(node->left), height(node->right));

        // ���ƽ�����ӣ������ƽ���������ת
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && balanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }
    node_pair* minValueNode(node_pair* node) {
        node_pair* current = node;
        while (current->left) {
            current = current->left;
        }
        return current;
    }
    void clear(node_pair* node) {
        if (node) {
            if (node->left)clear(node->left);  // �ݹ����������
            if (node->right)clear(node->right); // �ݹ����������
            node->left = node->right = nullptr;
            delete node; node = nullptr; size = 0;
            // �ͷŵ�ǰ�ڵ���ڴ�
        }
    }
    // ˽�г�Ա���������ڵݹ���Ƴ����������Ľڵ�
    template<typename compare>
    node_pair* remove_if_node(node_pair* node, compare predicate) {
        if (!node) {
            return nullptr;
        }
        node->left = remove_if_node(node->left, predicate);
        node->right = remove_if_node(node->right, predicate);
        try {
            if (predicate(node->data)) {  // ����ڵ�����������ɾ���ڵ�
                return removeNode(node, node->data.first);
            }
        }
        catch (...) {
            throw "remove_if error";
        }

        return node;
    }
    // ˽�г�Ա���������ڻ�ȡ�����Ľڵ�
    node_pair* getLeftmostNode(node_pair* node) {
        if (!node) {
            return nullptr;
        }
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    template<typename F>
    void traverse(node_pair* node, F func) {
        if (node) {
            func(node->data.first, node->data.second);
            if (node->left)// ���ʵ�ǰ�ڵ�
            {
                traverse<F>(node->left, func);
            }  // �ݹ����������
            if (node->right)
            {
                traverse<F>(node->right, func);
            } // �ݹ����������
        }
    }
    template<typename F>
    void traverse(const node_pair* node, const F func)const {
        if (node) {

            if (node->left)// ���ʵ�ǰ�ڵ�
            {
                traverse<F>(node->left, func);
            }  // �ݹ����������
            func(node->data.first, node->data.second);
            if (node->right)
            {
                traverse<F>(node->right, func);
            } // �ݹ����������

        }
    }


};


} // namespace mystl


namespace mystl {//�����ռ䣺rbtree
    using Color_type = bool;
    constexpr Color_type RED = false;
    constexpr Color_type BLACK = true;
    template<typename NodeType>
    struct rbtree_node_base {
        using node_type=NodeType; using node_ptr=node_type*;
        Color_type color;

        NodeType* parent;
        NodeType* left;
        NodeType* right;
        
        rbtree_node_base(Color_type col = RED, NodeType* p = nullptr, NodeType* l = nullptr, NodeType* r = nullptr) :
            color(col), parent(p), left(l), right(r) {}
        static node_type* minimum(node_type* x) {
            while (x->left) x = x->left;
            return x;
        }
        static node_type* maximum(node_type* x) {
            while (x->right) x = x->right;
            return x;
        }

        static void rotate_left(node_ptr x, node_ptr& root) {
            node_type* y = x->right;
            x->right = y->left;
            if (y->left) y->left->parent = x;
            y->parent = x->parent;

            if (x == root) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;

            y->left = x;
            x->parent = y;
        }
        static void rotate_right(node_ptr x, node_ptr& root) {
            node_type* y = x->left;
            x->left = y->right;

            if (y->right) y->right->parent = x;
            y->parent = x->parent;

            if (x == root) root = y;
            else if (x == x->parent->right) x->parent->right = y;
            else x->parent->left = y;

            y->right = x;
            x->parent = y;

        }
        static void rebalance(node_ptr x, node_ptr& root) {
            x->color = RED;
            while (x != root && x->parent->color == RED) {
                if (x->parent == x->parent->parent->left) {
                    node_type* y = x->parent->parent->right;
                    if (y && y->color == RED) {
                        x->parent->color = BLACK;
                        y->color = BLACK;
                        x->parent->parent->color = RED;
                        x = x->parent->parent;
                    }
                    else {
                        if (x == x->parent->right) {
                            x = x->parent;
                            rotate_left(x, root);
                        }
                        x->parent->color = BLACK;
                        x->parent->parent->color = RED;
                        rotate_right(x->parent->parent, root);
                    }
                }
                else {
                    node_type* y = x->parent->parent->left;
                    if (y && y->color == RED) {
                        x->parent->color = BLACK;
                        y->color = BLACK;
                        x->parent->parent->color = RED;
                        x = x->parent->parent;
                    }
                    else {
                        if (x == x->parent->left) {
                            x = x->parent;
                            rotate_right(x, root);
                        }
                        x->parent->color = BLACK;
                        x->parent->parent->color = RED;
                        rotate_left(x->parent->parent, root);
                    }
                }
            }
            root->color = BLACK;
        }
        static node_ptr rebalance_for_erase(node_ptr z, node_ptr& root, node_ptr& leftmost, node_ptr& rightmost)
        {
            node_ptr y = z, x = nullptr, x_parent = nullptr;
            
            if (!y->left)x = y->right;
            else {
                if (!y->right)x = y->left;
                else {
                    y = y->right;
                    while (y->left)y = y->left;
                    x = y->right;
                }
            }
            
            if (y != z) {
                z->left->parent = y;
                y->left = z->left;
                if (y != z->right) {
                    x_parent = y->parent;
                    if (x)x->parent = y->parent;

                    y->parent->left = x;
                    y->right = z->right;
                    z->right->parent = y;
                }
                else x_parent = y;

                if (root == z)root = y;
                else if (z->parent->left == z)z->parent->left = y;
                else z->parent->right = y;

                y->parent = z->parent;
                std::swap(y->color, z->color);
                y = z;//y����ָ��Ҫ��ɾ���Ľڵ�
            }
            else {
                x_parent = y->parent;
                if (x)x->parent = y->parent;
                if (root == z)root = x;
                else {
                    if (z->parent->left == z) z->parent->left = x;
                    else z->parent->right = x;
                }
                if (leftmost == z)
                {
                    if (!z->right)leftmost = z->parent;
                    else leftmost = minimum(x);
                }
                if (rightmost == z)
                {
                    if (!z->left)rightmost = z->parent;
                    else rightmost = maximum(x);
                }
            }
            if (y->color != RED) {
                while (x != root && (!x || x->color == BLACK)) {
                    if (x == x_parent->left) {
                        node_ptr w = x_parent->right;
                        if (w->color = RED) {
                            w->color = BLACK;
                            x_parent->color = RED;
                            rotate_left(x_parent, root);
                            w = x_parent->right;
                        }
                        if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                            w->color = RED; x = x_parent; x_parent = x_parent->parent;
                        }
                        else {
                            if (!w->right || w->right->color == BLACK) {
                                if (w->left)w->left->color = BLACK;
                                w->color = RED;
                                rotate_right(w, root);
                                w = x_parent->right;
                            }
                            w->color = x_parent->color;
                            x_parent->color = BLACK;
                            if (w->right)w->right->color = BLACK;
                            rotate_left(x_parent, root);
                            break;
                        }
                    }
                    else {
                        node_ptr w = x_parent->left;
                        if (w->color == RED) {
                            w->color = BLACK;
                            x_parent->color = RED;
                            rotate_right(x_parent, root);
                            w = x_parent->left;
                        }
                        if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                            w->color = RED; x = x_parent; x_parent = x_parent->parent;
                        }
                        else {
                            if (!w->left || w->left->color == BLACK) {
                                if (w->right)w->right->color = BLACK;
                                w->color = RED;
                                rotate_left(w, root);
                                w = x_parent->left;
                            }
                            w->color = x_parent->color;
                            x_parent->color = BLACK;
                            if (w->left)w->left->color = BLACK;
                            rotate_right(x_parent, root);
                            break;
                        }
                    }
                }
                if (x)x->color = BLACK;
            }
            return y;
        }
    };
    
    template<typename _Value>
    struct rbtree_node: public rbtree_node_base<rbtree_node<_Value>> {
        using node_type = rbtree_node<_Value>; using node_ptr = node_type*;
        _Value value;
        rbtree_node(const _Value& val = _Value(),Color_type col = RED,node_ptr p = nullptr,node_ptr l = nullptr,node_ptr r = nullptr) :
            rbtree_node_base<node_type>(col, p, l, r), value(val) {}
    };
    // using _Value=int;
    template<typename _Value>
    struct rbtree_iterator_base {
        using node_type = rbtree_node<_Value>;
        using node_ptr = node_type*;
        node_ptr node;

        void Increment() {
            if (node->right) {
                node = node->right;
                while (node->left)
                    node = node->left;
            }
            else {
                node_ptr y = node->parent;
                while (node == y->right) {
                    node = y; y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }
        void Decrement() {
            if (node->color == RED && node->parent->parent == node)
                node = node->right;
            else if (node->left) {
                node_ptr y = node->left;
                while (y->right)
                    y = y->right;
                node = y;
            }
            else {
                node_ptr y = node->parent;
                while (node == y->left) {
                    node = y; y = y->parent;
                }
                node = y;
            }
        }
    };
    template<class _Value, class _Ref, class _Ptr>
    struct rbtree_iterator : public rbtree_iterator_base<_Value> {
        using value_type = _Value; using reference = _Ref; using pointer = _Ptr;
        using iterator = rbtree_iterator<_Value, _Value&, _Value*>;
        using const_iterator = rbtree_iterator<_Value, const _Value&, const _Value*>;
        using self = rbtree_iterator<_Value, _Ref, _Ptr>;
        using link_type = rbtree_node<_Value>*;

        rbtree_iterator(link_type x = nullptr) :rbtree_iterator_base<_Value>{ x } {}
        rbtree_iterator(const iterator& x) :rbtree_iterator_base<_Value>{ x.node } {}

        reference operator*() const {
            return this->node->value;
        }
        pointer operator->() const {
            return &(this->node->value);
        }

        self& operator++() {
            this->Increment();
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            this->Increment();
            return tmp;
        }
        self& operator--() {
            this->Decrement();
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            this->Decrement();
            return tmp;
        }

        bool operator==(const self& x) const {
            return this->node == x.node;
        }
        bool operator!=(const self& x) const {
            return this->node != x.node;
        }
    };
    template<class _Key, class _Value, class _KeyofValue, class _Compare>
    class rbtree {
    protected:
        using node_type = rbtree_node<_Value>; using node_ptr = node_type*;
        using iterator = rbtree_iterator<_Value, _Value&, _Value*>;
        using const_iterator = rbtree_iterator<_Value, const _Value&, const _Value*>;
        struct _result { bool success; iterator position; };
        struct _const_result { bool success; const_iterator position; };
        struct _range_iterator { iterator begin, end; };
        struct _const_range_iterator{const_iterator begin, end; };
        struct _insert_unique_tag {}; struct _insert_equal_tag {};
    protected:
        node_ptr header; size_t size; _KeyofValue v_to_key; _Compare compare;
    protected:
        void _empty_initialize() {
            _color(header) = RED; _root() = nullptr; _leftmost() = header; _rightmost() = header;
        }

        node_ptr _copy(node_ptr x, node_ptr p) {
            node_ptr top = clone_node(x); top->parent = p;
            if (x->right) top->right = _copy(x->right, top);
            p = top; x = x->left;
            while (x) {
                node_ptr y = clone_node(x); p->left = y; y->parent = p;
                if (x->right) y->right = _copy(x->right, y);
                p = y; x = x->left;
            }
            return top;
        }
        void _erase(node_ptr z) {
            while (z) {
                _erase(_right(z));
                node_ptr y = _left(z);
                destroy_node(z);
                z = y;
            }
        }
        iterator _insert(node_ptr x, node_ptr y, const _Value& v) {
            node_ptr z = nullptr;
            if (y == header || x || compare(v_to_key(v), _key(y))) {
                z = create_node(v);
                _left(y) = z;
                if (y == header) {
                    _root() = z;
                    _rightmost() = z;
                }
                else if (y == _leftmost())
                    _leftmost() = z;
            }
            else {
                z = create_node(v);
                _right(y) = z;
                if (y == _rightmost())
                    _rightmost() = z;
            }
            _parent(z) = y; // �����½ڵ�ĸ��ڵ�
            _left(z) = nullptr;     // �����½ڵ�����ӽڵ�
            _right(z) = nullptr;    // �����½ڵ�����ӽڵ�
            node_type::rebalance(z, header->parent); // �����ڵ���root
            ++size;

            return iterator(z); // ����ָ�������ڵ�ĵ�����
        }
        node_ptr find_ptr(const _Key& k)const {
            node_ptr y = header, x = _root();
            //  compare��������
            while (x != nullptr) {
                if (!compare(_key(x), k)) { y = x; x = _left(x); }
                else  x = _right(x);
            }
            return y;
        }
        node_ptr lower_bound_ptr(const _Key& k)const {
            node_ptr y = header, x = _root();
            //  compare��������
            while (x != nullptr) {
                if (!compare(_key(x), k)) { y = x; x = _left(x); }
                else  x = _right(x);
            }
            return y;
        }
        node_ptr upper_bound_ptr(const _Key& k)const {
            node_ptr  y = header, x = _root();
            while (x) {
                if (compare(k, _key(x))) { y = x; x = _left(x); }
                else x = _right(x);
            }
            return y;
        }
    protected:

        static node_ptr create_node(const _Value& val = _Value()) { return new node_type(val); }
        static node_ptr clone_node(node_ptr x) { return new node_type(x->value, x->color); }
        static void destroy_node(node_ptr& x) { delete x; x = nullptr; }

        node_ptr& _root() { return header->parent; }
        node_ptr& _leftmost() { return header->left; }
        node_ptr& _rightmost() { return header->right; }
        const node_ptr& _root() const { return header->parent; }
        const node_ptr& _leftmost() const { return header->left; }
        const node_ptr& _rightmost() const { return header->right; }
        static node_ptr& _left(node_ptr x) { return x->left; }
        static node_ptr& _right(node_ptr x) { return x->right; }
        static node_ptr& _parent(node_ptr x) { return x->parent; }
        static _Value& _value(node_ptr x) { return x->value; }
        static const _Key& _key(node_ptr x) { return _KeyofValue()(_value(x)); }
        static Color_type& _color(node_ptr& x) { return x->color; }

        static node_ptr minimum(node_ptr x) { return node_type::minimum(x); }
        static node_ptr maximum(node_ptr x) { return node_type::maximum(x); }

    public:
        rbtree() :header(nullptr), size(0), v_to_key(), compare() {
            header = create_node(_Value()); _empty_initialize();
        }
        template<typename RandomIt>
        rbtree(RandomIt first, RandomIt last, _insert_unique_tag) : rbtree() {
            insert_unique(first, last);
        }
        template<typename RandomIt>
        rbtree(RandomIt first, RandomIt last, _insert_equal_tag) : rbtree() {
            insert_equal(first, last);
        }
        rbtree(const rbtree<_Key, _Value, _KeyofValue, _Compare>& x) :header(create_node()), size(x.size), v_to_key(x.v_to_key), compare(x.compare)
        {
            if (x._root() == nullptr)
                _empty_initialize();
            else {
                _color(header) = RED;
                _root() = _copy(x._root(), header);
                _leftmost() = minimum(_root());
                _rightmost() = maximum(_root());
            }
        }
        rbtree& operator=(const rbtree<_Key, _Value, _KeyofValue, _Compare>& x) {
            if (this != &x) {
                clear();
                v_to_key = x.v_to_key;
                compare = x.compare;
                if (x._root() == nullptr)
                    _empty_initialize();
                else {
                    _color(header) = RED;
                    _root() = _copy(x._root(), header);
                    _leftmost() = minimum(_root());
                    _rightmost() = maximum(_root());
                }
                size = x.size;
            }
            return *this;
        }
        rbtree(rbtree<_Key, _Value, _KeyofValue, _Compare>&& x) :header(x.header), size(x.size), v_to_key(x.v_to_key), compare(x.compare) {
            x.header = nullptr;
            x.size = 0;
        }
        rbtree& operator=(rbtree<_Key, _Value, _KeyofValue, _Compare>&& x)noexcept {
            if (this != &x) {// �������Ҹ�ֵ
                clear();
                v_to_key = x.v_to_key;
                compare = x.compare;
                header = x.header;
                size = x.size;
                x.header = nullptr;//�ƶ���x����ʹ��
                x.size = 0;
            }
            return *this;
        }

        virtual ~rbtree() {
            clear();
            destroy_node(header);
        }

        virtual void clear() {
            if (size != 0) {
                _erase(_root()); _leftmost() = header; _rightmost() = header;
                size = 0;
            }
        }
        void swap(rbtree<_Key, _Value, _KeyofValue, _Compare>& x) {
            std::swap(header, x.header); std::swap(size, x.size); std::swap(v_to_key, x.v_to_key); std::swap(compare, x.compare);
        }
    protected:
        _Compare _comp() const { return compare; }
        virtual iterator begin() { return iterator(_leftmost()); }
        virtual const_iterator begin() const { return const_iterator(_leftmost()); }
        virtual iterator end() { return iterator(header); }
        virtual const_iterator end() const { return const_iterator(header); }
        virtual size_t get_size() const { return size; }
        virtual bool empty() const { return size == 0; }
    protected:
        virtual iterator insert_equal(const _Value& v) {
            node_ptr y = header, x = _root();
            while (x) {
                y = x; x = compare(v_to_key(v), _key(x)) ? _left(x) : _right(x);
            }
            return _insert(x, y, v);// xΪ��ֵ����㣬 yΪ���ڵ㣬 vΪ��ֵ
        }

        virtual _result insert_unique(const _Value& v) {
            node_ptr y = header;
            node_ptr x = _root(); // �Ӹ��ڵ㿪ʼѰ�Ҳ����
            bool comp = true;
            while (x != 0) {
                y = x;// v�Ƿ�С�ڵ�ǰ�ڵ��ֵ
                comp = compare(_KeyofValue()(v), _key(x));// �������󣬷�������
                x = comp ? _left(x) : _right(x);
            }
            // ����whileѭ����yΪ�����ĸ��ڵ�
            iterator j = iterator(y);   // ������jָ��y�ĵ�����
            if (comp) // ��ʾy�ļ�ֵ���ڲ����ֵ������������
            {
                if (j == begin())     // ��������ĸ��ڵ�Ϊ����ڵ�
                    return _result{ true,_insert(x,y,v) };
                else // �������j
                    --j;
            }
            // y�ļ�ֵС�ڲ����ֵ����������Ҳ�
            if (compare(_key(j.node), _KeyofValue()(v)))
                return _result{ true,_insert(x,y,v) };
            // ��ֵ�����м�ֵ�ظ������ز���ʧ��
            return _result{ false,j };
        }

        //Ѱ��Ԫ��
        virtual iterator find(const _Key& k) {
            iterator j = iterator(find_ptr(k));
            return (j == end() || compare(k, _key(j.node))) ? end() : j;
        }
        virtual const_iterator find(const _Key& k)const {
            const_iterator j = const_iterator(find_ptr(k));
            return (j == end() || compare(k, _key(j.node))) ? end() : j;
        }
        virtual iterator lower_bound(const _Key& k) {
            return iterator(lower_bound_ptr(k));
        }
        virtual iterator upper_bound(const _Key& k) {
            return iterator(upper_bound_ptr(k));
        }
        virtual const_iterator lower_bound(const _Key& k)const {
            return const_iterator(lower_bound_ptr(k));
        }
        virtual const_iterator upper_bound(const _Key& k) const {
            return iterator(upper_bound_ptr(k));
        }
        virtual _range_iterator equal_range(const _Key& k) {
            return _range_iterator{ lower_bound(k), upper_bound(k) };
        }

        //ȥ��Ԫ��
        virtual size_t erase(iterator pos) {
            node_ptr y = node_type::rebalance_for_erase(pos.node, header->parent, header->left, header->right);
            destroy_node(y); --size; return 1ull;
        }
        virtual size_t erase(iterator first, iterator last) {
            size_t N = 0;
            if (first == begin() && last == end()) { N = size; clear(); }
            else { while (first != last)   N += erase(first++); }
            return N;
        }
        virtual size_t erase(const _Key& k) {
            _range_iterator range = equal_range(k);
            return erase(range.begin, range.end);
        }
        virtual size_t count(const _Key& k) const {
            const_iterator first = lower_bound(k);
            const_iterator last = upper_bound(k);
            size_t N = 0;
            while (first != last) {
                ++N; ++first;
            }
            return N;
        }
        template<class Condition>
        size_t erase_if(Condition c) {//�����Ƕ�_Key��_Value��ͬ���õ�
            size_t num_erased = 0; // ��¼��ɾ���ڵ������
            iterator it = begin(); // ��ȡ���������ʼ������
            while (it != end()) {
                if (c(v_to_key(it.node->value), it.node->value)) { // �����ǰ�ڵ��ֵ��������
                    // ����erase���ܵ����ڴ��ͷţ����һ�ı��������״̬�����������Ҫ������һ���ڵ�
                    iterator to_erase = it++;
                    num_erased += erase(to_erase); // ɾ�����������Ľڵ�
                }
                else ++it; // �������Ǽ��������һ���ڵ�
            }
            return num_erased; // ���ر�ɾ���ڵ��������
        }
    };
    template<class K,class V>
    struct _get_key_from_pair {
        const K &operator()(const mypair<const K,V>& p) const { return p.first; }
        const K &operator()(mypair<const K,V>& p) const { return p.first; }
    };
    template<class Identity>
    struct __identity {
        const Identity& operator()(const Identity& x) const { return x; }
    };
    template<class K ,class V ,class Compare=myless<K>>
    class mymap:public rbtree<K,mypair<const K,V>,_get_key_from_pair<K,V>,Compare>{
        using Base = rbtree<K,mypair<const K,V>,_get_key_from_pair<K,V>,Compare>;
        using iterator = typename Base::iterator;
        using const_iterator = typename Base::const_iterator;
        using _result = typename Base::_result;
        using _range_iterator=typename Base::_range_iterator;
        using pair_type=mypair<const K,V>;
    public:
        mymap():Base(){}
        mymap(const mymap<K,V,Compare>& x):Base(x){}
        mymap(mymap<K,V,Compare>&& x) noexcept :Base(std::move(x)){}
        mymap<K,V,Compare>& operator=(const mymap<K,V,Compare>& x){
            Base::operator=(x);
            return *this;
        }
        mymap<K,V,Compare>& operator=(mymap<K,V,Compare>&& x) noexcept {
            Base::operator=(std::move(x));
            return *this;
        }
        mymap(const std::initializer_list<mypair<K, V>>& il):Base() {
            for (const auto& p : il) { Base::insert_unique(pair_type{p.first,p.second}); }
        }

        template<typename InputIt>
        mymap(InputIt first, InputIt last):
            Base(first,last,typename Base::_insert_unique_tag()) {}
        V& operator[](const K& k){
            iterator it = Base::find(k);
            if(it == Base::end()) it = Base::insert_equal(pair_type{k,V()});
            return it->second;
        }
        const V& operator[](const K& k)const{
            const_iterator it = Base::find(k);
            if(it == Base::end())throw std::out_of_range("key not found");
            return it->second;
        }

        V& at(const K& k){
            iterator it = Base::find(k);
            if(it == Base::end())throw std::out_of_range("key not found");
            return it->second;
        }
        const V& at(const K& k)const{
            const_iterator it = Base::find(k);
            if(it == Base::end())throw std::out_of_range("key not found");
            return it->second;
        }

        size_t size(){return Base::get_size();}
        bool empty(){return Base::empty();}
        iterator begin(){return Base::begin();}
        const_iterator begin()const{return Base::begin();}
        iterator end(){return Base::end();}
        const_iterator end()const{return Base::end();}
        void swap(mymap<K,V,Compare>& x){Base::swap(x);}
        void clear(){Base::clear();}

        _result insert(const mypair<K,V>& p){return Base::insert_unique(pair_type{p.first,p.second});}
        _result insert(const K& k, const V& v){return insert(mypair<K,V>{k,v});}

        iterator find(const K& k){return Base::find(k);}
        const_iterator find(const K& k)const{return Base::find(k);}
        iterator upper_bound(const K& k){return Base::upper_bound(k);}
        const_iterator upper_bound(const K& k)const{return Base::upper_bound(k);}
        size_t count(const K& k)const{return Base::count(k);}
        bool contains(const K& k){return Base::find(k) != Base::end();}

        size_t erase(const K& k){return Base::erase(k);}
        size_t erase(iterator it){return Base::erase(it);}
        size_t erase(iterator first, iterator last){return Base::erase(first,last);}

        template<class Condition,
            class = std::enable_if_t<std::is_invocable_r_v<bool, Condition,const K&,const V&>>>//ǿ�쳣��ȫ��֤
        size_t erase_if(Condition c){
            size_t num_erased = 0; iterator it = begin(); 
            while (it != end()) {
                if (c(it->first, it->second)) {
                    iterator to_erase = it++;
                    num_erased += erase(to_erase); 
                }
                else ++it; 
            }
            return num_erased; 
        }
    };
    
    template<class K, class V, class Compare = myless<K>>
    class mymultimap :public rbtree<K, mypair<const K, V>, _get_key_from_pair<K, V>, Compare> {
        using Base = rbtree<K, mypair<const K, V>, _get_key_from_pair<K, V>, Compare>;
        using iterator = typename Base::iterator;
        using const_iterator = typename Base::const_iterator;
        using _result = typename Base::_result;
        using _range_iterator = typename Base::_range_iterator;
        using pair_type = mypair<const K, V>;
    public:
        mymultimap() :Base() {}
        mymultimap(const mymultimap<K, V, Compare>& x) :Base(x) {}
        mymultimap(mymultimap<K, V, Compare>&& x) noexcept :Base(std::move(x)) {}
        mymultimap<K, V, Compare>& operator=(const mymultimap<K, V, Compare>& x) {
            Base::operator=(x);
            return *this;
        }
        mymultimap<K, V, Compare>& operator=(mymultimap<K, V, Compare>&& x) noexcept {
            Base::operator=(std::move(x));
            return *this;
        }
        mymultimap(const std::initializer_list<mypair<K, V>>& il) :Base() {
            for (const auto& p : il) { Base::insert_equal(pair_type{ p.first,p.second }); }
        }

        template<typename InputIt>
        mymultimap(InputIt first, InputIt last) :
            Base(first, last, typename Base::_insert_equal_tag()) {}

     
        V& at(const K& k) {
            iterator it = Base::find(k);
            if (it == Base::end())throw std::out_of_range("key not found");
            return it->second;
        }
        const V& at(const K& k)const {
            const_iterator it = Base::find(k);
            if (it == Base::end())throw std::out_of_range("key not found");
            return it->second;
        }

        size_t size() { return Base::get_size(); }
        bool empty() { return Base::empty(); }
        iterator begin() { return Base::begin(); }
        const_iterator begin()const { return Base::begin(); }
        iterator end() { return Base::end(); }
        const_iterator end()const { return Base::end(); }
        void swap(mymultimap<K, V, Compare>& x) { Base::swap(x); }
        void clear() { Base::clear(); }

        iterator insert(const mypair<K, V>& p) { return Base::insert_equal(pair_type{ p.first,p.second }); }
        iterator insert(const K& k, const V& v) { return insert(mypair<K, V>{k, v}); }

        iterator find(const K& k) { return Base::find(k); }
        const_iterator find(const K& k)const { return Base::find(k); }
        iterator lower_bound(const K& k) { return Base::lower_bound(k); }
        const_iterator lower_bound(const K& k)const { return Base::lower_bound(k); }
        iterator upper_bound(const K& k) { return Base::upper_bound(k); }
        const_iterator upper_bound(const K& k)const { return Base::upper_bound(k); }
        _range_iterator equal_range(const K& k) { return Base::equal_range(k); }
        size_t count(const K& k)const { return Base::count(k); }
        bool  contains(const K& k) { return Base::find(k) != Base::end(); }

        size_t erase(const K& k) { return Base::erase(k); }
        size_t erase(iterator it) { return Base::erase(it); }
        size_t erase(iterator first, iterator last) { return Base::erase(first, last); }

        template<class Condition,
            class = std::enable_if_t<std::is_invocable_r_v<bool, Condition, const K&, const V&>>>//ǿ�쳣��ȫ��֤
        size_t erase_if(Condition c) {
            size_t num_erased = 0; iterator it = begin();
            while (it != end()) {
                if (c(it->first, it->second)) {
                    iterator to_erase = it++;
                    num_erased += erase(to_erase);
                }
                else ++it;
            }
            return num_erased;
        }
    };

    template<class K,class Compare=myless<K>>
    class myset :public rbtree<const K,const  K, __identity<K>, Compare> {
        using Base = rbtree<const K,const K, __identity<K>, Compare>;
        using iterator = typename Base::const_iterator;
        using const_iterator = typename Base::const_iterator;
        using _result = typename Base::_result;
        using _const_result = typename Base::_const_result;
    public:
        myset():Base(){}
        myset(const myset<K,Compare>& x):Base(x){}
        myset(myset<K,Compare>&& x) noexcept :Base(std::move(x)){}
        myset<K,Compare>& operator=(const myset<K,Compare>& x){
            Base::operator=(x);           return *this;
        }
        myset<K,Compare>& operator=(myset<K,Compare>&& x) noexcept {
            Base::operator=(std::move(x));return *this;
        }
        myset(const std::initializer_list<K>& il):Base() {
            for (const auto& p : il) { Base::insert_unique(p); }
        }
        template<typename InputIt>
        myset(InputIt first, InputIt last):
            Base(first,last,typename Base::_insert_unique_tag()) {}
        const K& at(const K& k)const{
            const_iterator it = Base::find(k);
            if(it == Base::end())throw std::out_of_range("key not found");
            return *it;
        }
        const K& operator[](const K& k)const {
            const_iterator it = Base::find(k);
            if (it == Base::end())throw std::out_of_range("key not found");
            return *it;
        }

        size_t size(){return Base::get_size();}
        bool empty(){return Base::empty();}
        const_iterator begin(){return Base::begin();}
        const_iterator end(){return Base::end();}
        void swap(myset<K,Compare>& x){Base::swap(x);}
        void clear(){Base::clear();}

        _const_result insert(const K& k){
            _result res = Base::insert_unique(k);
            return _const_result{ res.success, const_iterator{res.position.node} };
        }

        const_iterator find(const K& k)const{return Base::find(k);}
        const_iterator upper_bound(const K& k)const{return Base::upper_bound(k);}
        size_t count(const K& k)const{return Base::count(k);}
        bool contains(const K& k){return Base::find(k) != Base::end();}

        size_t erase(const K& k){return Base::erase(k);}
        size_t erase(iterator it){return Base::erase(it);}
        size_t erase(iterator first, iterator last){return Base::erase(first,last);}

        template<class Condition,
            class = std::enable_if_t<std::is_invocable_r_v<bool, Condition,const K&>>>//ǿ�쳣��ȫ��֤
        size_t erase_if(Condition c){
            size_t num_erased = 0; const_iterator it = begin(); 
            while (it != end()) {
                if (c(*it)) {
                    const_iterator to_erase = it++;
                    num_erased += erase(to_erase); 
                }
                else ++it; 
            }
            return num_erased; 
        }
    };

    template<class K, class Compare = myless<K>>
    class mymultiset :public rbtree<const K, const  K, __identity<K>, Compare> {
        using Base = rbtree<const K, const K, __identity<K>, Compare>;
        using iterator = typename Base::const_iterator;
        using const_iterator = typename Base::const_iterator;
        using _result = typename Base::_result;
        using _const_result = typename Base::_const_result;
        using _const_range_iterator=typename Base::_const_range_iterator;
        mymultiset() :Base() {}
        mymultiset(const mymultiset<K, Compare>& x) :Base(x) {}
        mymultiset(mymultiset<K, Compare>&& x) noexcept :Base(std::move(x)) {}
        mymultiset<K, Compare>& operator=(const mymultiset<K, Compare>& x) {
            Base::operator=(x);
            return *this;
        }
        mymultiset<K, Compare>& operator=(mymultiset<K, Compare>&& x) noexcept {
            Base::operator=(std::move(x));
            return *this;
        }
        mymultiset(const std::initializer_list<K>& il) :Base() {
            for (const auto& p : il) { Base::insert_unique(p); }
        }
        template<typename InputIt>
        mymultiset(InputIt first, InputIt last) :
            Base(first, last, typename Base::_insert_unique_tag()) {}
        const K& at(const K& k)const {
            const_iterator it = Base::find(k);
            if (it == Base::end())throw std::out_of_range("key not found");
            return *it;
        }
        const K& operator[](const K& k)const {
            const_iterator it = Base::find(k);
            if (it == Base::end())throw std::out_of_range("key not found");
            return *it;
        }

        size_t size() { return Base::get_size(); }
        bool empty() { return Base::empty(); }
        const_iterator begin() { return Base::begin(); }
        const_iterator end() { return Base::end(); }
        void swap(mymultiset<K, Compare>& x) { Base::swap(x); }
        void clear() { Base::clear(); }

        const_iterator insert(const K& k) {
            auto res = Base::insert_equal(k);
            return  const_iterator{ res.node };
        }

        const_iterator find(const K& k)const { return Base::find(k); }
        const_iterator upper_bound(const K& k)const { return Base::upper_bound(k); }
        const_iterator lower_bound(const K& k)const { return Base::lower_bound(k); }
        _const_range_iterator equal_range(const K& k)const { return _const_range_iterator{Base::lower_bound(k),Base::upper_bound(k)}; }
        size_t count(const K& k)const { return Base::count(k); }
        bool contains(const K& k) { return Base::find(k) != Base::end(); }

        size_t erase(const K& k) { return Base::erase(k); }
        size_t erase(iterator it) { return Base::erase(it); }
        size_t erase(iterator first, iterator last) { return Base::erase(first, last); }

        template<class Condition,
            class = std::enable_if_t<std::is_invocable_r_v<bool, Condition, const K&>>>//ǿ�쳣��ȫ��֤
        size_t erase_if(Condition c) {
            size_t num_erased = 0; const_iterator it = begin();
            while (it != end()) {
                if (c(*it)) {
                    const_iterator to_erase = it++;
                    num_erased += erase(to_erase);
                }
                else ++it;
            }
            return num_erased;
        }
    };
}


namespace mystl {
    template<class _Value, class _Ref, class _Ptr>
    struct myhashtale_iterator {
        using value_type = _Value;
        using reference = _Ref;
        using pointer = _Ptr;
        using node_type = node<_Value>; using self = myhashtale_iterator<_Value, _Ref, _Ptr>;
        using iterator = myhashtale_iterator<_Value, _Value&, _Value*>;
        using const_iterator = myhashtale_iterator<_Value, const _Value&, const _Value*>;

        node_type** table;
        size_t bucket_index;
        node_type* current;
        size_t m_capacity;

        myhashtale_iterator(node_type** tb, size_t bi, node_type* cur, size_t cap)
            : table(tb), bucket_index(bi), current(cur), m_capacity(cap) {}
        reference operator*() { return current->data; }
        pointer operator->() { return &(current->data); }
        self& operator++() {
            if (current->next) {
                current = current->next;
            }
            else {
                do {
                    ++bucket_index;  // ǰ������һ��Ͱ
                } while (bucket_index < m_capacity && !table[bucket_index]);
                // �������Ͱ����ǰ������Ͱ�ĵ�һ���ڵ㣻����current��Ϊ��ָ�룬��ʾ����ĩβ
                current = (bucket_index < m_capacity) ? table[bucket_index] : nullptr;
            }
            return *this;
        }
        self operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const self& other) const {
            return current == other.current && bucket_index == other.bucket_index;
        }
        bool operator!=(const self& other) const {
            return !(*this == other);
        }
    };
    template<class _Key, class _Value, class _KeyOfValue, class _Hash, class _Equal>
    class myhash_table {
    protected:
        using node_type = node<_Value>; using iterator = myhashtale_iterator<_Value, _Value&, _Value*>;
        using const_iterator = myhashtale_iterator<_Value, const _Value&, const _Value*>;
        using _hash_table = myhash_table<_Key, _Value, _KeyOfValue, _Hash, _Equal>;
        struct _insert_equal_tag {}; struct _insert_unique_tag {};
        node_type** buckets; size_t size, capacity, prime_index;
        _Hash hasher; _Equal equal;_KeyOfValue key_of_value;//hasher:�����ϣֵ��equal:�ж�����ֵ�Ƿ���ȣ�key_of_value:��ȡֵ��Ӧ�ļ�
        template<class _It_Type> struct product {
            bool success;_It_Type it; operator bool() const { return success; }};
        template<class _It_Type> struct _range{ _It_Type begin, end; 
        };
        void  __Tidy() {
            for (size_t i = 0; i < capacity; i++) {
                node_type* temp = buckets[i];
                while (temp) {
                    node_type* temp2 = temp;
                    temp = temp->next;
                    delete temp2;
                }
            }
            delete[]buckets; buckets = nullptr;
        }
        void __Reset() {
            size = 0; prime_index = 0; capacity = prime_num[prime_index];
            if (buckets) buckets = nullptr;
            buckets = new node_type * [capacity] {};
        }
        size_t gethash(const _Key& val)const {//��hasher������capacityȡ��
            return hasher(val) % capacity;
        }
        void rehash() {
            size_t new_prime_index = (prime_index + 1);
            size_t new_capacity = prime_num[new_prime_index];
            node_type** new_bucket = new node_type * [new_capacity] {};
            for (size_t i = 0; i < capacity; i++) {
                node_type* temp = buckets[i];
                while (temp) {
                    size_t index = hasher(temp->data) % new_capacity;//��hashֵ��
                    node_type* next = temp->next;
                    temp->next = new_bucket[index];
                    new_bucket[index] = temp;
                    temp = next;
                }
                buckets[i] = nullptr;
            }
            delete[]buckets;
            buckets = new_bucket;
            prime_index = new_prime_index;
            capacity = new_capacity;
        }
        mypair<size_t,node_type*>_find_node(const _Key& key)const {//�൱��lower_bound
            size_t pos = gethash(key); node_type* curr = buckets[pos];
            while (curr) {
                if (equal(key_of_value(curr->data), key))
                    return mypair<size_t,node_type*>{pos,curr};
                curr = curr->next;
            }
            return mypair<size_t,node_type*>{this->capacity,nullptr};
        }
        mypair<size_t ,node_type*> _upper_bound_node(const _Key& key)const {
            size_t pos = gethash(key); bool found = false;
            node_type* curr = buckets[pos]; node_type* result=nullptr;
            while (curr) {
                if (equal(key_of_value(curr->data), key))
                {
                    result = curr; found = true;
                }
                curr = curr->next;
            }
            if(!found) return mypair<size_t,node_type*>{this->capacity,nullptr};//��û�ҵ����򷵻�ָ��ĩβ��ָ��
            else {//���ҵ�...
                iterator it(buckets, pos, result, capacity); ++it;
                return mypair<size_t,node_type*>{it.bucket_index,it.current};
            }
        }
    public:
        myhash_table() :size(0), prime_index(0), capacity(prime_num[prime_index]), hasher(), equal(), key_of_value() {
            buckets = new node_type * [capacity] {};
        }
        template<typename InputIt>
        myhash_table(InputIt first, InputIt last, _insert_equal_tag) : myhash_table() {
            for (InputIt it = first; it != last; ++it)
                insert_equal(*it);
        }
        template<typename InputIt>
        myhash_table(InputIt first, InputIt last, _insert_unique_tag) : myhash_table() {
            for (InputIt it = first; it != last; ++it)
                insert_unique(*it);
        }
        myhash_table(const _hash_table& other) : myhash_table() {
            for (const_iterator it = other.begin(); it != other.end(); ++it) {
                insert_equal(*it);
            }
        }
        myhash_table(_hash_table&& other) :
            buckets(other.buckets), size(other.size), capacity(other.capacity),
            prime_index(other.prime_index), hasher(), equal(), key_of_value() {
            other.__Reset();
        }
        _hash_table& operator=(const _hash_table& other) {
            if (this != &other) {
                __Tidy(); __Reset();
                for (const_iterator it = other.begin(); it != other.end(); ++it) {
                    insert_equal(*it);
                }
            }
            return *this;
        }
        _hash_table& operator=(_hash_table&& other) {
            if (this != &other) {
                __Tidy();
                buckets = other.buckets;
                size = other.size;
                capacity = other.capacity;
                prime_index = other.prime_index;
                other.__Reset();
            }
            return *this;
        }
        size_t get_size() const {
            return size;
        }
        size_t get_capacity() const {
            return capacity;
        }
        double get_load_factor() const {
            return (double)size / capacity;
        }
        virtual ~myhash_table() {
            __Tidy();
        }
        virtual  bool empty() const {
            return size == 0;
        }

        virtual iterator begin() {
            for (size_t i = 0; i < capacity; ++i) {
                if (buckets[i]) {
                    return iterator(buckets, i, buckets[i], capacity);
                }
            }
            return iterator(buckets, capacity, nullptr, capacity);  // �����ϣ��Ϊ�գ�����ָ������ĵ�����
        }
        virtual const_iterator begin()const {
            for (size_t i = 0; i < capacity; ++i) {
                if (buckets[i]) {
                    return const_iterator(buckets, i, buckets[i], capacity);
                }
            }
            return const_iterator(buckets, capacity, nullptr, capacity);  // �����ϣ��Ϊ�գ�����ָ������ĵ�����
        }
        virtual iterator end() {
            return iterator(buckets, capacity, nullptr, capacity);  // ����ָ������ĵ�����
        }
        virtual const_iterator end()const {
            return const_iterator(buckets, capacity, nullptr, capacity);  // ����ָ������ĵ�����
        }

        void clear() {
            __Tidy(); __Reset();
        }
        iterator insert_equal(const _Value& val) {
            if (size >= capacity && capacity < prime_num[prime_num_size - 1]) {
                rehash();
            }
            size_t pos = gethash(key_of_value(val)); size++;
            node_type _temp_head(_Value(), buckets[pos]);//����һ����ʱ�ڵ�
            node_type* curr = buckets[pos];/*��¼ԭ�ȵ�ͷ�ڵ�*/
            node_type* prev = &_temp_head;//��¼ǰһ�������ݷǿսڵ�
            while (curr) {
                if (equal(key_of_value(curr->data), key_of_value(val))) {//�������ظ��ļ����֣�
                    node_type::_insert_after(prev, val);/*���뵽ԭ�ȵĽڵ��*/
                    buckets[pos] = _temp_head.next;/*ͬ������ԭ�ȵ�ͷ�ڵ�*/
                    return iterator(buckets, pos, prev->next, capacity);
                }
                prev = curr;
                curr = curr->next;
            }
            node_type::_insert_after(&_temp_head, val);buckets[pos] = _temp_head.next;
            
            return iterator(buckets, pos, _temp_head.next, capacity);
        }
        product<iterator> insert_unique(const _Value& val) {
            if (size >= capacity && capacity < prime_num[prime_num_size - 1]) { rehash(); }
            size_t pos = gethash(key_of_value(val));
            node_type _temp_head(_Value(), buckets[pos]);//����һ����ʱ�ڵ�
            node_type* curr = buckets[pos];//��¼ԭ�ȵ�ͷ�ڵ�
            node_type* prev = &_temp_head;//��¼ǰһ�������ݷǿսڵ�
            while (curr) {
                if (equal(key_of_value(curr->data), key_of_value(val))) {//�������ظ��ļ����֣�
                    return product<iterator>{false, { buckets, pos, prev->next, capacity }};
                }
                prev = curr;
                curr = curr->next;
            }
            node_type::_insert_after(&_temp_head, val); buckets[pos] = _temp_head.next;
            size++;
            return product<iterator>{true, { buckets, pos, _temp_head.next, capacity }};
        }
        
        virtual const_iterator find(const _Key& key) const {
            mypair<size_t,node_type*> res = _find_node(key);
            return const_iterator(buckets, res.first, res.second, capacity);
        }
        virtual iterator find(const _Key& key) {
            mypair<size_t, node_type*> res = _find_node(key);
            return iterator(buckets, res.first, res.second, capacity);
        }
        virtual const_iterator lower_bound(const _Key& key) const {
            return find(key);
        }
        virtual iterator lower_bound(const _Key& key) {
            return find(key);
        }
        virtual const_iterator upper_bound(const _Key& key) const {
            mypair<size_t, node_type*> res = _upper_bound_node(key);
            return const_iterator(buckets, res.first, res.second, capacity);
        }
        virtual iterator upper_bound(const _Key& key) {
            mypair<size_t, node_type*> res = _upper_bound_node(key);
            return iterator(buckets, res.first, res.second, capacity);
        }
        virtual _range<iterator> equal_range(const _Key& key)
        {return _range<iterator>      {lower_bound(key), upper_bound(key)};}
        virtual _range<const_iterator> equal_range(const _Key& key)const
        {return _range<const_iterator>{lower_bound(key), upper_bound(key)};}


        virtual size_t erase(const _Key& key) {
            size_t i = gethash(key);
            node_type _temp_head(_Value(), buckets[i]);
            node_type* prev = &_temp_head; node_type* curr = buckets[i];
            size_t found = 0;
            while (curr) {
                if (equal(key_of_value(curr->data), key)) {
                    node_type::_erase_after(prev); buckets[i] = _temp_head.next;/*ͬ������ԭ�ȵ�ͷ�ڵ�*/
                    this->size--; found++; curr = prev->next;
                }
                else { prev = curr; curr = curr->next; }
            }
            return found;
        }
        template<class Condition,
            class = std::enable_if_t<std::is_invocable_r_v<bool, Condition, const _Key&, const _Value&>>>
        size_t erase_if(Condition cond) {
            size_t deleted = 0; node_type* prev, * curr;
            for (size_t i = 0; i < capacity; i++) {
                node_type _temp_head(_Value(), buckets[i]);
                prev = &_temp_head; curr = buckets[i];
                while (curr) {
                    if (cond(key_of_value(curr->data), curr->data)) {
                        node_type::_erase_after(prev); buckets[i] = _temp_head.next;
                        this->size--; deleted++; curr = prev->next;
                    }
                    else { prev = curr; curr =curr->next; }
                }
            }
            return deleted;
        }
        void  _print_for_analysis(std::ostream& os)const {
            for (size_t i = 0; i < capacity; i++) {
                node_type* curr = buckets[i];
                if(curr)
                {
                    os << "bucket[" << i << "]:\t";

                    while (curr) {
                        os << curr->data << "\t";
                        curr = curr->next;
                    }
                    os << '\n';
                }
            }
        }
    };
}
