#include "WebsocketClient.hpp"

#include "IXNetSystem.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

std::string WebsocketClient::m_Url{"ws://localhost:80/"};

WebsocketClient::WebsocketClient(const ix::WebSocketHttpHeaders& header): m_Headers(header)
{
  ix::initNetSystem();
}

void WebsocketClient::setExtraHeader(const std::string& key, const std::string& value)
{
  m_Headers[key.c_str()] = (' ' + value).c_str();
}

void WebsocketClient::setOnMessageCallback(std::function<void(const ix::WebSocketMessagePtr&)> m_func)
{
  m_WebSocket.setOnMessageCallback(m_func);
}

WebsocketClient::~WebsocketClient()
{
  ix::uninitNetSystem();
}

void WebsocketClient::setHeartBeatPeriod(const int& HeartBeat)
{
  m_WebSocket.setPingInterval(HeartBeat);
}

ix::ReadyState WebsocketClient::getReadyState()
{
  return m_WebSocket.getReadyState();
}

void WebsocketClient::disableAutomaticReconnection()
{
  m_WebSocket.disableAutomaticReconnection();
}

void WebsocketClient::start()
{
  m_WebSocket.setUrl(m_Url);
  m_WebSocket.setExtraHeaders(m_Headers);
  m_WebSocket.start();
}

void WebsocketClient::stop()
{
  m_WebSocket.stop();
}

ix::WebSocketSendInfo WebsocketClient::send(const std::string& data, bool binary, const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.send(data, binary, onProgressCallback);
}

ix::WebSocketSendInfo WebsocketClient::sendBinary(const std::string& text, const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.sendBinary(text, onProgressCallback);
}

ix::WebSocketSendInfo WebsocketClient::sendText(const std::string& text, const ix::OnProgressCallback& onProgressCallback)
{
  return m_WebSocket.sendText(text, onProgressCallback);
}

void WebsocketClient::setURL(const std::string& url)
{
  m_Url = url;
}
