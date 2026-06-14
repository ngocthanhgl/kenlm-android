#include <jni.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include "lm/virtual_interface.hh"
#include "lm/model.hh"

struct ModelHandle {
    lm::base::Model* model;
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

    size_t stateSize = handle->model->StateSize();
    std::unique_ptr<char[]> contextState(new char[stateSize]);
    std::unique_ptr<char[]> scoreState(new char[stateSize]);
    handle->model->BeginSentenceWrite(contextState.get());

    if (prevWord != nullptr) {
        const char* prevStr = env->GetStringUTFChars(prevWord, nullptr);
        lm::WordIndex wordIdx = handle->model->BaseVocabulary().Index(prevStr);
        handle->model->BaseScore(contextState.get(), wordIdx, scoreState.get());
        memcpy(contextState.get(), scoreState.get(), stateSize);
        env->ReleaseStringUTFChars(prevWord, prevStr);
    }

    for (jsize i = 0; i < count; i++) {
        jstring candidate = (jstring)env->GetObjectArrayElement(candidates, i);
        if (candidate == nullptr) { scores[i] = -99.0f; continue; }
        const char* candStr = env->GetStringUTFChars(candidate, nullptr);
        lm::WordIndex wordIdx = handle->model->BaseVocabulary().Index(candStr);
        lm::FullScoreReturn ret = handle->model->BaseFullScore(
            contextState.get(), wordIdx, scoreState.get());
        scores[i] = ret.prob;
        env->ReleaseStringUTFChars(candidate, candStr);
        env->DeleteLocalRef(candidate);
    }

    jfloatArray result = env->NewFloatArray(count);
    env->SetFloatArrayRegion(result, 0, count, scores.get());
    return result;
}

} // extern "C"
