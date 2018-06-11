#include "ltp/segment_dll.h"
#include "edu_hit_ir_ltp4j_Segmentor.h"
#include "string_to_jstring.hpp"
#include <iostream>
#include <string>
#include <vector>

static void* segmentor = NULL;

JNIEXPORT jint JNICALL Java_edu_hit_ir_ltp4j_Segmentor_create__Ljava_lang_String_2
  (JNIEnv* env, jobject obj, jstring model_path) {
  const char* str = env->GetStringUTFChars( model_path , 0);

  if(!segmentor){
    segmentor = segmentor_create_segmentor(str);
  } else{
    segmentor_release_segmentor(segmentor);
    segmentor = segmentor_create_segmentor(str);
  }

  env->ReleaseStringUTFChars( model_path, str);

  if(segmentor) {
    return 1;
  }

  return -1;
}

JNIEXPORT jint JNICALL Java_edu_hit_ir_ltp4j_Segmentor_create__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv* env, jobject obj, jstring model_path, jstring lexicon_path) {

  const char* str_model = env->GetStringUTFChars( model_path , 0);
  const char* str_lexicon = env->GetStringUTFChars( lexicon_path , 0);

  if(!segmentor){
    segmentor = segmentor_create_segmentor(str_model,str_lexicon);
  } else{
    segmentor_release_segmentor(segmentor);
    segmentor = segmentor_create_segmentor(str_model,str_lexicon);
  }

  env->ReleaseStringUTFChars( model_path, str_model);
  env->ReleaseStringUTFChars( lexicon_path, str_lexicon);

  if(segmentor) {
    return 1;
  }
  return -1;
}

JNIEXPORT jint JNICALL Java_edu_hit_ir_ltp4j_Segmentor_segment
  (JNIEnv* env, jobject obj, jstring sent, jobject array_words) {

  jclass array_list = env->GetObjectClass(array_words);
  jmethodID list_add = env->GetMethodID(array_list, "add", "(Ljava/lang/Object;)Z");

  const char* str_sent = env->GetStringUTFChars( sent , 0);
  std::string sentence(str_sent);
  std::vector<std::string> words;

  int len = segmentor_segment(segmentor, sentence, words);

  for(int i = 0; i < len; i++) {
    jobject tmp = stringToJstring(env,words[i].c_str());
    env->CallBooleanMethod(array_words,list_add,tmp);
  }
  env->ReleaseStringUTFChars(sent, str_sent);
  return len;
}

JNIEXPORT void JNICALL Java_edu_hit_ir_ltp4j_Segmentor_release
  (JNIEnv* env, jobject obj) {
  segmentor_release_segmentor(segmentor);
  segmentor = NULL;
}

JNIEXPORT jint JNICALL Java_edu_hit_ir_ltp4j_Segmentor_loaded
  (JNIEnv *, jclass){
  if(segmentor) {
    return 1;
  }
  return -1;
}
