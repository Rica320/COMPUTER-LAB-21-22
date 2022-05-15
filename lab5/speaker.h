/*** Example of a music definition
 *
 * C does not have classes: use structures
 */

#ifndef _SPEAKER_H_
#define _SPEAKER_H_

typedef struct {
  int freq; // note’s frequency
  int time; // note’s duration
} note_t;

typedef struct {
  int length;    // number of notes in song
  int cur;       // index of next note to be played
  int pause;     // pause between notes (in ms)
  note_t *notes; // pointer to array of notes
} music_t;


#endif