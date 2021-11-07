#include <jni.h>
#include <stdio.h>
#include "ml_mckuhei_utils_FluidSynth.h"

#include <fluidsynth.h>
#if defined(WIN32)
#include <windows.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

jfieldID synth_id;
jfieldID settings_id;
jfieldID adriver_id;
jfieldID sfont_id;

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_init(JNIEnv *env, jobject thisObj)
{
  fluid_settings_t *settings;
  fluid_synth_t *synth;
  fluid_audio_driver_t *adriver;

  settings = new_fluid_settings();
  synth = new_fluid_synth(settings);
  adriver = new_fluid_audio_driver(settings, synth);
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  synth_id = (*env)->GetFieldID(env, clazz, "synth", "J");
  (*env)->SetLongField(env, thisObj, synth_id, synth);
  settings_id = (*env)->GetFieldID(env, clazz, "settings", "J");
  (*env)->SetLongField(env, thisObj, settings_id, settings);
  adriver_id = (*env)->GetFieldID(env, clazz, "adriver", "J");
  (*env)->SetLongField(env, thisObj, adriver_id, adriver);
  sfont_id = (*env)->GetFieldID(env, clazz, "sfont", "J");
}

JNIEXPORT jboolean JNICALL Java_ml_mckuhei_utils_FluidSynth_open(JNIEnv *env, jobject thisObj, jstring soundfont)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_synth_t *synth;
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  fluid_sfont_t *sfont;

  const char *soundfontPointer = (*env)->GetStringUTFChars(env, soundfont, NULL);
  int sfont_id = fluid_synth_sfload(synth, soundfontPointer, 1);
  if (sfont_id == FLUID_FAILED)
  {
    /*delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);*/
    return 0;
  }
  sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
  (*env)->SetLongField(env, thisObj, sfont_id, sfont);
  return 1;
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_noteOn(JNIEnv *env, jobject thisObj, jint channel, jint midiNumber, jint velocity)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_synth_t *synth;
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  fluid_synth_noteon(synth, channel, midiNumber, velocity);
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_noteOff(JNIEnv *env, jobject thisObj, jint channel, jint midiNumber)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_synth_t *synth;
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  fluid_synth_noteoff(synth, channel, midiNumber);
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_controlChange(JNIEnv *env, jobject thisObj, jint channel, jint controller, jint value)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_synth_t *synth;
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  fluid_synth_cc(synth, channel, controller, value);
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_programChange(JNIEnv *env, jobject thisObj, jint channel, jint program)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_synth_t *synth;
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  fluid_synth_program_change(synth, channel, program);
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_printLoadedInstruments(JNIEnv *env, jobject thisObj)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_sfont_t *sfont;
  sfont = (*env)->GetLongField(env, thisObj, sfont_id);
  fluid_preset_t *preset;
  fluid_sfont_iteration_start(sfont);
  while ((preset = fluid_sfont_iteration_next(sfont)) != NULL)
  {
    printf("%s\n", fluid_preset_get_name(preset));
  }
}

JNIEXPORT void JNICALL Java_ml_mckuhei_utils_FluidSynth_close(JNIEnv *env, jobject thisObj)
{
  jclass clazz = (*env)->GetObjectClass(env, thisObj);
  fluid_settings_t *settings;
  fluid_synth_t *synth;
  fluid_audio_driver_t *adriver;
  settings = (*env)->GetLongField(env, thisObj, settings_id);
  synth = (*env)->GetLongField(env, thisObj, synth_id);
  adriver = (*env)->GetLongField(env, thisObj, adriver_id);
  delete_fluid_audio_driver(adriver);
  delete_fluid_synth(synth);
  delete_fluid_settings(settings);
}
