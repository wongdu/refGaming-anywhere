
#include "CtrlClient.h"
#include "controller.h"


CtrlClient::CtrlClient(boost::asio::io_context& io_context,
	const tcp::resolver::results_type& endpoints) : io_context_(io_context),
	socket_(io_context) {
	do_connect(endpoints);
}

CtrlClient::~CtrlClient() {

}

void CtrlClient::do_connect(const tcp::resolver::results_type& endpoints) {
	boost::asio::async_connect(socket_, endpoints,
		[this](boost::system::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				struct ctrlhandshake hh;
				hh.length = 1 + strlen(CTRL_CURRENT_VERSION) + 1;	// msg total len, id, null-terminated
				if (hh.length > sizeof(hh))
					hh.length = sizeof(hh);
				strncpy(hh.id, CTRL_CURRENT_VERSION, sizeof(hh.id));
				boost::asio::write(socket_, boost::asio::buffer((char*)&hh, hh.length));
				receive_response();
			}
		});
}

void CtrlClient::receive_response() {
	boost::asio::async_read(socket_,
		boost::asio::buffer(reply_, max_length),
		[this](const boost::system::error_code& error, std::size_t length)
		{
			if (!error)
			{
			}
			else
			{
				socket_.close();
			}
		});
}

void CtrlClient::write(const CtrlMessage& msg) {
	boost::asio::post(io_context_,
		[this, msg]()
		{
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			{
				do_write();
			}
		});
}

void CtrlClient::do_write() {
	boost::asio::async_write(socket_,
		boost::asio::buffer(write_msgs_.front().data(),
			write_msgs_.front().length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				socket_.close();
			}
		});
}

void CtrlClient::close()
{
	boost::asio::post(io_context_, [this]() { socket_.close(); });
}