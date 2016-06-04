#ifndef _LONG_INT_
#define _LONE_INT_

//macros to indicate the in/out property of a parameter
#define _IN_
#define _OUT_

#include <iostream>
#include <assert.h>
typedef struct node
{
	struct node * next, *prev;
	int data;
	node():next(NULL),prev(NULL){}
}linkNode;

class longInt
{
private:
	linkNode *head, *tail;
	int count;
	longInt *result;
private:
	void clear()
	{
		linkNode* current = head;
		while(current)
		{
			linkNode* del = current;
			current = current->next;
			delete del;
		}
	}
public:
	//to indicate if it is a negative value.
	//1 means negative
	bool sign;

public:

	friend void add(_IN_ const longInt& a, 
		_IN_ const longInt& b, 
		_OUT_ longInt& result);
	friend void sub(_IN_ const longInt &a, 
		_IN_ const longInt& b, 
		_OUT_ longInt& result);

	//a >= b is true
	friend bool cmp(_IN_ const longInt& a, _IN_ const longInt& b);

	longInt()
		:head(NULL)
		,tail(NULL)
		,count(0)
		,sign(0)
	{}

	longInt(const longInt& ori)
	{
		count = 0;
		sign = ori.sign;
		linkNode* curr = ori.head;
		while(curr)
		{
			insertBack(curr->data);
			curr = curr->next;
		}
	}

	~longInt()
	{
		clear();
	}

	void insertBack(const int& item )
	{
		linkNode *new_node = new linkNode;
		if (!new_node)
		{
			std::cerr<<"Failed to allocate memory!"<<std::endl;
			return;
		}
		new_node->data = item;
		if (!count) head = tail = new_node;
		else
		{
			tail->next = new_node;
			new_node->prev = tail;
			tail = new_node;
		}
		count++;
	}

	void insertFront(const int& item)
	{
		linkNode *new_node = new linkNode;
		if (!new_node)
		{
			std::cerr<<"Failed to allocate memory!"<<std::endl;
			return;
		}
		new_node->data = item;
		if (!count) head = tail = new_node;
		else
		{
			new_node->next = head;
			head->prev = new_node;
			head = new_node;
		}
		count++;
	}

	friend std::istream& operator>>(std::istream& in, const longInt& dest);

	friend std::ostream& operator<<(std::ostream& out, const longInt& ori);

	longInt& operator+(const longInt& b)
	{
		result = new longInt;
		if (sign == 1 && b.sign == 1)
		{
			add(*this,b,*result);
			result->sign = 1;
		}
		else if(sign == 0 && b.sign == 0)
			add(*this,b,*result);
		else if(sign == 0 && b.sign == 1)
		{
			if (cmp(*this,b))
				sub(*this,b,*result);
			else
			{
				sub(b,*this,*result);
				result->sign = 1;
			}
		}
		else
		{
			if (cmp(b,*this))
				sub(b,*this,*result);
			else
			{
				sub(*this,b,*result);
				result->sign = 1;
			}
		}
		return *result;
	}

	longInt& operator-(const longInt& b)
	{
		result = new longInt;
		if (sign == 0 && b.sign == 0)
		{
			if (cmp(*this,b))
				sub(*this,b,*result);
			else
			{
				sub(b,*this,*result);
				result->sign = 1;
			}
		}
		else if (sign == 0 && b.sign == 1)
			add(*this,b,*result);
		else if (sign == 1 && b.sign == 0)
		{
			add(*this,b,*result);
			result->sign = 1;
		}
		else
		{
			if (cmp(b,*this))
				sub(b,*this,*result);
			else
			{
				sub(*this,b,*result);
				result->sign = 1;
			}
		}
		return *result;
	}

};

bool cmp( _IN_ const longInt& a, _OUT_ const longInt& b)
{
	int len_a,len_b;
	len_a = a.count;
	len_b = b.count;
	if (len_a == len_b)
	{
		linkNode * curr_a, *curr_b;
		int len = len_a;
		curr_a = a.head;
		curr_b = b.head;
		while(len--)
		{
			if (curr_a->data == curr_b->data)
			{
				curr_a = curr_a->next;
				curr_b = curr_b->next;
			}
			else
				return (curr_a->data>curr_b->data) ? true : false;
		}
		return true;
	}
	else
		return (len_a > len_b) ? true : false;
}

void add( _IN_ const longInt& a, 
	_IN_ const longInt& b, 
	_OUT_ longInt& result)
{
	linkNode *unit_a, *unit_b;
	int len;
	unit_a = a.tail;
	unit_b = b.tail;
	len = a.count >= b.count ? a.count : b.count;

	bool flag_a = false,flag_b = false;

	int temp_data, carry = 0, bit;
	//to make sure it have a carry to store
	len++;
	while(len--)
	{
		int x,y;
		x = flag_a ? 0 : unit_a->data;
		y = flag_b ? 0 : unit_b->data;
		temp_data = x + y + carry;
		carry = temp_data/10000;
		bit = temp_data%10000;
		result.insertFront(bit);
		if (unit_a->prev)	unit_a = unit_a->prev;
		else flag_a = true;
		if (unit_b->prev)	unit_b = unit_b->prev;
		else flag_b = true;
	}
	result.sign = 0;
}

void sub( _IN_ const longInt& a, 
	_IN_ const longInt& b, 
	_OUT_ longInt& result)
{
	linkNode *unit_a, *unit_b;
	unit_a = a.tail;
	unit_b = b.tail;

	bool flag_b = false;

	while(unit_a)
	{
		int x = unit_a->data, y;
		y = flag_b ? 0 : unit_b->data;
		int bowrrow = 0, bit;
		if (unit_a->data < unit_b->data)
		{
			bowrrow = 1;
			unit_a->prev->data--;
		}
		bit = bowrrow*10000 + x - y;
		result.insertFront(bit);
		unit_a = unit_a->prev;
		if (unit_b->prev)	unit_b = unit_b->prev;
		else flag_b = true;
	}
	result.sign = 0;
}

std::ostream& operator<<(std::ostream& out, const longInt& ori)
{
	linkNode *current = ori.head;
	if (ori.sign)
		out<<'-';
	//count the times
	int count = 0;
	while(current)
	{
		if (!count)
			out<<current->data;
		else
			printf("%04d",current->data);
		if (count != ori.count-1)
			out<<',';
		current = current->next;
		count++;
	}
	out<<std::endl;
	return out;
}

std::istream& operator>>(std::istream& in, longInt& dest)
{
	int node_data;
	char ch;
	in>>ch;
	if (ch == '-')
		dest.sign = 1;
	while(ch != '#')
	{
		if (isdigit(ch))
		{
			in.putback(ch);
			in>>node_data;
			dest.insertBack(node_data);
		}
		in>>ch;
	}
	return in;
}

#endif