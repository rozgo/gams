/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_gams_utility_Position */

#ifndef _Included_com_gams_utility_Position
#define _Included_com_gams_utility_Position
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_gams_utility_Position
 * Method:    jni_Position
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_gams_utility_Position_jni_1Position__
  (JNIEnv *, jobject);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_Position
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_gams_utility_Position_jni_1Position__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_Position
 * Signature: (DDD)J
 */
JNIEXPORT jlong JNICALL Java_com_gams_utility_Position_jni_1Position__DDD
  (JNIEnv *, jobject, jdouble, jdouble, jdouble);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_freePosition
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_gams_utility_Position_jni_1freePosition
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_gams_utility_Position_jni_1toString
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_getX
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_gams_utility_Position_jni_1getX
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_getY
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_gams_utility_Position_jni_1getY
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_getZ
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_gams_utility_Position_jni_1getZ
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_setX
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_gams_utility_Position_jni_1setX
  (JNIEnv *, jobject, jlong, jdouble);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_setY
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_gams_utility_Position_jni_1setY
  (JNIEnv *, jobject, jlong, jdouble);

/*
 * Class:     com_gams_utility_Position
 * Method:    jni_setZ
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_gams_utility_Position_jni_1setZ
  (JNIEnv *, jobject, jlong, jdouble);

#ifdef __cplusplus
}
#endif
#endif
