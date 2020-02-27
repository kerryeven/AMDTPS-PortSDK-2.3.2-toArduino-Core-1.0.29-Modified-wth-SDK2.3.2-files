// ****************************************************************************
//
//  amdtpc_main.c
//! @file
//!
//! @brief Ambiq Micro AMDTP client.
//!
//! @{
//
// ****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include <string.h>
#include <stdbool.h>
#include "wsf_types.h"
#include "wsf_assert.h"
#include "bstream.h"
#include "app_api.h"
#include "amdtpc_api.h"
#include "svc_amdtp.h"
#include "wsf_trace.h"

//KHE extra includes
#include "ble_menu.h"


//extern uint32_t am_menu_printf(const char *pcFmt, ...);


static void amdtpcHandleWriteResponse(attEvt_t *pMsg);

//*****************************************************************************
//
// Global variables
//
//*****************************************************************************

uint8_t rxPktBuf[AMDTP_PACKET_SIZE];
uint8_t txPktBuf[AMDTP_PACKET_SIZE];
uint8_t ackPktBuf[20];


/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/* UUIDs */
static const uint8_t amdtpSvcUuid[] = {ATT_UUID_AMDTP_SERVICE};    /*! AMDTP service */
static const uint8_t amdtpRxChUuid[] = {ATT_UUID_AMDTP_RX};        /*! AMDTP Rx */
static const uint8_t amdtpTxChUuid[] = {ATT_UUID_AMDTP_TX};        /*! AMDTP Tx */
static const uint8_t amdtpAckChUuid[] = {ATT_UUID_AMDTP_ACK};      /*! AMDTP Ack */

/* Characteristics for discovery */

/*! Proprietary data */
static const attcDiscChar_t amdtpRx =
{
  amdtpRxChUuid,
  ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

static const attcDiscChar_t amdtpTx =
{
  amdtpTxChUuid,
  ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

/*! AMDTP Tx CCC descriptor */
static const attcDiscChar_t amdtpTxCcc =
{
  attCliChCfgUuid,
  ATTC_SET_REQUIRED | ATTC_SET_DESCRIPTOR
};

static const attcDiscChar_t amdtpAck =
{
  amdtpAckChUuid,
  ATTC_SET_REQUIRED | ATTC_SET_UUID_128
};

/*! AMDTP Tx CCC descriptor */
static const attcDiscChar_t amdtpAckCcc =
{
  attCliChCfgUuid,
  ATTC_SET_REQUIRED | ATTC_SET_DESCRIPTOR
};

/*! List of characteristics to be discovered; order matches handle index enumeration  */
static const attcDiscChar_t *amdtpDiscCharList[] =
{
  &amdtpRx,                  /*! Rx */
  &amdtpTx,                  /*! Tx */
  &amdtpTxCcc,               /*! Tx CCC descriptor */
  &amdtpAck,                 /*! Ack */
  &amdtpAckCcc               /*! Ack CCC descriptor */
};

/* sanity check:  make sure handle list length matches characteristic list length */
//WSF_ASSERT(AMDTP_HDL_LIST_LEN == ((sizeof(amdtpDiscCharList) / sizeof(attcDiscChar_t *))));


/* Control block */
static struct
{
    bool_t                  txReady;                // TRUE if ready to send notifications
    uint16_t                attRxHdl;
    uint16_t                attAckHdl;
    amdtpCb_t               core;
}
amdtpcCb;

/*************************************************************************************************/
/*!
 *  \fn     AmdtpDiscover
 *
 *  \brief  Perform service and characteristic discovery for AMDTP service.
 *          Parameter pHdlList must point to an array of length AMDTP_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void
AmdtpcDiscover(dmConnId_t connId, uint16_t *pHdlList)
{
  AppDiscFindService(connId, ATT_128_UUID_LEN, (uint8_t *) amdtpSvcUuid,
                     AMDTP_HDL_LIST_LEN, (attcDiscChar_t **) amdtpDiscCharList, pHdlList);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover amdtpServiceUUID %02x%02x\n",amdtpSvcUuid[1],amdtpSvcUuid[0]);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover amdtpRxChUuid: %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",amdtpRxChUuid[15],amdtpRxChUuid[14],amdtpRxChUuid[13],amdtpRxChUuid[12],amdtpRxChUuid[11],amdtpRxChUuid[10],amdtpRxChUuid[9],amdtpRxChUuid[8],amdtpRxChUuid[7],amdtpRxChUuid[6],amdtpRxChUuid[5],amdtpRxChUuid[4],amdtpRxChUuid[3],amdtpRxChUuid[2],amdtpRxChUuid[1],amdtpRxChUuid[0]);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover amdtpTxChUuid: %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",amdtpTxChUuid[15],amdtpTxChUuid[14],amdtpTxChUuid[13],amdtpTxChUuid[12],amdtpTxChUuid[11],amdtpTxChUuid[10],amdtpTxChUuid[9],amdtpTxChUuid[8],amdtpTxChUuid[7],amdtpTxChUuid[6],amdtpTxChUuid[5],amdtpTxChUuid[4],amdtpTxChUuid[3],amdtpTxChUuid[2],amdtpTxChUuid[1],amdtpTxChUuid[0]);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover attCliChCfgUuid amdtpTxCcc: %02x%02x\n",attCliChCfgUuid[1],attCliChCfgUuid[0]);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover amdtpAckChUuid: %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",amdtpAckChUuid[15],amdtpAckChUuid[14],amdtpAckChUuid[13],amdtpAckChUuid[12],amdtpAckChUuid[11],amdtpAckChUuid[10],amdtpAckChUuid[9],amdtpAckChUuid[8],amdtpAckChUuid[7],amdtpAckChUuid[6],amdtpAckChUuid[5],amdtpAckChUuid[4],amdtpAckChUuid[3],amdtpAckChUuid[2],amdtpAckChUuid[1],amdtpAckChUuid[0]);
  am_menu_printf("KHE - amdtpc_main.c AmdtpcDiscover attCliChCfgUuid amdtpAckCcc: %02x%02x\n",attCliChCfgUuid[1],attCliChCfgUuid[0]);
    /*KHE*/
  //JUMP
      am_menu_printf(" Connection opened KHE\r\n"); //KHE
      am_hal_gpio_pinconfig(18, g_AM_HAL_GPIO_OUTPUT); //KHE
      am_hal_gpio_output_set(18);  //KHE
      am_menu_printf("Connected - Turned Blue light on... KHE\n");  //KHE

      //char s_Msg = char*(&pMsg->hdr.event);  //KHE
      //am_menu_printf(s_Msg); //KHE
}

//*****************************************************************************
//
// Send data to Server
//
//*****************************************************************************
static void
amdtpcSendData(uint8_t *buf, uint16_t len)
{
    dmConnId_t connId;
    if ((connId = AppConnIsOpen()) == DM_CONN_ID_NONE)
    {
        //am_menu_printf("KHE - amdtpc_main.c AmdtpcSendData to server, data = %d\n",(uint8_t)*buf);
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendData to server Connection DM_CONN_ID_NONE");
        APP_TRACE_INFO0("AmdtpcSendData() no connection\n");
        return;
    }
    if (amdtpcCb.attRxHdl != ATT_HANDLE_NONE)
    {
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendData: attRxHdl = 0x%x ...\r\n",amdtpcCb.attRxHdl);
        AttcWriteCmd(connId, amdtpcCb.attRxHdl, len, buf);
        amdtpcCb.txReady = false;
    }
    else
    {
        APP_TRACE_WARN1("Invalid attRxHdl = 0x%x\n", amdtpcCb.attRxHdl);
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendData: INVALID HANDLE attRxHdl = 0x%x\n", amdtpcCb.attRxHdl);
    }
}

static eAmdtpStatus_t
amdtpcSendAck(eAmdtpPktType_t type, bool_t encrypted, bool_t enableACK, uint8_t *buf, uint16_t len)
{
    dmConnId_t connId;

    AmdtpBuildPkt(&amdtpcCb.core, type, encrypted, enableACK, buf, len);

    if ((connId = AppConnIsOpen()) == DM_CONN_ID_NONE)
    {
        APP_TRACE_INFO0("AmdtpcSendAck() no connection\n");
        am_menu_printf("KHE - amdtpc_main.c amdtpcSendAck no connection\n");
        return AMDTP_STATUS_TX_NOT_READY;
    }

    if (amdtpcCb.attAckHdl != ATT_HANDLE_NONE)
    {
        //APP_TRACE_INFO2("rxHdl = 0x%x, ackHdl = 0x%x\n", amdtpcCb.attRxHdl, amdtpcCb.attAckHdl);
        //am_menu_printf("KHE - amdtpc_main.c amdtpcSendAck Send Ack Data = %d\n", amdtpcCb.core.ackPkt.data);
        AttcWriteCmd(connId, amdtpcCb.attAckHdl, amdtpcCb.core.ackPkt.len, amdtpcCb.core.ackPkt.data);
    }
    else
    {
        APP_TRACE_INFO1("Invalid attAckHdl = 0x%x\n", amdtpcCb.attAckHdl);
        am_menu_printf("KHE - amdtpc_main.c amdtpcSendAck Invalid attAckHdl = 0x%x\n", amdtpcCb.attAckHdl);
        return AMDTP_STATUS_TX_NOT_READY;
    }
    return AMDTP_STATUS_SUCCESS;
}

void
amdtpc_init(wsfHandlerId_t handlerId, amdtpRecvCback_t recvCback, amdtpTransCback_t transCback)
{
    memset(&amdtpcCb, 0, sizeof(amdtpcCb));
    amdtpcCb.txReady = false;
    amdtpcCb.core.txState = AMDTP_STATE_TX_IDLE;
    amdtpcCb.core.rxState = AMDTP_STATE_INIT;
    amdtpcCb.core.timeoutTimer.handlerId = handlerId;

    amdtpcCb.core.lastRxPktSn = 0;
    amdtpcCb.core.txPktSn = 0;

    resetPkt(&amdtpcCb.core.rxPkt);
    amdtpcCb.core.rxPkt.data = rxPktBuf;

    resetPkt(&amdtpcCb.core.txPkt);
    amdtpcCb.core.txPkt.data = txPktBuf;

    resetPkt(&amdtpcCb.core.ackPkt);
    amdtpcCb.core.ackPkt.data = ackPktBuf;

    amdtpcCb.core.recvCback = recvCback;
    amdtpcCb.core.transCback = transCback;

    amdtpcCb.core.txTimeoutMs = TX_TIMEOUT_DEFAULT;

    amdtpcCb.core.data_sender_func = amdtpcSendData;
    amdtpcCb.core.ack_sender_func = amdtpcSendAck;
}

static void
amdtpc_conn_close(dmEvt_t *pMsg)
{
    /* clear connection */
    WsfTimerStop(&amdtpcCb.core.timeoutTimer);
    amdtpcCb.txReady = false;
    amdtpcCb.core.txState = AMDTP_STATE_TX_IDLE;
    amdtpcCb.core.rxState = AMDTP_STATE_INIT;
    amdtpcCb.core.lastRxPktSn = 0;
    amdtpcCb.core.txPktSn = 0;
    resetPkt(&amdtpcCb.core.rxPkt);
    resetPkt(&amdtpcCb.core.txPkt);
    resetPkt(&amdtpcCb.core.ackPkt);
}

void
amdtpc_start(uint16_t rxHdl, uint16_t ackHdl, uint8_t timerEvt)
{
    amdtpcCb.txReady = true;
    amdtpcCb.attRxHdl = rxHdl;
    amdtpcCb.attAckHdl = ackHdl;
    amdtpcCb.core.timeoutTimer.msg.event = timerEvt;

    dmConnId_t connId;

    if ((connId = AppConnIsOpen()) == DM_CONN_ID_NONE)
    {
        APP_TRACE_INFO0("amdtpc_start() no connection\n");
        return;
    }

    amdtpcCb.core.attMtuSize = AttGetMtu(connId);
    APP_TRACE_INFO1("MTU size = %d bytes", amdtpcCb.core.attMtuSize);
}

//*****************************************************************************
//
// Timer Expiration handler
//
//*****************************************************************************
void
amdtpc_timeout_timer_expired(wsfMsgHdr_t *pMsg)
{
    uint8_t data[1];
    data[0] = amdtpcCb.core.txPktSn;
    APP_TRACE_INFO1("amdtpc tx timeout, txPktSn = %d", amdtpcCb.core.txPktSn);
    //am_menu_printf("KHE - amdtpc_main.c amdtpc_timeout_timer_expired %d\n", amdtpcCb.core.txPktSn);
    AmdtpSendControl(&amdtpcCb.core, AMDTP_CONTROL_RESEND_REQ, data, 1);
    // fire a timer for receiving an AMDTP_STATUS_RESEND_REPLY ACK
    WsfTimerStartMs(&amdtpcCb.core.timeoutTimer, amdtpcCb.core.txTimeoutMs);
}

/*************************************************************************************************/
/*!
 *  \fn     amdtpcValueNtf
 *
 *  \brief  Process a received ATT notification.
 *
 *  \param  pMsg    Pointer to ATT callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static uint8_t
amdtpcValueNtf(attEvt_t *pMsg)
{
    eAmdtpStatus_t status = AMDTP_STATUS_UNKNOWN_ERROR;
    amdtpPacket_t *pkt = NULL;
#if 0
    am_menu_printf("KHE - amdtpc_main.c amdtpcValueNtf: Received Notification from amdtp_proc_msg \n");
    APP_TRACE_INFO0("receive ntf data\n");
    am_menu_printf("KHE - amdtpc_main.c amdtpcValueNtf handle = 0x%x\n", pMsg->handle);
    APP_TRACE_INFO1("handle = 0x%x\n", pMsg->handle);
    am_menu_printf("KHE - amdtpc_main.c amdtpcValueNtf pMsg->value = ");
    for (int i = 0; i < pMsg->valueLen; i++)
    {
        am_menu_printf("%02x ", pMsg->pValue[i]);
        //APP_TRACE_INFO1("%02x ", pMsg->pValue[i]);
    }
    am_menu_printf("\n");
    //APP_TRACE_INFO0("\n");
#endif

    if (pMsg->handle == amdtpcCb.attRxHdl)
    {
        status = AmdtpReceivePkt(&amdtpcCb.core, &amdtpcCb.core.rxPkt, pMsg->valueLen, pMsg->pValue);
    }
    else if ( pMsg->handle == amdtpcCb.attAckHdl )
    {
        status = AmdtpReceivePkt(&amdtpcCb.core, &amdtpcCb.core.ackPkt, pMsg->valueLen, pMsg->pValue);
    }

    if (status == AMDTP_STATUS_RECEIVE_DONE)
    {
        if (pMsg->handle == amdtpcCb.attRxHdl)
        {
            pkt = &amdtpcCb.core.rxPkt;
        }
        else if (pMsg->handle == amdtpcCb.attAckHdl)
        {
            pkt = &amdtpcCb.core.ackPkt;
        }

        AmdtpPacketHandler(&amdtpcCb.core, (eAmdtpPktType_t)pkt->header.pktType, pkt->len - AMDTP_CRC_SIZE_IN_PKT, pkt->data);
    }
    am_menu_printf("KHE - amdtpc_main.c amdtpcValueNtf Success processed a received ATT notification %d\n ", ATT_SUCCESS);

    return ATT_SUCCESS;
}

static void
amdtpcHandleWriteResponse(attEvt_t *pMsg)
{
    am_menu_printf("KHE - amdtpc_main.c amdtpcHandlerWriteResponse: SENDING DATA TO SERVER: \n");
    am_menu_printf("KHE - amdtpc_main.c amdtpcHandlerWriteResponse: handle = 0x%x\n", pMsg->handle); //KHE = 0x802
    am_menu_printf("KHE - amdtpc_main.c amdtpcHandlerWriteResponse: pMsgLength = %d\n",pMsg->valueLen);
    for (int i = 0; i < pMsg->valueLen; i++)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtpcValueNtf pMsg->value = %02x\n ", pMsg->pValue[i]);
    }
    am_menu_printf("\n");
    am_menu_printf("amdtpcHandleWriteResponse, status = %d, hdl = 0x%x\n", pMsg->hdr.status, pMsg->handle); //KHE status = 0, hdl = 0x802
    //APP_TRACE_INFO2("amdtpcHandleWriteResponse, status = %d, hdl = 0x%x\n", pMsg->hdr.status, pMsg->handle);
    if (pMsg->hdr.status == ATT_SUCCESS && pMsg->handle == amdtpcCb.attRxHdl)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtpcHandleWriteResponse: MSG SENT - Tx IS READY - CALL amdtpSendPacketHandler for next msg...\r\n");
        amdtpcCb.txReady = true;
        // process next data
        AmdtpSendPacketHandler(&amdtpcCb.core);
    }
}

void
amdtpc_proc_msg(wsfMsgHdr_t *pMsg)
{
    am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: MSG RECEIVED = %i\r\n",pMsg->event);
    if (pMsg->event == DM_CONN_OPEN_IND)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: DM_CONN_OPEN_IND \r\n");
        //am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg pMsg->param = %d\n ", pMsg->param);
        //am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg pMsg->even = %d\n", pMsg->event);
        //am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg pMsg->status = %d\n", pMsg->status);
    }
    else if (pMsg->event == DM_CONN_CLOSE_IND)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: DM_CONN_CLOSE_IND \r\n");
        amdtpc_conn_close((dmEvt_t *) pMsg);
    }
    else if (pMsg->event == DM_CONN_UPDATE_IND)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: DM_CONN_UPDATE_IND \r\n");
    
    }
    else if (pMsg->event == amdtpcCb.core.timeoutTimer.msg.event)
    {
       amdtpc_timeout_timer_expired(pMsg);
    }
    else if (pMsg->event == ATTC_WRITE_CMD_RSP)
    {
        am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: ATTC_WRITE_CMD_RSP calling amdtpHandleWriteResponse\r\n");
        amdtpcHandleWriteResponse((attEvt_t *) pMsg);
    }
    else if (pMsg->event == ATTC_HANDLE_VALUE_NTF) //KHE = 13
    {
        am_menu_printf("KHE - amdtpc_main.c amdtp_proc_msg: ATTC_HANDLE_VALUE_NTF  - calling amdtpcValueNtf\r\n");
        amdtpcValueNtf((attEvt_t *) pMsg);
    }
}

//*****************************************************************************
//
//! @brief Send data to Server via write command
//!
//! @param type - packet type
//! @param encrypted - is packet encrypted
//! @param enableACK - does client need to response
//! @param buf - data
//! @param len - data length
//!
//! @return status
//
//*****************************************************************************
eAmdtpStatus_t
AmdtpcSendPacket(eAmdtpPktType_t type, bool_t encrypted, bool_t enableACK, uint8_t *buf, uint16_t len)
{
    am_menu_printf("KHE - called from amdtp_main.c AmdtpcSendTestData: (from ble_menu.c selection 1...\r\n");
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - EXECUTING SEND PACKET \r\n");
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - Data Length =  %i\r\n", len);
    //for ( uint16_t i = 0 ; i < 5 ; i++ ){
      am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket: Data to Send = %i\r\n",buf[1]);
    //}
    
    // Check if the service is idle to send
    //
    if ( amdtpcCb.core.txState != AMDTP_STATE_TX_IDLE )
    {
        APP_TRACE_INFO1("data sending failed, tx state = %d", amdtpcCb.core.txState);
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - data sending failed, tx state = %d\r\n", amdtpcCb.core.txState);
        return AMDTP_STATUS_BUSY;
    }

    //
    // Check if data length is valid
    //
    if ( len > AMDTP_MAX_PAYLOAD_SIZE )
    {
        APP_TRACE_INFO1("data sending failed, exceed maximum payload, len = %d.", len);
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - data sending failed, exceed maximum payload, len = %d\n", len);
        return AMDTP_STATUS_INVALID_PKT_LENGTH;
    }

    //
    // Check if ready to send notification
    //
    /*KHE    KHE*/
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - Check if CallBack txReady ... \n");
    if ( !amdtpcCb.txReady )
    {
        //set in callback amdtpsHandleValueCnf
        APP_TRACE_INFO1("data sending failed, not ready for notification.", NULL);
        am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - data sending failed, not ready for notification.\n", NULL);
        return AMDTP_STATUS_TX_NOT_READY;
    }

    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - txReady - calling AMdtpBuildPkt \n");
    AmdtpBuildPkt(&amdtpcCb.core, type, encrypted, enableACK, buf, len);

    // send packet
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - Packet built - calling AmdtpSendPacketHandler ... \n");
    AmdtpSendPacketHandler(&amdtpcCb.core);
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - SENT DATA packet %d\n",amdtpcCb.core);
    am_menu_printf("KHE - amdtpc_main.c AmdtpcSendPacket - AMDTP_STATUS_SUCCESS = %d\n",AMDTP_STATUS_SUCCESS);

    return AMDTP_STATUS_SUCCESS;
}
