LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := game

LOCAL_SRC_FILES := main.cpp \
../../../../extensions/AdvanceSpriteAndParticle/AdvanceParticleQuadSystem.cpp \
../../../../extensions/AdvanceSpriteAndParticle/AdvanceSprite.cpp \
../../../../extensions/NdControls/NdCxList.cpp \
../../../../extensions/NdControls/NdCxListItem.cpp \
../../../Classes/AppDelegate.cpp \
../../../Classes/tests/controller.cpp \
../../../Classes/tests/testBasic.cpp \
../../../Classes/tests/AdvancedParticleTest/AdvanceParticleTest.cpp \
../../../Classes/tests/AdvanceSpriteTest/AdvanceSpriteTest.cpp \
../../../Classes/tests/NdCxListTest/NdCxListTest.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../cocos2dx \
                    $(LOCAL_PATH)/../../../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../../extensions

# it is used for ndk-r5  
# if you build with ndk-r4, comment it  
# because the new Windows toolchain doesn't support Cygwin's drive
# mapping (i.e /cygdrive/c/ instead of C:/)  
LOCAL_LDLIBS := -L$(call host-path, $(LOCAL_PATH)/../../libs/armeabi) \
                -lcocos2d -llog \
                -lGLESv1_CM \
                -L$(call host-path, $(LOCAL_PATH)/../../../../../cocos2dx/platform/third_party/android/libraries)
            
include $(BUILD_SHARED_LIBRARY)
