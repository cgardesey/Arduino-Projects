const int speakerPin = 11; // connect speaker to pin 9
char noteNames[] = {'C','D','E','F','G','a','b'};
//unsigned int frequencies[] = {262,294,330,349,392,440,494};
unsigned int frequencies[] = {462,494,630,649,692,840,894};
const byte noteCount = sizeof(noteNames); // the number of notes (7 in this
//notes, a space represents a rest
char score[] = "CCGGaaGFFEEDDC GGFFEEDGGFFEED CCGGaaGFFEEDDC ";
const byte scoreLen = sizeof(score); // the number of notes in the score
void setup()
{}
void loop()
{
for (int i = 0; i < scoreLen; i++)
{
int duration = 333; // each note lasts for a third of a second
playNote(score[i], duration); // play the note
}
delay(4000); // wait four seconds before repeating the song
}
void playNote(char note, int duration)
{
// play the tone corresponding to the note name
for (int i = 0; i < noteCount; i++)
{
// try and find a match for the noteName to get the index to the note
if (noteNames[i] == note) // find a matching note name in the array
tone(speakerPin, frequencies[i], duration); // play the note using the

}
// if there is no match then the note is a rest, so just do the delay
delay(duration);
}
