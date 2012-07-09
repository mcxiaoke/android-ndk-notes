#include "com_demo_livecamera_CameraView.h"
#include <android/bitmap.h>
#include <android/log.h>

#define LOG_TAG "LiveCamera"
#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(LOG_TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGE(LOG_TAG, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define DLOG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define VLOG(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ELOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

inline int32_t toInt(jbyte byte) {
	return (0xff & (int32_t) byte);
}

inline int32_t max(int32_t m, int32_t n) {
	if (m < n) {
		return n;
	} else {
		return m;
	}
//	return m < n ? n : m;
}

inline int32_t clamp(int32_t value, int32_t lowest, int32_t highest) {
	if (value < 0) {
		return lowest;
	} else if (value > highest) {
		return highest;
	} else {
		return value;
	}
//	return value > highest ? highest : value;
}

inline int32_t color(int r, int g, int b) {
    return 0xFF000000 | ((r << 6)  & 0x00FF0000)
                      | ((g >> 2)  & 0x0000FF00)
                      | ((b >> 10) & 0x000000FF);
}

JNIEXPORT void JNICALL Java_com_demo_livecamera_CameraView_decode
(JNIEnv * pEnv, jclass pClass, jobject pTarget, jbyteArray pSource) {
	// Retrieves bitmap information and locks it for drawing.
	AndroidBitmapInfo lBitmapInfo;
	if (AndroidBitmap_getInfo(pEnv, pTarget, &lBitmapInfo) < 0) {
		return;
	}
	if (lBitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		return;
	}

	uint32_t* lBitmapContent;
	if (AndroidBitmap_lockPixels(pEnv, pTarget,
					(void**)&lBitmapContent) < 0) {
		return;
	}

	// Accesses source array data.
	jbyte* lSource = (*pEnv)->GetPrimitiveArrayCritical(pEnv,
			pSource, 0);
	if (lSource == NULL) {
		return;
	}

	int32_t lFrameSize = lBitmapInfo.width * lBitmapInfo.height;
	int32_t lYIndex, lUVIndex;
	int32_t lX, lY;
	int32_t lColorY, lColorU, lColorV;
	int32_t lColorR, lColorG, lColorB;
	int32_t y1192;

	// Processes each pixel and converts YUV to RGB color.
	// Algorithm originating from Ketai open source project.
	// See http://ketai.googlecode.com/.
	for (lY = 0, lYIndex = 0; lY < lBitmapInfo.height; ++lY) {
		lColorU = 0; lColorV = 0;
		// Y is divided by 2 because UVs are subsampled vertically.
		// This means that two consecutives iterations refer to the
		// same UV line (e.g when Y=0 and Y=1).
		lUVIndex = lFrameSize + (lY >> 1) * lBitmapInfo.width;

		for (lX = 0; lX < lBitmapInfo.width; ++lX, ++lYIndex) {
			// Retrieves YUV components. UVs are subsampled
			// horizontally too, hence %2 (1 UV for 2 Y).
			lColorY = max(toInt(lSource[lYIndex]) - 16, 0);
			if (!(lX % 2)) {
				lColorV = toInt(lSource[lUVIndex++]) - 128;
				lColorU = toInt(lSource[lUVIndex++]) - 128;
			}

			// Computes R, G and B from Y, U and V.
			y1192 = 1192 * lColorY;
			lColorR = (y1192 + 1634 * lColorV);
			lColorG = (y1192 - 833 * lColorV - 400 * lColorU);
			lColorB = (y1192 + 2066 * lColorU);

			lColorR = clamp(lColorR, 0, 262143);
			lColorG = clamp(lColorG, 0, 262143);
			lColorB = clamp(lColorB, 0, 262143);

			// Combines R, G, B and A into the final pixel color.
			lBitmapContent[lYIndex] = color(lColorR,lColorG,lColorB);
		}
	}

	// Unlocks the bitmap and releases the Java array when finished.
	(*pEnv)-> ReleasePrimitiveArrayCritical(pEnv,pSource,lSource, 0);
	AndroidBitmap_unlockPixels(pEnv, pTarget);
}

