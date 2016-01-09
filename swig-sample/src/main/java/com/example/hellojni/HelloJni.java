/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example.hellojni;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class HelloJni {
    private static String staticField = "Static Field";
    private String instanceField = "Instane Field";

    public static String staticMethod() {
        return "Static Method";
    }

    public String instanceMethod() {
        return "Instance Method";
    }

    private void throwingMethod() throws NullPointerException {
        throw new NullPointerException("Null pointer");
    }

    public static native String stringFromJNI();

    public native String instanceStringFromJNI();

    private native void accessMethods();

    static {
        System.loadLibrary("hello-jni");
    }
}


/**
 * javap -classpath build\intermediates\classes\debug -p -s com.example.hellojni.HelloJni
 * <p/>
 * public class com.example.hellojni.HelloJni {
 * <p/>
 * private static java.lang.String staticField;
 * Signature: Ljava/lang/String;
 * <p/>
 * private java.lang.String instanceField;
 * Signature: Ljava/lang/String;
 * <p/>
 * public com.example.hellojni.HelloJni();
 * Signature: ()V
 * <p/>
 * public static native java.lang.String stringFromJNI();
 * Signature: ()Ljava/lang/String;
 * <p/>
 * public native java.lang.String instanceStringFromJNI();
 * Signature: ()Ljava/lang/String;
 * }
 */
