/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "gams/GAMS_Export.h"
/* Header for class com_gams_utility_SearchArea */

#ifndef _Included_com_gams_utility_SearchArea
#define _Included_com_gams_utility_SearchArea
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_SearchArea
 * Signature: ()J
 */
GAMS_Export jlong JNICALL Java_com_gams_utility_SearchArea_jni_1SearchArea
  (JNIEnv *, jobject);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
GAMS_Export jstring JNICALL Java_com_gams_utility_SearchArea_jni_1getName
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_setName
 * Signature: (JLjava/lang/String;)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1setName
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_fromContainer
 * Signature: (JJLjava/lang/String;)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1fromContainer
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_toContainer
 * Signature: (JJLjava/lang/String;)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1toContainer
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_modify
 * Signature: (J)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1modify
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_freeSearchArea
 * Signature: (J)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1freeSearchArea
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_toString
 * Signature: (J)Ljava/lang/String;
 */
GAMS_Export jstring JNICALL Java_com_gams_utility_SearchArea_jni_1toString
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_addPrioritizedRegion
 * Signature: (JJ)V
 */
GAMS_Export void JNICALL Java_com_gams_utility_SearchArea_jni_1addPrioritizedRegion
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getConvexHull
 * Signature: (J)J
 */
GAMS_Export jlong JNICALL Java_com_gams_utility_SearchArea_jni_1getConvexHull
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_containsGps
 * Signature: (JJ)Z
 */
GAMS_Export jboolean JNICALL Java_com_gams_utility_SearchArea_jni_1containsGps
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMaxAlt
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMaxAlt
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMinAlt
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMinAlt
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMaxLat
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMaxLat
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMinLat
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMinLat
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMaxLong
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMaxLong
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getMinLong
 * Signature: (J)D
 */
GAMS_Export jdouble JNICALL Java_com_gams_utility_SearchArea_jni_1getMinLong
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getGpsPriority
 * Signature: (JJ)J
 */
GAMS_Export jlong JNICALL Java_com_gams_utility_SearchArea_jni_1getGpsPriority
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_gams_utility_SearchArea
 * Method:    jni_getRegions
 * Signature: (J)[J
 */
GAMS_Export jlongArray JNICALL Java_com_gams_utility_SearchArea_jni_1getRegions
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
