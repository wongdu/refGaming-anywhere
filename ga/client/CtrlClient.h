#ifndef __Ctrl_Client_H__
#define	__Ctrl_Client_H__

#include <deque>
#include "CtrlMessage.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using ctrl_message_queue = std::deque<CtrlMessage>;

class CtrlClient
{
	enum { max_length = 1024 };
public:
	CtrlClient(boost::asio::io_context& io_context,
		const tcp::resolver::results_type& endpoints);
	~CtrlClient();

public:
	void write(const CtrlMessage& msg);
	void do_write();
	void close();

private:
	void do_connect(const tcp::resolver::results_type& endpoints);
	void receive_response();

private:
	boost::asio::io_context& io_context_;
	tcp::socket socket_;
	ctrl_message_queue write_msgs_;
	char reply_[max_length];
};


#endif	
