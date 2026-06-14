#include <jni.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include "lm/model.hh"

struct ModelHandle {
    lm::ngram::Model* model;
};

extern "C" {

JNIEXPORT jlong JNICALL
Java_dev_ngocthanhgl_vikey_ime_nlp_vietnamese_KenlmNatives_nativeLoad(
    JNIEnv* env, jclass, jstring modelPath) {
    const char* path = env->GetStringUTFChars(modelPath, nullptr);
    auto* handle = new ModelHandle();
    try {
        handle->model = lm::ngram::LoadVirtual(path);
    } catch (const std::exception& e) {
        delete handle;
        env->ReleaseStringUTFChars(modelPath, path);
        return 0;
    }
    env->ReleaseStringUTFChars(modelPath, path);
    return reinterpret_cast<jlong>(handle);
}

JNIEXPORT void JNICALL
Java_dev_ngocthanhgl_vikey_ime_nlp_vietnamese_KenlmNatives_nativeUnload(
    JNIEnv*, jclass, jlong ptr) {
    auto* handle = reinterpret_cast<ModelHandle*>(ptr);
    if (handle) {
        delete handle->model;
        delete handle;
    }
}

JNIEXPORT jfloatArray JNICALL
Java_dev_ngocthanhgl_vikey_ime_nlp_vietnamese_KenlmNatives_nativeScoreCandidates(
    JNIEnv* env, jclass, jlong ptr, jstring prevWord, jobjectArray candidates) {
    auto* handle = reinterpret_cast<ModelHandle*>(ptr);
    if (!handle || !handle->model) return nullptr;

    jsize count = env->GetArrayLength(candidates);
    std::unique_ptr<jfloat[]> scores(new jfloat[count]);

    lm::ngram::State context_state = handle->model->BeginSentenceState();
    if (prevWord != nullptr) {
        const char* prevStr = env->GetStringUTFChars(prevWord, nullptr);
        lm::ngram::State temp;
        handle->model->FullScore(context_state, prevStr, temp);
        context_state = temp;
        env->ReleaseStringUTFChars(prevWord, prevStr);
    }

    for (jsize i = 0; i < count; i++) {
        jstring candidate = (jstring)env->GetObjectArrayElement(candidates, i);
        if (candidate == nullptr) { scores[i] = -99.0f; continue; }
        const char* candStr = env->GetStringUTFChars(candidate, nullptr);
        lm::ngram::State out_state;
        lm::FullScoreReturn ret = handle->model->FullScore(context_state, candStr, out_state);
        scores[i] = ret.prob;
        env->ReleaseStringUTFChars(candidate, candStr);
        env->DeleteLocalRef(candidate);
    }

    jfloatArray result = env->NewFloatArray(count);
    env->SetFloatArrayRegion(result, 0, count, scores.get());
    return result;
}

} // extern "C"
