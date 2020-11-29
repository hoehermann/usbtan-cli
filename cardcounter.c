/***************************************************************************
    begin       : Thu Jan 09 2020
    copyright   : (C) 2020 by Herbert Ellebruch
    email       :

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

//#include "chiptanusb_p.h"

#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>
#include <gwenhywfar/ctplugin_be.h>
#include <gwenhywfar/text.h>
#include <gwenhywfar/padd.h>
#include <gwenhywfar/gui.h>
#include <gwenhywfar/i18n.h>
#include <gwenhywfar/tlv.h>

#include <chipcard/cards/chiptanusb.h>
#include <chipcard/cards/processorcard.h>
#include <chipcard/ct/ct_card.h>

#include "card_p.h"

#define PROGRAM_VERSION "1.0"

int main(int argc, char **argv)
{
  GWEN_Logger_SetLevel(0, GWEN_LoggerLevel_Verbous);

  LC_CLIENT *cl;
  LC_CARD *card;
  LC_CLIENT_RESULT res;

  cl = LC_Client_new("PinTanKarte", PROGRAM_VERSION);//  client.c
  if (LC_Client_Init(cl)) {
    DBG_ERROR(0, "ERROR: Could not init libchipcard");
    LC_Client_free(cl);
    return GWEN_ERROR_NOT_CONNECTED;
  }


  DBG_INFO(0, "Connecting to server.");
  res = LC_Client_Start(cl);
  if (res != LC_Client_ResultOk) {
    return GWEN_ERROR_NOT_CONNECTED;
  }
  DBG_INFO(0, "Connected.");

  int num_cards = 0;
  int timeout_seconds = 20;

  DBG_INFO(0, "Waiting %d seconds for next card...", timeout_seconds);
  for (
    res = LC_Client_GetNextCard(cl, &card, timeout_seconds);
    res == LC_Client_ResultOk;
    res = LC_Client_GetNextCard(cl, &card, timeout_seconds)
  ) {
    num_cards++;
    DBG_INFO(0, "Found a card.");
    
    if (card->readerName) {
      DBG_INFO(0, "Access to this card is provided by reader: \"%s\"", card->readerName);
    } else {
      DBG_INFO(0, "Access to this card is provided by unnamed reader.");
    }
    if (card->cardType) {
      DBG_INFO(0, "This card is of type: \"%s\"", card->cardType);
    } else {
      DBG_INFO(0, "This card is untyped.");
    }
    
    if (LC_ChiptanusbCard_ExtendCard(card)) {
      DBG_ERROR(0, "ERROR: Could not extend card as CipTanUsb card.");
      return GWEN_ERROR_INVALID;
    }

    DBG_INFO(0, "Opening card.");
    res = LC_Card_Open(card);
    if (res != LC_Client_ResultOk) {
      DBG_ERROR(0, "ERROR: Error executing command CardOpen (%d).\n", res);
      return GWEN_ERROR_OPEN;
    } else {
      DBG_INFO(0, "Card is a CipTanUsb card as expected.");
      LC_Card_Close(card);
    }
    LC_Client_ReleaseCard(cl, card);
    LC_Card_free(card);
    DBG_INFO(0, "I am done with this card.");
    
    DBG_INFO(0, "Waiting %d seconds for next card...", timeout_seconds);
  }
  
  DBG_INFO(0, "Found a total of %d cards.", num_cards);
  LC_Client_free(cl);

  return (0);
}
