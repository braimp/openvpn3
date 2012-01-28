#ifndef OPENVPN_TUN_CLIENT_TUNNULL_H
#define OPENVPN_TUN_CLIENT_TUNNULL_H

#include <openvpn/tun/client/tunbase.hpp>

// Null tun object, intended for testing
namespace openvpn {
  namespace TunNull {

    class ClientConfig : public TunClientFactory
    {
    public:
      typedef boost::intrusive_ptr<ClientConfig> Ptr;

      Frame::Ptr frame;
      ProtoStats::Ptr stats;

      static Ptr new_obj()
      {
	return new ClientConfig;
      }

      virtual TunClient::Ptr new_client_obj(boost::asio::io_service& io_service,
					    TunClientParent& parent);
    private:
      ClientConfig() {}
    };

    class Client : public TunClient
    {
      friend class ClientConfig;  // calls constructor

    public:
      virtual void client_start(const OptionList& opt, TransportClient& transcli)
      {
	// signal that we are connected
	parent.tun_connected();
      }

      virtual bool tun_send(BufferAllocated& buf)
      {
	config->stats->inc_stat(ProtoStats::TUN_BYTES_OUT, buf.size());
	return true;
      }

      virtual std::string tun_name() const
      {
	return "TUN_NULL";
      }

      virtual void stop() {}

    private:
      Client(boost::asio::io_service& io_service_arg,
	     ClientConfig* config_arg,
	     TunClientParent& parent_arg)
	:  io_service(io_service_arg),
	   config(config_arg),
	   parent(parent_arg)
      {
      }

      boost::asio::io_service& io_service;
      ClientConfig::Ptr config;
      TunClientParent& parent;
    };

    inline TunClient::Ptr ClientConfig::new_client_obj(boost::asio::io_service& io_service,
						       TunClientParent& parent)
    {
      return TunClient::Ptr(new Client(io_service, this, parent));
    }

  }
} // namespace openvpn

#endif // OPENVPN_TUN_CLIENT_TUNNULL_H