/****************************************************************************************************************************
  cdecode.h - c header for a base64 decoding algorithm

  This is part of the libb64 project, and has been placed in the public domain.
  For details, see http://sourceforge.net/projects/libb64

  For RP2040W with CYW43439 WiFi
  
  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi
  
  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 
  Version: 1.1.2
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  1.0.1   K Hoang      15/08/2022 Fix bug in examples, `library.json`
  1.0.2   K Hoang      15/08/2022 Fix LED bug in examples
  1.0.3   K Hoang      22/09/2022 To display country-code and tempo method to modify in arduino-pico core
  1.1.0   K Hoang      25/09/2022 Fix issue with slow browsers or network
  1.1.2   K Hoang      26/09/2022 Add function and example to support favicon.ico
 *****************************************************************************************************************************/

#pragma once

// Reintroduce to prevent duplication compile error if other lib/core already has LIB64
// pragma once can't prevent that
#ifndef BASE64_CDECODE_H
#define BASE64_CDECODE_H

#define base64_decode_expected_len(n) ((n * 3) / 4)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
  step_a, step_b, step_c, step_d
} base64_decodestep;

typedef struct 
{
  base64_decodestep step;
  char plainchar;
} base64_decodestate;

void base64_init_decodestate(base64_decodestate* state_in);

int base64_decode_value(int value_in);

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in);

int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* BASE64_CDECODE_H */
