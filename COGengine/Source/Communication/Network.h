#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "ofxNetwork.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"

namespace Cog {

	class Network  {
		
	private:
		ofxTCPClient tcpClient;
		ofxTCPServer tcpServer;
		ofxUDPManager udpSender;
		ofxUDPManager udpReceiver;

		int udpListenPort = 0;

		// udp receiver buffers
		NetReader* bufferStream;
	public:

		Network() {
			udpSender.Create();
			udpReceiver.Create();
		}

		void SetupTCPClient(string ip, int port, string msgDelimiter) {
			tcpClient.setup(ip.c_str(), port);
			tcpClient.setMessageDelimiter(msgDelimiter);
		}

		void SetupTCPServer(int port, string msgDelimiter) {
			tcpServer.setup(port);
			// delimiter is set as soon as a new client is connected
			tcpServer.setMessageDelimiter(msgDelimiter);
		}

		ofxTCPClient& GetTCPClient() {
			return tcpClient;
		}

		ofxTCPServer& GetTCPServer() {
			return tcpServer;
		}

		void SetupUDPSender(string ip, int port, bool nonBlocking) {
			udpSender.Connect(ip.c_str(), port);
			udpSender.SetNonBlocking(nonBlocking);
		}


		ofxUDPManager& GetUDPSender() {
			return udpSender;
		}

		ofxUDPManager& GetUDPReceiver() {
			return udpSender;
		}

		void SetupUDPReceiver(int port, int bufferSize, bool nonBlocking) {
			udpReceiver.Bind(port);
			udpReceiver.SetNonBlocking(nonBlocking);
			udpReceiver.SetReceiveBufferSize(bufferSize);
			this->udpListenPort = port;
			bufferStream = new NetReader(bufferSize);
		}

		void SendUDPMessage(unsigned int applicationId, spt<NetOutputMessage> msg) {
			NetWriter* writer = new NetWriter(msg->GetMessageLength() + 2);
			// write application id and the content
			writer->WriteWord(applicationId);
			msg->SaveToStream(writer);

			auto buffer = writer->GetBuffer();
			udpSender.Send((char*)buffer, writer->GetUsedBites() / 8);
			delete writer;
		}

		spt<NetInputMessage> ReceiveUDPMessage(unsigned int applicationId, int timeoutSec) {

			auto time = ofGetElapsedTimeMillis();
			int timeOutMillis = timeoutSec * 1000;

			NetReader* filledBuffer = nullptr;

			while (true) {

				bufferStream->Reset();
				int bytesBuff = udpReceiver.Receive((char*)bufferStream->GetBuffer(), bufferStream->GetBufferBites() / 8);

				if (bytesBuff > 0 && bufferStream->ReadWord() == applicationId) {
					
					// size of content
					unsigned int size = bytesBuff - 2;

					// from now, bufferStream1 contains the proper content
					spt<NetInputMessage> msg = spt<NetInputMessage>(new NetInputMessage(size));
					msg->LoadFromStream(bufferStream);
					string ipAddress = "";
					int port = 0;
					udpReceiver.GetRemoteAddr(ipAddress, port);
					msg->SetSourceIp(ipAddress);
					msg->SetSourcePort(port);

					return msg;
				}

				if ((ofGetElapsedTimeMillis() - time) > timeOutMillis) {
					return spt<NetInputMessage>();
				}
			}
		}
	};

}// namespace