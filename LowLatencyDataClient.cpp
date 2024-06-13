#include "LowLatencyDataClient.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#if defined(__linux__)
#include <arpa/inet.h>
#endif

#include <boost/range.hpp>

//
// Constructor
//
LowLatencyDataClient::LowLatencyDataClient(boost::asio::io_context& io_context,
    std::string& host, std::string& service, EventHandler fCb)
    : m_fEventHandler(fCb)
    , m_Data(nullptr)
    , m_nServerVersion(1)
{
    m_Data = new uint8_t[recvBufSize];
    Connect(io_context, host, service);
}

//
// Destructor
//
LowLatencyDataClient::~LowLatencyDataClient()
{
    m_bSocketReadThreadExit = true;

    // This causes the read in the thread to return error
    m_Socket->shutdown(tcp::socket::shutdown_both);
    m_Socket->close();

    m_SocketReadThread->join();
    delete m_SocketReadThread;

    delete [] m_Data;
}

//
// Thread used to read packets from the socket
//
void LowLatencyDataClient::SocketReadThread(void)
{
    // Set pointer to header
    LowLatencyStreamPacketHeader  *pHeader =
        reinterpret_cast<LowLatencyStreamPacketHeader *>(m_Data);

    // Start with getting a stream packet header
    size_t  nBufferOffset = 0;
    size_t  nAmount2Read = sizeof(LowLatencyStreamPacketHeader);

    // Loop forever
    while(!m_bSocketReadThreadExit) {

        int nAmountRead = 0;
        if(nAmount2Read + nBufferOffset > recvBufSize) {
            std::cerr << "Invalid amount to read " << nAmount2Read << std::endl;
            std::cerr << "Read " << nAmount2Read << " to " << nBufferOffset <<
                std::endl;
            std::cerr << "Last pHeader->length " << ::ntohl(pHeader->length) <<
                std::endl;
            std::cerr << "Last pHeader->id " << ::ntohl(pHeader->id) <<
                std::endl;
            break;
        }

        try {
            // Receive some data on from the socket
            nAmountRead = m_Socket->receive(boost::asio::buffer(
                m_Data + nBufferOffset, nAmount2Read));
        }
        catch(...) { break; }

        if(nAmountRead > 0) {
            // Adjust the buffer offset by the amount of data received
            nBufferOffset += nAmountRead;

            if(nBufferOffset < sizeof(LowLatencyStreamPacketHeader)) {
                // Don't have enough in the buffer for the entire header
                nAmount2Read =
                    sizeof(LowLatencyStreamPacketHeader) - nBufferOffset;
                continue;

            } else if(nBufferOffset < ::ntohl(pHeader->length)) {
                nAmount2Read = ::ntohl(pHeader->length) - nBufferOffset;

                // Don't have enough in the buffer for the entire packet
                continue;

            } else {
                // Process the packet and reset for the next packet
                pHeader->id = ::ntohl(pHeader->id);
                pHeader->length = ::ntohl(pHeader->length);
                ProcessPacket(pHeader);
                nAmount2Read = sizeof(LowLatencyStreamPacketHeader);
                nBufferOffset = 0;
            }
        } else if(nAmountRead < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            break;
        }
    }

    // If we didn't get here because we are exiting, shutdown and close the
    // socket
    if(!m_bSocketReadThreadExit) {
        m_Socket->shutdown(tcp::socket::shutdown_both);
        m_Socket->close();
    }
}

//
// Function used to connect to the server
//
void LowLatencyDataClient::Connect(boost::asio::io_context& io_context,
    std::string& host, std::string& service)
{
    tcp::resolver   resolver(io_context);

    // Create the socket
    m_Socket =
        std::make_unique<boost::asio::ip::tcp::socket>(io_context, tcp::v4());

    // Connect to the specified port on the specified server
    boost::asio::connect(*m_Socket, resolver.resolve(host, service));

    m_bSocketReadThreadExit = false;
    m_SocketReadThread = new std::thread([this]() { SocketReadThread(); });
}

//
// Function used to process incoming packets
//
void LowLatencyDataClient::ProcessPacket(LowLatencyStreamPacketHeader *pHeader)
{
    if(pHeader->id & METADATA_ID) {
        if(pHeader->id == METADATA_ID) ProcessMetadataPacket(pHeader);
        else std::cerr << "Corrupt metadata id " << std::hex <<
            pHeader->id << std::endl;
    }
    else ProcessDataPacket(pHeader);
}

//
// Function used to process incoming data packets
//
void LowLatencyDataClient::ProcessDataPacket(
    LowLatencyStreamPacketHeader *pHeader)
{
    if(m_mSubscribedChannelsList.find(pHeader->id) !=
        m_mSubscribedChannelsList.end()) {

        ChannelDataInfo cdi;
        cdi.nId = static_cast<int>(pHeader->id);
        cdi.pData = reinterpret_cast<float *>(pHeader + 1);
        cdi.nSamples = (pHeader->length - sizeof(*pHeader)) / sizeof(float);

        m_fEventHandler(EVENT_TYPE_CHANNEL_DATA, &cdi, sizeof(cdi));
    }
}

//
// Function used to process incoming unsubscribe response packets
//
void LowLatencyDataClient::ProcessUnsubscribeResponsePacket(json& j)
{
    for(auto& it : j["unsubscribed"]) {
        std::string sName(it);

        for(auto sit = m_mSubscribedChannelsList.begin();
            sit != m_mSubscribedChannelsList.end(); sit++) {

            if((*sit).second.sName == sName) {
                ChannelUnsubscribedInfo cui;
                cui.nId = (*sit).first;

                m_fEventHandler(EVENT_TYPE_CHANNEL_UNSUBSCRIBED, &cui,
                    sizeof(cui));

                m_mSubscribedChannelsList.erase(sit);
                break;
            }
        }
    }
}

//
// Function used to process include subscribe response packets
//
void LowLatencyDataClient::ProcessSubscribeResponsePacket(json& j)
{
    // List of channels that were successfull subscribed and need to be
    // removed from the pendig subscribe channels list
    std::vector<std::string>    vSubscribedChannels;

    // Process all of the channels given
    for(auto& [ idx, ci ] : j["/subscribed"_json_pointer].items()) {
        std::string sName =  ci["/name"_json_pointer];
        size_t      nId = ci["/id"_json_pointer];
        uint64_t    fsts_ns = ci["/first_sample_timestamp_ns"_json_pointer];

        // Find the channel in the pending subscribe channels list
        for(auto& cci : m_vPendingSubscribeChannelsList) {
            if(cci.sName == sName) {
                // Channel found, get a copy of the channel info from the
                // available channels list
                if(m_mAvailableChannelsList.find(sName) ==
                    m_mAvailableChannelsList.end()) {
                    std::cerr << "Channel " << sName <<
                        "is no longer available for subcribe" << std::endl;
                    continue;
                }
                ChannelInfo& aci = m_mAvailableChannelsList[sName];

                // Add the channel info to the list of subscribed channels
                m_mSubscribedChannelsList[nId] = aci;

                // Add this channel to the list of subscribed channels to be
                // removed from the pending list
                vSubscribedChannels.push_back(cci.sName);

                // Make channel subscribe info for this channel
                ChannelSubscribedInfo   csi;
                csi.sName = sName;
                csi.nId = nId;

                // Let the user know the channel was subscribed
                m_fEventHandler(EVENT_TYPE_CHANNEL_SUBSCRIBED, &csi,
                    sizeof(csi));

                // Save the first sample time stamp for the channel
                m_mFSTS[sName] = static_cast<double>(fsts_ns) / 1000000000.0;

                // Let the user know what the first sample timestamp for the
                // channel is
                ChannelTimestampInfo   ctsi;
                ctsi.sName = sName;
                ctsi.nId = nId;
                ctsi.dFirstSampleTimestamp = m_mFSTS[sName];

                m_fEventHandler(EVENT_TYPE_CHANNEL_FIRST_SAMPLE_TS, &ctsi,
                    sizeof(ctsi));

                // Done
                break;
            }
        }
    }

    // The the subscrirbed channels list is the same size as the pending
    // subscribed channels list, all of the specified channels were subscribed
    // to and the pending subscribed channels list can be emptied.
    if(vSubscribedChannels.size() == m_vPendingSubscribeChannelsList.size())
        m_vPendingSubscribeChannelsList.clear();

    else {
        // List are different sizes, remove entries from pending list one-by-one
        for(auto& sce : vSubscribedChannels) {
            for(auto it = m_vPendingSubscribeChannelsList.begin();
                it != m_vPendingSubscribeChannelsList.end(); it++) {
                if(sce == (*it).sName) {
                    m_vPendingSubscribeChannelsList.erase(it);
                    break;
                }
            }
        }
    }
}

void LowLatencyDataClient::ProcessOffsetsPacket(json& j)
{
    // Clear the offsets for all channels
    for(auto& [ name, ci ] : m_mAvailableChannelsList) ci.dGroundReading = NAN;

    for(auto& [ k, v ] : j["/offsets"_json_pointer].items()) {
        std::string sName(k);

        if(m_mAvailableChannelsList.find(sName) ==
            m_mAvailableChannelsList.end()) continue;

        m_mAvailableChannelsList[sName].dGroundReading = static_cast<double>(v);

        ChannelOffsetInfo   ci;
        ci.sName = sName;
        ci.dOffset = static_cast<double>(v);

        m_fEventHandler(EVENT_TYPE_CHANNEL_OFFSET, &ci, sizeof(ci));
    }
}

//
// Function used to process incoming available packets
//
void LowLatencyDataClient::ProcessAvailableChannelsPacket(json& j)
{
    // Process all of the available channel information given
    for(auto& [ k, v ] : j["/available"_json_pointer].items()) {
        std::string sName(k);
        json        jInfo(v);

        // Get/create the channel info entry for this channel
        ChannelInfo ci;

        // Extract channel specific informaton from the JSON and store it
        // int eh channel information structure
        ci.sName.assign(sName);
        ci.dSamplePeriod = jInfo["/sample_period"_json_pointer];
        ci.sDataType.assign(jInfo["/data_type"_json_pointer]);
        ci.dScale = jInfo["/scale"_json_pointer];
        ci.dOffset = jInfo["/offset"_json_pointer];
        ci.sAdcType = jInfo["/device"_json_pointer];
        ci.dGroundReading = NAN;
        ci.nDecimationFactor = 1;
        ci.jConfig.clear();
        if(jInfo.contains("/current_configuration"_json_pointer))
            ci.jConfig = jInfo["/current_configuration"_json_pointer];

        if(m_mAvailableChannelsList.find(sName) !=
            m_mAvailableChannelsList.end()) {
            std::cerr << "Channel '" << sName << "' is already available" <<
                std::endl;
            continue;
        }
        m_mAvailableChannelsList[sName] = ci;

        // Let the user know about the available channel
        m_fEventHandler(EVENT_TYPE_AVAILABLE_CHANNEL, &ci, sizeof(ci));
    }
}

//
// Function used to process incoming unavailable packets
//
void LowLatencyDataClient::ProcessUnavailableChannelsPacket(json& j)
{
    // Process all of the channels listed
    for(auto& [ idx, name ] : j["/unavailable"_json_pointer].items()) {
        std::string sName(name);

        // If the channel is in the available channels list, remove it
        if(m_mAvailableChannelsList.find(sName) !=
            m_mAvailableChannelsList.end()) {
            m_mAvailableChannelsList.erase(sName);
        }

        // The channel is in the subscribed channels list, remove it from
        // that as well.
        for(auto& [ id, ci ] : m_mSubscribedChannelsList) {
            if(ci.sName == sName) {
                UnsubscribeChannel(id);
                m_mSubscribedChannelsList.erase(id);
                break;
            }
        }

        // Remove the first sample timestamp for this channel
        if(m_mFSTS.find(sName) != m_mFSTS.end()) m_mFSTS.erase(sName);

        // Let the user know the channel is no longer available
        m_fEventHandler(EVENT_TYPE_UNAVAILABLE_CHANNEL,
            sName.c_str(), sName.size());
    }
}

//
// Function used to get the precise start time of the acquisition
//
const std::string& LowLatencyDataClient::PreciseAcquisitionStartTime(void)
{
    return m_sPreciseAcquisitionStartTime;
}


//
// Function used to process an acquisition state packet
//
void LowLatencyDataClient::ProcessAcquisitionStatePacket(json& j)
{
    m_sPreciseAcquisitionStartTime.assign("NOT ACQUIRING");

    if(j["acquisition_state"] == "off") {
        // If acquisition state is "off" reset the first sample timestamps for
        // all of the subscribed channels.

        for(auto& e: m_mSubscribedChannelsList) {

            std::string sName = e.second.sName;
            m_mFSTS[sName] = 0.0;
            ChannelTimestampInfo   ctsi;
            ctsi.sName = sName;
            ctsi.nId = e.first;
            ctsi.dFirstSampleTimestamp = m_mFSTS[sName];

            m_fEventHandler(EVENT_TYPE_CHANNEL_FIRST_SAMPLE_TS, &ctsi,
                sizeof(ctsi));
        }

        m_bAcquisitionState = false;

    } else if(j["acquisition_state"] == "on") {
        m_bAcquisitionState = true;

        if(j.contains("precise_acquisition_start_time")) {
            m_sPreciseAcquisitionStartTime.assign(
                j["precise_acquisition_start_time"]);

            m_fEventHandler(EVENT_TYPE_PRECISE_START_TIME,
                m_sPreciseAcquisitionStartTime.c_str(),
                m_sPreciseAcquisitionStartTime.size());
        }
    }

    m_fEventHandler(EVENT_TYPE_ACQUIRE, &m_bAcquisitionState,
        sizeof(m_bAcquisitionState));
}

//
// Function used to process incoming metadata packets
//
void LowLatencyDataClient::ProcessMetadataPacket(
    LowLatencyStreamPacketHeader *pHeader)
{
    std::string str(reinterpret_cast<char *>(pHeader + 1),
        reinterpret_cast<char *>(pHeader) + pHeader->length);

//        std::cerr << std::endl << std::endl << "Got: " << str << std::endl;

    try {
        json    j = json::parse(str);

        if(j.contains("/unsubscribed"_json_pointer))
            ProcessUnsubscribeResponsePacket(j);

        else if(j.contains("/subscribed"_json_pointer))
            ProcessSubscribeResponsePacket(j);

        else if(j.contains("/available"_json_pointer))
            ProcessAvailableChannelsPacket(j);

        else if(j.contains("/unavailable"_json_pointer))
            ProcessUnavailableChannelsPacket(j);

        else if(j.contains("/acquisition_state"_json_pointer))
            ProcessAcquisitionStatePacket(j);

        else if(j.contains("/configuration_capabilities"_json_pointer)) {
            m_ConfigJSON = j["/configuration_capabilities"_json_pointer];
            m_fEventHandler(EVENT_TYPE_CONFIG_INFO_AVAILABLE, nullptr, 0);

        } else if(j.contains("/offsets"_json_pointer))
            ProcessOffsetsPacket(j);

        else if(j.contains("/status"_json_pointer)) {
            if(static_cast<std::string>(j["/status"_json_pointer]) != "success")
            {
                std::string msg(j["/status"_json_pointer]);
                m_fEventHandler(EVENT_TYPE_ERROR, msg.c_str(), msg.size());
            }

        } else if(j.contains("/keepalive"_json_pointer)) {
            ;

        } else if(j.contains("/version"_json_pointer))
            m_nServerVersion = j["/version"_json_pointer];
    }
    catch(...) {
        std::stringstream   ss;
        ss << "Incoming JSON Parse Failure" << std::endl;
        ss << "Length: " << pHeader->length << std::endl;
        ss << "ID: " << std::hex << pHeader->id << std::endl;
        uint8_t *p = reinterpret_cast<uint8_t *>(pHeader + 1);
        for(int i = 0; i < 16; i++) {
            ss << " " << std::setw(2) << std::setfill('0') <<
                std::hex << static_cast<int>(p[i]);
        }
        ss << std::endl << std::endl;
        m_fEventHandler(EVENT_TYPE_ERROR, ss.str().c_str(), ss.str().size());
    }
}

//
// Function used to send subscribe channels
//
void LowLatencyDataClient::SendSubscribeChannels(
    const std::vector<ChannelInfo>& vChannelsList)
{
    json    j;

    bool    bHaveChannels = false;
    for(auto& e : vChannelsList) {
        // If the channel is NOT available, skip it
        if(m_mAvailableChannelsList.find(e.sName) ==
            m_mAvailableChannelsList.end()) continue;

        bHaveChannels = true;
        j["subscribe"][e.sName] = e.nDecimationFactor;
    }

    if(!bHaveChannels) return;

    std::string str(j.dump());
    std::vector<boost::asio::const_buffer>  vSendList;

    LowLatencyStreamPacketHeader    Header;

    Header.id = ::htonl(METADATA_ID);
    Header.length = ::htonl(str.length() + sizeof(Header));

    vSendList.push_back(boost::asio::buffer(&Header, sizeof(Header)));
    vSendList.push_back(boost::asio::buffer(str.data(), str.length()));

    m_Socket->send(boost::make_iterator_range(vSendList.begin(),
        vSendList.end()));
}

//
// Function used to send configure request
//
void LowLatencyDataClient::SendConfigureChannel(const json& configJSON)
{
    json    j;

    j["/configure"_json_pointer] = configJSON;

    std::string str(j.dump());
    std::vector<boost::asio::const_buffer>  vSendList;

    LowLatencyStreamPacketHeader    Header;

    Header.id = ::htonl(METADATA_ID);
    Header.length = ::htonl(str.length() + sizeof(Header));

    vSendList.push_back(boost::asio::buffer(&Header, sizeof(Header)));
    vSendList.push_back(boost::asio::buffer(str.data(), str.length()));

    m_Socket->send(boost::make_iterator_range(vSendList.begin(),
        vSendList.end()));
}

//
// Function used to get the id for a subscribed channel
//
int LowLatencyDataClient::SubscribedChannelId(std::string& sName)
{
    for(auto& e : m_mSubscribedChannelsList)
        if(e.second.sName == sName) return e.first;

    return -1;
}

//
// Function used to subscribe to one or more channels by name
//
void LowLatencyDataClient::SubscribeChannels(
    const std::vector<ChannelInfo>& vChannelsList)
{
    // Add the channels to the pending channel subscribe list
    m_vPendingSubscribeChannelsList.insert(
        m_vPendingSubscribeChannelsList.end(),
        vChannelsList.begin(), vChannelsList.end());

    // Send the subscribe request
    SendSubscribeChannels(vChannelsList);
}

//
// Function used to subscribe to a single channel
//
void LowLatencyDataClient::SubscribeChannel(std::string& sName,
    int nDecimationFactor)
{
    ChannelInfo ci;

    ci.sName.assign(sName);
    ci.nDecimationFactor = nDecimationFactor;

    const std::vector<ChannelInfo>    vChannelsList = { ci };

    SubscribeChannels(vChannelsList);
}

static void ShowObject(const json& j, std::string sBase = "")
{
    std::string sOldBase;

    for(auto& [ key, value ] : j.items()) {
        if(value.type() == json::value_t::object) {
            sOldBase = sBase;
            sBase += key;
            sBase += "/";
            ShowObject(value, sBase);
            sBase = sOldBase;

        } else if(value.type() == json::value_t::array) {
            std::cout << sBase << key << std::endl << "    " << std::flush;
            bool    bFirst = true;
            for(auto& [ i, v ] : value.items()) {
                if(!bFirst) std::cout << ", " << std::flush;
                if(v.type() == json::value_t::number_float && v >= 1e32)
                    std::cout << "any number" << std::flush;
                else std::cout << v << std::flush;
                bFirst = false;
            }
            std::cout << std::endl << std::flush;

        } else
            std::cout << key << " -> " << value << std::endl << std::flush;
    }
}

//
// Function used to configure a single channel
//
void LowLatencyDataClient::ConfigureChannel(std::string& sName, const json& j)
{
    if(m_nServerVersion < 3) {
        m_fEventHandler(EVENT_TYPE_ERROR,"Unsupported operation\n"
            "Configuration requires the server to be "
            "running at least version 3", 0);
        return;
    }

    if(m_mAvailableChannelsList.find(sName) ==
        m_mAvailableChannelsList.end()) return;

    json    jConfig;

    jConfig[sName] = j;

    SendConfigureChannel(jConfig);
}

//
// Function used to send unsubscribe packet
//
void LowLatencyDataClient::SendUnsubscribeChannels(
    const std::vector<int>& vChannelIdsList)
{
    json    j;

    bool    bHaveChannels = false;
    for(auto& nChId : vChannelIdsList) {
        if(m_mSubscribedChannelsList.find(nChId) ==
            m_mSubscribedChannelsList.end()) continue;

        bHaveChannels = true;
        j["unsubscribe"].push_back(std::to_string(nChId));
    }

    if(!bHaveChannels) return;

    std::string str(j.dump());
    std::vector<boost::asio::const_buffer>  vSendList;

    LowLatencyStreamPacketHeader    Header;

    Header.id = ::htonl(METADATA_ID);
    Header.length = ::htonl(str.length() + sizeof(Header));

    vSendList.push_back(boost::asio::buffer(&Header, sizeof(Header)));
    vSendList.push_back(boost::asio::buffer(str.data(), str.length()));

    m_Socket->send(boost::make_iterator_range(vSendList.begin(),
        vSendList.end()));
}

//
// Function used to unsubscribe from one or more channels by channel id
//
void LowLatencyDataClient::UnsubscribeChannels(
    const std::vector<int>& vChannelIdsList)
{
    SendUnsubscribeChannels(vChannelIdsList);
}

//
// Function used to unsubscribe from a single channel by channel id
//
void LowLatencyDataClient::UnsubscribeChannel(int nChannelId)
{
    const std::vector vIdList = {nChannelId};
    UnsubscribeChannels(vIdList);
}

//
// Function used to toggle acquisition on/off
//
void LowLatencyDataClient::Acquire(bool bRedundantRecord)
{
    json    j;

    j["/acquire"_json_pointer] = m_bAcquisitionState ? false : true;

    // record is only used when enabling acquisition
    if(!m_bAcquisitionState) j["/record"_json_pointer] = bRedundantRecord;

    std::string str(j.dump());
    std::vector<boost::asio::const_buffer>  vSendList;

    LowLatencyStreamPacketHeader    Header;

    Header.id = ::htonl(METADATA_ID);
    Header.length = ::htonl(str.length() + sizeof(Header));

    vSendList.push_back(boost::asio::buffer(&Header, sizeof(Header)));
    vSendList.push_back(boost::asio::buffer(str.data(), str.length()));

    m_Socket->send(boost::make_iterator_range(vSendList.begin(),
        vSendList.end()));
}
