/**************************************************************************
 * Copyright (C) 2018-2019 Junlon2006
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_wav_header.h
 * Author      : junlon2006@163.com
 * Date        : 2019.05.11
 *
 **************************************************************************/
#ifndef WAV_PCM_PARSE_TOOLS_UNI_WAV_HEADER_H_
#define WAV_PCM_PARSE_TOOLS_UNI_WAV_HEADER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PACKED  __attribute__ ((packed))

typedef struct {
  char  riff[4];
  int   file_len;
  char  wave[4];
  char  fmt[4];
  char  filter[4];
  short type;
  short channel;
  int   sample_rate;
  int   bitrate;
  short adjust;
  short bit;
  char  data[4];
  int   audio_len;
} PACKED WavHeader;

#endif  /* WAV_PCM_PARSE_TOOLS_UNI_WAV_HEADER_H_ */

