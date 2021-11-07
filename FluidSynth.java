package ml.mckuhei.utils;

import java.io.File;
import java.util.ArrayList;

import javax.sound.midi.*;

public class FluidSynth {
  static {
    System.loadLibrary("FluidSynth");
  }

  private static final FluidSynth instance = new FluidSynth();

  public FluidSynth() {
    init();
  }

  protected long synth;
  protected long settings;
  protected long adriver;
  protected long sfont = -1;

  private native void init();

  public native boolean open(String soundfontFile);

  public native void noteOn(int channel, int midiNumber, int velocity);

  public native void noteOff(int channel, int midiNumber);

  public native void controlChange(int channel, int controller, int value);

  public native void programChange(int channel, int program);

  public native void printLoadedInstruments();

  public native void close();

  public void send(MidiMessage message, long timeStamp) {

  }

  public void noteOn(int midiNumber) {
    noteOn(0, midiNumber, 127);
  }

  public void noteOff(int midiNumber) {
    noteOff(0, midiNumber);
  }

  protected void finalize() {
    close();
  }

  public static void main(String[] args) throws Exception {
    FluidSynth synthesizer = FluidSynth.instance;
    boolean success = synthesizer.open("example.sf2");
    if (!success) return;
    Sequence seq = MidiSystem.getSequence(new File("nyan.mid"));
    Track[] tracks = seq.getTracks();
    long progress = 0;
    long length = seq.getTickLength();
    double bpm = 240;
    while(progress<length) {
      ArrayList<MidiEvent> events = new ArrayList<>();
      for (Track track : tracks) {
        int trackLength = track.size();
        for(int i=0;i<trackLength;i++) {
          MidiEvent event = track.get(i);
          if(event.getTick()==progress) {
            events.add(event);
          }
        }
      }
      for (MidiEvent event : events) {
        MidiMessage message = event.getMessage();
        if(message instanceof ShortMessage) {
          ShortMessage msg = (ShortMessage) message;
          switch (msg.getCommand()) {
            case ShortMessage.NOTE_ON:
              //System.out.println(msg.getChannel());
              synthesizer.noteOn(msg.getChannel(),msg.getData1(),msg.getData2());
              break;
            case ShortMessage.NOTE_OFF:
              synthesizer.noteOff(msg.getChannel(), msg.getData1());
              break;
            case ShortMessage.PROGRAM_CHANGE:
              synthesizer.programChange(msg.getChannel(),msg.getData1());
              break;
            case ShortMessage.CONTROL_CHANGE:
              synthesizer.controlChange(msg.getChannel(), msg.getData1(), msg.getData2());
            default:
              break;
          }
        }
      }
      Thread.sleep((long) (500/bpm));
      progress++;
    }
  }
}
