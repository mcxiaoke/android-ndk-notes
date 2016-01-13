package com.demo.livecamera;

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Extention of SurfaceView which starts a camera preview and decode video
 * content on the native side.
 */
public class CameraView extends SurfaceView implements SurfaceHolder.Callback,
		Camera.PreviewCallback {
	private static final String TAG = "LiveCamera";

	static {
		System.loadLibrary("livecamera");
	}

	public native void decode(Bitmap pTarget, byte[] pSource);

	private Camera mCamera;
	private byte[] mVideoSource;
	private Bitmap mBackBuffer;
	private Paint mPaint;

	public CameraView(Context context) {
		super(context);

		// Registers current class so that it listens to surface
		// event (creation, destruction and changes).
		getHolder().addCallback(this);
		// Clears the flag keeping the surface from getting drawn.
		// Necessary when not drawing from a thread.
		setWillNotDraw(false);
	}

	public void surfaceCreated(SurfaceHolder holder) {
		try {
			// Acquires the default camera.
			mCamera = Camera.open();
			// Sets landscape mode to avoid complications related to
			// screen orientation handling.
			mCamera.setDisplayOrientation(0);
			// Registers callbacks. Automatic preview is deactivated
			// as we want to process data ourself (in a buffer).
			mCamera.setPreviewDisplay(null);
			mCamera.setPreviewCallbackWithBuffer(this);
		} catch (IOException eIOException) {
			mCamera.release();
			mCamera = null;
			throw new IllegalStateException();
		}
	}

	public void surfaceChanged(SurfaceHolder pHolder, int pFormat, int pWidth,
			int pHeight) {
		mCamera.stopPreview();

		// Finds a suitable resolution.
		Size lSize = findBestResolution(pWidth, pHeight);

		// Prepares video source and back buffers.
		PixelFormat lPixelFormat = new PixelFormat();
		PixelFormat.getPixelFormatInfo(mCamera.getParameters()
				.getPreviewFormat(), lPixelFormat);
		int lSourceSize = lSize.width * lSize.height
				* lPixelFormat.bitsPerPixel / 8;
		mVideoSource = new byte[lSourceSize];
		mBackBuffer = Bitmap.createBitmap(lSize.width, lSize.height,
				Bitmap.Config.ARGB_8888);

		// Set-up camera size and video format. YCbCr_420_SP
		// should be the default on Android anyway.
		Camera.Parameters lParameters = mCamera.getParameters();
		lParameters.setPreviewSize(lSize.width, lSize.height);
		lParameters.setPreviewFormat(PixelFormat.YCbCr_420_SP);
		mCamera.setParameters(lParameters);

		// Starts receiving pictures from the camera.
		mCamera.addCallbackBuffer(mVideoSource);
		mCamera.startPreview();
	}

	private Size findBestResolution(int pWidth, int pHeight) {
		List<Size> lSizes = mCamera.getParameters().getSupportedPreviewSizes();
		// Finds the biggest resolution which fits the screen.
		// Else, returns the first resolution found.
		Size lSelectedSize = mCamera.new Size(0, 0);
		for (Size lSize : lSizes) {
			if ((lSize.width <= pWidth) && (lSize.height <= pHeight)
					&& (lSize.width >= lSelectedSize.width)
					&& (lSize.height >= lSelectedSize.height)) {
				lSelectedSize = lSize;
			}
		}
		// Previous code assume that there is a preview size smaller
		// than screen size. If not, hopefully the Android API
		// guarantees that at least one preview size is available.
		if ((lSelectedSize.width == 0) || (lSelectedSize.height == 0)) {
			lSelectedSize = lSizes.get(0);
		}
		Log.d(TAG, "findBestResolution: " + lSelectedSize.width + ","
				+ lSelectedSize.height);
		return lSelectedSize;
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		// Releases camera which is a shared resource.
		if (mCamera != null) {
			mCamera.stopPreview();
			mCamera.release();
			// These variables can take a lot of memory. Gets rid of
			// them as fast as we can.
			mCamera = null;
			mVideoSource = null;
			mBackBuffer = null;
		}
	}

	public void onPreviewFrame(byte[] pData, Camera pCamera) {
		// New data has been received from camera. Processes it and
		// requests surface to be redrawn right after.
		decode(mBackBuffer, pData);
		invalidate();
	}

	@Override
	protected void onDraw(Canvas pCanvas) {
		if (mCamera != null) {
			// Draws resulting image at screen origin.
			pCanvas.drawBitmap(mBackBuffer, 0, 0, mPaint);
			// Enqueues buffer again to get next image.
			mCamera.addCallbackBuffer(mVideoSource);
		}
	}
}
