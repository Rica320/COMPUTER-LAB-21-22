#include <lcom/lcf.h>

#ifndef _LCOM_COM_PROTOCOL_H_
#  define _LCOM_COM_PROTOCOL_H_

/**
 * @brief Serial Port
 * Communication protocol struct definition
 *
 */
typedef struct Protocol {
  uint8_t origin;
  uint8_t message;
  uint8_t dest;
  bool move;
  bool more_chars;
  bool col;
  bool com_status;
} Protocol;

/**
 * @brief Serial Port communication status
 *
 */
enum Communication_Status {
  no_one,
  waiting,
  connected,
};

/**
 * @brief Encode the given protocol struct into a
 * byte, following the protocol
 *
 * @param pro protocol to be encoded
 * @return uint8_t encoded byte message
 */
uint8_t encode_protocol(Protocol pro);

/**
 * @brief Decods the given protocol into
 *
 * @param pro struct to be filled with the decoded message
 * @param byte byte to be decoded into the struct
 * @return int 0 if decode was OK
 */
int decode_protocol(Protocol *pro, uint8_t byte);

#endif
