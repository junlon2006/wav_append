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
 * Description : wav_append_silence.c
 * Author      : junlon2006@163.com
 * Date        : 2019.05.11
 *
 **************************************************************************/

#include "uni_wav_header.h"
#include "uni_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>

#define WAV_APPEND_SILENCE_TAG "wav_append_silece"

static void _append_silence_process(int fd, WavHeader *wavheader, char *msec) {
  int msecond = atoi(msec);
  int append_silence_len = wavheader->sample_rate / 1000 * wavheader->bit / 8 * msecond;
  char *data = (char *)malloc(wavheader->audio_len + append_silence_len);
  memset(data + wavheader->audio_len, 0, append_silence_len);
  LOGT(WAV_APPEND_SILENCE_TAG, "process [%d] len=%d", fd, append_silence_len);
  if (wavheader->audio_len != read(fd, data, wavheader->audio_len)) {
    LOGE(WAV_APPEND_SILENCE_TAG, "read audio_data failed");
    goto L_END;
  }
  wavheader->file_len += append_silence_len;
  wavheader->audio_len += append_silence_len;
  lseek(fd, 0, SEEK_SET);
  if (sizeof(WavHeader) != write(fd, wavheader, sizeof(WavHeader))) {
    LOGE(WAV_APPEND_SILENCE_TAG, "write data failed");
    goto L_END;
  }
  if (wavheader->audio_len != write(fd, data, wavheader->audio_len)) {
    LOGE(WAV_APPEND_SILENCE_TAG, "write data failed");
    goto L_END;
  }
L_END:
  free(data);
  close(fd);
}

static void _print(WavHeader *wavheader) {
  LOGT(WAV_APPEND_SILENCE_TAG, "riff[%c%c%c%c]", wavheader->riff[0], wavheader->riff[1],
       wavheader->riff[2], wavheader->riff[3]);
  LOGT(WAV_APPEND_SILENCE_TAG, "file_len=%d", wavheader->file_len);
  LOGT(WAV_APPEND_SILENCE_TAG, "wave[%c%c%c%c]", wavheader->wave[0], wavheader->wave[1],
       wavheader->wave[2], wavheader->wave[3]);
  LOGT(WAV_APPEND_SILENCE_TAG, "fmt[%c%c%c%c]", wavheader->fmt[0], wavheader->fmt[1],
       wavheader->fmt[2], wavheader->fmt[3]);
  LOGT(WAV_APPEND_SILENCE_TAG, "filter[%d%d%d%d]", wavheader->filter[0], wavheader->filter[1],
       wavheader->filter[2]);
  LOGT(WAV_APPEND_SILENCE_TAG, "type=%d", wavheader->type);
  LOGT(WAV_APPEND_SILENCE_TAG, "channel=%d", wavheader->channel);
  LOGT(WAV_APPEND_SILENCE_TAG, "sample_rate=%d", wavheader->sample_rate);
  LOGT(WAV_APPEND_SILENCE_TAG, "bitrate=%d", wavheader->bitrate);
  LOGT(WAV_APPEND_SILENCE_TAG, "adjust=%d", wavheader->adjust);
  LOGT(WAV_APPEND_SILENCE_TAG, "bit=%d", wavheader->bit);
  LOGT(WAV_APPEND_SILENCE_TAG, "data[%c%c%c%c]", wavheader->data[0], wavheader->data[1],
       wavheader->data[2], wavheader->data[3]);
  LOGT(WAV_APPEND_SILENCE_TAG, "audio_len=%d", wavheader->audio_len);

}

static bool _is_wav_format(char *file_name, int *fd, WavHeader *wavheader) {
  *fd = open(file_name, O_RDWR, 0664);
  int read_len;
  int len;
  if (*fd < 0) {
    LOGT(WAV_APPEND_SILENCE_TAG, "open file[%s] failed", file_name);
    goto L_ERROR;
  }
  len = lseek(*fd, 0, SEEK_END);
  lseek(*fd, 0, SEEK_SET);
  LOGT(WAV_APPEND_SILENCE_TAG, "#########[%d], [%d]", sizeof(WavHeader), len);
  if (sizeof(WavHeader) != (read_len = read(*fd, wavheader, sizeof(WavHeader)))) {
    LOGT(WAV_APPEND_SILENCE_TAG, "read file[%s] failed, read_len=%d", file_name, read_len);
    goto L_ERROR;
  }
  _print(wavheader);
  if (wavheader->riff[0] == 'R' &&
      wavheader->riff[1] == 'I' &&
      wavheader->riff[2] == 'F' &&
      wavheader->riff[3] == 'F' &&
      wavheader->wave[0] == 'W' &&
      wavheader->wave[1] == 'A' &&
      wavheader->wave[2] == 'V' &&
      wavheader->wave[3] == 'E') {
    return true;
  }
L_ERROR:
  if (*fd > 0) {
    close(*fd);
    *fd = -1;
  }
  return false;
}

static void _get_all_wav_files(char *path, char *msec) {
  DIR *dir;
  struct dirent *ent;
  char file_name[512];
  int fd;
  WavHeader wavheader;
  dir = opendir(path);
  LOGT(WAV_APPEND_SILENCE_TAG, "path=%s, msec=%s", path, msec);
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_type == 0) {
      if (strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) {
        continue;
      }
      sprintf(file_name,"%s/%s", path, ent->d_name);
      LOGT(WAV_APPEND_SILENCE_TAG, "%s", file_name);
      //LOGT(WAV_APPEND_SILENCE_TAG, "%s----%d", ent->d_name, ent->d_type);
      if (_is_wav_format(file_name, &fd, &wavheader)) {
        _append_silence_process(fd, &wavheader, msec);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    LOGW(WAV_APPEND_SILENCE_TAG, "wav_append_silence wavdirectory silence msec.");
    return -1;
  }
  _get_all_wav_files(argv[1], argv[2]);
  return 0;
}
