# KenLM Android

Builds [KenLM](https://github.com/kpu/kenlm) language model query library for Android via GitHub Actions.

## Output

- `libkenlm_jni.so` for arm64-v8a, armeabi-v7a, x86_64, x86
- Released as GitHub Release artifacts on each build

## Usage in Android project

1. Download `.so` files from the latest release
2. Place in `app/src/main/jniLibs/<abi>/libkenlm_jni.so`
3. Add `System.loadLibrary("kenlm_jni")` in Kotlin
4. Use the JNI API from `KenlmNatives` Kotlin object
