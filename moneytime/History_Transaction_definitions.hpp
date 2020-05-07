#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//

Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
	unsigned year_date, bool buy_sel_trans, unsigned int number_shares, double trans_amount)
{
	//std::cout << "dsa" << std::endl;

	trans_id = assigned_trans_id  ;
	symbol= ticker_symbol;
	day = day_date;
	month = month_date ;
	year = year_date;
	if (buy_sel_trans)
		trans_type = "Buy";
	else
		trans_type = "Sell";
	//use if statement
	shares = number_shares ;
	amount = trans_amount;
	p_next = nullptr;

	acb = 0;
	acb_per_share = 0;
	share_balance = 0;
	cgl = 0;

	assigned_trans_id++;
}




// Destructor
// TASK
//
Transaction::~Transaction() {

}


// TASK
// Overloaded < operator.
//

bool Transaction::operator < (Transaction const & other)
{

	if (year < other.year)
		return true;
	else if (year > other.year)
		return false;
	else
	{
		if (month < other.month)
			return true;
		else if (month > other.month)
			return false;
		else
		{
			if (day < other.day)
				return true;
			else if (day > other.day)
				return false;
				
		}
	}
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//

History::History()
{
	p_head = nullptr;
}


// Destructor
// TASK
//
History::~History()
{
	
	while (p_head != nullptr)
	{
		Transaction *temp_p_head{p_head};
		p_head = p_head->get_next();
		delete temp_p_head;
	}
}


// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history()
{

	
	//open file, initialize using constructor, 
	ece150::open_file();

	while (ece150::next_trans_entry())
	{
		Transaction* money = new Transaction{
			ece150::get_trans_symbol(),
			ece150::get_trans_day(),
			ece150::get_trans_month(),
			ece150::get_trans_year(),
			ece150::get_trans_type(),
			ece150::get_trans_shares(),
			ece150::get_trans_amount()
		};

		History::insert(money);

		//std::cout << "sdfs" << std::endl;
	}
	ece150::close_file();
}


// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction* p_new_trans)
{


	Transaction* p_traverse{ p_head };
	if (p_head == nullptr)
	{
		p_head =  p_new_trans;
	}
	else
	{
		while (p_traverse->get_next() != nullptr)
		{
			
			p_traverse = p_traverse->get_next();
		}
	
		p_traverse->set_next(p_new_trans);
	}

	//std::cout << "sda" << std::endl;
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date()
{
	
	Transaction* p_new{ nullptr };
	Transaction* temp{ p_head };
	while (p_head != nullptr)
	{

		temp = p_head;
		p_head = p_head->get_next();


		if (p_new == nullptr)
		{
			temp->set_next(nullptr);
			p_new = temp;

		}
		else
		{
			//std::cout << "dsdf" << std::endl;

			Transaction* compare{ p_new };
			if (*temp < *compare)
			{
				temp->set_next(p_new);
				p_new = temp;
			}
			else
			{
				while ((compare->get_next() != nullptr) && *(compare->get_next()) < *temp)
				{
					compare = compare->get_next();
				}
				temp->set_next(compare->get_next());
				compare->set_next(temp);
			}

		}
		//std::cout << "d" << std::endl;
	}
	p_head = p_new;
}


// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//
void History::update_acb_cgl()
{
	Transaction* p_last{ p_head};
	Transaction* p{ p_head };
	while (p != nullptr)
	{
		if (p == p_head)
		{
			p->set_acb(p->get_amount());
			p->set_share_balance(p->get_shares());
			p->set_acb_per_share((p->get_acb()) / (p->get_share_balance()));
			
			p = p->get_next();

		}
		else
		{
			if (p->get_trans_type())
			{
				double x = (p_last->get_acb()) + (p->get_amount());

				p->set_acb(x);
				p->set_share_balance((p_last->get_share_balance())+(p->get_shares()));
				p->set_acb_per_share((p->get_acb()) / (p->get_share_balance()));
				
			}
			else
			{
				double x = (p_last->get_acb()) - (p->get_shares())*(p_last->get_acb_per_share());

				p->set_acb(x);
				p->set_share_balance((p_last->get_share_balance()) - (p->get_shares()));
				p->set_acb_per_share((p->get_acb()) / (p->get_share_balance()));
				p->set_cgl(p->get_amount()-(p->get_shares()) * (p_last->get_acb_per_share()));
			}
			p = p->get_next();
			p_last = p_last->get_next();
		
		}
		
	}

}



// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year)
{
	Transaction* p_traverse{ p_head };
	double sum = 0;
	while (p_traverse != nullptr)
	{

		if ((p_traverse->get_year()) == year)
			sum += p_traverse->get_cgl();

		p_traverse = p_traverse->get_next();
	}
	return sum;
}



// TASK
// print() Print the transaction history.
//
void History::print()
{
	Transaction* p_traverse{ p_head };
	//std::cout << "hgfg";
	std::cout << "========= BEGIN TRANSACTION HISTORY =========" << std::endl;
	while (p_traverse != nullptr)
	{
		
		p_traverse->print();
		p_traverse = p_traverse->get_next();
	}
	std::cout << "========= END TRANSACTION HISTORY =========" << std::endl;
	
}



// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

/*
Old codef


Transaction* p_to_sort{ p_head };
	while (p_to_sort != nullptr)
	{

		if (  p_to_sort->get_next()< p_to_sort)
		{

			Transaction* p{ p_to_sort };
			while ((p->get_next()) < p)
			{
				if (p = p_head)
				{
					Transaction* p_temp{ p };
					p->set_next((p->get_next())->get_next());
					(p_temp->get_next())->set_next(p_temp);
				}
				else if (p->get_next() == nullptr)
				{
					Transaction* p_temp{ p };
					Transaction* p_behind{ p_head };

					while (p_behind->get_next() != p)
						p_behind = p_behind->get_next();

					p_behind->set_next(p->get_next());
					p->set_next(nullptr);
					(p_behind->get_next())->set_next(p_temp);
				}
			else {
				Transaction* p_temp{ p };
				Transaction* p_behind{ p_head };

				while (p_behind->get_next() != p)
					p_behind = p_behind->get_next();

				p_behind->set_next(p->get_next());
				p->set_next((p->get_next())->get_next());
				(p_behind->get_next())->set_next(p_temp);
		}
				
			}

		}
		p_to_sort = p_to_sort->get_next();

*/